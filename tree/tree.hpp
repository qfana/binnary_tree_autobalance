template <typename T>
class Node {
public:
    T val;
    Node* left;
    Node* right;
    int height;

    Node(T k) : val(k), left(nullptr), right(nullptr), height(1) {};
};

template <typename T>
class Tree {
public:
    Node* insert(const T& value); // Возвращает пару (итератор, bool), где bool указывает, был ли элемент вставлен.
    int erase(const T& value);
    Node* find(const T& value); // Возвращает итератор на найденный элемент или end().
    Node* begin(); // Методы для получения итераторов на начало и конец контейнера.
    Node* end(); // Методы для получения итераторов на начало и конец контейнера.
    int size();
    bool empty();
private:
    Tree<T>* root;
    int height(Tree<T>* node);
    int balanceFactor(Tree<T>* node);
};