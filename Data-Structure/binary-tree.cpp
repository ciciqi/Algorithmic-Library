#include <iostream>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int _val) : val(_val), left(nullptr), right(nullptr) {}
};

/*
 * Traversal
 */

// Morris遍历二叉搜索树，并按中序排列返回为单链表
TreeNode* Morris(TreeNode* root) {
    TreeNode* dummy = new TreeNode();
    TreeNode* pre = dummy;
    while (root) {
        while (root->left) {
            TreeNode* most_right = root->left;
            while (most_right->right && most_right->right != root) {
                most_right = most_right->right;
            }
            if (most_right->right) {
                most_right->right = nullptr;
                break;
            }
            most_right->right = root;
            root = root->left;
        }
        pre->right = root;
        pre = root;
        root->left = nullptr;
        root = root->right;
    }
    return dummy->right;
}
