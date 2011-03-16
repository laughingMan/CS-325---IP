//-------------------------------------------------------------------------------------- 
//  main.cpp
//  IP02
//  Program is a lexicographical analzier plus a LOC counter
//
//  Created by Isaac Hatton on 2/6/11.
//  Copyright 2011 sublimeIke LLC. All rights reserved.
//
//  EXECUTIVE DECISION: I'm going to count "for loops" and 
//      "else if statements" as LOC, but not "else lines"
//-------------------------------------------------------------------------------------- 

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
using namespace std;

// Functions
void loadFile(ofstream &out_stream, string tokens, int &locCount);
void checkString(ofstream &out_stream, string fileString, char fileChar, int &locCount);
void checkChar(ofstream &out_stream, string fileString, char fileChar, ifstream& inputFile, int &locCount);
void locCounter(string fileString, char fileChar, int &locCount);

// Global Arrays
const string reserved[] = {"const", "double", "float", "int", "short", "struct", "continue",
	"else", "for", "long", "void", "char", "do", "if", "return", "while", "bool", "new", 
	"false", "class", "private", "this", "public", "delete", "true"};

const string predefined[] = {"cin", "endl", "main", "cout", "NULL", "string"};

const string operators[] = {"+", "-", "=", "==", "--", "++", "<", ">", "<=", ">=", "!", "!=", 
	"<<", ">>", ":", "::"};

int main()
{	
	string token;
	int locCount = 0;
	ofstream out_stream;

	out_stream.open("circoutput.txt");
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
	ifstream inputFile;
	inputFile.unsetf(ios_base::skipws);

		


	inputFile.open("input.txt");
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
		
		checkString(out_stream, fileString, fileChar, locCount);
		checkChar(out_stream, fileString, fileChar, inputFile, locCount);
	}
}
		
void checkString(ofstream &out_stream, string fileString, char fileChar, int &locCount)
{
	bool found = false;
	
	// if string is empty
	if(fileString == "")
		return;

	// check against RESERVED array
	for(int i = 0; i < 25; i++)
	{
		if(reserved[i] == fileString)
		{
			out_stream << "RESERVED" << "          " << fileString << endl;
			found = true;
			locCounter(fileString, fileChar, locCount);
		}
	}

	// check against PREDEFINED array
	for(int i = 0; i < 6; i++)
	{
		if(predefined[i] == fileString)
		{
			out_stream << "PREDEFINED" << "        " << fileString << endl;
			found = true;
		}
	}

	// defaults to IDENTIFIER
	if(!found)
	out_stream << "IDENTIFIER" << "        " << fileString << endl;
}

void checkChar(ofstream &out_stream, string fileString, char fileChar, ifstream& inputFile, int &locCount)
{
	string extraString;
	
	// check against OPERATORS array
	for(int i = 0; i < 16; i++)
		{
			if(operators[i] == extraString)
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
		locCounter(fileString, '#', locCount);
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
		out_stream << "CONST_STR" << "         " << extraString << endl;
	}

	else if(fileChar == '(')
		out_stream << "PAREN_LFT" << "         " << fileChar << endl;

	else if(fileChar == ')')
		out_stream << "PAREN_RGT" << "         " << fileChar << endl;

	else if(fileChar == '{')
		out_stream << "PAREN_LFT" << "         " << fileChar << endl;

	else if(fileChar == '}')
		out_stream << "PAREN_RGT" << "         " << fileChar << endl;
	
	else if(fileChar == ';')
	{
		out_stream << "SEMI" << "              " << fileChar << endl;
		locCounter(fileString, fileChar, locCount);
	}

	else 
		return;
}

void locCounter(string fileString, char fileChar, int &locCount)
{
	// empty LOC
	if(fileString == " " && fileChar == ';')
		return;

	// corrects for "for loop" meyhem
	if(fileString == "for")
	{
		locCount -= 2;
		return;
	}

	// regular LOC or Meta/ Directive
	if(fileChar == ';' || fileChar == '#')
	{
		locCount++;
		return;
	}

	// loops
	if(fileString == "if" || fileString  == "while" || fileString == "switch")
	{
			locCount++;
			return;
	}
}