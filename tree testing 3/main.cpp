#include "tree.h"
#include "constructor_utils.hpp"

int main() {
    // Создание дерева
    RedBlackTree<int> tree = {5, 3, 7, 2, 4, 6, 8};
    
    // Итерация
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *it << " ";
    }
    
    // Чтение из файла
    RedBlackTree<int> tree2 = ConstructorsUtil<int>::fromEdgeList("tree.txt");
    
    return 0;
}