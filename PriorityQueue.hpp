//
// Created by Diego Delgado on 10/15/25.
//

#ifndef P3_PART1_PRIORITYQUEUE_H
#define P3_PART1_PRIORITYQUEUE_H

#pragma once
#include <vector>
#include <iostream>
#include "TreeNode.hpp"

class PriorityQueue {
public:
    // Non‑owning: does NOT delete the TreeNode* it stores.
    // The constructor takes an initial set of leaves and sorts them internally.
    explicit PriorityQueue(std::vector<TreeNode*> nodes);
    ~PriorityQueue() = default;

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    // Min accessors (MIN = items_.back() under our ordering)
    [[nodiscard]] TreeNode* findMin() const noexcept;   // nullptr if empty
    TreeNode* extractMin() noexcept;                    // remove+return min, or nullptr
    void deleteMin() noexcept;                          // remove min if present

    // Insert while maintaining the invariant (O(N) due to shifting)
    // Stores the pointer without taking ownership.
    void insert(TreeNode* node);

    // Debug printing
    void print(std::ostream& os = std::cout) const;

private:
    // Invariant: items_ is kept sorted by HigherPriority(a,b)
    // i.e., (freq desc, key_word asc). Therefore the MIN is items_.back().
    // Ownership: items_ does NOT own the pointers.
    std::vector<TreeNode*> items_;

    static bool higherPriority(const TreeNode* a, const TreeNode* b) noexcept; // a before b?
    bool isSorted() const; // for assertions/tests only
};

#endif //P3_PART1_PRIORITYQUEUE_H