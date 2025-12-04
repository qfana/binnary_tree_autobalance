#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include "tree.h"
#include "constructor_utils.hpp"

void demonstrateSTLCompatibility() {
    std::cout << "=== STL Compatibility Demonstration ===" << std::endl;
    
    RedBlackTree<int> tree = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 85};
    
    // 1. size() и empty()
    std::cout << "1. Size: " << tree.size() << std::endl;
    std::cout << "   Empty: " << std::boolalpha << tree.empty() << std::endl;
    
    // 2. Итерация с range-based for
    std::cout << "2. Level-order traversal: ";
    for (int val : tree) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 3. Использование с STL алгоритмами
    std::cout << "3. Using STL algorithms:" << std::endl;
    
    // std::find
    auto it = std::find(tree.begin(), tree.end(), 40);
    if (it != tree.end()) {
        std::cout << "   Found 40: " << *it << std::endl;
    }
    
    // std::count
    int count = std::count(tree.begin(), tree.end(), 50);
    std::cout << "   Count of 50: " << count << std::endl;
    
    // std::for_each
    std::cout << "   Values * 2: ";
    std::for_each(tree.begin(), tree.end(), [](int x) {
        std::cout << x * 2 << " ";
    });
    std::cout << std::endl;
    
    // std::copy в вектор
    std::vector<int> vec;
    std::copy(tree.begin(), tree.end(), std::back_inserter(vec));
    std::cout << "   Copied to vector, size: " << vec.size() << std::endl;
    
    // 4. Обратная итерация
    std::cout << "4. Reverse traversal: ";
    auto rit = tree.end();
    while (rit != tree.begin()) {
        --rit;
        std::cout << *rit << " ";
    }
    std::cout << std::endl;
    
    // 5. insert() возвращает pair<iterator, bool>
    std::cout << "5. Testing insert():" << std::endl;
    auto [insert_it, inserted] = tree.insert(42);
    std::cout << "   Inserted 42: " << inserted << ", value: " << *insert_it << std::endl;
    
    auto [dup_it, dup_inserted] = tree.insert(50);
    std::cout << "   Inserted 50 again: " << dup_inserted << std::endl;
    
    // 6. find() и erase()
    std::cout << "6. Testing find() and erase():" << std::endl;
    auto find_it = tree.find(30);
    if (find_it != tree.end()) {
        std::cout << "   Found 30: " << *find_it << std::endl;
    }
    
    size_t erased = tree.erase(30);
    std::cout << "   Erased 30: " << erased << " element(s)" << std::endl;
    std::cout << "   New size: " << tree.size() << std::endl;
    
    // 7. Проверка свойств красно-черного дерева
    std::cout << "7. Red-Black Tree properties:" << std::endl;
    if (tree.isValidRedBlackTree()) {
        std::cout << "   ✓ Valid Red-Black Tree" << std::endl;
        std::cout << "   Black height: " << tree.getBlackHeight() << std::endl;
    } else {
        std::cout << "   ✗ Invalid Red-Black Tree" << std::endl;
    }
    
    // 8. Вывод дерева по уровням
    std::cout << "8. Tree structure (level-order):" << std::endl;
    tree.printLevelOrder();
}

void testConstructorFromFile() {
    std::cout << "\n=== Testing Constructor from File ===" << std::endl;
    
    // Создаем тестовый файл
    std::ofstream file("test_tree.txt");
    file << "# Binary Search Tree structure\n";
    file << "50 30 70\n";
    file << "30 20 40\n";
    file << "70 60 80\n";
    file << "20 10 25\n";
    file << "40 35 45\n";
    file << "60 55 65\n";
    file << "80 75 85\n";
    file.close();
    
    try {
        RedBlackTree<int> tree = ConstructorsUtil<int>::fromEdgeList("test_tree.txt");
        
        std::cout << "Tree loaded successfully!" << std::endl;
        std::cout << "Size: " << tree.size() << std::endl;
        
        std::cout << "\nLevel-order traversal: ";
        for (int val : tree) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        
        std::cout << "\nTree structure:" << std::endl;
        tree.printLevelOrder();
        
        // Проверяем свойства
        if (tree.isValidRedBlackTree()) {
            std::cout << "\n✓ Tree is a valid Red-Black Tree" << std::endl;
        } else {
            std::cout << "\n✗ Tree is NOT a valid Red-Black Tree" << std::endl;
        }
        
        // Удаляем тестовый файл
        std::remove("test_tree.txt");
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void testAllConstructors() {
    std::cout << "\n=== Testing All Constructors ===" << std::endl;
    
    // 1. Конструктор по умолчанию
    RedBlackTree<int> tree1;
    std::cout << "1. Default constructor: size = " << tree1.size() << std::endl;
    
    // 2. Конструктор из initializer_list
    RedBlackTree<int> tree2 = {100, 50, 150, 25, 75, 125, 175};
    std::cout << "2. Initializer list constructor: size = " << tree2.size() << std::endl;
    std::cout << "   Level-order: ";
    for (int val : tree2) std::cout << val << " ";
    std::cout << std::endl;
    
    // 3. Копирующий конструктор
    RedBlackTree<int> tree3 = tree2;
    std::cout << "3. Copy constructor: size = " << tree3.size() << std::endl;
    std::cout << "   Trees equal: " << std::boolalpha 
              << (tree2.size() == tree3.size()) << std::endl;
    
    // 4. Перемещающий конструктор
    RedBlackTree<int> tree4 = std::move(tree3);
    std::cout << "4. Move constructor:" << std::endl;
    std::cout << "   Original size after move: " << tree3.size() << std::endl;
    std::cout << "   New tree size: " << tree4.size() << std::endl;
    
    // 5. Копирующее присваивание
    tree1 = tree4;
    std::cout << "5. Copy assignment: size = " << tree1.size() << std::endl;
    
    // 6. Перемещающее присваивание
    RedBlackTree<int> tree5;
    tree5 = std::move(tree4);
    std::cout << "6. Move assignment:" << std::endl;
    std::cout << "   Source size after move: " << tree4.size() << std::endl;
    std::cout << "   Destination size: " << tree5.size() << std::endl;
}

int main() {
    std::cout << "=========================================" << std::endl;
    std::cout << "  Red-Black Tree Implementation" << std::endl;
    std::cout << "  Variant D: Level-Order Iterator" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    testAllConstructors();
    demonstrateSTLCompatibility();
    testConstructorFromFile();
    
    std::cout << "\n=========================================" << std::endl;
    std::cout << "  All tests completed successfully!" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    return 0;
}