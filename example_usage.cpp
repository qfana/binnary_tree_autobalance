#include <iostream>
#include "tree/tree.hpp"
#include "constructor_utils/constructor_utils.hpp"

int main() {
    try {
        // Чтение дерева из файла
        Tree<int> tree = ConstructorsUtil<int>::fromEdgeList("tree_example.txt");
        
        std::cout << "Tree loaded from file!" << std::endl;
        std::cout << "Size: " << tree.size() << std::endl;
        
        // Выводим все элементы дерева
        std::cout << "Elements (in-order): ";
        for (auto it = tree.begin(); it != tree.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        
        // Проверяем наличие элементов
        auto it = tree.find(5);
        if (it != tree.end()) {
            std::cout << "Found element 5!" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

