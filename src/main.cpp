#include <string>
#include <iostream>
#include "keyboardNinja.h"
#include <filesystem>

int main()
{
    setlocale(LC_ALL, "Russian");
    while (true) {
        string path = "./src/txt/" + fileChoice();
        char choice = run(path);
        system("clear");

        if (choice == 'q' || choice == 'Q') {
            exit(0);
        }
    }
}
