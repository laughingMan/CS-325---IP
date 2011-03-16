//-------------------------------------------------------------------------------------- 
//  CS325_IP03_ihatton_main.cpp
//  IP03
//  Program is a lexicographical analzier, LOC counter, and faults detecter
//
//  Created by Isaac Hatton on 3/13/11.
//  Copyright 2011 sublimeIke LLC. All rights reserved.
//
//-------------------------------------------------------------------------------------- 

#include "faults.h"

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <fstream>
using namespace std;

// Functions
void loadFile(ofstream &out_stream, string tokens, int &locCount);
void checkString(ofstream &out_stream, string fileString, char fileChar, int &locCount, Counter &loc, Counter &parenthesis);
void checkChar(ofstream &out_stream, string fileString, char fileChar, ifstream& inputFile, int &locCount, Counter &parenthesis, Counter &bracket, Counter &loc);
void locCounter(string fileString, char fileChar, int &locCount, Counter &loc);
void checkDepth(Counter &braces, int &locCount);
void methodLength(Counter &braces, Counter &loc, int &locCount);
void checkMagic(ofstream &out_stream, Counter &parenthesis, int &locCount);

// Global Items
const string OUTPUT_FILE = "IP03_output.txt";
const int MAX_LOC = 20;
const int MAX_DEPTH = 4;

// Global Arrays
const string RESERVED[] = {"const", "double", "float", "int", "short", "struct", "continue",
	"else", "for", "long", "void", "char", "do", "if", "return", "while", "bool", "new", 
	"false", "class", "private", "this", "public", "delete", "true"};

const string PREDEFINED[] = {"cin", "endl", "main", "cout", "NULL", "string"};

const string OPERATORS[] = {"+", "-", "=", "==", "--", "++", "<", ">", "<=", ">=", "!", "!=", 
	"<<", ">>", ":", "::"};

int main()
{	
	string token;
	int locCount = 0;
	ofstream out_stream;

	out_stream.open(OUTPUT_FILE);
	if (out_stream.fail())
	{
		cout << "Output file opening failed.\n";
		exit(1);
	}

	loadFile(out_stream, token, locCount);
	out_stream << endl << "Total LOC: " << locCount << endl << endl;
	system("pause");
}

void loadFile(ofstream &out_stream, string token, int &locCount)
{
	char fileChar; 
	string fileString;
	string fileName;
	ifstream inputFile;
	inputFile.unsetf(ios_base::skipws);
	Counter parenthesis;
	Counter braces;
	Counter loc;

	// Prompt user for input file's name
	cout << "Please enter the file name to be processed" << endl;
	cin >> fileName;

	inputFile.open(fileName);
	if(inputFile.fail())
	{
		cout << "Input file opening failed.\n";
		exit(1);
	}

	while(!inputFile.eof())
	{
		fileString.clear();
		fileChar = NULL;
		inputFile >> fileChar;

		while(isalnum(fileChar) || fileChar == '.')
		{
			fileString += fileChar;
			inputFile >> fileChar;
		}
		
		checkString(out_stream, fileString, fileChar, locCount, loc, parenthesis);
		checkChar(out_stream, fileString, fileChar, inputFile, locCount, parenthesis, braces, loc);
		checkDepth(braces, locCount);
		methodLength(braces, loc, locCount);
		checkMagic(out_stream, parenthesis, locCount);
	}
}
		
void checkString(ofstream &out_stream, string fileString, char fileChar, int &locCount, Counter &loc, Counter &parenthesis)
{
	bool found = false;
	
	// if string is empty
	if(fileString == "")
		return;

	// check against RESERVED array
	for(int i = 0; i < 25; i++)
	{
		if(RESERVED[i] == fileString)
		{
			out_stream << "RESERVED" << "          " << fileString << endl;
			found = true;
			locCounter(fileString, fileChar, locCount, loc);
		}
	}

	// check against PREDEFINED array
	for(int i = 0; i < 6; i++)
	{
		if(PREDEFINED[i] == fileString)
		{
			out_stream << "PREDEFINED" << "        " << fileString << endl;
			found = true;
		}
	}

	// defaults to IDENTIFIER
	if(!found)
	{
		out_stream << "IDENTIFIER" << "        " << fileString << endl;
		if(parenthesis.Count() == 1)
			parenthesis.SetFlag(1);
	}
}

void checkChar(ofstream &out_stream, string fileString, char fileChar, ifstream& inputFile, int &locCount, Counter &parenthesis, Counter &braces, Counter &loc)
{
	string extraString;
	
	// check against OPERATORS array
	for(int i = 0; i < 16; i++)
		{
			if(OPERATORS[i] == extraString)
			{
				extraString = fileChar;
				inputFile >> fileChar;
				if(fileChar != ' ')
					extraString += fileChar;;
				out_stream << "OPERATOR" << "          " << extraString << endl;
			}
		}
	
	// checks for COMMMENTS
	if(fileChar == '/')
	{
		while(fileChar != '\n')
		{
			extraString += fileChar;
			inputFile >> fileChar;
		}
		out_stream << "COMMENTS" << "          " << extraString << endl;
	}

	// checks for META
	else if(fileChar == '#')
	{
		while(fileChar != '\n')
		{
			extraString += fileChar;
			inputFile >> fileChar;
		}
		out_stream << "META" << "              " << extraString << endl;
		locCounter(fileString, '#', locCount, loc);
	}

	// checks for CONST_STR
	else if(fileChar == '"')
	{
		extraString += fileChar;
		inputFile >> fileChar;

		while(fileChar != '"')
		{
			extraString += fileChar;
			inputFile >> fileChar;
		}
        
		extraString += fileChar;
		parenthesis.SetFlag(2);
		out_stream << "CONST_STR" << "         " << extraString << endl;
	}

	else if(fileChar == '(')
	{
		parenthesis.Add();
		out_stream << "PAREN_LFT" << "         " << fileChar << endl;
	}

	else if(fileChar == ')')
	{
		parenthesis.Sub();
		out_stream << "PAREN_RGT" << "         " << fileChar << endl;
	}

	else if(fileChar == '{')
	{
		braces.Add();
		out_stream << "PAREN_LFT" << "         " << fileChar << endl;
	}

	else if(fileChar == '}')
	{
		braces.Sub();
		out_stream << "PAREN_RGT" << "         " << fileChar << endl;
	}
	
	else if(fileChar == ';')
	{
		out_stream << "SEMI" << "              " << fileChar << endl;
		locCounter(fileString, fileChar, locCount, loc);
	}

	else 
		return;
}

void locCounter(string fileString, char fileChar, int &locCount, Counter &loc)
{
	// empty LOC
	if(fileString == " " && fileChar == ';')
		return;

	// corrects for "for loop" meyhem
	if(fileString == "for")
	{
		locCount -= 2;
        if(loc.Flag() == 1)
			loc.Sub(2);
		return;
	}

	// regular LOC or Meta/ Directive
	if(fileChar == ';' || fileChar == '#')
	{
		locCount++;
        if(loc.Flag() == 1)
			loc.Add();
		return;
	}

	// loops
	if(fileString == "if" || fileString  == "while" || fileString == "switch")
	{
        locCount++;
		if(loc.Flag() == 1)
			loc.Add();
        return;
	}
}

void checkDepth(Counter &braces, int &locCount)
{
	if(braces.Flag() == 0 && braces.Count() > MAX_DEPTH)
	{
		// allows for the error to only be output once
		braces.SetFlag(2);
		cout << "The code complexity standard of nested items (" << MAX_DEPTH 
			 << " deep) has been exceeded at line (" << locCount << ").\n";
	}

	// resets the counter and flag
	if(braces.Count() == 0)
		braces.Reset();
}

void methodLength(Counter &braces, Counter &loc, int &locCount)
{
    if(braces.Count() == 1)
		loc.SetFlag(1);
    
	if(braces.Count() != 1)
		loc.SetFlag(0);

	if(loc.Temp() != 1 && loc.Count() > MAX_LOC)
	{
		cout << "The max method length was exceded at line (" << locCount << ")." << endl;
		loc.SetTemp(1);
	}

	if(braces.Count() == 0)
		loc.Reset();
}

void checkMagic(ofstream &out_stream, Counter &parenthesis, int &locCount)
{
	if (parenthesis.Count() == 1) 
    {
		// Flag bool true(2) == string
        if(parenthesis.Flag() == 2)
            cout << "A magic string was found at line (" << locCount << ")." << endl;
        
		// Flag bool false(1) == number
        if(parenthesis.Flag() == 1)
            cout << "A magic number was found at line (" << locCount << ")." << endl;

		parenthesis.SetFlag(0);
    }
}