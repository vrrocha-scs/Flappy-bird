#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <cctype>

inline std::string trim(const std::string& str) { // funcao para limpar a entrada de nomes
    std::string cleaned_str;
    for (char c : str) {
        if (isprint(static_cast<unsigned char>(c))) {
            cleaned_str += c;
        }
    }
    return cleaned_str;
}

#endif