#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <iomanip>

#include "Scanner.hpp"
#include "utils.hpp"
#include "TreeNode.hpp"
#include "BinSearchTree.hpp"
#include "PriorityQueue.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }




    const std::string dirName = std::string("input_output");
    const std::string givenName = std::string(argv[1]);

    std::string inputFileName = givenName;
    if (error_type s = regularFileExistsAndIsAvailable(inputFileName); s != NO_ERROR) {
        std::string alt = dirName + "/" + givenName;
        if (error_type s2 = regularFileExistsAndIsAvailable(alt); s2 != NO_ERROR) {
            exitOnError(s, givenName);
        }
        inputFileName = alt;
    }

    const std::string inputFileBaseName = baseNameWithoutTxt(givenName);

    // build the path to the .tokens output file.
    const std::string wordTokensFileName = dirName + "/" + inputFileBaseName + ".tokens";

    const std::string frequenciesFileName = dirName + "/" + inputFileBaseName + ".freq";


    // The next several if-statement make sure that the input file, the directory exist
    // and that the output file is writeable.
    if( error_type status; (status = regularFileExistsAndIsAvailable(givenName)) != NO_ERROR )
        exitOnError(status, givenName);


    if (error_type status; (status = directoryExists(dirName)) != NO_ERROR )
        exitOnError(status, dirName);

    if (error_type status; (status = canOpenForWriting(wordTokensFileName)) != NO_ERROR)
        exitOnError(status, wordTokensFileName);

    if (error_type status; (status = canOpenForWriting(frequenciesFileName)) != NO_ERROR)
        exitOnError(status, frequenciesFileName);


    std::vector<std::string> words;
    namespace fs = std::filesystem;
    Scanner scanner(givenName);
    if (error_type status; (status = scanner.tokenize(words)) != NO_ERROR)
        exitOnError(status,givenName);

    if (error_type status; (status = writeVectorToFile(wordTokensFileName, words)) != NO_ERROR)
        exitOnError(status, wordTokensFileName);

    BinSearchTree bst;
    bst.bulkInsert(words);

    std::vector<std::pair<std::string,int>> frequencies;
    bst.inorderCollect(frequencies);

    unsigned H = bst.height();
    std::size_t U = bst.size();
    std::size_t T = words.size();
    int minF = 0, maxF = 0;

    if (!frequencies.empty()) {
        minF = frequencies[0].second;
        maxF = frequencies[0].second;
        for (std::size_t i = 1; i < frequencies.size(); i++) {
            if (frequencies[i].second < minF) {
                minF = frequencies[i].second;
            }
            if (frequencies[i].second > maxF) {
                maxF = frequencies[i].second;
            }
        }
    } else {
        H = 0;
        U = 0;
        T = 0;
        minF = 0;
        maxF = 0;
    }

    std::cout << "BST height: " << H << "\n";
    std::cout << "BST unique words: " << U << "\n";
    std::cout << "Total tokens: " << T << "\n";
    std::cout << "Min frequency: " << minF << "\n";
    std::cout << "Max frequency: " << maxF << "\n";

    std::vector<TreeNode*> leaves;
    leaves.reserve(frequencies.size());
    for (std::size_t i = 0; i < frequencies.size(); i++) {
        const std::string& word = frequencies[i].first;
        int c = frequencies[i].second;
        leaves.push_back(new TreeNode(word, c));
    }

    PriorityQueue pq(leaves);

    std::vector<TreeNode*> extracted;
    extracted.reserve(pq.size());
    while (!pq.empty()) {
        TreeNode* m = pq.extractMin();
        if (m != nullptr) {
            extracted.push_back(m);
        }
    }
    {
        std::ofstream out(frequenciesFileName, std::ios::out | std::ios::trunc);
        if (!out.is_open()) {
            exitOnError(UNABLE_TO_OPEN_FILE_FOR_WRITING, frequenciesFileName);
        }

        for (std::size_t i = extracted.size(); i-- > 0;) {
            TreeNode* node = extracted[i];
            out << std:: setw(10) << node ->freq << ' ' << node->keyWord() << '\n';
        }
    }

    for (std::size_t i = 0; i < extracted.size(); i++) {
        delete extracted[i];
    }

    for (std::size_t i = 0; i < leaves.size(); i++) {
        leaves[i] = nullptr;
    }

    return 0;
}
