//
// Created by Diego Delgado on 10/15/25.
//

#include "BinSearchTree.hpp"
#include <optional>

BinSearchTree::~BinSearchTree() {destroy(root_); }

// Deletes an entire subtree rooted at 'node'
// pre: 'node' is either nullptr or a valid pointer to a TreeNode in this tree.
// post: all nodes in the subtree are deleted; no leaf, children remain.
void BinSearchTree::destroy(TreeNode *node) noexcept {
    if (!node)
        return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// Inserts 'word' into the BST subtree rooted at 'node'
// if 'word' exists, increment frequency
// pre: 'node' is either nullptr or the root of a valid BST subtree
// post: Returns the root of the subtree with 'word' in it, frequency is incremented
TreeNode *BinSearchTree::insertHelper(TreeNode *node, const std::string &word) {
    if (!node)
        return new TreeNode(word, 1);
    if (word == node->word) {
        node->freq += 1;
    } else if (word < node->word) {
        node->left = insertHelper(node->left, word);
    } else {
        node->right = insertHelper(node->right, word);
    }
    return node;
}

// Public insert, adds 'word' to the tree of increments its frequency
// pre: none
// post: Tree contains 'word', if 'word' is present, frequency plus 1
void BinSearchTree::insert(const std::string &word) {
    root_ = insertHelper(root_, word);
}

// Inserts all words from 'words' into the tree
// pre: 'words' is a valid vector reference
// post: Each word in 'words' has been inserted or frequency plus 1
void BinSearchTree::bulkInsert(const std::vector<std::string> &words) {
    for (const auto &word : words) insert(word);
}

// Iteratively seraches for 'word' starting at 'node'
// pre: 'node' is either nullptr or the root of a valid BST subtree
// post: returns pointer to the node with 'word' or nullptr if not found
const TreeNode *BinSearchTree::findNode(const TreeNode *node, std::string_view word) noexcept {
    while (node) {
        if (word == node->word)
            return node;
        node = (word < node->word) ? node->left : node->right;
    }
    return nullptr;
}

// Checks whether 'word' is present in the tree
// pre: none
// post: returns true if a node with 'word' exists
bool BinSearchTree::contains(std::string_view word) const noexcept {
    return findNode(root_, word) != nullptr;
}

// Retrieves the frequency of 'word' if present
// pre: none
// post: returns frequency if found, else nullopt
std::optional<int> BinSearchTree::countOf(std::string_view word) const noexcept {
    if (auto* node = findNode(root_, word))
        return node->freq;
    return std::nullopt;
}

// In-order traversal appending word, freq to 'out'
// pre: 'node' is nullptr or a valid subtree root, 'out' is a valid vector reference
// post: appends elements from this subtree to 'out' in ascending order by word
void BinSearchTree::inorderHelper(const TreeNode *node, std::vector<std::pair<std::string, int> > &out) {
    if (!node)
        return;
    inorderHelper(node->left, out);
    out.push_back({node->word, node->freq});
    inorderHelper(node->right, out);
}

// Collects word, freq, from the whole tree in sorted order
// pre: 'out' is a valid vector reference
// post: 'out' is cleared and then filled with the entire tree's contents in ascdening order
void BinSearchTree::inorderCollect(std::vector<std::pair<std::string, int> > &out) const {
    out.clear();
    inorderHelper(root_, out);
}

// Counts nodes in a subtree, unique words
// pre: 'node' is nullptr of a valid subtree root
// post: Returns the number of nodes in the subtree
std::size_t BinSearchTree::sizeHelper(const TreeNode* node)  noexcept {
    if (!node)
        return 0;
    return 1 + sizeHelper(node->left)+ sizeHelper(node->right);
}

// Returns the number of unique words in the tree
// pre: none
// post: returns size_t count
std::size_t BinSearchTree::size() const noexcept {
    return sizeHelper(root_);
}

// Finds the height of a subtree in nodes
// pre: 'node' is nullptr or a valid subtree root
// post: returns the heigh as an unsigned int
unsigned int BinSearchTree::heightHelper(const TreeNode* node) noexcept {
    if (!node)
        return 0;
    unsigned lh = heightHelper(node->left);
    unsigned rh = heightHelper(node->right);
    return 1 + (lh > rh ? lh : rh);
}

// returns the height of the tree in nodes
// pre: none
// post: returns an unsigned height
unsigned BinSearchTree::height() const noexcept {
    return heightHelper(root_);
}


