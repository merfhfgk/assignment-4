#pragma once
#include <iostream>
#include <string>
#include <dlfcn.h>
#include <stdexcept>

enum CipherAlgorithm {
    CAESAR = 1,
    VIGENERE = 2
};

class Cipher {
private:
    std::string libPath;

    typedef void* (*CreateCaesarFunc)(int);
    typedef void* (*CreateVigenereFunc)(const char*);
    typedef char* (*ProcessFunc)(void*, const char*);
    typedef void (*DestroyFunc)(void*);
    typedef void (*FreeFunc)(char*);

    std::string processText(const std::string& text, const std::string& keyStr, bool isEncrypt, CipherAlgorithm algo) {
        void* handle = dlopen(libPath.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "\nerror: couldn't upload library " << dlerror() << '\n';
            return "";
        }
        dlerror();

        ProcessFunc process_text = (ProcessFunc)dlsym(handle, isEncrypt ? "cipher_encrypt" : "cipher_decrypt");
        DestroyFunc destroy_cipher = (DestroyFunc)dlsym(handle, "cipher_destroy");
        FreeFunc free_str = (FreeFunc)dlsym(handle, "cipher_free");

        if (!process_text || !destroy_cipher || !free_str) {
            std::cerr << "\nerror: couldn't find functions\n";
            dlclose(handle);
            return "";
        }

        std::string result = "";
        try {
            void* cipher_ctx = nullptr;

            if (algo == CAESAR) {
                CreateCaesarFunc create_caesar = (CreateCaesarFunc)dlsym(handle, "cipher_create_caesar");
                
                int keyInt = 0;
                try {
                    keyInt = std::stoi(keyStr);
                } catch (...) {
                    std::cerr << "\nerror: key must be a number\n";
                    dlclose(handle);
                    return "";
                }
                cipher_ctx = create_caesar(keyInt);
            }
            else if (algo == VIGENERE) {
                CreateVigenereFunc create_vigenere = (CreateVigenereFunc)dlsym(handle, "cipher_create_vigenere");
                
                cipher_ctx = create_vigenere(keyStr.c_str());
            }

            if (cipher_ctx) {
                char* c_result = process_text(cipher_ctx, text.c_str());
                if (c_result) {
                    result = std::string(c_result);
                    free_str(c_result);
                }
                destroy_cipher(cipher_ctx);
            }
            
        } catch (const std::exception& e) {
            std::cerr << "\nError " << e.what() << '\n';
        } catch (...) {
            std::cerr << "\nerror\n";
        }

        dlclose(handle);
        return result;
    }

public:
    Cipher(const std::string& path) : libPath(path) {}

    std::string encrypt(const std::string& rawText, const std::string& key, CipherAlgorithm algo) {
        return processText(rawText, key, true, algo);
    }

    std::string decrypt(const std::string& encryptedText, const std::string& key, CipherAlgorithm algo) {
        return processText(encryptedText, key, false, algo);
    }
};
