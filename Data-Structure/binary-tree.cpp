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

// 非递归后序遍历1
// 根据栈顶元素是否为当前结点的右结点，来判断当前结点所有子树的访问状态
// 若栈顶元素为当前结点的右结点，则当前结点的右子树未遍历，应先遍历右子树再访问当前结点
vector<int> postorderTraversal1(TreeNode* root) {
    vector<int> res;
    stack<TreeNode*> nodes;
    while (root || !nodes.empty()) {
        while (root) {
            if (root->right) nodes.push(root->right);
            nodes.push(root);
            root = root->left;
        }
        root = nodes.top();
        nodes.pop();
        if (nodes.empty() || nodes.top() != root->right) {
            res.push_back(root->val);
            root = nullptr;
        } else {
            swap(nodes.top(), root);
        }
    }
    return res;
}

// 非递归后序遍历2
// 染色法
// 用白色表示当前结点的所有子树还未遍历，不该先访问当前结点
// 用灰色表示当前结点的所有子树已经遍历，可以访问当前结点
vector<int> postorderTraversal2(TreeNode* root) {
    enum Color { WHITE, GREY };
    struct ColorNode {
        TreeNode* node;
        Color color;
        ColorNode(TreeNode* _node, Color _color) : node(_node), color(_color) {}
    };
    vector<int> res;
    stack<ColorNode> nodes;
    nodes.emplace(root, Color::WHITE);
    while (!nodes.empty()) {
        ColorNode cn = nodes.top();
        nodes.pop();
        if (cn.node == nullptr) continue;
        if (cn.color == Color::WHITE) {
            nodes.emplace(cn.node, Color::GREY);
            nodes.emplace(cn.node->right, Color::WHITE);
            nodes.emplace(cn.node->left, Color::WHITE);
        } else {
            res.emplace_back(cn.node->val);
        }
    }
    return res;
}

// 非递归后序遍历3
// 用pre指针来指示右子树的遍历状态
// 已知右子树最后输出的结点为右子树的根结点，因此右子树完成遍历后，pre指向右子树根结点
// 若当前结点的右结点等于pre，则右子树已经完成遍历，可以输出当前结点，否则应该先遍历右子树
vector<int> postorderTraversal(TreeNode* root) {
    vector<int> res;
    stack<TreeNode*> nodes;
    TreeNode* pre = nullptr;
    while (root || !nodes.empty()) {
        while (root) {
            nodes.push(root);
            root = root->left;
        }
        root = nodes.top();
        nodes.pop();
        if (root->right == nullptr || root->right == pre) {
            res.emplace_back(root->val);
            pre = root;
            root = nullptr;
        } else {
            nodes.push(root);
            root = root->right;
        }
    }
    return res;
}

// 非递归后序遍历4
// Morris遍历，将左->根->右反转为左->右->根
//
