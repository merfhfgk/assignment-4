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


typedef std::string (*EncryptFunc)(const std::string&, const std::string&);
typedef std::string (*DecryptFunc)(const std::string&, const std::string&);

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
        auto func = (std::string (*)(const std::string&, const std::string&)) dlsym(handle, funcName);
        const char *dlsym_error = dlerror();
        if (dlsym_error) {
            std::cerr << "Cannot load symbol '" << funcName << "': " << dlsym_error << '\n';
            dlclose(handle);
            return "";
        }

        std::string result = func(text, key);

        dlclose(handle);

        return result;
    }

public:
    Cipher(const std::string& path) : libPath(path) {}

    std::string encrypt(const std::string& rawText, const std::string& key) {
        return processText(rawText, key, "encrypt");
    }

    std::string decrypt(const std::string& encryptedText, const std::string& key) {
        return processText(encryptedText, key, "decrypt");
    }
};
