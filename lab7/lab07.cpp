#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
 
void one(long number);
void two(long number);
void pass() { std::cout << "You pass :)\n"; }
void fail() { std::cout << "You've failed :(\n"; }
const char* passMessage = ":)";
const char* failMessage = ":(";
 
void DisplaySegmentAddresses();
 
/**********************************************
 * MAIN : The top of the callstack.
 **********************************************/
int main()
{
    char text[8] = "*MAIN**";
    long number = 123456;
    void (*pointerFunction)() = fail;
    const char* message = failMessage;
 
    // Adding local variables
    long pointlessNumber = 56793157;
    char pointlessText[8] = " Okay! ";
 
    // display the initial values of the local variables
    std::cout << "main() : " << (void*)main << std::endl
              << "\ttext:             " << text << std::endl
              << "\tnumber:           " << number << std::endl
              << "\tmessage:          " << message << std::endl
              << "\tfunction pointer: ";
    pointerFunction();
 
    DisplaySegmentAddresses();
 
    // call the other functions
    one(number + 111111);     // 234567
 
    // display the new values of the local variables
    std::cout << "main() - after\n"
              << "\ttext:             " << text << std::endl
              << "\tnumber:           " << number << std::endl
              << "\tmessage:          " << message << std::endl
              << "\tfunction pointer: ";
    pointerFunction();
 
    return 0;
}
 
/************************************************
 * CONVERT TO STRING
 * Convert the data from p into a human-readable string
 * by removing all the unprintable characters and replacing
 * them with a dot
 ***********************************************/
std::string displayCharArray(const char* p)
{
    std::string output;
    for (int i = 0; i < 8; i++)
        output += std::string(" ") + (p[i] >= ' ' && p[i] <= 'z' ? p[i] : '.');
    return output;
}
 
/**********************************************
 * ONE : The next item on the call stack
 **********************************************/
void one(long number)               // 234567
{
    char text[8] = "**ONE**";
 
    std::cout << "one() : " << (void*)one << std::endl
              << "\tmessage: " << (void*)failMessage << std::endl
              << "\tfail():  " << (void*)fail << std::endl;
 
    two(number + 111111);    // 345678
}
 
/**********************************************
 * TWO : The bottom of the call stack
 **********************************************/
void two(long number)              // 345678
{
    // start your display of the stack from this point
    long bow = number + 111111;     // 456789
    char text[8] = "**TWO**";
    long* pLong = NULL;
    char* pChar = NULL;
 
    // header for our table. Use these setw() offsets in your table
    std::cout << '[' << std::setw(2) << 'i' << ']'
              << std::setw(15) << "address"
              << std::setw(20) << "hexadecimal"
              << std::setw(20) << "decimal"
              << std::setw(18) << "characters"
              << std::endl;
    std::cout << "----+"
              << "---------------+"
              << "-------------------+"
              << "-------------------+"
              << "-----------------+\n";
 
    for (long i = 24; i >= -24; i--)   // You may need to change 24 and -4 to another number
    {
        ////////////////////////////////////////////////
        // Insert code here to display the callstack
        std::cout << std::setw(4)  << i
                  << std::setw(16) << &bow + (i)
                  << std::setw(20) << std::hex << *(&bow + (i))
                  << std::setw(20) << std::dec << *(&bow + (i))
                  << std::setw(18) << displayCharArray((const char*)(&bow + (i))) 
                  << std::endl;
 
        //
        ////////////////////////////////////////////////
    }
 
    ////////////////////////////////////////////////
    // Insert code here to change the variables in main()
 
    // change text in main() to "*main**"
    pChar = (char*)&bow;
 
    while (std::string(pChar) != "*MAIN**")
    {
        pChar++;
    }
 
    assert(std::string(pChar) == "*MAIN**");
 
    pChar[0] = '*';
    pChar[1] = 'm';
    pChar[2] = 'a';
    pChar[3] = 'i';
    pChar[4] = 'n';
    pChar[5] = '*';
    pChar[6] = '*';
 
    // change number in main() to 654321
    pLong = (long*)&bow;
 
    while (*pLong != 123456)
    {
        pLong++;
    }
 
    assert(*pLong == 123456);
    *pLong = 654321;
 
    // change pointerFunction in main() to point to pass
    pLong = (long*)&bow;
 
    while (*pLong != (long)fail)
    {
        pLong++;
    }
 
    assert(*pLong == (long)fail);
    *pLong = (long)pass;
 
    // change message in main() to point to passMessage
    pLong = (long*)&bow;
 
    while (*pLong != (long)failMessage)
    {
        pLong++;
    }
 
    assert(*pLong == (long)failMessage);
    *pLong = (long)passMessage;
 
    //
    ////////////////////////////////////////////////
}
 
void DisplaySegmentAddresses() 
{
    struct test_struct 
    {
        int i;
        int ii;
    };
 
    test_struct* heap_struct = new test_struct();
 
    char* stack_ptr = NULL;
    test_struct** heap_ptr = &heap_struct;
    void (*code_ptr)() = &DisplaySegmentAddresses;
 
    std::cout << std::endl
              << "Stack Address: " << &stack_ptr << std::endl
              << "Heap Address: " << &(*(*heap_ptr)) << std::endl
              << "Code Segment Address: " << &(*code_ptr) << std::endl << std::endl;
 
}