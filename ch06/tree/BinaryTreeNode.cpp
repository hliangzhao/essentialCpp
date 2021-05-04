//
// Created by hliangzhao on 2021/5/4.
//

#include "BinaryTreeNode.h"
template <typename valType>
void BinaryTreeNode<valType>::insert_value(const valType &val) {
    if (val == _val) {
        _cnt++;
        return;
    }
    if (val < _val) {
        // 插入左子树（<valType>是不必要的，因为定义处已经限定为BinaryTreeNode<valType>::）
        if (!_l_child) _l_child = new BinaryTreeNode<valType>(val);
        else _l_child->insert_value(val);
    } else {
        // 插入右子树
        if (!_r_child) _r_child = new BinaryTreeNode(val);
        else _r_child->insert_value(val);
    }
}

// 遍历sub_tree的整个左子树，寻找到可接上leaf的那个null左子节点。
template <typename valType>
void BinaryTreeNode<valType>::left_child_leaf(BinaryTreeNode<valType> *leaf, BinaryTreeNode<valType> *sub_tree) {
    while (sub_tree->_l_child) sub_tree = sub_tree->_l_child;
    sub_tree->_l_child = leaf;
}

template <typename valType>
void BinaryTreeNode<valType>::remove_value(const valType &val, BinaryTreeNode<valType> *&prev) {
    if (val < _val) {
        if (!_l_child) return;      // 不在本二叉树中
        else _l_child->remove_value(val, _l_child);
    } else if (val > _val) {
        if (!_r_child) return;      // 不在本二叉树中
        else _r_child->remove_value(val, _r_child);
    }
    else {
        // found。重制此子树，然后删除该节点
        if (_r_child) {
            // 以右子节点取代当前节点
            prev = _r_child;
            if (_l_child) {
                if (!prev->_l_child) prev->_l_child = _l_child;
                else BinaryTreeNode<valType>::left_child_leaf(_l_child, prev->_l_child);
            }
        } else {
            prev = _l_child;
        }
        delete this;
    }
}