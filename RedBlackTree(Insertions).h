#include <iostream>


template <typename T>
class RedBlackTree {
private:
    struct Node {
        using TreeNode = Node*;
        static const bool black = true, red = false;

        T val;
        bool color = red;
        TreeNode parent, left = nullptr, right = nullptr;

        explicit Node(const T& val, const TreeNode& parent = nullptr)
                : val(val)
                , parent(parent) {}

        ~Node() {
            delete left;
            delete right;
        }
    };

    using TreeNode = typename Node::TreeNode;
    inline TreeNode create_node(const T& val, const TreeNode& parent = nullptr) {
        return new Node(val, parent);
    }

    TreeNode root = nullptr;
    size_t sz = 0;

    inline TreeNode almost_find(const T& val) const {
        TreeNode current = root, parent = nullptr;
        while (current) {
            parent = current;
            if (val == current->val) return current;
            current = val < current->val ? current->left : current->right;
        }
        return parent;
    }

    static inline bool is_left_child(const TreeNode& node) {
        return node == node->parent->left;
    }
    static inline bool is_right_child(const TreeNode& node) {
        return node == node->parent->right;
    }

    static inline TreeNode find_brother(const TreeNode& node) {
        return is_left_child(node) ? node->parent->right : node->parent->left;
    }

    inline void rotate_left(TreeNode node) {
        if (auto parent = node->parent) {
            if (is_left_child(node)) {
                parent->left = node->right;
            } else {
                parent->right = node->right;
            }
        } else {
            root = node->right;
        }
        node->right->parent = node->parent;
        node->parent = node->right;
        node->right = node->parent->left;
        if (node->right) node->right->parent = node;
        node->parent->left = node;
    }
    inline void rotate_right(TreeNode node) {
        if (node->parent) {
            if (is_left_child(node)) {
                node->parent->left = node->left;
            } else {
                node->parent->right = node->left;
            }
        } else {
            root = node->left;
        }
        node->left->parent = node->parent;
        node->parent = node->left;
        node->left = node->parent->right;
        if (node->left) node->left->parent = node;
        node->parent->right = node;
    }

    inline void rebalance_from(TreeNode node) {
        while (node != root && node->parent->color == Node::red) {
            if (TreeNode uncle = find_brother(node->parent); uncle && uncle->color == Node::red) {
                node->parent->color = uncle->color = Node::black;
                node->parent->parent->color = Node::red;
                node = node->parent->parent;
            } else {
                if (is_left_child(node->parent)) {
                    if (is_right_child(node)) {
                        node = node->parent;
                        rotate_left(node);
                    }
                    node->parent->color = Node::black;
                    node->parent->parent->color = Node::red;
                    rotate_right(node->parent->parent);
                } else {
                    if (is_left_child(node)) {
                        node = node->parent;
                        rotate_right(node);
                    }
                    node->parent->color = Node::black;
                    node->parent->parent->color = Node::red;
                    rotate_left(node->parent->parent);
                }
            }
        }
        root->color = Node::black;
    }

public:
    [[nodiscard]] inline size_t size() const {
        return sz;
    }
    [[nodiscard]] inline bool empty() const {
        return root == nullptr;
    }
    inline void clear() {
        root = nullptr;
        sz = 0;
    }

    inline bool contains(const T& val) const {
        return empty() ? false : almost_find(val)->val == val;
    }

    inline void insert(const T& val) {
        if (empty()) {
            rebalance_from(root = create_node(val));
            ++sz;
        }

        TreeNode parent = almost_find(val);
        if (parent->val == val) return;

        TreeNode new_node = create_node(val, parent);
        if (val < parent->val) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
        ++sz;

        rebalance_from(new_node);
    }

    ~RedBlackTree() {
        delete root;
    }
};
