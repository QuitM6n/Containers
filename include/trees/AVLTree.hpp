#ifndef ADS_INCLUDE_TREES_AVLTREE_HPP_
#define ADS_INCLUDE_TREES_AVLTREE_HPP_

#include "TreeNode.hpp"
#include <memory>

namespace ADS {
template <typename Type> class AVLTree : TreeNode<Type> {
  private:
    using Node = TreeNode<Type>;

    using const_reference = const Type&;

    using value_type = Type;

    using reference_type = Type&;

    template <typename T, typename TT>
    using is_base_of = typename std::enable_if_t<std::is_base_of<T, TT>::value, bool>;

  public:
    constexpr AVLTree() noexcept { root = new Node(10); }

    constexpr AVLTree(const std::initializer_list<Type>& t_list) noexcept {
        std::for_each(t_list.begin(), t_list.end(), [&](auto& element) { insert(element); });
    }

    virtual ~AVLTree() noexcept {
        if (root == nullptr) {
            return;
        }

        delete root;
    }

    AVLTree<Type>& operator=(AVLTree<Type>&& t_node) noexcept {
        if (this != &t_node) {
            *this = std::move(t_node);
        }

        return *this;
    }

    AVLTree<Type>& operator=(const AVLTree<Type>& t_node) noexcept {
        if (this != &t_node) {
            *this = t_node;
        }

        return *this;
    }

  public:
    template <typename Derived, is_base_of<TreeNode<Type>, Derived> = true>
    void insert(const Type& t_element) {
        Node* new_node = new Node(t_element);

        if (root == nullptr) {
            root = new_node;
        }

        Node* current = nullptr;
        Node* temp = root;

        while (temp != nullptr) {
            current = temp;
            if (temp->val < t_element) {
                temp = temp->right;
            } else {
                temp = temp->left;
            }
        }

        if (current == nullptr) {
            current = new_node;
        } else if (current->val < t_element) {
            current->right = new_node;
        } else {
            current->left = new_node;
        }

        toBalanceTree(root);
    }

    template <typename Derived, is_base_of<TreeNode<Type>, Derived> = true>
    void erase(const Type& t_element) {
        if (root == nullptr) {
            return;
        }

        Node* parent = nullptr;
        Node* temp = root;
        while (temp != nullptr && temp->val != t_element) {
            parent = temp;
            if (temp->val < t_element) {
                temp = temp->right;
            } else {
                temp = temp->left;
            }
        }

        if (temp == nullptr) {
            return;
        }

        if (temp->right == nullptr && temp->left == nullptr) {
            if (parent != nullptr) {
                if (parent->left == temp) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
                delete temp;
            }
        } else if (temp->left == nullptr) {
            if (parent != nullptr) {
                if (parent->left == temp) {
                    parent->left = temp->right;
                } else {
                    parent->right = temp->right;
                }
            } else {
                root = temp->right;
            }
            delete temp;

        } else if (temp->right == nullptr) {
            if (parent != nullptr) {
                if (parent->left == temp) {
                    parent->left = temp->left;
                } else {
                    parent->right = temp->left;
                }
            } else {
                root = temp->left;
            }
            delete temp;

        } else {
            Node* minRight = temp->right;
            while (minRight->left != nullptr) {
                parent = minRight;
                minRight = minRight->left;
            }
            temp->val = minRight->val;
            if (parent == nullptr) {
                temp->right = minRight->right;
            } else {
                parent->left = minRight->left;
            }
            delete minRight;
        }

        toBalanceTree(root);
    }

    [[nodiscard]] const int getHeight(Node* node) const noexcept {
        if (node == nullptr) {
            return 0;
        }
        return std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    [[nodiscard]] int balanceFactor(Node* node) const noexcept {
        return getHeight(node->left) - getHeight(node->right);
    }

    void toBalanceTree(Node* node) {
        if (balanceFactor(node) > 1) { // левое поддерево слишком высоко
            if (balanceFactor(node->left) < 0) {
                node->left = leftRotation(node->left);
            }
            rightRotation(node);
        } else if (balanceFactor(node) < -1) { // правое поддерево слишком высоко
            if (balanceFactor(node->right) > 0) {
                node->right = rightRotation(node->right);
            }
            leftRotation(node);
        }
    }

    [[nodiscard]] Node*
    leftRotation(Node* node) noexcept { // используется когда разность высот правого
                                        // поддерева больше чем  левого поддерева на 1
        Node* t_node = node->right;
        Node* p_node = t_node->left;
        t_node->left = node;
        node->right = p_node;
        return t_node;
    }

    [[nodiscard]] Node* rightRotation(Node* node) noexcept { // используется когда разность высот
                                                             // левого поддерева больше чем на 1
        Node* t_node = node->left;
        Node* p_node = t_node->right;
        t_node->right = node;
        node->left = p_node;
        return t_node;
    }

    template <typename Func> void inorderPrint(Node* node, Func fnc) const noexcept {
        if (node != nullptr) {
            inorderPrint(node->left, fnc);
            fnc(node);
            inorderPrint(node->right, fnc);
        }
    }

    template <typename Derived, is_base_of<TreeNode<Type>, Derived> = true>
    constexpr void printElements() noexcept {
        inorderPrint(root, [](auto element) { std::cout << element->val << ' '; });
    }

  private:
    Node* root = nullptr;
};

} // namespace ADS

#endif