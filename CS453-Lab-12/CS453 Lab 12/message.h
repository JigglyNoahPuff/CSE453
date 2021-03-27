/***********************************************************************
* COMPONENT:
*    MESSAGE
* Author:
*    Br. Helfrich, <your name here if you made a change>
* Summary:
*    This class stores the notion of a message
************************************************************************/

#pragma once

#include <string>     // for convenience
#include "control.h"  // for all the control stuff


/**************************************************
 * MESSAGE
 * one message to be displayed to the user or not
 **************************************************/
class Message
{
public:
	// default constructor for an empty message
	Message();

	// the most commonly used constructor: create a message
	Message(const std::string& text,
		const std::string& author,
		const std::string& date, Control controlLevel);

	// determine the unique ID of this message
	int getID() const { return id; }

	// determine if the user can read message
	bool canRead(Control userControl);

	// determine if the user can write message
	bool canWrite(Control userControl);
	// display the properties but not content of the message
	void displayProperties(Control userControl);

	// display the content of the message
	void displayText(Control userControl);

	// update the text componnet of the message
	void updateText(const std::string& newText, Control userControl);

	// clear out a message
	void clear(Control userControl);


private:
	int id;                   // the unique ID of this message
	static int idNext;        // the id of the next message created
	bool empty;               // is this message empty / cleared?
	Control control;          // Bell Lapudula control level
	std::string text;         // the textual content of this message
	std::string author;       // the author of this message
	std::string date;         // the date this message was created

	bool verifyRead(Control userControl);
	bool verifyWrite(Control userControl);
};
