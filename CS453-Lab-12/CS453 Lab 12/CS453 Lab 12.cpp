/***********************************************************************
* Program:
*    Lab 12, Bell-LaPadula
*    Brother Helfrich, CS470
* Author:
*    your name
* Summary:
*    This program is designed to keep track of a number of secret
*    messages. IT will display messages to the appropriate users
*    and withhold messages from those lacking the authority.
************************************************************************/

#include <iostream>   // standard input and output
#include <string>     // for convenience
#include <cassert>    // because I am paraniod
#include <assert.h> // TESTING
#include <vector> // Testing purposes
#include "interact.h" // the interaction code
#include "messages.h" // the collectio of messages
using namespace std;

const char* FILE_NAME = "messages.txt";


/***********************************************
 * DISPLAY OPTIONS
 ***********************************************/
void displayOptions()
{
	cout << "\td .. Display the list of messages\n";
	cout << "\ts .. Show one message\n";
	cout << "\ta .. Add a new message\n";
	cout << "\tu .. Update an existing message\n";
	cout << "\tr .. Delete an existing message\n";
	cout << "\tt .. Run Test Caes\n";
	cout << "\to .. Display this list of options\n";
	cout << "\tl .. Log out\n";
}

/************************************************
 * SIMPLE PROMPT
 ************************************************/
string simplePrompt(const char* prompt)
{
	string input;
	cout << prompt;
	cin >> input;
	return input;
}

/***************************************************
 * GET EXPECTED READ COUNT
 * Gets the expected amount of messages the user can read
 ***************************************************/
int getExpectedReadCount(Control userControl) 
{
	switch (userControl)
	{
	case 0:
		return 3;
	case 1:
		return 6;
	case 2:
		return 8;
	case 3:
		return 10;
	default:
		cout << "Unknown Control: '" << userControl << "'\n";
		break;
	}
	return 0;
}

/***************************************************
 * GET EXPECTED WRITE COUNT
 * Gets the expected amount of messages the user write
 ***************************************************/
int getExpectedWriteCount(Control userControl) 
{
	switch (userControl)
	{
	case 0:
		return 10;
	case 1:
		return 7;
	case 2:
		return 4;
	case 3:
		return 2;
	default:
		cout << "Unknown Control: '" << userControl << "'\n";
		break;
	}
	return 0;
}

/***************************************************
 * TEST FUNCTION
 * Tese all of the subject and asset control levels.
 ***************************************************/
void testFunction(string userName) 
{
	Messages messages(FILE_NAME);

	// Arrange
	vector<string> names;
	names.push_back("SeamanSam");
	names.push_back("SeamanSue");
	names.push_back("SeamanSly");
	names.push_back(userName);
	names.push_back("CaptainCharlie");
	names.push_back("AdmiralAbe");

	int readCount = 0;
	int writeCount = 0;
	string password = "password";
	for (vector <string> ::iterator it = names.begin();
		it != names.end();
		++it) 
	{
		Interact interact(*it, password, messages);

		readCount = interact.testControls(1); // READ TEST = 1
		writeCount = interact.testControls(2); // WRITE TEST = 2

		int expectedReadCount = getExpectedReadCount(interact.getControlLevel());
		int expectedWriteCount = getExpectedWriteCount(interact.getControlLevel());


		//Assert
		assert(expectedReadCount == readCount);
		assert(expectedWriteCount == writeCount);
		cout << "User Name: " << *it << " Control Level: " << interact.getControlLevel() << endl;
		cout << *it << " Should be able to read " << expectedReadCount << " Messages and can actually read " << readCount << " Messages" << endl;
		cout << *it << " Should be able to write to " << expectedWriteCount << " Messages and can actually write to " << writeCount << " Messages" << endl;
		cout << "-------------------------------------------------------------------\n\n";

	}

}

/***************************************************
 * SESSION
 * One login session
 **************************************************/
void session(Messages& messages)
{
	displayUsers();
	string userName = simplePrompt("What is your username? ");
	string password = simplePrompt("What is your password? ");

	// start a session
	Interact interact(userName, password, messages);
	cout << "Welcome, " << userName << " please select an option:\n";
	displayOptions();

	int count = interact.testControls(1);
	// inteact loop
	while (true)
	{
		char option;
		cout << "\n<" << userName << "> ";
		cin >> option;
		cin.ignore();

		switch (option)
		{
		case 'o':
			displayOptions();
			break;
		case 'd':
			interact.display();
			break;
		case 's':
			interact.show();
			break;
		case 'a':
			interact.add();
			break;
		case 'u':
			interact.update();
			break;
		case 't':
			testFunction(userName);
			break;
		case 'r':
			interact.remove();
			break;
		case 'l':
			cout << "Goodbye, " << userName << ".\n";
			return;

		default:
			cout << "Unknown option: '" << option << "'\n";
			break;
		}
	}
}

/***************************************************
 * MAIN
 * Where it all begins and where it all ends
 ***************************************************/
int main()
{
	Messages messages(FILE_NAME);

	bool done;
	do
	{
		session(messages);

		char selection;
		cout << "Will another user be logging in? (y/n) ";
		cin >> selection;
		cin.ignore();
		done = (selection == 'y' || selection == 'Y') ? false : true;
	} while (!done);

	return 0;
}
