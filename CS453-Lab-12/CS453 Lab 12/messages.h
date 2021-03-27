/***********************************************************************
 * COMPONENT:
 *    MESSAGES
 * Author:
 *    Br. Helfrich, <your name here if you made a change>
 * Summary:
 *    This class stores the notion of a collection of messages
 ************************************************************************/

#pragma once

#include <string>     // for convenience
#include <list>       // to store the messages
#include "control.h"  // all the Bell-LaPadula code
#include "message.h"  // all the code about a single message

 /***********************************************
  * MESSAGES
  * The collection of high-tech messages
  ***********************************************/
class Messages
{
public:
	// constructor: read a file to fill the messages
	Messages(const char* fileName) { readMessages(fileName); }

	//get the number of messages (TESTING)
	int getMessageCount(int testType, Control userControl);

	// display the list of messages
	void display(Control userControl);

	// show a single message
	void show(int id, Control userControl);

	// update one single message
	void update(int id,
		const std::string& text, Control userControl);

	// remove a single message
	void remove(int id, Control userControl);

	// add a new message
	void add(const std::string& text,
		const std::string& author,
		const std::string& date, Control userControl);


private:
	// the list of messages
	std::list <Message> messages;

	// read the messages from a file
	void readMessages(const char* fileName);
};
