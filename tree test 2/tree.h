#ifndef TREE_H
#define TREE_H

#include <cstddef>
#include <utility>
#include <memory>
#include <initializer_list>
#include <vector>
#include <queue>
#include "iterator.h"

template<typename T>
class RedBlackTree {
private:
    struct Node {
        T data;
        Node* parent;
        Node* left;
        Node* right;
        bool is_red;
        
        Node(const T& value, Node* p = nullptr) 
            : data(value), parent(p), left(nullptr), right(nullptr), is_red(true) {}
        
        Node(T&& value, Node* p = nullptr) 
            : data(std::move(value)), parent(p), left(nullptr), right(nullptr), is_red(true) {}
    };

    Node* root;
    size_t tree_size;
    
    // Вспомогательные методы для красно-черного дерева
    void rotateLeft(Node* x);
    void rotateRight(Node* y);
    void fixInsert(Node* node);
    void fixDelete(Node* node);
    void transplant(Node* u, Node* v);
    Node* minimum(Node* node) const;
    Node* maximum(Node* node) const;
    Node* successor(Node* node) const;
    Node* predecessor(Node* node) const;
    void clear(Node* node);
    Node* copyTree(Node* node, Node* parent);
    
    // Метод для получения level-order последовательности
    std::vector<Node*> getLevelOrder() const;
    
    // Метод для инвалидации кэша итераторов
    void invalidateIterators();
    
public:
    using iterator = TreeIterator<T>;
    using const_iterator = ConstTreeIterator<T>;
    
    // Конструкторы и деструктор
    RedBlackTree();
    RedBlackTree(std::initializer_list<T> init);
    RedBlackTree(const RedBlackTree& other);
    RedBlackTree(RedBlackTree&& other) noexcept;
    ~RedBlackTree();
    
    // Операторы присваивания
    RedBlackTree& operator=(const RedBlackTree& other);
    RedBlackTree& operator=(RedBlackTree&& other) noexcept;
    
    // Основные операции
    std::pair<iterator, bool> insert(const T& value);
    size_t erase(const T& value);
    iterator find(const T& value);
    const_iterator find(const T& value) const;
    
    // Доступ к итераторам
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    
    // Емкость
    size_t size() const { return tree_size; }
    bool empty() const { return tree_size == 0; }
    
    // Вспомогательные методы
    void clear();
    
    // Метод для отладки - вывод дерева по уровням
    void printLevelOrder() const;
    
private:
    friend class TreeIterator<T>;
    friend class ConstTreeIterator<T>;
};

#endif