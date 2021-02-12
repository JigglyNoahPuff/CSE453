#include <iostream>
#include <string>
#include <regex>

using namespace std;

struct login_creds;
bool TestValidUsername(string);
bool TestValidPassword(string);
bool CaseInsensitiveSearch(string, string);
login_creds GetLoginCreds();
string GenerateQuery(login_creds);
bool TestTautologyWeak(string);
bool TestAdditionalStatementWeak(string);
bool TestUnionQueryWeak(string);
bool TestCommentWeak(string);
bool TestQueryWeak(string);
bool TestQueryStrong(login_creds);
void RunWeakProgram();
void RunStrongProgram();

// Simple struct to hold username and password
struct login_creds {
    string username;
    string password;
};

// Runs a regex expression to see if username conforms with an acceptable format
bool TestValidUsername(string str) {
    return regex_match(str, regex("[a-zA-Z0-9_]*"));
}

// Runs a regex expression to see if password conforms with an acceptable format
bool TestValidPassword(string str) {
    return regex_match(str, regex("[a-zA-Z0-9!@#%&*()$^_=+]*"));
}

// Returns true if a substring is found within a string, regardless of capitalization 
bool CaseInsensitiveSearch(string str, string searched_term) {

    for (int i = 0; i < str.length(); i++) {
        str[i] = toupper(str[i]);
    }

    for (int i = 0; i < searched_term.length(); i++) {
        searched_term[i] = toupper(searched_term[i]);
    }

    return (str.find(searched_term) == -1) ? false : true;

}

// Prompts user for username and password, returns login_creds
login_creds GetLoginCreds() {
    login_creds creds;
    login_creds test = {"logan", "pwd" };
    cout << "Type username: ";
    getline(cin, creds.username);
    cin.clear();
    cout << "Type password: ";
    getline(cin, creds.password);
    cin.clear();

    return creds;
}

// Receives login_creds as an argument, formats it into a SQL query to be processed
string GenerateQuery(login_creds creds) {
    return "SELECT * FROM users WHERE username='" + creds.username +
        "' AND password='" + creds.password + "';";
}

// Tests if a query contains an attempted Tautology attack
bool TestTautologyWeak(string query) {
    return (!CaseInsensitiveSearch(query, " or ")) ? false : true;
}

// Tests if a query contains an attempted Additional Statement attack
bool TestAdditionalStatementWeak(string query) {
    return (query.substr(0, query.length() - 2).find(';') == -1) ? false : true;
}

bool TestCommentWeak(string query) {
    return (query.find("--") == -1 and query.find("/*") == -1) ? false : true;
}

// Tests if a query contains a Union Query attack
bool TestUnionQueryWeak(string query) {
    return (!CaseInsensitiveSearch(query, " union ") && !CaseInsensitiveSearch(query, " join ")) 
        ? false : true;
}

// Tests the query (weakly) for any attempted SQL injection attacks
bool TestQueryWeak(string query) {
    bool result = true;

    if (TestTautologyWeak(query)) {
        cout << "Failed 'tautology' test \n";
        result = false;
    }

    if (TestUnionQueryWeak(query)) {
        cout << "Failed 'union query' test \n";
        result = false;
    }

    if (TestAdditionalStatementWeak(query)) {
        cout << "Failed 'additional statement' test \n";
        result = false;
    }

    if (TestCommentWeak(query)) {
        cout << "Failed 'comment' test \n";
        result = false;
    }

    return result;
}

// To test the query strongly, both the username and password are filtered through a regex expression.
// The regex expression is formatted so that any SQL injection attacks will be detected and rejected.
bool TestQueryStrong(login_creds creds) {
    return(TestValidUsername(creds.username) && TestValidPassword(creds.password));
}

// Start of weak test.
void RunWeakProgram() {
    TestQueryWeak(GenerateQuery(GetLoginCreds())) ? 
        cout << "Test passed, valid credentials! \n"
    :
        cout << "Test failed, invalid credentials. \n";
}

// To run the strong program, first it filters the query through TestQueryStrong. If that fails, it
// runs it through TestQueryWeak which will help to hone in on what the potential issues are.
void RunStrongProgram() {
    login_creds creds = GetLoginCreds();

    if (TestQueryStrong(creds)) {
        cout << "Test passed, valid credentials! \n";
    }
    else {
        TestQueryWeak(GenerateQuery(creds));
        cout << "Test failed, invalid credentials. \n";
    }
}


// Sanitizes the input string if any of the tests detect an error
string sanitizeInput(string input) {
    string upperInput = input;
  for (int i = 0; i < input.length(); i++) {
        upperInput[i] = toupper(upperInput[i]);
    }
    cout << upperInput << endl;
    cout << input << endl;
    
    while (upperInput.find(" ") != -1) {
        input.erase(upperInput.find(" "), 1);
        upperInput.erase(upperInput.find(" "), 1);
    }
    while (input.find(';') != -1) {
        input.erase(input.find(';'), 1);
        upperInput.erase(upperInput.find(';'), 1);
    } 
    
    while (input.find('-') != -1) {
        input.erase(input.find('-'), 1);
        upperInput.erase(upperInput.find('-'), 1);
    } 

    while (input.find("/*") != -1) {
        input.erase(input.find("/*"), 2);
        upperInput.erase(upperInput.find("/*"), 2);
    }
    // Removes instances of " OR " and " Union ", but removing whitespaces should
    // solve the threat that these words pose.
    /*if (upperInput.find(" OR ") != -1) {
        input.erase(upperInput.find(" OR "), 4);
        upperInput.erase(upperInput.find(" OR "), 4);
    }*/
    /*while (upperInput.find(" UNION ") != -1) {
        input.erase(upperInput.find(" UNION "), 7);
        upperInput.erase(upperInput.find(" UNION "), 7);
    }*/
    
    return input;
}

int main()
{
    string input = "";

    while (input != "exit") {
        cout << "\n *********** \n"
            << "Type one of the following commands: \n"
            << "To run strong test: strong \n"
            << "To run weak test: weak \n"
            << "To exit program: exit \n"
            << " >>> ";
        cin >> input;
        cin.ignore();

        if (input == "weak")
            RunWeakProgram();
        else if (input == "strong")
            RunStrongProgram();
        else if (input == "exit")
            cout << "Goodbye\n";
    }
}  
