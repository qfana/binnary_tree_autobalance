#include "tree.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <queue>

// Вспомогательные методы для красно-черного дерева
template<typename T>
void RedBlackTree<T>::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    
    if (y->left != nullptr) {
        y->left->parent = x;
    }
    
    y->parent = x->parent;
    
    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
}

template<typename T>
void RedBlackTree<T>::rotateRight(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    
    if (x->right != nullptr) {
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    
    if (y->parent == nullptr) {
        root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    
    x->right = y;
    y->parent = x;
}

template<typename T>
void RedBlackTree<T>::fixInsert(Node* node) {
    while (node != root && node->parent->is_red) {
        if (node->parent == node->parent->parent->left) {
            Node* uncle = node->parent->parent->right;
            
            if (uncle != nullptr && uncle->is_red) {
                // Case 1: Uncle is red
                node->parent->is_red = false;
                uncle->is_red = false;
                node->parent->parent->is_red = true;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    // Case 2: Node is right child
                    node = node->parent;
                    rotateLeft(node);
                }
                // Case 3: Node is left child
                node->parent->is_red = false;
                node->parent->parent->is_red = true;
                rotateRight(node->parent->parent);
            }
        } else {
            Node* uncle = node->parent->parent->left;
            
            if (uncle != nullptr && uncle->is_red) {
                // Case 1: Uncle is red
                node->parent->is_red = false;
                uncle->is_red = false;
                node->parent->parent->is_red = true;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    // Case 2: Node is left child
                    node = node->parent;
                    rotateRight(node);
                }
                // Case 3: Node is right child
                node->parent->is_red = false;
                node->parent->parent->is_red = true;
                rotateLeft(node->parent->parent);
            }
        }
    }
    
    root->is_red = false;
}

template<typename T>
void RedBlackTree<T>::transplant(Node* u, Node* v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

template<typename T>
typename RedBlackTree<T>::Node* RedBlackTree<T>::minimum(Node* node) const {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

template<typename T>
typename RedBlackTree<T>::Node* RedBlackTree<T>::maximum(Node* node) const {
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}

template<typename T>
typename RedBlackTree<T>::Node* RedBlackTree<T>::successor(Node* node) const {
    if (node->right != nullptr) {
        return minimum(node->right);
    }
    
    Node* parent = node->parent;
    while (parent != nullptr && node == parent->right) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

template<typename T>
typename RedBlackTree<T>::Node* RedBlackTree<T>::predecessor(Node* node) const {
    if (node->left != nullptr) {
        return maximum(node->left);
    }
    
    Node* parent = node->parent;
    while (parent != nullptr && node == parent->left) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

template<typename T>
void RedBlackTree<T>::fixDelete(Node* node) {
    while (node != root && (node == nullptr || !node->is_red)) {
        if (node == node->parent->left) {
            Node* sibling = node->parent->right;
            
            if (sibling->is_red) {
                // Case 1: Sibling is red
                sibling->is_red = false;
                node->parent->is_red = true;
                rotateLeft(node->parent);
                sibling = node->parent->right;
            }
            
            if ((sibling->left == nullptr || !sibling->left->is_red) &&
                (sibling->right == nullptr || !sibling->right->is_red)) {
                // Case 2: Both children of sibling are black
                sibling->is_red = true;
                node = node->parent;
            } else {
                if (sibling->right == nullptr || !sibling->right->is_red) {
                    // Case 3: Sibling's right child is black
                    if (sibling->left != nullptr) {
                        sibling->left->is_red = false;
                    }
                    sibling->is_red = true;
                    rotateRight(sibling);
                    sibling = node->parent->right;
                }
                
                // Case 4: Sibling's right child is red
                sibling->is_red = node->parent->is_red;
                node->parent->is_red = false;
                if (sibling->right != nullptr) {
                    sibling->right->is_red = false;
                }
                rotateLeft(node->parent);
                node = root;
            }
        } else {
            Node* sibling = node->parent->left;
            
            if (sibling->is_red) {
                // Case 1: Sibling is red
                sibling->is_red = false;
                node->parent->is_red = true;
                rotateRight(node->parent);
                sibling = node->parent->left;
            }
            
            if ((sibling->right == nullptr || !sibling->right->is_red) &&
                (sibling->left == nullptr || !sibling->left->is_red)) {
                // Case 2: Both children of sibling are black
                sibling->is_red = true;
                node = node->parent;
            } else {
                if (sibling->left == nullptr || !sibling->left->is_red) {
                    // Case 3: Sibling's left child is black
                    if (sibling->right != nullptr) {
                        sibling->right->is_red = false;
                    }
                    sibling->is_red = true;
                    rotateLeft(sibling);
                    sibling = node->parent->left;
                }
                
                // Case 4: Sibling's left child is red
                sibling->is_red = node->parent->is_red;
                node->parent->is_red = false;
                if (sibling->left != nullptr) {
                    sibling->left->is_red = false;
                }
                rotateRight(node->parent);
                node = root;
            }
        }
    }
    
    if (node != nullptr) {
        node->is_red = false;
    }
}

// Методы для level-order итератора
template<typename T>
typename RedBlackTree<T>::Node* RedBlackTree<T>::getNextInLevelOrder(Node* current) const {
    if (!current || !root) return nullptr;
    
    std::vector<Node*> traversal = generateLevelOrder();
    
    for (size_t i = 0; i < traversal.size(); ++i) {
        if (traversal[i] == current) {
            if (i + 1 < traversal.size()) {
                return traversal[i + 1];
            }
            break;
        }
    }
    
    return nullptr;
}

template<typename T>
typename RedBlackTree<T>::Node* RedBlackTree<T>::getPrevInLevelOrder(Node* current) const {
    if (!current || !root) return nullptr;
    
    std::vector<Node*> traversal = generateLevelOrder();
    
    for (size_t i = 0; i < traversal.size(); ++i) {
        if (traversal[i] == current) {
            if (i > 0) {
                return traversal[i - 1];
            }
            break;
        }
    }
    
    return nullptr;
}

template<typename T>
std::vector<typename RedBlackTree<T>::Node*> RedBlackTree<T>::generateLevelOrder() const {
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
typename RedBlackTree<T>::Node* RedBlackTree<T>::findNode(const T& value) const {
    Node* current = root;
    
    while (current != nullptr) {
        if (value < current->data) {
            current = current->left;
        } else if (current->data < value) {
            current = current->right;
        } else {
            return current;
        }
    }
    
    return nullptr;
}

// Методы проверки свойств
template<typename T>
bool RedBlackTree<T>::isBST(Node* node, T min, T max) const {
    if (!node) return true;
    
    if (node->data < min || max < node->data) {
        return false;
    }
    
    return isBST(node->left, min, node->data) && 
           isBST(node->right, node->data, max);
}

template<typename T>
bool RedBlackTree<T>::hasNoDoubleRed(Node* node) const {
    if (!node) return true;
    
    if (node->is_red) {
        if (node->left && node->left->is_red) return false;
        if (node->right && node->right->is_red) return false;
    }
    
    return hasNoDoubleRed(node->left) && hasNoDoubleRed(node->right);
}

template<typename T>
int RedBlackTree<T>::checkBlackHeight(Node* node) const {
    if (!node) return 1;
    
    int leftBlackHeight = checkBlackHeight(node->left);
    int rightBlackHeight = checkBlackHeight(node->right);
    
    if (leftBlackHeight == -1 || rightBlackHeight == -1 || 
        leftBlackHeight != rightBlackHeight) {
        return -1;
    }
    
    return leftBlackHeight + (node->is_red ? 0 : 1);
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
typename RedBlackTree<T>::Node* RedBlackTree<T>::copyTree(Node* node, Node* parent) {
    if (node == nullptr) return nullptr;
    
    Node* new_node = new Node(node->data, parent);
    new_node->is_red = node->is_red;
    new_node->left = copyTree(node->left, new_node);
    new_node->right = copyTree(node->right, new_node);
    
    return new_node;
}

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
void RedBlackTree<T>::clear(Node* node) {
    if (node == nullptr) return;
    
    clear(node->left);
    clear(node->right);
    delete node;
}

// Операторы присваивания
template<typename T>
RedBlackTree<T>& RedBlackTree<T>::operator=(const RedBlackTree& other) {
    if (this != &other) {
        clear();
        root = copyTree(other.root, nullptr);
        tree_size = other.tree_size;
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
    
    if (new_node->parent == nullptr) {
        new_node->is_red = false;
    } else if (new_node->parent->parent != nullptr) {
        fixInsert(new_node);
    }
    
    return std::make_pair(iterator(new_node, this), true);
}

template<typename T>
void RedBlackTree<T>::insertFromConstructor(const T& value) {
    Node* parent = nullptr;
    Node* current = root;
    
    while (current != nullptr) {
        parent = current;
        if (value < current->data) {
            current = current->left;
        } else if (current->data < value) {
            current = current->right;
        } else {
            return; // Элемент уже существует
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
    
    if (new_node->parent == nullptr) {
        new_node->is_red = false;
    } else if (new_node->parent->parent != nullptr) {
        fixInsert(new_node);
    }
}

template<typename T>
size_t RedBlackTree<T>::erase(const T& value) {
    Node* z = findNode(value);
    if (z == nullptr) {
        return 0;
    }
    
    Node* y = z;
    Node* x = nullptr;
    bool y_original_color = y->is_red;
    
    if (z->left == nullptr) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->is_red;
        x = y->right;
        
        if (y->parent == z) {
            if (x != nullptr) {
                x->parent = y;
            }
        } else {
            transplant(y, y->right);
            y->right = z->right;
            if (y->right != nullptr) {
                y->right->parent = y;
            }
        }
        
        transplant(z, y);
        y->left = z->left;
        if (y->left != nullptr) {
            y->left->parent = y;
        }
        y->is_red = z->is_red;
    }
    
    delete z;
    tree_size--;
    
    if (!y_original_color && x != nullptr) {
        fixDelete(x);
    }
    
    return 1;
}

template<typename T>
typename RedBlackTree<T>::iterator RedBlackTree<T>::find(const T& value) {
    Node* node = findNode(value);
    if (node) {
        return iterator(node, this);
    }
    return end();
}

template<typename T>
typename RedBlackTree<T>::const_iterator RedBlackTree<T>::find(const T& value) const {
    Node* node = findNode(value);
    if (node) {
        return const_iterator(node, this);
    }
    return end();
}

// Итераторы
template<typename T>
typename RedBlackTree<T>::iterator RedBlackTree<T>::begin() {
    if (!root) return end();
    return iterator(root, this);
}

template<typename T>
typename RedBlackTree<T>::const_iterator RedBlackTree<T>::begin() const {
    if (!root) return end();
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

// Вспомогательные методы
template<typename T>
void RedBlackTree<T>::clear() {
    clear(root);
    root = nullptr;
    tree_size = 0;
}

template<typename T>
bool RedBlackTree<T>::isValidRedBlackTree() const {
    if (!root) return true;
    
    // Свойство 2: Корень черный
    if (root->is_red) {
        return false;
    }
    
    // Свойство 4: Нет двух красных узлов подряд
    if (!hasNoDoubleRed(root)) {
        return false;
    }
    
    // Свойство 5: Черная высота одинакова для всех путей
    if (checkBlackHeight(root) == -1) {
        return false;
    }
    
    // Проверяем что дерево BST
    T minVal = std::numeric_limits<T>::lowest();
    T maxVal = std::numeric_limits<T>::max();
    
    if (!isBST(root, minVal, maxVal)) {
        return false;
    }
    
    return true;
}

template<typename T>
int RedBlackTree<T>::getBlackHeight() const {
    int height = checkBlackHeight(root);
    return (height == -1) ? 0 : height;
}

// Отладочный вывод
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

// Явное инстанцирование для часто используемых типов
template class RedBlackTree<int>;
template class RedBlackTree<unsigned int>;
template class RedBlackTree<long>;
template class RedBlackTree<long long>;
template class RedBlackTree<float>;
template class RedBlackTree<double>;
template class RedBlackTree<std::string>;