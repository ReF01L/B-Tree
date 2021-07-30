//
// Created by dsalushkin on 30.07.2021.
//

#ifndef B_TREE_NODE_H
#define B_TREE_NODE_H

#include <cstdio>

namespace fefu {
    template <typename T, typename K>
    class Node {
    private:
        std::size_t ref = 0;
        int height = 0;
        T value;
        K key;
        Node *left, *right, *parent;
        Node *children;
    public:
        Node() = delete;
        explicit Node(T *data, K key);
        Node(T value, K key, Node *parent);
        bool is_ref();

        Node& operator++(); // Prefix
        Node operator++(int); // Postfix

        Node& operator--(); // Prefix
        Node operator--(int); // Postfix
    };
}

#endif //B_TREE_NODE_H
