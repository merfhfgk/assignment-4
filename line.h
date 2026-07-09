//
//  line.h
//  assignment4
//
//  Created by Maria Goncharuk on 08.07.2026.
//

#include <iostream>
#include <string>
#include <sstream>

class Line {
public:
    virtual void print() const = 0;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
    virtual bool toggleStatus() { return false; }
    virtual bool insertText(int pos, const std::string& str) { return false; }
    virtual bool deleteText(int pos, int count) { return false; }
        
    virtual ~Line() {}
};

class TextLine : public Line {
private:
    std::string text;

public:
    TextLine(const std::string& t = "") : text(t) {}

    void print() const override {
        std::cout << "Text: " << text << std::endl;
    }

    std::string serialize() const override {
        return "Text: " + text;
    }

    void deserialize(const std::string& data) override {
        if (data.find("Text: ") == 0) {
            text = data.substr(6);
        }
    }
    
    bool insertText(int pos, const std::string& str) override {
        if (pos >= 0 && pos <= text.length()) {
            text.insert(pos, str);
            return true;
        }
        return false;
    }
    
    bool deleteText(int pos, int count) override {
            if (pos >= 0 && pos < text.length()) {
                text.erase(pos, count);
                return true;
            }
            return false;
        }

};

class ChecklistLine : public Line {
private:
    std::string item;
    bool checked;

public:
    ChecklistLine() : item(""), checked(false) {}
    ChecklistLine(const std::string& i, bool c) : item(i), checked(c) {}
    
    void print() const override {
            std::cout << (checked ? "[X] " : "[ ] ") << item << std::endl;
    }

    std::string serialize() const override {
        return (checked ? "[ x ] " : "[   ] ") + item;
    }

    void deserialize(const std::string& data) override {
        if (data.find("[ x ] ") == 0) {
            checked = true;
            item = data.substr(6);
        } else if (data.find("[   ] ") == 0) {
            checked = false;
            item = data.substr(6);
        }
    }

    bool toggleStatus() override {
        checked = !checked;
        return true;
    }
        
};

class ContactLine : public Line {
private:
    std::string name;
    std::string surname;
    std::string email;

public:
    ContactLine(const std::string& n = "", const std::string& s = "", const std::string& e = "")
        : name(n), surname(s), email(e) {}

    void print() const override {
        std::cout << "Contact - " << name << " " << surname
                  << ", E-mail: " << email << std::endl;
    }

    std::string serialize() const override {
        return "Contact - " + name + " " + surname + ", Email: " + email;
    }

    void deserialize(const std::string& data) override {
        if (data.find("Contact - ") == 0) {
            size_t hyphen = data.find("- ");
            size_t comma = data.find(", Email: ");
                
            if (hyphen != std::string::npos && comma != std::string::npos) {
                std::string fullName = data.substr(hyphen + 2, comma - (hyphen + 2));
                size_t space = fullName.find(' ');
                    
                if (space != std::string::npos) {
                    name = fullName.substr(0, space);
                    surname = fullName.substr(space + 1);
                } else {
                    name = fullName;
                    surname = "";
                }
                    
                email = data.substr(comma + 9);
            }
        }
    }
};
