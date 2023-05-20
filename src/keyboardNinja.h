#ifndef KEYBOARD_NINJA_H
#define KEYBOARD_NINJA_H

#include <string>
#include <vector>

bool fileCheck(std::string path);
std::string fileBrowser();
std::string fileChoice();
std::string removeExtraSpaces(const std::string& str);
std::vector<std::string> splitString(const std::string& str);
std::string getHighlightedText(const std::string& text);
std::string highlightWords(const std::string& str1, const std::string& str2);
char run(std::string path);

#endif // KEYBOARD_NINJA_H
