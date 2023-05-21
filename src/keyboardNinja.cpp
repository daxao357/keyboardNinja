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

    int fileCount = 0; // Переменная для подсчета количества файлов

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            ++fileCount;
        }
    }

    std::cout << "\n\tAmount of files: " << fileCount << std::endl;

    int fileNumber = 0; // Переменная для нумерации файлов
    vector<string> fileNames;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::cout << "File " << fileNumber << ": " << entry.path().filename().string() << std::endl;
            fileNames.push_back(entry.path().filename().string());
            ++fileNumber;
        }
    }
    cout << endl << "What file do you choose?" << endl;
    size_t selectedFileIndex;
    cin >> selectedFileIndex;
    if (selectedFileIndex < fileNames.size()) {
        return fileNames[selectedFileIndex];
    }
    else {
        cout << "Неверный индекс файла!" << std::endl;
    }
    return " ";
}

string fileChoice() {
    cout << "\n\tWelcome to the keyboardNinja!\nWrite a digit to select one of the default texts,\nor browse your files!\n\n";
    cout << "0. The quick fox" << endl;
    cout << "1. Silent stars" << endl;
    cout << "2. A journey" << endl;
    cout << "3. Choose your own!" << endl;
    int number;
    cout << "\nYour choice: ";
    cin >> number;
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

    //result += "String 1: " + str1 + "\n";
    //result += "String 2: " + str2 + "\n";

    //result += "Highlighted words and punctuation:\n";

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
            if (word1 != " ") {
                c++;
            }

        }
        else {
            result += "\033[31m" + word1 + "\033[0m";
            if (word1 != " ") {
                n++;
            }
        }
    }
    result += "\n";

    return result;
}

#include <chrono>
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
        while (!(fin.eof())) {
            system("clear");

            getline(fin, line);
            cout << line << endl;

            getline(cin >> ws, urline);

            urline = removeExtraSpaces(urline);
            text.push_back(urline);
            string result = highlightWords(line, urline);
            string finalResult = getHighlightedText(result);
            finalResults.push_back(finalResult);
            numberOfCharacters += urline.size();
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

        cout << "Correct: " << c << endl;
        cout << "Incorrect: " << n << endl;
        if (c == 0 && n == 0) {
            cout << "Percent of correct elements: N/A" << endl;
        }
        else if (n == 0 && c != 0) {
            cout << "Percent of correct elements: 100%! Grats!" << endl;
        }
        else {
            cout << "Percent of correct elements: " << fixed << std::setprecision(2) << static_cast<double>(c) / (c + n) * 100.0 << "%" << endl;
        }
        double roundedSeconds = round(seconds * 100) / 100;
        cout << "Time: " << fixed << std::setprecision(2) << roundedSeconds << " seconds" << endl;
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
