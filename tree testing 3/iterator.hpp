#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include <stdexcept>
#include <vector>
#include <queue>

// Предварительное объявление Tree
template <typename T>
class RedBlackTree;

// Реализация итератора для level-order обхода
template <typename T>
class TreeIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

private:
    using Node = typename RedBlackTree<T>::Node;
    
    // Для level-order итератора храним:
    // - текущую позицию в обходе
    // - кэшированный обход
    // - указатель на дерево
    size_t current_pos;
    std::vector<Node*> cached_traversal;
    const RedBlackTree<T>* tree;

public:
    TreeIterator() : current_pos(0), tree(nullptr) {}
    
    TreeIterator(size_t pos, const std::vector<Node*>& traversal, const RedBlackTree<T>* t) 
        : current_pos(pos), cached_traversal(traversal), tree(t) {}
    
    TreeIterator(const TreeIterator& other) 
        : current_pos(other.current_pos), 
          cached_traversal(other.cached_traversal), 
          tree(other.tree) {}
    
    TreeIterator& operator=(const TreeIterator& other) {
        if (this != &other) {
            current_pos = other.current_pos;
            cached_traversal = other.cached_traversal;
            tree = other.tree;
        }
        return *this;
    }
    
    reference operator*() const {
        if (current_pos >= cached_traversal.size()) {
            throw std::runtime_error("Dereferencing end iterator");
        }
        return cached_traversal[current_pos]->data;
    }
    
    pointer operator->() const {
        if (current_pos >= cached_traversal.size()) {
            throw std::runtime_error("Dereferencing end iterator");
        }
        return &(cached_traversal[current_pos]->data);
    }
    
    TreeIterator& operator++() {
        if (current_pos < cached_traversal.size()) {
            ++current_pos;
        }
        return *this;
    }
    
    TreeIterator operator++(int) {
        TreeIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    // Для forward iterator достаточно этих операций
    
    bool operator==(const TreeIterator& other) const {
        return current_pos == other.current_pos && tree == other.tree;
    }
    
    bool operator!=(const TreeIterator& other) const {
        return !(*this == other);
    }
};

// Константный итератор
template <typename T>
class ConstTreeIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

private:
    using Node = typename RedBlackTree<T>::Node;
    
    size_t current_pos;
    std::vector<Node*> cached_traversal;
    const RedBlackTree<T>* tree;

public:
    ConstTreeIterator() : current_pos(0), tree(nullptr) {}
    
    ConstTreeIterator(size_t pos, const std::vector<Node*>& traversal, const RedBlackTree<T>* t) 
        : current_pos(pos), cached_traversal(traversal), tree(t) {}
    
    ConstTreeIterator(const ConstTreeIterator& other) 
        : current_pos(other.current_pos), 
          cached_traversal(other.cached_traversal), 
          tree(other.tree) {}
    
    ConstTreeIterator(const TreeIterator<T>& other) 
        : current_pos(other.current_pos), 
          cached_traversal(other.cached_traversal), 
          tree(other.tree) {}
    
    ConstTreeIterator& operator=(const ConstTreeIterator& other) {
        if (this != &other) {
            current_pos = other.current_pos;
            cached_traversal = other.cached_traversal;
            tree = other.tree;
        }
        return *this;
    }
    
    reference operator*() const {
        if (current_pos >= cached_traversal.size()) {
            throw std::runtime_error("Dereferencing end iterator");
        }
        return cached_traversal[current_pos]->data;
    }
    
    pointer operator->() const {
        if (current_pos >= cached_traversal.size()) {
            throw std::runtime_error("Dereferencing end iterator");
        }
        return &(cached_traversal[current_pos]->data);
    }
    
    ConstTreeIterator& operator++() {
        if (current_pos < cached_traversal.size()) {
            ++current_pos;
        }
        return *this;
    }
    
    ConstTreeIterator operator++(int) {
        ConstTreeIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    bool operator==(const ConstTreeIterator& other) const {
        return current_pos == other.current_pos && tree == other.tree;
    }
    
    bool operator!=(const ConstTreeIterator& other) const {
        return !(*this == other);
    }
};

#endif // ITERATOR_HPP