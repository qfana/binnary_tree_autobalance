#include "tree.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <queue>

// Вспомогательные функции для красно-черного дерева
// (реализация rotateLeft, rotateRight, fixInsert, fixDelete, transplant
// остается такой же, как в предыдущей версии)

template<typename T>
std::vector<typename RedBlackTree<T>::Node*> RedBlackTree<T>::getLevelOrder() const {
    std::vector<Node*> result;
    if (!root) return result;
    
    std::queue<Node*> q;
    q.push(root);
    
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        
        result.push_back(current);
        
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
    
    return result;
}

template<typename T>
void RedBlackTree<T>::invalidateIterators() {
    // В реальной реализации здесь нужно было бы уведомлять
    // все активные итераторы об изменении дерева.
    // В упрощенной версии мы полагаемся на то, что итераторы
    // сами перестроят кэш при необходимости.
}

// Конструкторы и деструкторы
template<typename T>
RedBlackTree<T>::RedBlackTree() : root(nullptr), tree_size(0) {}

template<typename T>
RedBlackTree<T>::RedBlackTree(std::initializer_list<T> init) : RedBlackTree() {
    for (const auto& value : init) {
        insert(value);
    }
}

template<typename T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree& other) 
    : root(copyTree(other.root, nullptr)), tree_size(other.tree_size) {}

template<typename T>
RedBlackTree<T>::RedBlackTree(RedBlackTree&& other) noexcept 
    : root(other.root), tree_size(other.tree_size) {
    other.root = nullptr;
    other.tree_size = 0;
}

template<typename T>
RedBlackTree<T>::~RedBlackTree() {
    clear();
}

template<typename T>
RedBlackTree<T>& RedBlackTree<T>::operator=(const RedBlackTree& other) {
    if (this != &other) {
        clear();
        root = copyTree(other.root, nullptr);
        tree_size = other.tree_size;
        invalidateIterators();
    }
    return *this;
}

template<typename T>
RedBlackTree<T>& RedBlackTree<T>::operator=(RedBlackTree&& other) noexcept {
    if (this != &other) {
        clear();
        root = other.root;
        tree_size = other.tree_size;
        other.root = nullptr;
        other.tree_size = 0;
        invalidateIterators();
    }
    return *this;
}

// Основные операции
template<typename T>
std::pair<typename RedBlackTree<T>::iterator, bool> RedBlackTree<T>::insert(const T& value) {
    Node* parent = nullptr;
    Node* current = root;
    
    while (current != nullptr) {
        parent = current;
        if (value < current->data) {
            current = current->left;
        } else if (current->data < value) {
            current = current->right;
        } else {
            // Элемент уже существует
            return std::make_pair(iterator(current, this), false);
        }
    }
    
    Node* new_node = new Node(value, parent);
    
    if (parent == nullptr) {
        root = new_node;
    } else if (value < parent->data) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
    
    tree_size++;
    invalidateIterators();
    
    if (new_node->parent == nullptr) {
        new_node->is_red = false;
        return std::make_pair(iterator(new_node, this), true);
    }
    
    if (new_node->parent->parent == nullptr) {
        return std::make_pair(iterator(new_node, this), true);
    }
    
    fixInsert(new_node);
    return std::make_pair(iterator(new_node, this), true);
}

template<typename T>
size_t RedBlackTree<T>::erase(const T& value) {
    Node* node = root;
    Node* z = nullptr;
    
    // Поиск узла для удаления
    while (node != nullptr) {
        if (node->data < value) {
            node = node->right;
        } else if (value < node->data) {
            node = node->left;
        } else {
            z = node;
            break;
        }
    }
    
    if (z == nullptr) {
        return 0; // Элемент не найден
    }
    
    // (Реализация удаления из красно-черного дерева остается той же)
    // ...
    
    // После удаления
    tree_size--;
    invalidateIterators();
    
    return 1;
}

template<typename T>
typename RedBlackTree<T>::iterator RedBlackTree<T>::begin() {
    if (root == nullptr) return end();
    
    // Для level-order итератора begin() - это первый элемент в level-order обходе
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        
        // Первый элемент в level-order
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
        
        // Возвращаем итератор на корень (первый элемент level-order)
        return iterator(root, this);
    }
    
    return iterator(root, this);
}

template<typename T>
typename RedBlackTree<T>::const_iterator RedBlackTree<T>::begin() const {
    if (root == nullptr) return end();
    return const_iterator(root, this);
}

template<typename T>
typename RedBlackTree<T>::const_iterator RedBlackTree<T>::cbegin() const {
    return begin();
}

template<typename T>
typename RedBlackTree<T>::iterator RedBlackTree<T>::end() {
    return iterator(nullptr, this);
}

template<typename T>
typename RedBlackTree<T>::const_iterator RedBlackTree<T>::end() const {
    return const_iterator(nullptr, this);
}

template<typename T>
typename RedBlackTree<T>::const_iterator RedBlackTree<T>::cend() const {
    return end();
}

template<typename T>
void RedBlackTree<T>::printLevelOrder() const {
    if (!root) {
        std::cout << "Tree is empty" << std::endl;
        return;
    }
    
    std::queue<Node*> q;
    q.push(root);
    int level = 0;
    
    while (!q.empty()) {
        int level_size = q.size();
        std::cout << "Level " << level << ": ";
        
        for (int i = 0; i < level_size; ++i) {
            Node* current = q.front();
            q.pop();
            
            std::cout << current->data;
            if (current->is_red) std::cout << "(R)";
            else std::cout << "(B)";
            
            if (i < level_size - 1) std::cout << ", ";
            
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        
        std::cout << std::endl;
        level++;
    }
}