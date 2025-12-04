#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <stdexcept>

// Предварительное объявление Tree (Tree уже определен в tree.hpp)
template <typename T>
class Tree;

// Реализация итератора
template <typename T>
class TreeIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

private:
    using Node = typename Tree<T>::Node;
    Node* current;
    Node* nil;
    const Tree<T>* tree;

public:
    TreeIterator() : current(nullptr), nil(nullptr), tree(nullptr) {}
    TreeIterator(Node* node, Node* nilNode, const Tree<T>* t) : current(node), nil(nilNode), tree(t) {}
    TreeIterator(const TreeIterator& other) : current(other.current), nil(other.nil), tree(other.tree) {}

    TreeIterator& operator=(const TreeIterator& other) {
        if (this != &other) {
            current = other.current;
            nil = other.nil;
            tree = other.tree;
        }
        return *this;
    }

    reference operator*() const {
        if (current == nil || current == nullptr) {
            throw std::runtime_error("Dereferencing end iterator");
        }
        return current->val;
    }

    pointer operator->() const {
        if (current == nil || current == nullptr) {
            throw std::runtime_error("Dereferencing end iterator");
        }
        return &(current->val);
    }

    TreeIterator& operator++() {
        if (current == nil || current == nullptr) {
            return *this;
        }
        current = tree->successor(current);
        return *this;
    }

    TreeIterator operator++(int) {
        TreeIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    TreeIterator& operator--() {
        if (current == nil || current == nullptr) {
            // Если итератор на end(), переходим к максимальному элементу
            if (tree && tree->root != tree->nil) {
                current = tree->maximum(tree->root);
            }
            return *this;
        }
        current = tree->predecessor(current);
        return *this;
    }

    TreeIterator operator--(int) {
        TreeIterator tmp = *this;
        --(*this);
        return tmp;
    }

    bool operator==(const TreeIterator& other) const {
        return current == other.current;
    }

    bool operator!=(const TreeIterator& other) const {
        return !(*this == other);
    }

    Node* getNode() const { return current; }
};

#endif // ITERATOR_HPP

