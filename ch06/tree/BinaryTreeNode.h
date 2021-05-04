//
// Created by hliangzhao on 2021/5/4.
//

#ifndef CODING_BINARYTREENODE_H
#define CODING_BINARYTREENODE_H

#include <iostream>
using namespace std;

template <typename elemType> class BinaryTree;

// 模版类：二叉树节点
// valType指定了节点内存放的数值的数据类型
template <typename valType>
class BinaryTreeNode {
    // 我们还没有定义类BinaryTree的具体内容，因此必须要有前置声明，否则这里报错（"找不到类BinaryTree"）
    friend class BinaryTree<valType>;

public:
    explicit BinaryTreeNode(const valType &);

    void insert_value(const valType &);
    static void left_child_leaf(BinaryTreeNode *, BinaryTreeNode *);
    // 第二个参数是reference to pointer，如果去掉&，那么只能更改该pointer所指向的对象；
    // 如果加上&，那么还可以更改该pointer本身！
    void remove_value(const valType &, BinaryTreeNode* &);

    // 遍历
    void preorder(BinaryTreeNode *node, ostream &os = cout) const {
        if (node) {
            os << node->_val << " ";
            if (node->_l_child) preorder(node->_l_child, os);
            if (node->_r_child) preorder(node->_r_child, os);
        }
    }
    void inorder(BinaryTreeNode *node, ostream &os = cout) const {
        if (node) {
            if (node->_l_child) inorder(node->_l_child, os);
            os << node->_val << " ";
            if (node->_r_child) inorder(node->_r_child, os);
        }
    }
    void postorder(BinaryTreeNode *node, ostream &os = cout) const {
        if (node) {
            if (node->_l_child) postorder(node->_l_child, os);
            if (node->_r_child) postorder(node->_r_child, os);
            os << node->_val << " ";
        }
    }

private:
    valType _val;
    int _cnt;
    BinaryTreeNode *_l_child;
    BinaryTreeNode *_r_child;
};



// 建议选择成员列表的方式初始化valType类型的对象_val。这是因为valType可能是一个类而非内置数据类型
// 因此将其声明为const reference，并通过copy constructor直接赋给_val是最快的（避免以传值的方式传递参数）。
// 经验总结：将所有的template类型参数视为"class类型"并通过传址的方式来处理。
template <typename valType>
inline BinaryTreeNode<valType>::BinaryTreeNode(const valType &val): _val(val) {
    _cnt = 1;
    _l_child = _r_child = nullptr;
}
//template <typename valType>
//inline BinaryTreeNode<valType>::BinaryTreeNode(const valType &val) {
//    _val = val;
//    _cnt = 1;
//    _l_child = _r_child = nullptr;
//}


#endif //CODING_BINARYTREENODE_H
