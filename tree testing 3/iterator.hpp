#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <stdexcept>
#include <vector>
#include <queue>

// Предварительное объявление
template <typename T>
class RedBlackTree;

// Итератор для level-order обхода (Вариант D)
template <typename T>
class TreeIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

private:
    using Node = typename RedBlackTree<T>::Node;
    
    Node* current;
    const RedBlackTree<T>* tree;
    
    Node* getNextNode() const {
        if (!current || !tree) return nullptr;
        return tree->getNextInLevelOrder(current);
    }
    
    Node* getPrevNode() const {
        if (!current || !tree) return nullptr;
        return tree->getPrevInLevelOrder(current);
    }

public:
    TreeIterator() : current(nullptr), tree(nullptr) {}
    
    TreeIterator(Node* node, const RedBlackTree<T>* t) 
        : current(node), tree(t) {}
    
    TreeIterator(const TreeIterator& other) 
        : current(other.current), tree(other.tree) {}
    
    TreeIterator& operator=(const TreeIterator& other) {
        if (this != &other) {
            current = other.current;
            tree = other.tree;
        }
        return *this;
    }
    
    reference operator*() const {
        if (current == nullptr) {
            throw std::runtime_error("Dereferencing end iterator");
        }
        return current->data;
    }
    
    pointer operator->() const {
        if (current == nullptr) {
            throw std::runtime_error("Dereferencing end iterator");
        }
        return &(current->data);
    }
    
    TreeIterator& operator++() {
        current = getNextNode();
        return *this;
    }
    
    TreeIterator operator++(int) {
        TreeIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    TreeIterator& operator--() {
        if (current == nullptr) {
            if (tree && tree->getRoot()) {
                auto traversal = tree->generateLevelOrder();
                if (!traversal.empty()) {
                    current = traversal.back();
                }
            }
        } else {
            current = getPrevNode();
        }
        return *this;
    }
    
    TreeIterator operator--(int) {
        TreeIterator tmp = *this;
        --(*this);
        return tmp;
    }
    
    bool operator==(const TreeIterator& other) const {
        return current == other.current && tree == other.tree;
    }
    
    bool operator!=(const TreeIterator& other) const {
        return !(*this == other);
    }
    
    Node* getNode() const { return current; }
};

// Константный итератор
template <typename T>
class ConstTreeIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = const T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

private:
    using Node = typename RedBlackTree<T>::Node;
    
    Node* current;
    const RedBlackTree<T>* tree;
    
    Node* getNextNode() const {
        if (!current || !tree) return nullptr;
        return tree->getNextInLevelOrder(current);
    }
    
    Node* getPrevNode() const {
        if (!current || !tree) return nullptr;
        return tree->getPrevInLevelOrder(current);
    }

public:
    ConstTreeIterator() : current(nullptr), tree(nullptr) {}
    
    ConstTreeIterator(Node* node, const RedBlackTree<T>* t) 
        : current(node), tree(t) {}
    
    ConstTreeIterator(const ConstTreeIterator& other) 
        : current(other.current), tree(other.tree) {}
    
    ConstTreeIterator(const TreeIterator<T>& other) 
        : current(other.getNode()), tree(other.tree) {}
    
    ConstTreeIterator& operator=(const ConstTreeIterator& other) {
        if (this != &other) {
            current = other.current;
            tree = other.tree;
        }
        return *this;
    }
    
    reference operator*() const {
        if (current == nullptr) {
            throw std::runtime_error("Dereferencing end iterator");
        }
        return current->data;
    }
    
    pointer operator->() const {
        if (current == nullptr) {
            throw std::runtime_error("Dereferencing end iterator");
        }
        return &(current->data);
    }
    
    ConstTreeIterator& operator++() {
        current = getNextNode();
        return *this;
    }
    
    ConstTreeIterator operator++(int) {
        ConstTreeIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    ConstTreeIterator& operator--() {
        if (current == nullptr) {
            if (tree && tree->getRoot()) {
                auto traversal = tree->generateLevelOrder();
                if (!traversal.empty()) {
                    current = traversal.back();
                }
            }
        } else {
            current = getPrevNode();
        }
        return *this;
    }
    
    ConstTreeIterator operator--(int) {
        ConstTreeIterator tmp = *this;
        --(*this);
        return tmp;
    }
    
    bool operator==(const ConstTreeIterator& other) const {
        return current == other.current && tree == other.tree;
    }
    
    bool operator!=(const ConstTreeIterator& other) const {
        return !(*this == other);
    }
};

#endif // ITERATOR_HPP