#include <string>
#include <iostream>
#include "keyboardNinja.h"
#include <filesystem>

int main()
{
	std::cout << std::filesystem::current_path();
    setlocale(LC_ALL, "Russian");
    while (true) {
        std::string path = "./src/txt/" + fileChoice();
        char choice = run(path);
        system("clear");

        if (choice == 'q' || choice == 'Q') {
            exit(0);
        }
    }
}
