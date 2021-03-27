/***********************************************************************
 * COMPONENT:
 *    MESSAGE
 * Author:
 *    Br. Helfrich, <your name here if you made a change>
 * Summary:
 *    This class stores the notion of a message
 ************************************************************************/

#include <iostream>   // standard input and output
#include <cassert>    // because I am paraniod
#include "message.h"  // for the header file
#include "interact.h"
using namespace std;

/**************************************************
 * MESSAGE DEFAULT CONSTRUCTOR
 * Set a message to empty
 **************************************************/
Message::Message()
{
	empty = true;
	text = "Empty";
	id = idNext++;
}


/**************************************************
 * MESSAGE NON-DEFAULT CONSTRUCTOR
 * Create a message and fill it
 **************************************************/
Message::Message(const string& text,
	const string& author,
	const string& date, Control controlLevel)
{
	this->text = text;
	this->author = author;
	this->date = date;
	this->id = idNext++;
	this->control = controlLevel;
	empty = false;
}

/**************************************************
 * MESSAGE :: DISPLAY PROPERTIES
 * Display the attributes/properties but not the
 * content of this message
 **************************************************/
void Message::displayProperties(Control userControl)
{
	if (verifyRead(userControl))
	{
		// skip this one if there is nothing to see
		if (empty)
			return;

		// display the message
		cout << "\t[" << id << "] ";
		cout << "Message from " << author << " at " << date;
		cout << endl;
	}
}

/**************************************************
 * MESSAGE :: DISPLAY TEXT
 * Display the contents or the text of the message
 **************************************************/
void Message::displayText(Control userControl)
{
	if (verifyRead(userControl))
	{
		cout << "\tMessage: "
			<< text
			<< endl;
	}
}

/**************************************************
 * MESSAGE :: UPDATE TEXT
 * Update the contents or text of the message
 **************************************************/
void Message::updateText(const string& newText, Control userControl)
{
	if (verifyWrite(userControl))
	{
		text = newText;
	}
}

/**************************************************
 * MESSAGE :: CLEAR
 * Delete the contents of a message and mark it as empty
 *************************************************/
void Message::clear(Control userControl)
{
	if (verifyRead(userControl))
	{
		text = "Empty";
		author.clear();
		date.clear();
		empty = true;
	}
}

/******************************************************
 * MESSAGE :: CAN READ
 * check if the user can read the message (TESTING)
 ******************************************************/
bool Message::canRead(Control userControl)
{
	return verifyRead(userControl);
}

/******************************************************
 * MESSAGE :: CAN WRITE
 * check if the user can write to the message (TESTING)
 *****************************************************/
bool Message::canWrite(Control userControl)
{
	return verifyWrite(userControl);
}

/**************************************************
 * MESSAGE :: VERIFY READ
 * check if the subject can read the object
 *************************************************/
bool Message::verifyRead(Control userControl)
{
	return userControl >= control;
}

/**************************************************
 * MESSAGE :: VERIFY WRITE
 * check if the subject can write to the object
 *************************************************/
bool Message::verifyWrite(Control userControl)
{
	return userControl <= control;
}

int Message::idNext = 100;