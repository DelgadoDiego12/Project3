//
// Created by Diego Delgado on 10/23/25.
//

#include "HuffmanTree.h"
#include "PriorityQueue.hpp"
#include <cassert>

HuffmanTree::~HuffmanTree () { destroy(root_); }

void HuffmanTree::destroy (TreeNode* n) noexcept {
    if (!n) return;
    destroy (n->left);
    destroy (n->right);
    delete n;
}

HuffmanTree HuffmanTree::buildFromCounts(const std::vector<std::pair<std::string, int> > &counts) {
    std::vector<TreeNode*> nodes;
    nodes.reserve(counts.size());
    for (const auto& [w,c] : counts) {
        if (c > 0) nodes.push_back(new TreeNode(w, c));
    }
    HuffmanTree ht;

    if (nodes.empty()) {
        ht.root_ = nullptr;
        return ht;
    }
    if (nodes.size() == 1) {
        ht.root_ = nodes.front();
        return ht;
    }

    PriorityQueue pq(std::move(nodes));
    while (pq.size() > 1) {
        TreeNode* a = pq.extractMin();
        TreeNode* b = pq.extractMin();
        TreeNode* parent = new TreeNode(std::string{}, a->freq + b->freq);

        parent->left = a;
        parent->right = b;
        pq.insert(parent);
    }
    ht.root_ = pq.extractMin();
    return ht;
}

void HuffmanTree::assignCodes(std::vector<std::pair<std::string, std::string>>& out) const {
    out.clear();
    if (!root_) return;
    std::string prefix;
    assignCodesDFS(root_, prefix, out);
}

void HuffmanTree::assignCodesDFS(const TreeNode* n, std::string& prefix, std::vector<std::pair<std::string, std::string>>& out)  {
    if (!n) return;
    const bool isALeaf = (n->left == nullptr && n->right == nullptr);
    if (isALeaf) {
        out.emplace_back(n->word, prefix.empty() ? std::string("0") : prefix);
        return;
    }
    prefix.push_back('0');
    assignCodesDFS(n->left, prefix, out);
    prefix.pop_back();

    prefix.push_back('1');
    assignCodesDFS(n->right, prefix, out);
    prefix.pop_back();
}

error_type HuffmanTree::writeHeader(std::ostream &os) const {
    if (!root_) {
        return NO_ERROR;
    }
    if (!os.good()) return FAILED_TO_WRITE_FILE;

    std::string prefix;
    writeHeaderPreorder(root_, os, prefix);

    if (os.fail()) return FAILED_TO_WRITE_FILE;

    return NO_ERROR;
}

void HuffmanTree::writeHeaderPreorder(const TreeNode *n, std::ostream &os, std::string &prefix) {
    if (!n) return;

    const bool isALeaf = (n->left == nullptr && n->right == nullptr);
    if (isALeaf) {
        const std::string& code = prefix.empty() ? std::string("0") : prefix;
        os << n->word << ' ' << code << '\n';
        return;
    }
    prefix.push_back('0');
    writeHeaderPreorder(n->left, os, prefix);
    prefix.pop_back();

    prefix.push_back('1');
    writeHeaderPreorder(n->right, os, prefix);
    prefix.pop_back();
}

error_type HuffmanTree::encode(const std::vector<std::string>& tokens, std::ostream& os_bits, int wrap_cols) const {
    if (!root_) return FAILED_TO_WRITE_FILE;

    if (!os_bits.good()) return FAILED_TO_WRITE_FILE;

    std::vector<std::pair<std::string, std::string>> pairs;
    assignCodes(pairs);
    std::map<std::string, std::string> codes;
    codes.insert(pairs.begin(), pairs.end());

    constexpr int WRAP = 80;
    int col = 0;
    for (const auto& t : tokens) {
        auto it = codes.find(t);
        if (it == codes.end()) {
            return FAILED_TO_WRITE_FILE;
        }
        const std::string& code = it->second;
        for (char bit : code) {
            os_bits.put(bit);
            if (!os_bits) return FAILED_TO_WRITE_FILE;
            if (++col == WRAP) {
                os_bits.put('\n');
                if (!os_bits) return FAILED_TO_WRITE_FILE;
                col = 0;
            }
        }
    }
    if (col != 0) {
        os_bits.put('\n');
        if (!os_bits) return FAILED_TO_WRITE_FILE;
    }
    return os_bits.fail() ? FAILED_TO_WRITE_FILE : NO_ERROR;
}