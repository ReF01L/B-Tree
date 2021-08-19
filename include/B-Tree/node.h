//
// Created by dsalushkin on 30.07.2021.
//

#ifndef B_TREE_NODE_H
#define B_TREE_NODE_H

namespace fefu {
template<typename T, typename K>
class Node {
  using key_type = K;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type &;
  using pointer = value_type *;
  using size_type = long;
 public:
  Node() noexcept
      : key_value_pair_(value_type()),
        parent_(nullptr),
        left_(nullptr),
        right_(nullptr),
        next_(nullptr),
        ref_count_(0),
        deleted_(false),
        height_(0) {}
  explicit Node(const value_type &value)
      : key_value_pair_(value_type(value)),
        parent_(nullptr),
        left_(nullptr),
        right_(nullptr),
        next_(nullptr),
        ref_count_(0),
        deleted_(false),
        height_(0) {}
  reference get_ref() {
      return key_value_pair_;
  }
  pointer get_pointer() {
      return &key_value_pair_;
  }
  const key_type &get_key() {
      return key_value_pair_.first;
  }
  size_type get_height() const {
      return height_;
  }
  size_type get_balance() {
      size_type l_height = left_ == nullptr ? 0 : left_->get_height();
      size_type r_height = right_ == nullptr ? 0 : right_->get_height();
      height_ = l_height - r_height;
  }
  void set_left(Node *node) {
      left_ = node;
      if (left_ != nullptr)
          left_->set_parent(this);
      height_update();
  }
  void set_right(Node *node) {
      right_ = node;
      if (right_ != nullptr)
          right_->set_parent(this);
      height_update();
  }
  void set_parent(Node *node) {
      parent_ = node;
  }
  void set_next(Node *node) {
      next_ = node;
      if (right_ != nullptr)
          left_->set_parent(this);
      height_update();
  }
  Node *get_left() {
      return left_;
  }
  Node *get_right() {
      return right_;
  }
  Node *get_parent() {
      return parent_;
  }
  Node *get_next() {
      return next_;
  }
  size_type get_ref_count() {
      return ref_count_;
  }

  Node &operator++() {
      ++ref_count_;
      return *this;
  }

  Node &operator--() {
      --ref_count_;
      if (ref_count_ == 0 && next_ != nullptr)
          next_--;
      return *this;
  }

  bool is_deleted() {
      return deleted_;
  }
  void set_deleted(bool cond) {
      deleted_ = cond;
  }
 private:
  value_type key_value_pair_;
  Node *parent_;
  Node *left_;
  Node *right_;
  Node *next_;
  size_type ref_count_;
  bool deleted_;
  size_type height_;

  void height_update() {
      size_type l_height = left_ == nullptr ? 0 : left_->get_height();
      size_type r_height = right_ == nullptr ? 0 : right_->get_height();
      height_ = std::max(l_height, r_height) + 1;
  }
};
}

#endif //B_TREE_NODE_H
