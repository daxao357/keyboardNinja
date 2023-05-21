#ifndef KEYBOARD_NINJA_H
#define KEYBOARD_NINJA_H

#include <string>
#include <vector>

bool fileCheck(string path);
string fileBrowser();
string fileChoice();
string removeExtraSpaces(const string& str);
vector<string> splitString(const string& str);
string getHighlightedText(const string& text);
string highlightWords(const string& str1, const string& str2);
char run(string path);

#endif // KEYBOARD_NINJA_H
