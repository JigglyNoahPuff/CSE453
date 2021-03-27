/***********************************************************************
 * COMPONENT:
 *    INTERACT
 * Author:
 *    Br. Helfrich, <your name here if you made a change>
 * Summary:
 *    This class allows one user to interact with the system
 ************************************************************************/

#pragma once

#include <string>     // for convenience
#include "messages.h" // to interact with the collection of messages
#include "control.h"  // all the Bell-LaPadula stuff

 /**************************************************************
  * USER
  * All the users currently in the system
  *************************************************************/
struct User
{
	const char* name;
	const char* password;
	Control control;
};

/****************************************************
 * INTERACT
 * One user interacting with the system
 ***************************************************/
class Interact
{
public:
	Interact(const std::string& userName,
		const std::string& password,
		Messages& messages);

	int testControls(int testType);

	// show a single message
	void show();

	// display the set of messages
	void display();

	// add a single message
	void add();

	// update a single message
	void update();

	// remove one message from the list
	void remove();

	// get the control level of the user for testing
	Control getControlLevel();

private:
	Messages* pMessages;
	std::string userName;

	// prompt for a line of input
	std::string promptForLine(const char* verb) const;

	// prompt for a message ID
	int promptForId(const char* verb) const;

	// authenticate the user;
	void authenticate(const std::string& userName,
		const std::string& password);

	// find the ID of a given user
	int idFromUser(const std::string& userName) const;

	Control userControl;
};

// display the set of users in the system
void displayUsers();
