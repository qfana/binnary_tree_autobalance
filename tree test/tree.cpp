#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>

// Цвет узла
enum Color { RED, BLACK };

// Узел красно-черного дерева
template <typename T>
class Node {
public:
    T val;
    Node* left;
    Node* right;
    Node* parent;
    Color color;

    Node(T k) : val(k), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
};

// Предварительное объявление
template <typename T>
class Tree;

// Итератор
template <typename T>
class TreeIterator {
private:
    Node<T>* current;
    Node<T>* nil;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    TreeIterator() : current(nullptr), nil(nullptr) {}
    TreeIterator(Node<T>* node, Node<T>* nilNode) : current(node), nil(nilNode) {}
    TreeIterator(const TreeIterator& other) : current(other.current), nil(other.nil) {}

    TreeIterator& operator=(const TreeIterator& other);
    reference operator*() const;
    pointer operator->() const;
    TreeIterator& operator++();
    TreeIterator operator++(int);
    bool operator==(const TreeIterator& other) const;
    bool operator!=(const TreeIterator& other) const;

    Node<T>* getNode() const { return current; }
};

// Красно-черное дерево
template <typename T>
class Tree {
private:
    Node<T>* root;
    Node<T>* nil;
    int treeSize;

    // Вращения
    void rotateLeft(Node<T>* x);
    void rotateRight(Node<T>* x);

    // Балансировка
    void fixInsert(Node<T>* z);
    void fixDelete(Node<T>* x);

    // Вспомогательные методы
    Node<T>* search(Node<T>* node, const T& value) const;
    Node<T>* minimum(Node<T>* node) const;
    Node<T>* maximum(Node<T>* node) const;
    void transplant(Node<T>* u, Node<T>* v);

    // Рекурсивные методы
    void clearRecursive(Node<T>* node);
    Node<T>* copyRecursive(Node<T>* node, Node<T>* parent);
    int checkPropertiesHelper(Node<T>* node) const;
    void printTreeHelper(Node<T>* node, int depth) const;

public:
    // Конструкторы и деструктор
    Tree();
    Tree(const Tree& other);
    Tree(Tree&& other) noexcept;
    ~Tree();

    // Операторы присваивания
    Tree& operator=(const Tree& other);
    Tree& operator=(Tree&& other) noexcept;

    // Основные операции
    Node<T>* insert(const T& value);
    int erase(const T& value);
    Node<T>* find(const T& value);
    Node<T>* begin();
    Node<T>* end();
    Node<T>* getRoot() { return (root != nil) ? root : nullptr; }
    Node<T>* getNil() { return nil; }

    // Информация о дереве
    int size() const { return treeSize; }
    bool empty() const { return treeSize == 0; }
    void clear();

    // Отладка
    bool checkProperties() const;
    void print() const;

    // Итераторы
    TreeIterator<T> iteratorBegin();
    TreeIterator<T> iteratorEnd();

    // Дружественные классы
    template<typename U>
    friend class ConstructorsUtil;
};

// Включение реализации методов
#include "tree_impl.hpp"

#endif // TREE_HPP