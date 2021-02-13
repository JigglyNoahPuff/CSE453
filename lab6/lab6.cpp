#include <iostream>
#include <string>
#include <regex>
#include<vector>  

using namespace std;
using std::cout;
using std::cin;

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
void RunTestCases();
string sanitizeInput(string input);

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
    login_creds test = { "logan", "pwd" };
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
    login_creds creds = GetLoginCreds();
    TestQueryWeak(GenerateQuery(creds)) ?
        cout << "Test passed, valid credentials! \n"
        :
        cout << "Test failed, invalid credentials. \nSanitized Username: " << sanitizeInput(creds.username) << "\nSanitized Username: " << sanitizeInput(creds.password) << '\n';
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
        cout << "Test failed, invalid credentials. \nSanitized Username: " << sanitizeInput(creds.username) << "\nSanitized Username: " << sanitizeInput(creds.password) << '\n';
    }
}

// Runs test cases of different sets of usernames and passwords
void RunTestCases() {
    string input;
    login_creds creds;
    login_creds test = { "logan", "pwd" };
    vector<login_creds> validVector;
    vector<login_creds> tautologyVector;
    vector<login_creds> unionVector;
    vector<login_creds> extraStatementVector;
    vector<login_creds> commentVector;
    
    cout << "Running Test Program\n";

    // Ask which tests to run
    cout << "Which test would you like to run?\n\tOptions:\n\t\tValid\n\t\tTautology\n\t\tUnion\n\t\tAdditional Statement\n\t\tComment\n\t\tAll\n";
    getline(cin, input);
    cin.clear();

    if ((input == "Valid") || (input == "All" )) {
        // Valid Input Tests
        validVector.push_back({"username", "guessMyPassword"});
        validVector.push_back({"Jigglyname", "Jigglypassword!"});
        validVector.push_back({"Tyler", "DeFreitas"});
        validVector.push_back({"__sqdwrd__", "c1@RIne7"});
        validVector.push_back({"FireFrog22", "1stick0fgum"});
        validVector.push_back({"secureUser", "c2VjdXJlUGFzc3dvcmQ"});

        
        cout << "\nValid Input Tests: \n";

        for (int i = 0; i < validVector.size(); i++) {
            creds = validVector[i];
            cout << "Test " << i + 1 << "\n";
            cout << "\tUsername: " << creds.username << "\n\tPassword: " << creds.password << "\n";
            if (TestQueryStrong(creds)) {
                cout << "\tTest passed, valid credentials! \n";
            }
            else {
                TestQueryWeak(GenerateQuery(creds));
                cout << "\tTest failed, invalid credentials. \n\tSanitized Username: " << sanitizeInput(creds.username) << "\n\tSanitized Username: " << sanitizeInput(creds.password) << '\n';
            }
        }
    }

    if ((input == "Tautology") || (input == "All")) {
        // Tautology Tests
        tautologyVector.push_back({ "username", "whatever' OR 'a'='a" });
        tautologyVector.push_back({ "Jigglyname", "Jigglypassword!' OR 1=1" });
        tautologyVector.push_back({ "spngbb", "qwerty' or 'a'='a';" });
        tautologyVector.push_back({ "Tyler", "DeFreitas" });
        tautologyVector.push_back({ "JSmith", "qwerty' or 'a'='a';" });
        tautologyVector.push_back({ "hackerlvl1", "coolio\' OR 1=1; --" });

        cout << "\nTautology Tests: \n";

        for (int i = 0; i < tautologyVector.size(); i++) {
            creds = tautologyVector[i];
            cout << "Test " << i + 1 << "\n";
            cout << "\tUsername: " << creds.username << "\n\tPassword: " << creds.password << "\n";
            if (TestQueryStrong(creds)) {
                cout << "\tTest passed, valid credentials! \n";
            }
            else {
                TestQueryWeak(GenerateQuery(creds));
                cout << "\tTest failed, invalid credentials. \n\tSanitized Username: " << sanitizeInput(creds.username) << "\n\tSanitized Username: " << sanitizeInput(creds.password) << '\n';
            }
        }
    }

    if ((input == "Union") || (input == "All")) {
        // Union Tests
        unionVector.push_back({ "username", "password' UNION SELECT * FROM USERS" });
        unionVector.push_back({ "Jigglyname", "Jigglypassword' UNION SELECT * FROM passwords!" });
        unionVector.push_back({ "Tyler", "DeFreitas" });
        unionVector.push_back({ "mrkrbs' FULL OUTER JOIN confidential_info; --", "qwerty" });
        unionVector.push_back({ "FireFrog22", "fakePass'  UNION SELECT * FROM users" });
        unionVector.push_back({ "get", "hacked\' UNION SELECT * FROM users; --" });

        cout << "\nUnion Tests: \n";

        for (int i = 0; i < unionVector.size(); i++) {
            creds = unionVector[i];
            cout << "Test " << i + 1 << "\n";
            cout << "\tUsername: " << creds.username << "\n\tPassword: " << creds.password << "\n";
            if (TestQueryStrong(creds)) {
                cout << "\tTest passed, valid credentials! \n";
            }
            else {
                TestQueryWeak(GenerateQuery(creds));
                cout << "\tTest failed, invalid credentials. \n\tSanitized Username: " << sanitizeInput(creds.username) << "\n\tSanitized Username: " << sanitizeInput(creds.password) << '\n';
            }
        }
    }

    if ((input == "Additional Statement") || (input == "All")) {
        // Additional Statement Tests
        extraStatementVector.push_back({ "username", "something'; INSERT INTO products (product_id, product_name, price) VALUES (nextval(product_sequence), Awesome Sauce', '1.00');, " });
        extraStatementVector.push_back({ "Jigglyname", "Jigglypassword!'; INSERT INTO passwords (Jigglyname, JigglyPassword);" });
        extraStatementVector.push_back({ "Tyler", "" });
        extraStatementVector.push_back({ "mrkrbs' FULL OUTER JOIN confidential_info; --", "qwerty" });
        extraStatementVector.push_back({ "FireFrog22", "fakePass'  UNION SELECT * FROM users" });
        extraStatementVector.push_back({ "get", "hacked\' UNION SELECT * FROM users; --" });

        cout << "\nAdditional Statement Tests: \n";

        for (int i = 0; i < extraStatementVector.size(); i++) {
            creds = extraStatementVector[i];
            cout << "Test " << i + 1 << "\n";
            cout << "\tUsername: " << creds.username << "\n\tPassword: " << creds.password << "\n";
            if (TestQueryStrong(creds)) {
                cout << "\tTest passed, valid credentials! \n";
            }
            else {
                TestQueryWeak(GenerateQuery(creds));
                cout << "\tTest failed, invalid credentials. \n\tSanitized Username: " << sanitizeInput(creds.username) << "\n\tSanitized Username: " << sanitizeInput(creds.password) << '\n';
            }
        }
    }

    if ((input == "Comment") || (input == "All")) {
        // Comment Tests aka // Tests
        commentVector.push_back({ "something' OR USERNAME LIKE 'ryan%';--", "password" });
        commentVector.push_back({ "Jigglyname'; /*", "Jigglypassword!" });
        commentVector.push_back({ "Tyler", "DeFreitas" });
        commentVector.push_back({ "sndychks'; /*", "pwd_removed" });
        commentVector.push_back({ "admin\'; --", "wrongPass" });
        commentVector.push_back({ "admin\'; --", "admin" });

        cout << "\nComment Tests: \n";

        for (int i = 0; i < commentVector.size(); i++) {
            creds = commentVector[i];
            cout << "Test " << i + 1 << "\n";
            cout << "\tUsername: " << creds.username << "\n\tPassword: " << creds.password << "\n";
            if (TestQueryStrong(creds)) {
                cout << "\tTest passed, valid credentials! \n";
            }
            else {
                TestQueryWeak(GenerateQuery(creds));
                cout << "\tTest failed, invalid credentials. \n\tSanitized Username: " << sanitizeInput(creds.username) << "\n\tSanitized Username: " << sanitizeInput(creds.password) << '\n';
            }
        }
    }

    return;
}

// Sanitizes the input string if any of the tests detect an error
string sanitizeInput(string input) {
    string upperInput = input;
    for (int i = 0; i < input.length(); i++) {
        upperInput[i] = toupper(upperInput[i]);
    }

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

    while (input.find("/") != -1) {
        input.erase(input.find("/"), 1);
        upperInput.erase(upperInput.find("/"), 1);
    }

    while (input.find("*") != -1) {
        input.erase(input.find("*"), 1);
        upperInput.erase(upperInput.find("*"), 1);
    }

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
            << "To run test cases: test \n"
            << "To exit program: exit \n"
            << " >>> ";
        cin >> input;
        cin.ignore();

        if (input == "strong")
            RunStrongProgram();
        else if (input == "weak")
            RunWeakProgram();
        else if (input == "test")
            RunTestCases();
        else if (input == "exit")
            cout << "Goodbye\n";
    }

    return 0;
}
