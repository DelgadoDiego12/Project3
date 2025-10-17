//
// Created by Ali Kooshesh on 9/27/25.
//

#include "Scanner.hpp"

#include <utility>
#include <iostream>
#include <fstream>

#include "utils.hpp"
// Constructor
//pre: inputPath is a valid filesystem path
//post: Scanner object is initialized with inputPath stored in inputPath_
Scanner::Scanner(std::filesystem::path inputPath) : inputPath_(std::move(inputPath)) {}

//Tokenize: Reads words from the file into a vector
//pre: 'words' is a valid reference to a vector<string>, inputPath_ must be initialized
//post: If the file exists and can be opened, 'words' contains all tokens
error_type Scanner::tokenize(std::vector<std::string>& words) {
    const std::filesystem::path parent = inputPath_.parent_path();
    if (!parent.empty()) {
        if (auto status = directoryExists(parent.string()); status != NO_ERROR) {
            return status;
        }
    }
    if (auto status = regularFileExistsAndIsAvailable(inputPath_.string()); status != NO_ERROR) {
        return status;
        }

    std::ifstream in(inputPath_, std::ios::binary);
    if (!in.is_open()) {
        return UNABLE_TO_OPEN_FILE;
    }
    words.clear();
    while (true) {
        std::string word = readWord(in);
        if (word.empty()) break;
        words.push_back(std::move(word));

    }
    return NO_ERROR;
}

//tokenize (overload): Read words and writes them to an output file.
//pre: 'words' is a valid vector reference, 'outputFile' is a valid filesystem path.
//post: On success, 'words' is populated, and 'outputFile' is created and filled with one word per line.
//      Returns NO_ERROR on success, or an appropriate error_type otherwise
error_type Scanner::tokenize(std::vector<std::string>& words,
                    const std::filesystem::path& outputFile) {
    if(auto status = this->tokenize(words); status != NO_ERROR) {
        return status;
    }
    if(auto status = canOpenForWriting(outputFile.string()); status != NO_ERROR) {
        return status;
    }
    if (auto status = writeVectorToFile(outputFile.string(), words); status != NO_ERROR) {
        return status;
    }
    return NO_ERROR;
}

// readWord: Reads the next word-like token from the input stream
//pre: 'in' is an open input stream
//post: returns the next lowercase word consisting of letters and valid apostrophes,
//      if no more valid tokens are available, returns an empty string
std::string Scanner::readWord(std::istream &in) {
    std::string token;
    int c_int;

    while ((c_int = in.get()) != EOF) {
        unsigned char ch = static_cast<unsigned char>(c_int);
        if (std::isalpha(ch) && ch < 128) {
            token.push_back(static_cast<char>(std::tolower(ch)));
            break;
        }
    }

    if (token.empty()) return {};

    while ((c_int = in.peek()) != EOF) {
        unsigned char ch = static_cast<unsigned char>(c_int);
        if (std::isalpha(ch) && ch < 128) {
            in.get();
            token.push_back(static_cast<char>(std::tolower(ch)));
            continue;
        }
        if (ch == '\'') {
            in.get();
            int next = in.peek();
            if (next != EOF) {
                unsigned char nch = static_cast<unsigned char>(next);
                if (std::isalpha(nch) && nch < 128) {
                    token.push_back('\'');
                }
            }
            break;
        }
        break;
    }
    return token;
}