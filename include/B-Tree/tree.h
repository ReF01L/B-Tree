//
// Created by dsalushkin on 30.07.2021.
//

#ifndef B_TREE_TREE_H
#define B_TREE_TREE_H

#include <iterator>
#include <limits>
#include <stdexcept>
#include <list>
#include "node.h"

namespace fefu {
    template<typename T>
    class allocator {
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using const_pointer = const T *;
        using reference = typename std::add_lvalue_reference<T>::type;
        using const_reference = typename std::add_lvalue_reference<const T>::type;
    public:
        allocator() noexcept = default;

        allocator(const allocator &) noexcept = default;

        template<typename K>
        explicit allocator(const allocator<K> &) noexcept;

        ~allocator() = default;

        pointer allocate(size_type n) {
            void *ptr = ::operator new(n * sizeof(value_type));
            return static_cast<pointer>(ptr);
        }

        void deallocate(pointer ptr) noexcept {
            ::operator delete(ptr);
        }
    };

    template<typename T, typename Node>
    class tree_iterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using reference = T &;
        using pointer = T *;

        template<typename U, typename N>
        friend class tree_const_iterator;

        template<typename U, typename N, class Compare, typename Alloc>
        friend class tree;

    public:
        tree_iterator() noexcept = default;

        explicit tree_iterator(Node *node) : node_(node) {
            node_++;
        }

        tree_iterator(const tree_iterator &other) noexcept: tree_iterator(other.node_) {}

        ~tree_iterator() = default;

        reference operator*() const {
            return node_->get_ref();
        }

        pointer operator->() const {
            return node_->get_pointer();
        }

        tree_iterator &operator++() {
            node_--;
            if (node_->is_deleted()) {
                node_ = node_->get_next();
            } else {
                if (node_->get_right() != nullptr) {
                    node_ = node_->get_right();
                    while (node_->get_left() != nullptr)
                        node_ = node_->get_left();
                } else {
                    Node *tmp;
                    while (node_->get_parent() != nullptr) {
                        tmp = node_;
                        node_ = node_->get_parent();
                        if (tmp == node_->get_left())
                            break;
                    }
                }
            }
            node_++;
            return *this;
        }

        tree_iterator operator++(int) {
            auto tmp = tree_iterator(*this);
            operator++();
            return tmp;
        }

        tree_iterator &operator--() {
            node_--;
            if (node_->is_deleted()) {
                node_ = node_->get_next();
            } else {
                if (node_->get_left() != nullptr) {
                    node_ = node_->get_left();
                    while (node_->get_right() != nullptr)
                        node_ = node_->get_right();
                } else {
                    Node *tmp;
                    while (node_->get_parent() != nullptr) {
                        tmp = node_;
                        node_ = node_->get_parent();
                        if (tmp == node_->get_right())
                            break;
                    }
                }
            }
            node_++;
            return *this;
        }

        tree_iterator operator--(int) {
            auto tmp = tree_iterator(*this);
            operator--();
            return tmp;
        }

        friend bool operator==(const tree_iterator<T, Node> &a, const tree_iterator<T, Node> &b) {
            return a.node_ == b.node_;
        }

        friend bool operator!=(const tree_iterator<T, Node> &a, const tree_iterator<T, Node> &b) {
            return !(a == b);
        }

    private:
        Node *node_;
    };

    template<typename T, typename Node>
    class tree_const_iterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using reference = const T &;
        using pointer = const T *;

        template<typename U, typename N, class Compare, typename Alloc>
        friend class tree;

    public:
        tree_const_iterator() noexcept = default;

        tree_const_iterator(const tree_iterator<value_type, Node> &other) noexcept: node_(other.node_) {
            node_++;
        }

        tree_const_iterator(const tree_const_iterator &other) noexcept: tree_const_iterator(other.node_) {
        }

        ~tree_const_iterator() {
            node_--;
        }

        reference operator*() const {
            return node_->get_ref();
        }

        pointer operator->() const {
            return node_->get_pointer();
        }

        tree_const_iterator& operator++()
        {
            node_--;

            if (node_->is_deleted())
            {
                node_ = node_->get_next();
            }
            else
            {
                if (node_->get_right() != nullptr)
                {
                    node_ = node_->get_right();
                    while (node_->get_left() != nullptr)
                        node_ = node_->get_left();
                }
                else
                {
                    Node* tmp;
                    while (node_->get_parent() != nullptr)
                    {
                        tmp = node_;
                        node_ = node_->get_parent();
                        if (tmp == node_->get_left())
                            break;
                    }
                }
            }

            node_--;
            return *this;
        }

        tree_const_iterator operator++(int)
        {
            auto tmp = tree_const_iterator(*this);
            operator++();
            return tmp;
        }

        tree_const_iterator& operator--()
        {
            node_--;

            if (node_->is_deleted())
            {
                node_ = node_->get_next();
            }
            else
            {
                if (node_->get_left() != nullptr)
                {
                    node_ = node_->get_left();
                    while (node_->get_right() != nullptr)
                        node_ = node_->get_right();
                }
                else
                {
                    Node* tmp;
                    while (node_->get_parent() != nullptr)
                    {
                        tmp = node_;
                        node_ = node_->get_parent();
                        if (tmp == node_->get_right())
                            break;
                    }
                }
            }

            node_++;
            return *this;
        }

        tree_const_iterator operator--(int)
        {
            auto tmp = tree_const_iterator(*this);
            operator--();
            return tmp;
        }

        friend bool operator==(const tree_const_iterator<value_type, Node>& a, const tree_const_iterator<value_type, Node>& b)
        {
            return a.node_ == b.node_;
        }

        friend bool operator!=(const tree_const_iterator<value_type, Node>& a, const tree_const_iterator<value_type, Node>& b)
        {
            return !(a == b);
        }

    private:
        Node *node_;
    };

    template<typename K, typename T,
            class Compare = std::less<K>,
            typename Alloc = std::allocator<Node<const K, T>>>
    class tree {
        using key_type = K;
        using mapped_type = T;
        using key_compare = Compare;
        using allocator_type = Alloc;
        using value_type = std::pair<const key_type, mapped_type>;
        using reference = value_type&;
        using const_reference = const value_type &;
        using node_type = Node<const key_type, mapped_type>;
        using iterator = tree_iterator<value_type, node_type>;
        using const_iterator = tree_const_iterator<value_type, node_type>;
        using revers_iterator = std::reverse_iterator<iterator>;
        using const_revers_iterator = std::reverse_iterator<const_iterator>;
        using size_type = std::size_t;

    public:
    private:
    };
}

#endif //B_TREE_TREE_H
