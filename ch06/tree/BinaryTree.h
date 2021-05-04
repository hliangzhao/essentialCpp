//
// Created by hliangzhao on 2021/5/4.
//

#ifndef CODING_BINARYTREE_H
#define CODING_BINARYTREE_H

#include <iostream>
using namespace std;

template <typename valType> class BinaryTreeNode;
#include "BinaryTreeNode.h"

// 模版类：二叉树
// elemType指定了二叉树节点内存放的数值的数据类型
template <typename elemType>
class BinaryTree {
public:
    BinaryTree();                                   // constructor
    ~BinaryTree();                                  // destructor
    BinaryTree(const BinaryTree &);                 // copy constructor
    BinaryTree& operator=(const BinaryTree &);      // copy assignment operator

    void insert(const elemType &elem);
    void remove(const elemType &elem);
    void remove_root();

    bool empty() { return _root == nullptr; }
    void clear() {
        if (_root) {
            clear(_root);
            _root = nullptr;
        }
    }

    // 遍历
    void preorder(ostream &os = cout) const { _root->preorder(_root, os); }
    void inorder(ostream &os = cout) const { _root->inorder(_root, os); }
    void postorder(ostream &os = cout) const { _root->postorder(_root, os); }

private:
    BinaryTreeNode<elemType> *_root;
    // 将src所指向的子树复制到tar所指向的子树
    void copy(BinaryTreeNode<elemType>* tar, BinaryTreeNode<elemType>* src) {
        tar->_val = src->_val;
        tar->_cnt = src->_cnt;
        tar->_l_child = src->_l_child;
        tar->_r_child = src->_r_child;
    }
    void clear(BinaryTreeNode<elemType>*);
};

// 在类外定义类模版成员函数，首先需要带上template <typename elemType>，
// 然后，类作用域是BinaryTree<elemType>::而非BinaryTree::。
template <typename elemType>
inline BinaryTree<elemType>::BinaryTree(): _root(nullptr) {}

template <typename elemType>
inline BinaryTree<elemType>::BinaryTree(const BinaryTree &rhs) {
    copy(_root, rhs._root);
}

template <typename elemType>
inline BinaryTree<elemType>::~BinaryTree() {
    clear();
}

template <typename elemType>
inline BinaryTree<elemType>& BinaryTree<elemType>::operator=(const BinaryTree &rhs) {
    if (this != &rhs) {
        clear();
        copy(_root, rhs._root);
    }
    return *this;
}

#endif //CODING_BINARYTREE_H
