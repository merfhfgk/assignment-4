//
//  Cipher.h
//  assignment4
//
//  Created by Maria Goncharuk on 08.07.2026.
//

#pragma once
#include <iostream>
#include <string>
#include <dlfcn.h>

class Cipher {
private:
    std::string libPath;

    std::string processText(const std::string& text, const std::string& key, const char* funcName) {
        void* handle = dlopen(libPath.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "Cannot open library: " << dlerror() << '\n';
            return "";
        }
     
        dlerror();
        using FuncType = std::string (*)(const std::string&, const std::string&);
                
                FuncType func = (FuncType) dlsym(handle, funcName);
        const char *dlsym_error = dlerror();
        if (dlsym_error) {
            std::cerr << "Cannot load symbol '" << funcName << "': " << dlsym_error << '\n';
            dlclose(handle);
            return "";
        }

        std::string result = "";
        try {
                result = func(text, key);
        }
        catch (const std::exception& e) {
            std::cerr << "\n[ПОМИЛКА] Краш всередині самої бібліотеки під час роботи: " << e.what() << '\n';
        } catch (...) {
            std::cerr << "\n[ПОМИЛКА] Невідомий краш всередині бібліотеки! Можливо, не збігаються типи параметрів (наприклад, ключ очікувався int, а передано string).\n";
        }
        dlclose(handle);

        return result;
    }

public:
    Cipher(const std::string& path) : libPath(path) {}

    std::string encrypt(const std::string& rawText, const std::string& key) {
        return processText(rawText, key, "cipher_encrypt");
    }

    std::string decrypt(const std::string& encryptedText, const std::string& key) {
        return processText(encryptedText, key, "cipher_decrypt");
    }
};
