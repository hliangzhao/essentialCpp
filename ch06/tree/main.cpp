#include <iostream>
#include "BinaryTree.h"
using namespace std;


int main() {
    BinaryTree<string> tree;
    tree.insert("Piglet");
    tree.insert("Eeyore");
    tree.insert("Roo");
    tree.insert("Tigger");
    tree.insert("Chris");
    tree.insert("Pooh");
    tree.insert("Kanga");

    cout << "Preorder: ";
    tree.preorder();
    cout << endl;

    tree.remove("Piglet");
    cout << "Preorder: ";
    tree.preorder();
    cout << endl;

    tree.remove("Eeyore");
    cout << "Preorder: ";
    tree.preorder();
    cout << endl;
}