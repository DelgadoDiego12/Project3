//
// Created by Diego Delgado on 10/15/25.
//

#include "PriorityQueue.hpp"
#include <algorithm>

// Order for queue items, higher frequency wins, tie by lexicographically smaller keyWord()
// pre: a and b are non-null pointers to valid TreeNode objects
// post: Returns true if 'a' should come before 'b' in the queue's sorted order
bool PriorityQueue::higherPriority(const TreeNode *a, const TreeNode *b) noexcept {
    if (a->freq > b->freq)
        return true;
    else if (a->freq < b->freq)
        return false;
    else
        return a-> keyWord() < b->keyWord();
}

// constructs an initial vector of nodes by higherPriority
// pre: 'nodes' may be empty, all pointers in 'nodes' are valid TreeNode*
// post: items_ holds the input nodes sorted in non-increasing priority
PriorityQueue::PriorityQueue(std::vector<TreeNode *> nodes) : items_(std::move(nodes)) {
    const std::size_t n = items_.size();
    if (n < 2 )
        return;

    for (std::size_t i = 1; i < n; ++i) {
        TreeNode* key = items_[i];
        std::size_t j = i;
        while (j > 0 && !higherPriority(items_[j - 1], key)) {
            items_[j] = items_[j - 1];
            --j;
        }
        items_[j] = key;
    }
}

// Returns the number of elements currently in the queue
// pre: none
// post: returns items_.size()
std::size_t PriorityQueue::size() const noexcept {
    return items_.size();
}

// Indicates whether the queue has no elements
// pre: none
// post: returns true if size() == 0
bool PriorityQueue::empty() const noexcept {
    return items_.empty();
}

// Peeks at the minimum-priority element without removing
// pre: none
// post: returns pointer to min element if not empty, else nullptr
TreeNode* PriorityQueue::findMin() const noexcept {
    if (items_.empty())
        return nullptr;
    return items_.back();
}

// removes and returns the minimum-priority element
// pre: none
// post: if non-empty, returns previous min and queue size decreases by 1, else nullptr
TreeNode* PriorityQueue::extractMin() noexcept {
    if (items_.empty())
        return nullptr;

    TreeNode* min = items_.back();
    items_.pop_back();
    return min;
}

// removes the minimum-priority element and discards it
// pre: none
// post: if non-empty, size decreases by 1, otherwise no effect
void PriorityQueue::deleteMin() noexcept {
    if (!items_.empty())
        items_.pop_back();
}

// Inserts 'node' into the queue while maintaining sorted order
// pre: 'node' is a valid TreeNode*, Current items_ are sorted by higherPriority
// post: 'node' is placed so that items_ remains sorted, size increases by 1
void PriorityQueue::insert(TreeNode* node) {
    std::size_t position = 0;
    const std::size_t n = items_.size();

    while (position < n) {
        if (higherPriority(items_[position], node)) {
            ++position;
        } else {
            break;
        }
    }

    items_.push_back(nullptr);
    for (std::size_t i = items_.size() - 1; i > position; --i) {
        items_[i] = items_[i - 1];
    }
    items_[position] = node;
}

// Verifies that the internal ordering invariant holds over items_
// pre: None
// post: Returns true if for every i, items[i-1] has higherPriority than items_[i]
bool PriorityQueue::isSorted() const {
    if (items_.size() < 2)
        return true;

    for (std::size_t i = 1; i < items_.size(); ++i) {
        if (!higherPriority(items_[i - 1], items_[i]))
            return false;
    }
    return true;
}

// Writes a text representation of the queue to the provided stream , in current order
// pre: 'os' is a valid std::ostream reference
// post: Queue contents are written to 'os'. Queue remains unchanged
void PriorityQueue::print(std::ostream& os) const {
    os << "Priority Queue size:" << items_.size() << "\n";
    for (const auto* node : items_) {
        os << " (" << node->keyWord() << ", " << node->freq << ")\n";
    }
}

