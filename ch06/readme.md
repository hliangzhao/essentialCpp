## Chapter 6 以template进行编程

1、以一个模版二叉树类来描述如何编写一个模版类。

我们需要准备两个类，一个是`BinaryTreeNode`，用于存储当前节点的数值`_val`、当前数值被插入次数`_count`（我们要求一个值只能在二叉树中出现一次，
但是`_count`记录了同一个数值尝试被插入的次数），以及指向左右节点的指针`_left_child`和`_right_child`。第二个类是`BinaryTree`，
存储了指向一棵二叉树的根节点的指针。`BinaryTree`需要提供如下能力：将一个新节点插入到二叉树（insert）、从二叉树中移除特定元素、
在树中搜索某特定元素、清除所有元素、以前序、中序和后序的方式遍历。

在我们的二叉树中，第一个插入空树的值，将会成为本树的根节点，接下来每个节点将按照如下规则插入：
+ 如果小于根节点，就递归地置于左子树；
+ 如果大于跟节点，就递归地置于右子树。

全体代码如下（将下面的代码放到一个程序代码文件中编译一切顺序，但是拆开到两个类的头/程序代码文件中会报错，详见`ch06/tree/`）：
```C++
#include <iostream>
using namespace std;

// 前置声明
template <typename elemType> class BinaryTree;
template <typename valType> class BinaryTreeNode;

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
```

2、为模版类重载cout运算符：
```C++
// 为模版类BinaryTree重载cout运算符
template <typename elemType> ostream& operator<<(ostream &os, const BinaryTree<elemType> &tree) {
    os << "Tree: ";
    tree.preorder(os);
    return os;
}

class BinaryTree {
    friend ostream& operator<<(ostream &, const BinaryTree<elemType> &);
    // ...
}
```

3、将常量表达式而非数据类型作为模版的参数：

下面的例子中，我们将模版的参数列表从常见的`<typename Type, ...>`替换成了`<int len, ...>`，这是模版给我们提供的另一种高级用法。
```C++
#include <iostream>
#include <vector>
using namespace std;

// 头文件
// 以常量表示作为template的参数，这样长度和起始位置就不必作为数据成员了
template <int len, int begin_pos>
class NumSequence {
public:
    virtual ~NumSequence() {};
    int elem(int pos) const;
    string what_am_i() const;
    ostream& print(ostream &os = cout) const;
    static int max_elems() { return _max_elems; }

protected:
    virtual void gen_elems(int pos) const = 0;
    bool check_integrity(int pos, int size) const;

    NumSequence(vector<int> *pe): _pelems(pe) {}
    const static int _max_elems = 1024;
    vector<int> *_pelems;
};

template <int len, int begin_pos = 1>
class Fibonacci: public NumSequence<len, begin_pos> {
public:
    Fibonacci(): NumSequence<len, begin_pos>(&_elems){}

private:
    virtual void gen_elems(int pos) const;
    static vector<int> _elems;
};


// 程序代码文件
template <int len, int begin_pos>
vector<int> Fibonacci<len, begin_pos>::_elems;

template <int len, int begin_pos>
string NumSequence<len, begin_pos>::what_am_i() const {
    return typeid(*this).name();
}

template <int len, int begin_pos>
bool NumSequence<len, begin_pos>::check_integrity(int pos, int size) const {
    if (pos <= 0 || pos > max_elems()) {
        cerr << "Invalid position: " << pos << endl;
        return false;
    }
    if (pos > size) gen_elems(pos);
    return true;
}

template <int len, int begin_pos>
ostream& operator<<(ostream &os, const NumSequence<len, begin_pos> &ns) {
    return ns.print(os);
}

template <int len, int begin_pos>
int NumSequence<len, begin_pos>::elem(int pos) const {
    // 调用自基类继承而来的函数check_integrity()
    if (!check_integrity(pos, _pelems->size())) return 0;
    return (*_pelems)[pos - 1];
}

template <int len, int begin_pos>
ostream& NumSequence<len, begin_pos>::print(ostream &os) const {
    int elem_pos = begin_pos - 1;
    int end_pos = elem_pos + len;
    if (!check_integrity(end_pos, _pelems->size())) return os;
    os << "(" << elem_pos << ", " << end_pos << "): ";
    while (elem_pos < end_pos) os << (*_pelems)[elem_pos++] << " ";
    os << endl;
    return os;
}

template <int len, int begin_pos>
void Fibonacci<len, begin_pos>::gen_elems(int pos) const {
    if (_elems.empty()) {
        _elems.push_back(1);
        _elems.push_back(1);
    }
    if (_elems.size() <= pos) {
        int ix = _elems.size();
        int n_1 = _elems[ix - 1];
        int n_2 = _elems[ix - 2];
        for (; ix <= pos; ix++) {
            int tmp = n_2 + n_1;
            _elems.push_back(tmp);
            cout << "Generated elem: " << tmp << endl;
            n_2 = n_1;
            n_1 = tmp;
        }
    }
}

int main() {
    Fibonacci<8> fib;
    cout << "fib: begin at 1 for 1 element: \n" << fib << endl;

    Fibonacci<8, 8> fib2;
    cout << "fib2: begin at 1 for 16 element: \n" << fib2 << endl;

    Fibonacci<12, 8> fib3;
    cout << "fib3: begin at 12 for 8 element: \n" << fib3 << endl;
}
```

4、实际上，模版参数可以玩出让人眼花缭乱的花样。我们应该将模版参数的选择理解为一种**设计策略**。

（1）在第四章我们实现了一个函数对象`LessThan`:
```C++
class LessThan {
public:
    // 通过基值进行初始化。并实现对其的读取和写入操作
    LessThan(int val): _val(val) {}
    int read_val() { return _val; }
    void write_val(int val) { _val = val; }
    bool operator()(int val) const { return val < _val; }
    
private:
    int _val;
};
```
现在，我们将其改造成模版函数对象：
```C++
template <typename elemType> class LessThan {
public:
    LessThan(const elemType &val): _val(val) {}
    bool operator()(const elemType &val) const { return val < _val; }
    void val(const elemType &val) { _val = val; }
    elemType val() const { return _val; }
    
private:
    elemType _val;
};
```

这样就可以了吗？看似成功了，但其实有一个小小的问题，那就是`elemType`必须提供了比较运算符`<`的使用方法。因此，进一步模版化，
我们可以将比较运算符从中提取出来：
```C++
// 此时可将任意一种二元比较操作应用于两个同为elemType类型的对象上
template <typename elemType, typename Comp = less<elemType>>
class LessThan {
public:
    LessThan(const elemType &val): _val(val) {}
    bool operator()(const elemType &val) const { return Comp(val, _val); }
    void val(const elemType &val) { _val = val; }
    elemType val() const { return _val; }

private:
    elemType _val;
};
```
假如我们为`string`定义了比较运算符如下，
```C++
class StrLt {
public:
    bool operator()(const string &s1, const string &s2) {
        return s1.size() < s2.size();
    }
};
```
那么就可以通过`LessThan<string, StrLt> func_lt("abc")`来定义函数对象了。

（2）在这个例子中，**`typename`不仅仅是内置数据类型了，而是类（甚至是模版类！）**。这就给模版的使用带来了极其灵活的使用空间。
我们可以用这种思路再一次改造数列问题：
```C++
template<typename NumSequence>
class NumericSequence {
public:
    // ...
    void calc_elems(int size) const {
        _ns.calc_elems(size);
    }
    void is_elem(int elem) const {
        return _ns.is_elem(elem);
    }
private:
    NumericSequence _ns;
}
```
这种策略要求各个`NumericSequence`必须提供给自己的`calc_elems()`和`is_elem()`成员函数。

（3）成员函数也可以被声明为模版函数：
```C++
class PrintIt {
public:
    PrintIt(ostream &os): _os(os) {}
    template <typename elemType> void print(const elemType &elem, char delim  = '\n') {
        _os << elem << delim;
    }
private:
    ostream& _os;
};
```
上面这个例子中`PrintIt`是一个非模版类，我们当然也可以将其定义为一个模版类：
```C++
// 用了两次模版，二者互不冲突
template <typename OutStream>
class PrintIt {
public:
    PrintIt(OutStream &os): _os(os) {}
    template <typename elemType> void print(const elemType &elem, char delim  = '\n') {
        _os << elem << delim;
    }
private:
    OutStream& _os;
};
```
