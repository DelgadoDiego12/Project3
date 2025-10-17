//
// Created by Diego Delgado on 10/15/25.
//

#ifndef P3_PART1_TREENODE_H
#define P3_PART1_TREENODE_H

#pragma once
#include <string>
#include <algorithm>

struct TreeNode {
    std::string word;
    int freq = 0;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;

    TreeNode(std::string w, int f = 1): word(std::move(w)), freq(f), left(nullptr), right(nullptr) {};

    std::string keyWord() const {
        if (left == nullptr && right == nullptr)
            return word;

        std::string lk;
        std::string rk;

        if (left != nullptr)
            lk = left->keyWord();
        else
            lk = word;

        if (right != nullptr)
            rk = right->keyWord();
        else
            rk = word;

        if (lk < rk)
            return lk;
        else
            return rk;
    }
};

#endif //P3_PART1_TREENODE_H