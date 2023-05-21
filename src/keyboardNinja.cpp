#include <iostream>
#include <fstream>
#include <cstdlib>
#include <regex>
#include <vector>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <cmath>
#include "keyboardNinja.h"
#include <limits> // Для использования numeric_limits

using namespace std;
namespace fs = filesystem;

bool fileCheck(string path) { // Проверка открытия файла
    ifstream fin;
    fin.exceptions(ifstream::badbit | ifstream::failbit);
    try {
        fin.open(path);
    }
    catch (const ifstream::failure& ex) {
        cout << ex.what();
        cout << "File opening error";
        return false;
    }
    fin.close();
    return true;
}

string fileBrowser() {
    system("clear");

    string folderPath = "./src/txt";

    int fileCount = 0;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            ++fileCount;
        }
    }

    cout << "\n\tAmount of files: " << fileCount << endl;

    int fileNumber = 0;
    vector<string> fileNames;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            cout << "File " << fileNumber << ": " << entry.path().filename().string() << endl;
            fileNames.push_back(entry.path().filename().string());
            ++fileNumber;
        }
    }

    cout << "\nWhat file do you choose?" << endl;

    int selectedFileIndex;

    while (true) {
        if (cin >> selectedFileIndex && selectedFileIndex >= 0 && selectedFileIndex < static_cast<int>(fileNames.size())) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка символа новой строки
            return fileNames[selectedFileIndex];
        } else {
            cout << "Invalid input or file index. Please enter a valid file index: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка оставшихся символов во входном потоке
        }
    }
}

string fileChoice() {
    cout << "\n\tWelcome to the keyboardNinja!\nWrite a digit to select one of the default texts,\nor browse your files!\n\n";
    cout << "0. The quick fox" << endl;
    cout << "1. Silent stars" << endl;
    cout << "2. A journey" << endl;
    cout << "3. Choose your own!" << endl;

    int number;
    cout << "\nYour choice: ";

    // Проверка, что введено корректное число
    do {
        if (!(cin >> number)) {
            cout << "Invalid input. Please enter a valid number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else if (number < 0 || number > 3) {
            cout << "Invalid choice. Please enter a number from 0 to 3: ";
        }
    } while (number < 0 || number > 3);

    string path;
    switch (number) {
        case 0:
            path = "The quick fox.txt";
            break;
        case 1:
            path = "Silent stars.txt";
            break;
        case 2:
            path = "A journey.txt";
            break;
        case 3:
            return fileBrowser();
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return path;
}

string removeExtraSpaces(const string& str) { // Для сторонних файлов и строк пользователя
    string result = str;

    // Удаление двух и более пробелов
    regex pattern("\\s{2,}");
    result = regex_replace(result, pattern, " ");

    // Удаление пробелов перед знаками пунктуации
    regex punctuation("\\s+([.,!?;:])");
    result = regex_replace(result, punctuation, "$1");

    // Удаление пробелов в начале и в конце строки
    result = regex_replace(result, regex("^\\s+|\\s+$"), "");

    return result;
}

vector<string> splitString(const string& str) {
    vector<string> words;
    string word;
    for (char c : str) {
        if (isalpha(c) || isdigit(c)) {
            word += c;
        }
        else {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
            string punctuation;
            punctuation += c;
            words.push_back(punctuation);
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}

string getHighlightedText(const string& text) {
    string highlightedText;
    string currentColorCode;

    for (size_t i = 0; i < text.length(); ++i) {
        if (text[i] == '\033') {
            size_t colorCodeEnd = text.find('m', i);
            if (colorCodeEnd != string::npos) {
                string colorCode = text.substr(i, colorCodeEnd - i + 1);
                if (colorCode != currentColorCode) {
                    highlightedText += colorCode;
                    currentColorCode = colorCode;
                }
                i = colorCodeEnd;
            }
        }
        else {
            highlightedText += text[i];
        }
    }

    return highlightedText;
}

int c = 0, n = 0;
string highlightWords(const string& str1, const string& str2) {
    vector<string> words1 = splitString(str1);
    vector<string> words2 = splitString(str2);

    string result;

    for (size_t i = 0; i < words1.size(); ++i) {
        const string& word1 = words1[i];
        bool found = false;
        for (const string& word2 : words2) {
            if (word1 == word2) {
                found = true;
                break;
            }
        }

        if (found) {
            result += "\033[32m" + word1 + "\033[0m";
            if (!word1.empty() && word1 != " " && word1 != "\n") {
                c++;
            }
        }
        else {
            result += "\033[31m" + word1 + "\033[0m";
            if (!word1.empty() && word1 != " " && word1 != "\n") {
                n++;
            }
        }
    }
    result += "\n";

    return result;
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    if (first == string::npos || last == string::npos) {
        return "";
    }
    return str.substr(first, last - first + 1);
}

char run(string path) {
    c = 0; n = 0;
    double numberOfCharacters = 0;
    setlocale(LC_ALL, "Russian");
    system("clear");

    if (fileCheck(path)) {
        ifstream fin;
        fin.open(path);
        string line, urline;
        vector<string> hlines, finalResults, text;
        chrono::steady_clock::time_point startTime, endTime;
        startTime = chrono::steady_clock::now();
        
        size_t lineCount = 0;  // Переменная для подсчета строк в файле

        while (getline(fin, line)) {
            system("clear");

            cout << line << endl;

            getline(cin >> ws, urline);
            urline = removeExtraSpaces(urline);
            urline = trim(urline);
            text.push_back(urline);
            string result = highlightWords(line, urline);
            string finalResult = getHighlightedText(result);
            finalResults.push_back(finalResult);
            numberOfCharacters += urline.size();
            lineCount++;  // Увеличиваем счетчик строк
        }
        
        endTime = chrono::steady_clock::now();
        system("clear");

        // Вывод содержимого finalResults построчно
        size_t maxLength = max(finalResults.size(), text.size());

        for (size_t i = 0; i < maxLength; ++i) {
            if (i < finalResults.size()) {
                cout << finalResults[i];
            }
            if (i < text.size()) {
                cout << "\t" << text[i];
            }
            cout << endl;

            if (i == finalResults.size() - 1) {
                cout << endl;
            }
        }
        
        chrono::duration<double> elapsedTime = endTime - startTime;
        double seconds = elapsedTime.count();
		n = n -  lineCount + 1;
        cout << "Correct: " << c << endl;
        cout << "Incorrect: " << n << endl;
        if (c == 0 && n == 0) {
            cout << "Percent of correct elements: N/A" << endl;
        } else if (n == 0 && c != 0) {
            cout << "Percent of correct elements: 100%! Grats!" << endl;
        } else {
            cout << "Percent of correct elements: " << fixed << setprecision(2) << static_cast<double>(c) / (c + n) * 100.0 << "%" << endl;
        }
        
        double roundedSeconds = round(seconds * 100) / 100;
        cout << "Time: " << fixed << setprecision(2) << roundedSeconds << " seconds" << endl;
        double typingSpeed = (static_cast<double>(numberOfCharacters) / roundedSeconds) * 60;
        cout << "Typing Speed: " << fixed << setprecision(2) << typingSpeed << " characters per minute" << endl;
        
        cout << "\nPress 'q' to exit or any other key to continue: ";
        char choice;
        cin >> choice;
        fin.close();
        return choice; // Возвращаем правильную переменную choice
    }
    return ' '; // Возвращаем пустой символ в случае ошибки чтения файла
}
