#define CTEST_MAIN
#ifndef CTEST_IMPL_NAME
#define CTEST_IMPL_NAME(name) ctest_##name

#include "ctest/ctest.h"
#include "../src/keyboardNinja.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
CTEST(remove_extra_spaces_suite, remove_extra_spaces_test) {
    // Given
    const string input = "   Hello,    world!  ";
    const string expected = "Hello, world!";

    // When
    const string result = removeExtraSpaces(input);

    // Then
    ASSERT_TRUE(expected == result);
}

CTEST(fileChoice_test, test_option_0)
{
    // Set input stream
    const char* input = "0\n";
    const int inputLength = strlen(input);

    fwrite(input, sizeof(char), inputLength, stdin);
    fflush(stdin);

    // Capture the output
    std::string output;
    FILE* pipe = popen("./bin/program", "r");
    if (pipe)
    {
        char buffer[128];
        while (!feof(pipe))
        {
            if (fgets(buffer, 128, pipe) != NULL)
                output += buffer;
        }
        pclose(pipe);
    }

    // Restore the input stream
    fclose(stdin);
    freopen("/dev/tty", "r", stdin);

    // Assertions
    ASSERT_STR(output.c_str(), "The quick fox.txt");
}

CTEST(fileChoice_test, test_option_3)
{
    // Set input stream
    const char* input = "3\n";
    const int inputLength = strlen(input);

    fwrite(input, sizeof(char), inputLength, stdin);
    fflush(stdin);

    // Capture the output
    std::string output;
    FILE* pipe = popen("./bin/program", "r");
    if (pipe)
    {
        char buffer[128];
        while (!feof(pipe))
        {
            if (fgets(buffer, 128, pipe) != NULL)
                output += buffer;
        }
        pclose(pipe);
    }

    // Restore the input stream
    fclose(stdin);
    freopen("/dev/tty", "r", stdin);

    // Assertions
    ASSERT_STR(output.c_str(), "Custom.txt");
}

CTEST(fileCheck_test, test_valid_file)
{
    // Create a temporary file
    std::ofstream file("temp.txt");
    file << "Hello, world!";
    file.close();

    // Test the function with the temporary file
    bool result = fileCheck("temp.txt");

    // Assertions
    ASSERT_TRUE(result);

    // Clean up the temporary file
    std::remove("temp.txt");
}



CTEST(fileBrowser_test, test_invalid_index)
{
    // Set input stream
    const char* input = "10\n";
    const int inputLength = strlen(input);

    fwrite(input, sizeof(char), inputLength, stdin);
    fflush(stdin);

    // Capture the output
    std::ostringstream outputStream;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(outputStream.rdbuf());

    // Run the function
    std::string result = fileBrowser();

    // Restore the input stream
    fclose(stdin);
    freopen("/dev/tty", "r", stdin);
    std::cout.rdbuf(oldCoutBuffer);

    // Assertions
    ASSERT_STR(outputStream.str().c_str(), "Неверный индекс файла!\n");
}

CTEST(fileCheck_test, test_invalid_file)
{
    // Test the function with a non-existent file
    bool result = fileCheck("nonexistent.txt");

    // Assertions
    ASSERT_FALSE(result);
}

CTEST(highlightWords_test, test_highlight_words)
{
    // Input strings
    std::string str1 = "The quick brown fox jumps over the lazy dog.";
    std::string str2 = "quick fox dog";

    // Expected result
    std::string expected = "\033[32mThe\033[0m \033[32mquick\033[0m \033[31mbrown\033[0m \033[32mfox\033[0m \033[31mjumps\033[0m \033[31mover\033[0m \033[31mthe\033[0m \033[32mlazy\033[0m \033[32mdog.\033[0m\n";

    // Call the function
    std::string result = highlightWords(str1, str2);

    // Assertion
    ASSERT_STR(expected.c_str(), result.c_str());
}

CTEST(getHighlightedText_test, test_get_highlighted_text)
{
    // Input text with color codes
    std::string text = "\033[32mThe\033[0m \033[31mquick\033[0m brown \033[32mfox\033[0m jumps over the \033[31mlazy\033[0m dog.";

    // Expected result
    std::string expected = "The quick brown fox jumps over the lazy dog.";

    // Call the function
    std::string result = getHighlightedText(text);

    // Assertion
    ASSERT_STR(expected.c_str(), result.c_str());
}

CTEST(run_test, test_run_function)
{
    // Test input file path
    std::string filePath = "test.txt";

    // Call the function
    char result = run(filePath);

    // Assertions
    ASSERT_TRUE(result == 'q');

}

#endif // CTEST_H_INCLUDED