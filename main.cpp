#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include "tree/tree.hpp"
#include "constructor_utils/constructor_utils.hpp"

void testBasicOperations() {
    std::cout << "=== Basic Operations Test ===" << std::endl;
    
    Tree<int> tree;
    
    // Тест empty и size
    std::cout << "Empty tree: empty=" << tree.empty() << ", size=" << tree.size() << std::endl;
    
    // Тест insert
    auto result1 = tree.insert(5);
    std::cout << "Insert 5: success=" << result1.second << std::endl;
    
    auto result2 = tree.insert(3);
    std::cout << "Insert 3: success=" << result2.second << std::endl;
    
    auto result3 = tree.insert(7);
    std::cout << "Insert 7: success=" << result3.second << std::endl;
    
    auto result4 = tree.insert(5); // Дубликат
    std::cout << "Insert 5 (duplicate): success=" << result4.second << std::endl;
    
    std::cout << "Size after inserts: " << tree.size() << std::endl;
    
    // Тест find
    auto it = tree.find(3);
    if (it != tree.end()) {
        std::cout << "Found element: " << *it << std::endl;
    }
    
    auto it2 = tree.find(10);
    if (it2 == tree.end()) {
        std::cout << "Element 10 not found (as expected)" << std::endl;
    }
    
    // Тест erase
    size_t erased = tree.erase(3);
    std::cout << "Erased elements: " << erased << ", new size: " << tree.size() << std::endl;
    
    erased = tree.erase(10); // Несуществующий
    std::cout << "Attempt to erase 10: erased=" << erased << std::endl;
    
    std::cout << std::endl;
}

void testIterator() {
    std::cout << "=== Iterator Test ===" << std::endl;
    
    Tree<int> tree;
    std::vector<int> values = {5, 3, 7, 2, 4, 6, 8};
    
    for (int val : values) {
        tree.insert(val);
    }
    
    std::cout << "Tree traversal (in-order): ";
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Reverse traversal: ";
    auto it = tree.end();
    --it; // Переходим к последнему элементу
    while (it != tree.begin()) {
        std::cout << *it << " ";
        --it;
    }
    std::cout << *it << std::endl;
    
    std::cout << std::endl;
}

void testSTLAlgorithms() {
    std::cout << "=== STL Algorithms Test ===" << std::endl;
    
    Tree<int> tree;
    std::vector<int> values = {5, 3, 7, 2, 4, 6, 8, 1, 9};
    
    for (int val : values) {
        tree.insert(val);
    }
    
    // std::find
    auto it = std::find(tree.begin(), tree.end(), 4);
    if (it != tree.end()) {
        std::cout << "std::find found: " << *it << std::endl;
    }
    
    // std::count
    size_t count = std::count(tree.begin(), tree.end(), 5);
    std::cout << "std::count for 5: " << count << std::endl;
    
    // std::for_each
    std::cout << "std::for_each (doubling values): ";
    std::for_each(tree.begin(), tree.end(), [](int& val) {
        std::cout << val * 2 << " ";
    });
    std::cout << std::endl;
    
    // std::lower_bound (требует random_access, но попробуем)
    // Для bidirectional итератора используем ручной поиск
    int target = 5;
    auto lower = std::lower_bound(tree.begin(), tree.end(), target);
    if (lower != tree.end()) {
        std::cout << "std::lower_bound for " << target << ": " << *lower << std::endl;
    }
    
    std::cout << std::endl;
}

void testConstructors() {
    std::cout << "=== Constructors Test ===" << std::endl;
    
    // Конструктор по умолчанию
    Tree<int> tree1;
    tree1.insert(1);
    tree1.insert(2);
    std::cout << "Default constructor: size=" << tree1.size() << std::endl;
    
    // Копирующий конструктор
    Tree<int> tree2(tree1);
    std::cout << "Copy constructor: size=" << tree2.size() << std::endl;
    
    // Перемещающий конструктор
    Tree<int> tree3(std::move(tree1));
    std::cout << "Move constructor: size=" << tree3.size() << ", original tree: size=" << tree1.size() << std::endl;
    
    // Оператор присваивания
    Tree<int> tree4;
    tree4 = tree2;
    std::cout << "Copy assignment operator: size=" << tree4.size() << std::endl;
    
    Tree<int> tree5;
    tree5 = std::move(tree2);
    std::cout << "Move assignment operator: size=" << tree5.size() << std::endl;
    
    std::cout << std::endl;
}

void testFromFile() {
    std::cout << "=== File Constructor Test ===" << std::endl;
    
    try {
        // Читаем дерево из файла tree_example.txt
        Tree<int> tree = ConstructorsUtil<int>::fromEdgeList("tree_example.txt");
        std::cout << "Tree from file: size=" << tree.size() << std::endl;
        std::cout << "Elements: ";
        for (auto it = tree.begin(); it != tree.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    std::cout << std::endl;
}

int main() {
    try {
        testBasicOperations();
        testIterator();
        testSTLAlgorithms();
        testConstructors();
        testFromFile();
        
        std::cout << "All tests completed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
