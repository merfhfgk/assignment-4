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
        return "TEXT|" + text;
    }

    void deserialize(const std::string& data) override {
        size_t pos = data.find('|');
        if (pos != std::string::npos) {
            text = data.substr(pos + 1);
        }
    }
};

class ChecklistLine : public Line {
private:
    std::string item;
    bool checked;

public:
    ChecklistLine(const std::string& i = "", bool c = false) : item(i), checked(c) {}

    void print() const override {
        std::cout << "[ " << (checked ? "x" : " ") << " ] " << item << std::endl;
    }

    std::string serialize() const override {
        return "CHECKLIST|" + item + "|" + (checked ? "1" : "0");
    }

    void deserialize(const std::string& data) override {
        size_t firstDelimiter = data.find('|');
        size_t secondDelimiter = data.find('|', firstDelimiter + 1);
        
        if (firstDelimiter != std::string::npos && secondDelimiter != std::string::npos) {
            item = data.substr(firstDelimiter + 1, secondDelimiter - firstDelimiter - 1);
            std::string status = data.substr(secondDelimiter + 1);
            checked = (status == "1");
        }
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
        return "CONTACT|" + name + "|" + surname + "|" + email;
    }

    void deserialize(const std::string& data) override {
        std::stringstream ss(data);
        std::string token;
        
        std::getline(ss, token, '|');
        std::getline(ss, name, '|');
        std::getline(ss, surname, '|');
        std::getline(ss, email);
    }
};
