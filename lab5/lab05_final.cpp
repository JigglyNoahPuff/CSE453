#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <cassert>
#include <assert.h>
const char SLASH = '/';
const std::string DOT_SLASH = "./";
const std::string DOT_DOT_SLASH = "../";
namespace fs = std::filesystem;


/* Replaces all instances of a certain character within a string */
std::string ReplaceAllSlashes(std::string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '\\') {
            str.replace(i, 1, "/");
        }
    }
    return str;
}


/* Intention of this function is to take a path(in the form of a string) and separate it into its
component commands.*/
std::vector<std::string> SplitPath(std::string str) {
    // path_elements holds the commands contained in the path
    std::vector<std::string> path_elements = std::vector<std::string>();
    int i = 0;
    for (int ii = 0; ii < str.length(); ii++) {
        if (str[ii] == SLASH || ii == str.length() - 1) {
            path_elements.push_back(str.substr(i, ii - i + 1));
            i = ii + 1;
        }
    }
    return path_elements;
}


/* SanitizePath takes all of the commands from path_elements and applies them to the current working
directory. It then returns the "clean" path that does not include any special commands. */
std::string SanitizePath(std::vector<std::string> path_elements) {
    /* In the interest of usability, all backslashes are to be replaced by forward slashes*/
    std::string clean_string = ReplaceAllSlashes(std::filesystem::current_path().string());
    clean_string.push_back(SLASH);
    // If the first character of the command is a slash, it starts the path from root.
    if (path_elements[0][0] == SLASH) {
        clean_string.erase(3, clean_string.size() - 1);
        path_elements.erase(path_elements.begin());
    }

    // This IF statemenet detects if the first command refers to a root partition, such as 'C:/'
    else if (path_elements[0].length() == 3 && path_elements[0][1] == ':' &&
        (((int)path_elements[0][0] >= (int)'a' && (int)path_elements[0][0] <= (int)'z') ||
            ((int)path_elements[0][0] >= (int)'A' && (int)path_elements[0][0] <= (int)'Z'))) {

        // If the letter is lowercase, convert to uppercase
        if (path_elements[0][0] >= 'a') {
            path_elements[0][0] -= 32;
        }
        // Set the filepath to start with the designated root partition
        clean_string = path_elements[0];
        path_elements.erase(path_elements.begin());
    }

    // This loop is designed to iterate through the various commands
    for (int i = 0; i < path_elements.size(); i++) {
        // Two dots and then a slash refers to going back to the parent directory.
        if (path_elements[i] == DOT_DOT_SLASH) {
            // This removes everything after the second-to-last slash
            clean_string.erase(clean_string.find_last_of(SLASH), clean_string.size() - 1);
            clean_string.erase(clean_string.find_last_of(SLASH) + 1, clean_string.size() - 1);
        }
        // A single dot and then a slash refers to the current directory. Do nothing.
        else if (path_elements[i] == DOT_SLASH) {
        }
        // Otherwise, add the current command to the path.
        else {
            clean_string.append(path_elements[i]);
        }
    }
    return clean_string;
}


bool PathsAreHomographs(std::string first_path, std::string second_path) {
    /* Replace backslashes with forward slashes */
    first_path = ReplaceAllSlashes(first_path);
    second_path = ReplaceAllSlashes(second_path);
    if (SanitizePath(SplitPath(first_path)) == SanitizePath(SplitPath(second_path))) {
        return true;
    }
    return false;
}


std::string getFilePath(std::string nth)
{
    std::string file_path;
    std::cout << "Enter the " << nth << " filepath: ";
    std::cin >> file_path;
    return file_path;
}


std::string mainProgram()
{
    std::string first_path = getFilePath("first");
    std::string second_path = getFilePath("second");
    return PathsAreHomographs(first_path, second_path) ?
        "The filepaths are homographs" :
        "The filepaths are not homographs";
}


void runHomographTests()
{
    std::string test1_path1 = "testing.txt";
    std::string test1_path2 = "./././././testing.txt";
    std::string test2_path1 = "/test/path/here/testing.txt";
    std::string test2_path2 = "/./test/path/here/testing.txt";

    std::string test3_path1 = "test/path/here/testing.txt";
    std::string test3_path2 = "test/../test/.././test/path/here/testing.txt";

    std::string test4_path1 = "C:/test.txt";
    std::string test4_path2 = "/test.txt";

    std::string test5_path1 = "C:/test.txt";
    std::string test5_path2 = "c:\\./test.txt";

    std::cout
        << "\nShould be homographs:\n"
        << test1_path1 << std::endl
        << test1_path2 << std::endl << std::endl;
    assert(PathsAreHomographs(test1_path1, test1_path2));
    std::cout
        << test2_path1 << std::endl
        << test2_path2 << std::endl << std::endl;
    assert(PathsAreHomographs(test2_path1, test2_path2));
    std::cout
        << test3_path1 << std::endl
        << test3_path2 << std::endl << std::endl;
    assert(PathsAreHomographs(test3_path1, test3_path2));
    std::cout
        << test4_path1 << std::endl
        << test4_path2 << std::endl << std::endl;
    assert(PathsAreHomographs(test4_path1, test4_path2));
    std::cout
        << test5_path1 << std::endl
        << test5_path2 << std::endl << std::endl;
    assert(PathsAreHomographs(test5_path1, test5_path2));
    std::cout << "\nAll Homograph Tests Passed.\n";
}


void runNonHomographTests()
{
    std::string test1_path1 = "testing.txt";
    std::string test1_path2 = "././../././testing.txt";

    std::string test2_path1 = "/test/path/here/testing.txt";
    std::string test2_path2 = "/../test/path/here/testing.txt";

    std::string test3_path1 = "test/path/here/testing.txt";
    std::string test3_path2 = "test/../test/././test/path/here/testing.txt";

    std::string test4_path1 = "test.txt";
    std::string test4_path2 = "C:/test.txt";

    std::cout
        << "\nShouldn't be homographs:\n"
        << test1_path1 << std::endl
        << test1_path2 << std::endl << std::endl;
    assert(!PathsAreHomographs(test1_path1, test1_path2));
    std::cout
        << test2_path1 << std::endl
        << test2_path2 << std::endl << std::endl;
    assert(!PathsAreHomographs(test2_path1, test2_path2));
    std::cout
        << test3_path1 << std::endl
        << test3_path2 << std::endl << std::endl;
    assert(!PathsAreHomographs(test3_path1, test3_path2));
    std::cout
        << test4_path1 << std::endl
        << test4_path2 << std::endl << std::endl;
    assert(!PathsAreHomographs(test4_path1, test4_path2));
    std::cout << "\nAll Non Homograph Tests Passed.\n";
}


void runAllTests()
{
    runHomographTests();
    runNonHomographTests();
}


int main()
{
    std::string option;
    while (option != "e")
    {
        std::cout
            << "\n===================\n"
            << "Options:\n"
            << "  r: Run\n"
            << "  t: Run Test Cases\n"
            << "  e: Exit\n"
            << "===================\n";
        std::cin >> option;
        if (option == "r")
            std::cout << mainProgram() << std::endl;
        else if (option == "t")
            runAllTests();
        else if (option == "e")
            return 0;
        else
            std::cout << "Invalid option\n\n";
    }
    return 0;
}
