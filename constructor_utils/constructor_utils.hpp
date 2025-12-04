#ifndef CONSTRUCTOR_UTILS_HPP
#define CONSTRUCTOR_UTILS_HPP

#include "../tree/tree.hpp"
#include <string>
#include <string_view>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <functional>
#include <vector>
#include <iostream>
#include <stdexcept>

template <typename T>
class ConstructorsUtil {
public:
    // Конструктор из файла со списком ребер
    // Формат: value left right (каждая строка)
    // Пример:
    // 5 2 7
    // 2 1 3
    // 7 6 8
    static Tree<T> fromEdgeList(std::string_view filename);
    
private:
    struct EdgeData {
        T value;
        T left;
        T right;
    };
    
    static std::vector<EdgeData> parseEdgeList(std::string_view filename);
};

template <typename T>
std::vector<typename ConstructorsUtil<T>::EdgeData> ConstructorsUtil<T>::parseEdgeList(std::string_view filename) {
    std::vector<EdgeData> edges;
    std::string filenameStr(filename);
    std::ifstream file(filenameStr);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filenameStr);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue; // Пропускаем пустые строки и комментарии
        }
        
        std::istringstream iss(line);
        EdgeData edge;
        
        if (iss >> edge.value >> edge.left >> edge.right) {
            edges.push_back(edge);
        } else {
            std::cerr << "Warning: Invalid line format: " << line << std::endl;
        }
    }
    
    file.close();
    return edges;
}

template <typename T>
Tree<T> ConstructorsUtil<T>::fromEdgeList(std::string_view filename) {
    Tree<T> tree;
    std::vector<EdgeData> edges = parseEdgeList(filename);
    
    if (edges.empty()) {
        return tree;
    }
    
    // Создаем карту связей: значение -> (левое, правое)
    std::map<T, std::pair<T, T>> connections;
    for (const auto& edge : edges) {
        connections[edge.value] = {edge.left, edge.right};
    }
    
    // Находим корень (узел, который не является потомком)
    std::set<T> children;
    for (const auto& edge : edges) {
        if (edge.left != T{}) children.insert(edge.left);
        if (edge.right != T{}) children.insert(edge.right);
    }
    
    T rootValue = T{};
    for (const auto& edge : edges) {
        if (children.find(edge.value) == children.end()) {
            rootValue = edge.value;
            break;
        }
    }
    
    if (rootValue == T{} && !edges.empty()) {
        rootValue = edges[0].value;
    }
    
    // Рекурсивно строим дерево
    std::function<void(T)> buildTree = [&](T val) {
        if (val == T{}) return;
        
        tree.insert(val);
        
        auto it = connections.find(val);
        if (it != connections.end()) {
            buildTree(it->second.first);
            buildTree(it->second.second);
        }
    };
    
    if (rootValue != T{}) {
        buildTree(rootValue);
    }
    
    return tree;
}

#endif // CONSTRUCTOR_UTILS_HPP
