//
// Created by hliangzhao on 2021/5/4.
//

#include "BinaryTree.h"
#include "BinaryTreeNode.h"

template <typename valType> class BinaryTreeNode;
template <typename elemType>
void BinaryTree<elemType>::insert(const elemType &elem) {
    if (!_root) {
        _root = new BinaryTreeNode<elemType>(elem);
    } else {
        _root->insert_value(elem);
    }
}

// 移除一个节点之后必须保持二叉树的次序不变。
// 算法：找到数值为elem的节点A，以该节点A的右子节点B取代自身，然后搬移A的左子节点C，
// 使C成为右子节点B的左子树的叶节点。
// 如果该节点A没有右子节点，则以左子节点C直接取代A本身即可。
template <typename elemType>
void BinaryTree<elemType>::remove(const elemType &elem) {
    if (_root) {
        if (_root->_val == elem) {
            // 根节点的移除特别处理
            remove_root();
        } else {
            // 移除非根节点
            _root->remove_value(elem, _root);
        }
    }
}

template <typename elemType>
void BinaryTree<elemType>::remove_root() {
    if (!_root) return;
    BinaryTreeNode<elemType> *tmp = _root;
    if (_root->_r_child) {
        // 以右子节点取代根节点
        _root = _root->_r_child;
        // 将左子节点搬移到右子节点的左子树的底部
        if (tmp->_l_child) {
            BinaryTreeNode<elemType> *lc = tmp->_l_child;
            BinaryTreeNode<elemType> *new_lc = _root->_l_child;
            if (!new_lc) {
                // 新的_root的左子树只有一个节点，直接将该节点接到右子节点的左子树上即可
                _root->_r_child = lc;
            } else {
                // 新的_root的左子树的确是一棵树，遍历找到那个可接上的null节点，把lc接上去
                BinaryTreeNode<elemType>::left_child_leaf(lc, new_lc);
            }
        }
    }
    else {
        // 没有右子树，直接以左子节点取代根节点
        _root = _root->_l_child;
    }
    delete tmp;
}

template <typename elemType>
void BinaryTree<elemType>::clear(BinaryTreeNode<elemType> *node) {
    if (node) {
        clear(node->_l_child);
        clear(node->_r_child);
        delete node;
    }
}