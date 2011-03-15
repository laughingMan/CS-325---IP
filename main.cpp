//-------------------------------------------------------------------------------------- 
//   File: main.cpp
//   Name: Isaac Hatton
// Course: CS 325 - Software Engineering
//   Desc: Program accepts a file name as input and produces a token class and value 
// 		for output
//  Usage: The program will require the user to provide a (source code) file. Running
// 		the program will produce a tokenized output for the user.
//  Other files required: token.cpp, token.h
//-------------------------------------------------------------------------------------- 

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
using namespace std;

// Functions
void loadFile(string tokens);
void checkString(string fileString);
void checkChar(char fileChar, ifstream& inputFile);

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

	loadFile(token);
	system("pause");
}

void loadFile(string token)
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

		checkString(fileString);
		checkChar(fileChar, inputFile);
	}
}
		
void checkString(string fileString)
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
			cout << "RESERVED" << "          " << fileString << endl;
			found = true;
		}
	}

	// check against PREDEFINED array
	for(int i = 0; i < 6; i++)
	{
		if(predefined[i] == fileString)
		{
			cout << "PREDEFINED" << "        " << fileString << endl;
			found = true;
		}
	}

	// defaults to IDENTIFIER
	if(!found)
	cout << "IDENTIFIER" << "        " << fileString << endl;
}

void checkChar(char fileChar, ifstream& inputFile)
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
				cout << "OPERATOR" << "          " << extraString << endl;
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
		cout << "COMMENTS" << "          " << extraString << endl;
	}

	// checks for META
	else if(fileChar == '#')
	{
		while(fileChar != '\n')
		{
			extraString += fileChar;
			inputFile >> fileChar;
		}
		cout << "META" << "              " << extraString << endl;
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
		cout << "CONST_STR" << "         " << extraString << endl;
	}

	else if(fileChar == '(')
		cout << "PAREN_LFT" << "         " << fileChar << endl;

	else if(fileChar == ')')
		cout << "PAREN_RGT" << "         " << fileChar << endl;

	else if(fileChar == '{')
		cout << "PAREN_LFT" << "         " << fileChar << endl;

	else if(fileChar == '}')
		cout << "PAREN_RGT" << "         " << fileChar << endl;
	
	else if(fileChar == ';')
		cout << "SEMI" << "              " << fileChar << endl;

	else 
		return;
}