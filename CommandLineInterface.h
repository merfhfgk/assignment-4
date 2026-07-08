//
//  CommandLineInterface.h
//  assignment4
//
//  Created by Maria Goncharuk on 08.07.2026.
//

#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "text.h"
#include "Cipher.h"

class CommandLineInterface {
private:
    TabManager tabManager;

    void printMenu() const {
        std::cout << "\n=== TEXT EDITOR MENU ===" << std::endl;
        std::cout << "1. Add text" << std::endl;
        std::cout << "2. Add to checklist" << std::endl;
        std::cout << "3. Add contact" << std::endl;
        std::cout << "4. Print text" << std::endl;
        std::cout << "5. Create/switch between tabs" << std::endl;
        std::cout << "6. Encrypt and save to file" << std::endl;
        std::cout << "7. Decrype and load from file" << std::endl;
        std::cout << "0. Exit" << std::endl;
    }

    void clearInputBuffer() const {
        std::cin.clear();
        while (std::cin.get() != '\n') {
            continue;
        }
    }
    
    

public:
    CommandLineInterface() {
        tabManager.createNewTab();
    }

    void start() {
        int choice;
        bool running = true;
        printMenu();
        while (running) {
            std::cout << "\nChoose: "; 

            if (!(std::cin >> choice)) {
                clearInputBuffer();
                std::cout << "Unknown command, please enter a number" << std::endl;
                continue;
            }
            clearInputBuffer();

            Text* currentText = tabManager.getActiveTab();
            if (!currentText && choice >= 1 && choice <= 4) {
                std::cout << "No active tab" << std::endl;
                continue;
            }

            switch (choice) {
                case 1: {
                    std::string text;
                    std::cout << "Add text: ";
                    std::getline(std::cin, text);
                    currentText->addLine(new TextLine(text));
                    std::cout << "Text was added successfully" << std::endl;
                    break;
                }
                case 2: {
                    std::string item;
                    int status;
                    std::cout << "Add punkt to a checklist: ";
                    std::getline(std::cin, item);
                    std::cout << "Status (1 - done, 0 - not done yet): ";
                    std::cin >> status;
                    currentText->addLine(new ChecklistLine(item, status == 1));
                    std::cout << "Added to checklist successfully" << std::endl;
                    break;
                }
                case 3: {
                    std::string name, surname, email;
                    std::cout << "Enter name: ";
                    std::getline(std::cin, name);
                    std::cout << "Enter surname: ";
                    std::getline(std::cin, surname);
                    std::cout << "Enter e-mail: ";
                    std::getline(std::cin, email);
                    currentText->addLine(new ContactLine(name, surname, email));
                    std::cout << "Contact was added successfully" << std::endl;
                    break;
                }
                case 4: {
                    std::cout << "\nTab " << tabManager.getActiveTab() << std::endl;
                    currentText->printText();
                    break;
                }
                case 5: {
                    std::cout << "Tabs: " << tabManager.getTabsCount() << std::endl;
                    std::cout << "Enter 1 to create a new tab and 2 to switch: ";
                    int tabChoice;
                    std::cin >> tabChoice;
                    if (tabChoice == 1) {
                        tabManager.createNewTab();
                        std::cout << "New tab was created (and activated)" << std::endl;
                    } else if (tabChoice == 2) {
                        std::cout << "Number of tab (from 1 to " << tabManager.getTabsCount() << "): ";
                        int index;
                        std::cin >> index;
                        tabManager.switchTab(index - 1);
                    }
                    break;
                }
                case 6: {
                    std::string filename, key;
                    std::cout << "Enter the name of file: ";
                    std::getline(std::cin, filename);
                    std::cout << "key (for cipher): ";
                    std::getline(std::cin, key);

                    std::string serializedData = currentText->serializeAll();
                    
                    Cipher cipher("./libcipher.dylib");
                    std::string encryptedData = cipher.encrypt(serializedData, key);

                    if (encryptedData.empty()) {
                        std::cout << "Unexpected error with ciphering" << std::endl;
                        break;
                    }

                    std::ofstream outFile(filename, std::ios::binary);
                    if (outFile.is_open()) {
                        outFile << encryptedData;
                        outFile.close();
                        std::cout << "File was successfully ciphered and saved " << filename << "!" << std::endl;
                    } else {
                        std::cout << "Error with opening file" << std::endl;
                    }
                    break;
                }
                case 7: {
                    std::string filename, key;
                    std::cout << "Enter the name of file: ";
                    std::getline(std::cin, filename);
                    std::cout << "key (for cipher): ";
                    std::getline(std::cin, key);

                    std::ifstream inFile(filename, std::ios::binary);
                    if (!inFile.is_open()) {
                        std::cout << "Error with opening file" << std::endl;
                        break;
                    }
                    
                    std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
                    inFile.close();

                    Cipher cipher("./libcipher.dylib");
                    std::string decryptedData = cipher.decrypt(fileContent, key);

                    if (decryptedData.empty()) {
                        std::cout << "Could not descrypt or open file" << std::endl;
                        break;
                    }

                    currentText->clear();
                    
                    std::stringstream ss(decryptedData);
                    std::string lineData;
                    
                    while (std::getline(ss, lineData)) {
                        if (lineData.empty()) continue;

                        if (lineData.rfind("TEXT|", 0) == 0) {
                            TextLine* tl = new TextLine();
                            tl->deserialize(lineData);
                            currentText->addLine(tl);
                        }
                        else if (lineData.rfind("CHECKLIST|", 0) == 0) {
                            ChecklistLine* cl = new ChecklistLine();
                            cl->deserialize(lineData);
                            currentText->addLine(cl);
                        }
                        else if (lineData.rfind("CONTACT|", 0) == 0) {
                            ContactLine* conl = new ContactLine();
                            conl->deserialize(lineData);
                            currentText->addLine(conl);
                        }
                    }

                    std::cout << "Success!" << std::endl;
                    break;
                }
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Unknown command, try again" << std::endl;
                    break;
            }
        }
    }
};
