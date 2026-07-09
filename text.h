//
//  text.h
//  assignment4
//
//  Created by Maria Goncharuk on 08.07.2026.
//

#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "line.h"

class Text {
private:
    std::vector<Line*> lines;

public:
    void addLine(Line* line) {
        lines.push_back(line);
    }

    void printText() const {
        for (size_t i = 0; i < lines.size(); ++i) {
            std::cout << i + 1 << ". ";
            lines[i]->print();
        }
    }
    
    std::string serializeAll() const {
        std::string result = "";
        for (size_t i = 0; i < lines.size(); ++i) {
            result += lines[i]->serialize() + "\n";
        }
        return result;
    }
    
    void deserializeAll(const std::string& fullData) {
        clear();

        std::stringstream ss(fullData);
        std::string lineData;

        while (std::getline(ss, lineData)) {
            if (!lineData.empty() && lineData.back() == '\r') {
                lineData.pop_back();
            }

            if (lineData.empty()) {
                continue;
            }

            Line* newLine = nullptr;

            if (lineData.find("Text: ") == 0) {
                newLine = new TextLine();
            }
            else if (lineData.find("Contact - ") == 0) {
                newLine = new ContactLine();
            }
            else if (lineData.find("[ x ] ") == 0 || lineData.find("[   ] ") == 0) {
                newLine = new ChecklistLine();
            }

            if (newLine != nullptr) {
                newLine->deserialize(lineData);
                lines.push_back(newLine);
            }
        }
    }

    void clear() {
        for (size_t i = 0; i < lines.size(); ++i) {
            delete lines[i];
        }
        lines.clear();
    }
    
    
    bool toggleLineStatus(int index) {
        if (index >= 0 && index < lines.size()) {
            return lines[index]->toggleStatus();
        }
        return false;
    }

    bool editLineInsert(int index, int pos, const std::string& str) {
        if (index >= 0 && index < lines.size()) {
            return lines[index]->insertText(pos, str);
        }
        return false;
    }

    bool editLineDelete(int index, int pos, int count) {
        if (index >= 0 && index < lines.size()) {
            return lines[index]->deleteText(pos, count);
        }
        return false;
    }

    ~Text() {
        clear();
    }
};


class TabManager {
private:
    std::vector<Text*> tabs;
    int activeTabIndex;

public:
    TabManager() {
        activeTabIndex = -1;
    }
    
    int getActiveTabIndex() const {
        return activeTabIndex;
    }

    void createNewTab() {
        tabs.push_back(new Text());
        activeTabIndex = (int)tabs.size() - 1;
    }

    void switchTab(int index) {
        if (index >= 0 && index < tabs.size()) {
            activeTabIndex = index;
            std::cout << "Switched to Tab " << (index + 1) << std::endl;
        } else {
            std::cout << "Invalid tab index" << std::endl;
        }
    }

    Text* getActiveTab() {
        if (activeTabIndex >= 0 && activeTabIndex < tabs.size()) {
            return tabs[activeTabIndex];
        }
        return nullptr;
    }

    size_t getTabsCount() const {
        return tabs.size();
    }

    ~TabManager() {
        for (size_t i = 0; i < tabs.size(); ++i) {
            delete tabs[i];
        }
    }
};
