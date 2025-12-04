#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <iterator>

// Предварительное объявление для итератора
template <typename T>
class TreeIterator;

template <typename T>
class Tree {
public:
    // Типы
    using iterator = TreeIterator<T>;
    using value_type = T;
    using size_type = size_t;

    // Конструкторы и деструктор
    Tree();
    Tree(const Tree& other);
    Tree(Tree&& other) noexcept;
    ~Tree();

    // Операторы присваивания
    Tree& operator=(const Tree& other);
    Tree& operator=(Tree&& other) noexcept;

    // Основные операции
    std::pair<iterator, bool> insert(const T& value);
    size_type erase(const T& value);
    iterator find(const T& value);
    iterator begin();
    iterator end();
    iterator begin() const;
    iterator end() const;
    iterator cbegin() const;
    iterator cend() const;

    // Информация о дереве
    size_type size() const;
    bool empty() const;
    void clear();

private:
    // Узел дерева
    struct Node {
        T val;
        Node* left;
        Node* right;
        Node* parent;
        enum Color { RED, BLACK } color;

        Node(const T& k) : val(k), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
    };

    Node* root;
    Node* nil;  // Sentinel node
    size_type treeSize;

    // Вращения
    void rotateLeft(Node* x);
    void rotateRight(Node* x);

    // Балансировка
    void fixInsert(Node* z);
    void fixDelete(Node* x);

    // Вспомогательные методы
    Node* search(Node* node, const T& value) const;
    Node* minimum(Node* node) const;
    Node* maximum(Node* node) const;
    void transplant(Node* u, Node* v);
    Node* successor(Node* node) const;
    Node* predecessor(Node* node) const;

    // Рекурсивные методы
    void clearRecursive(Node* node);
    Node* copyRecursive(Node* node, Node* parent);

    // Дружественный класс для итератора
    friend class TreeIterator<T>;
};

// Включаем реализацию итератора после определения Tree
#include "../iterator/iterator.hpp"

// Реализация методов Tree

template <typename T>
Tree<T>::Tree() : treeSize(0) {
    nil = new Node(T{});
    nil->color = Node::BLACK;
    nil->left = nil;
    nil->right = nil;
    nil->parent = nil;
    root = nil;
}

template <typename T>
Tree<T>::Tree(const Tree& other) : treeSize(0) {
    nil = new Node(T{});
    nil->color = Node::BLACK;
    nil->left = nil;
    nil->right = nil;
    nil->parent = nil;
    root = nil;
    
    if (other.root != other.nil) {
        root = copyRecursive(other.root, nil);
        treeSize = other.treeSize;
    }
}

template <typename T>
Tree<T>::Tree(Tree&& other) noexcept 
    : root(other.root), nil(other.nil), treeSize(other.treeSize) {
    other.root = nullptr;
    other.nil = nullptr;
    other.treeSize = 0;
}

template <typename T>
Tree<T>::~Tree() {
    clear();
    if (nil) {
        delete nil;
    }
}

template <typename T>
Tree<T>& Tree<T>::operator=(const Tree& other) {
    if (this != &other) {
        clear();
        if (other.root != other.nil) {
            root = copyRecursive(other.root, nil);
            treeSize = other.treeSize;
        } else {
            root = nil;
            treeSize = 0;
        }
    }
    return *this;
}

template <typename T>
Tree<T>& Tree<T>::operator=(Tree&& other) noexcept {
    if (this != &other) {
        clear();
        if (nil) {
            delete nil;
        }
        root = other.root;
        nil = other.nil;
        treeSize = other.treeSize;
        other.root = nullptr;
        other.nil = nullptr;
        other.treeSize = 0;
    }
    return *this;
}

template <typename T>
typename Tree<T>::Node* Tree<T>::copyRecursive(Node* node, Node* parent) {
    if (node == nullptr || node->left == nullptr) {
        return nil;
    }
    
    Node* newNode = new Node(node->val);
    newNode->color = node->color;
    newNode->parent = parent;
    newNode->left = copyRecursive(node->left, newNode);
    newNode->right = copyRecursive(node->right, newNode);
    
    if (newNode->left == nullptr) newNode->left = nil;
    if (newNode->right == nullptr) newNode->right = nil;
    
    return newNode;
}

template <typename T>
void Tree<T>::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    
    if (y->left != nil) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == nil) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

template <typename T>
void Tree<T>::rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    
    if (y->right != nil) {
        y->right->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == nil) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    
    y->right = x;
    x->parent = y;
}

template <typename T>
void Tree<T>::fixInsert(Node* z) {
    while (z->parent->color == Node::RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == Node::RED) {
                z->parent->color = Node::BLACK;
                y->color = Node::BLACK;
                z->parent->parent->color = Node::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(z);
                }
                z->parent->color = Node::BLACK;
                z->parent->parent->color = Node::RED;
                rotateRight(z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y->color == Node::RED) {
                z->parent->color = Node::BLACK;
                y->color = Node::BLACK;
                z->parent->parent->color = Node::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(z);
                }
                z->parent->color = Node::BLACK;
                z->parent->parent->color = Node::RED;
                rotateLeft(z->parent->parent);
            }
        }
    }
    root->color = Node::BLACK;
}

template <typename T>
std::pair<typename Tree<T>::iterator, bool> Tree<T>::insert(const T& value) {
    Node* y = nil;
    Node* x = root;
    
    while (x != nil) {
        y = x;
        if (value < x->val) {
            x = x->left;
        } else if (x->val < value) {
            x = x->right;
        } else {
            // Элемент уже существует
            return std::make_pair(iterator(x, nil, this), false);
        }
    }
    
    Node* z = new Node(value);
    z->parent = y;
    z->left = nil;
    z->right = nil;
    z->color = Node::RED;
    
    if (y == nil) {
        root = z;
    } else if (z->val < y->val) {
        y->left = z;
    } else {
        y->right = z;
    }
    
    treeSize++;
    fixInsert(z);
    
    return std::make_pair(iterator(z, nil, this), true);
}

template <typename T>
void Tree<T>::transplant(Node* u, Node* v) {
    if (u->parent == nil) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

template <typename T>
typename Tree<T>::Node* Tree<T>::minimum(Node* node) const {
    while (node->left != nil) {
        node = node->left;
    }
    return node;
}

template <typename T>
typename Tree<T>::Node* Tree<T>::maximum(Node* node) const {
    while (node->right != nil) {
        node = node->right;
    }
    return node;
}

template <typename T>
typename Tree<T>::Node* Tree<T>::successor(Node* node) const {
    if (node->right != nil) {
        return minimum(node->right);
    }
    
    Node* y = node->parent;
    while (y != nil && node == y->right) {
        node = y;
        y = y->parent;
    }
    return y;
}

template <typename T>
typename Tree<T>::Node* Tree<T>::predecessor(Node* node) const {
    if (node->left != nil) {
        return maximum(node->left);
    }
    
    Node* y = node->parent;
    while (y != nil && node == y->left) {
        node = y;
        y = y->parent;
    }
    return y;
}

template <typename T>
void Tree<T>::fixDelete(Node* x) {
    while (x != root && x->color == Node::BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == Node::RED) {
                w->color = Node::BLACK;
                x->parent->color = Node::RED;
                rotateLeft(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == Node::BLACK && w->right->color == Node::BLACK) {
                w->color = Node::RED;
                x = x->parent;
            } else {
                if (w->right->color == Node::BLACK) {
                    w->left->color = Node::BLACK;
                    w->color = Node::RED;
                    rotateRight(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = Node::BLACK;
                w->right->color = Node::BLACK;
                rotateLeft(x->parent);
                x = root;
            }
        } else {
            Node* w = x->parent->left;
            if (w->color == Node::RED) {
                w->color = Node::BLACK;
                x->parent->color = Node::RED;
                rotateRight(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == Node::BLACK && w->left->color == Node::BLACK) {
                w->color = Node::RED;
                x = x->parent;
            } else {
                if (w->left->color == Node::BLACK) {
                    w->right->color = Node::BLACK;
                    w->color = Node::RED;
                    rotateLeft(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = Node::BLACK;
                w->left->color = Node::BLACK;
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    x->color = Node::BLACK;
}

template <typename T>
typename Tree<T>::size_type Tree<T>::erase(const T& value) {
    Node* z = search(root, value);
    if (z == nil) {
        return 0;
    }
    
    Node* y = z;
    Node* x;
    typename Node::Color yOriginalColor = y->color;
    
    if (z->left == nil) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nil) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    delete z;
    treeSize--;
    
    if (yOriginalColor == Node::BLACK) {
        fixDelete(x);
    }
    
    return 1;
}

template <typename T>
typename Tree<T>::Node* Tree<T>::search(Node* node, const T& value) const {
    while (node != nil && node->val != value) {
        if (value < node->val) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return node;
}

template <typename T>
typename Tree<T>::iterator Tree<T>::find(const T& value) {
    Node* node = search(root, value);
    if (node == nil) {
        return end();
    }
    return iterator(node, nil, this);
}

template <typename T>
typename Tree<T>::iterator Tree<T>::begin() {
    if (root == nil) {
        return end();
    }
    return iterator(minimum(root), nil, this);
}

template <typename T>
typename Tree<T>::iterator Tree<T>::end() {
    return iterator(nil, nil, this);
}

template <typename T>
typename Tree<T>::iterator Tree<T>::begin() const {
    if (root == nil) {
        return end();
    }
    return iterator(minimum(root), nil, this);
}

template <typename T>
typename Tree<T>::iterator Tree<T>::end() const {
    return iterator(nil, nil, this);
}

template <typename T>
typename Tree<T>::iterator Tree<T>::cbegin() const {
    return begin();
}

template <typename T>
typename Tree<T>::iterator Tree<T>::cend() const {
    return end();
}

template <typename T>
typename Tree<T>::size_type Tree<T>::size() const {
    return treeSize;
}

template <typename T>
bool Tree<T>::empty() const {
    return treeSize == 0;
}

template <typename T>
void Tree<T>::clearRecursive(Node* node) {
    if (node != nil && node != nullptr) {
        clearRecursive(node->left);
        clearRecursive(node->right);
        delete node;
    }
}

template <typename T>
void Tree<T>::clear() {
    clearRecursive(root);
    root = nil;
    treeSize = 0;
}

#endif // TREE_HPP
