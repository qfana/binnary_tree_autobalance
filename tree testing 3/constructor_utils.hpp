#ifndef CONSTRUCTOR_UTILS_HPP
#define CONSTRUCTOR_UTILS_HPP

#include "tree.h"
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
    // Конструктор из файла со списком ребер (Вариант D)
    static RedBlackTree<T> fromEdgeList(std::string_view filename);
    
private:
    struct EdgeData {
        T value;
        T left;
        T right;
    };
    
    static std::vector<EdgeData> parseEdgeList(std::string_view filename);
    static T getDefaultValue();
};

template <typename T>
T ConstructorsUtil<T>::getDefaultValue() {
    return T{};
}

template <>
std::string ConstructorsUtil<std::string>::getDefaultValue() {
    return "";
}

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
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        EdgeData edge;
        T defaultVal = getDefaultValue();
        
        if (iss >> edge.value) {
            if (!(iss >> edge.left)) {
                edge.left = defaultVal;
            }
            if (!(iss >> edge.right)) {
                edge.right = defaultVal;
            }
            edges.push_back(edge);
        }
    }
    
    file.close();
    return edges;
}

template <typename T>
RedBlackTree<T> ConstructorsUtil<T>::fromEdgeList(std::string_view filename) {
    RedBlackTree<T> tree;
    std::vector<EdgeData> edges = parseEdgeList(filename);
    
    if (edges.empty()) {
        return tree;
    }
    
    std::map<T, std::pair<T, T>> connections;
    std::set<T> allNodes;
    
    for (const auto& edge : edges) {
        connections[edge.value] = {edge.left, edge.right};
        allNodes.insert(edge.value);
        if (edge.left != getDefaultValue()) allNodes.insert(edge.left);
        if (edge.right != getDefaultValue()) allNodes.insert(edge.right);
    }
    
    std::set<T> children;
    for (const auto& edge : edges) {
        if (edge.left != getDefaultValue()) children.insert(edge.left);
        if (edge.right != getDefaultValue()) children.insert(edge.right);
    }
    
    T rootValue = getDefaultValue();
    for (const auto& node : allNodes) {
        if (children.find(node) == children.end()) {
            rootValue = node;
            break;
        }
    }
    
    if (rootValue == getDefaultValue() && !edges.empty()) {
        rootValue = edges[0].value;
    }
    
    if (rootValue != getDefaultValue()) {
        std::queue<T> nodesToProcess;
        std::set<T> processedNodes;
        
        nodesToProcess.push(rootValue);
        
        while (!nodesToProcess.empty()) {
            T current = nodesToProcess.front();
            nodesToProcess.pop();
            
            if (processedNodes.find(current) != processedNodes.end()) {
                continue;
            }
            
            tree.insertFromConstructor(current);
            processedNodes.insert(current);
            
            auto it = connections.find(current);
            if (it != connections.end()) {
                T leftChild = it->second.first;
                T rightChild = it->second.second;
                
                if (leftChild != getDefaultValue()) {
                    nodesToProcess.push(leftChild);
                }
                if (rightChild != getDefaultValue()) {
                    nodesToProcess.push(rightChild);
                }
            }
        }
    }
    
    return tree;
}

#endif // CONSTRUCTOR_UTILS_HPP