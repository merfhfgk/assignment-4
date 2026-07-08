//
//  text.h
//  assignment4
//
//  Created by Maria Goncharuk on 08.07.2026.
//

#pragma once
#include <vector>
#include <string>
#include "Line.h"

class Text {
private:
    std::vector<Line*> lines;

public:
    void addLine(Line* line) {
        lines.push_back(line);
    }

    void printText() const {
        for (size_t i = 0; i < lines.size(); ++i) {
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

    void clear() {
        for (size_t i = 0; i < lines.size(); ++i) {
            delete lines[i];
        }
        lines.clear();
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

    void createNewTab() {
        tabs.push_back(new Text());
        activeTabIndex = tabs.size() - 1;
    }

    void switchTab(int index) {
        if (index >= 0 && index < tabs.size()) {
            activeTabIndex = index;
            std::cout << "Switched to Tab " << (index + 1) << std::endl;
        } else {
            std::cout << "Invalid tab index!" << std::endl;
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
