## Chapter 4 基于对象的编程风格

1、通过写一个自定义的`MyStack`类来理解实现一个类的基本规范：
```C++
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

// class主体部分应当放在头文件中
class MyStack {
public:
    // member function（成员函数）
    bool push(const string &elem);
    bool pop(string &elem);
    bool peek(string &elem);
    bool empty();
    bool full() {
        return _stack.size() == _stack.max_size();
    }
    bool find(const string &elem);
    bool find2(const string &elem);
    int count(const string &elem);
    long count2(const string &elem);
    // 在class主体内定义的函数，被自动视为inline func
    bool size() {
        return _stack.size();
    }

private:
    // data member（数据成员）
    vector<string> _stack;
};

// 在class主体外定义inline函数（"ClassName::"是类作用域解析符）
// class主体外定义的inline函数也应该定义在头文件中（在ch02已经介绍过）
inline bool MyStack::empty() {
    return _stack.empty();
}

// 在class主体外定义函数
bool MyStack::pop(string &elem) {
    if (empty()) {
        return false;
    }
    elem = _stack.back();
    _stack.pop_back();
    return true;
}

bool MyStack::peek(string &elem) {
    if (empty()) return false;
    elem = _stack.back();
    return true;
}

bool MyStack::push(const string &elem) {
    if (full()) {
        return false;
    }
    _stack.push_back(elem);
    return true;
}

bool MyStack::find(const string &elem) {
    if (empty()) {
        return false;
    }
    auto it = _stack.begin(), it_end = _stack.end();
    while (it != it_end) {
        if (elem == *it) {
            return true;
        }
    }
    return false;
}

bool MyStack::find2(const string &elem) {
    // 使用全局作用域"::"选择<algorithm>头文件中的find算法
    return ::find(_stack.begin(), _stack.end(), elem) != _stack.end();
}

int MyStack::count(const string &elem) {
    if (empty()) {
        return 0;
    }
    int res = 0;
    auto it = _stack.begin(), it_end = _stack.end();
    while (it != it_end) {
        if (elem == *it) {
            res++;
        }
    }
    return res;
}

long MyStack::count2(const string &elem) {
    return ::count(_stack.begin(), _stack.end(), elem);
}

void fill_stack(MyStack &stack, istream &is = cin) {
    string s;
    while (is >> s && !stack.full()) stack.push(s);
    cout << "Read in " << stack.size() << " members" << endl;
}

int main() {
    MyStack stack;
    string str;
    while (cin >> str && !stack.full()) {
        stack.push(str);
    }
    if (stack.empty()) {
        cout << "No strings read" << endl;
        return 0;
    }
    cout << "Read in " << stack.size() << " strings" << endl;
    cout << "In reverse order: ";
    while (stack.size()) {
        if (stack.pop(str)) {
            cout << str << " ";
        }
    }
}
```

2、定义构造函数（constructor）和析构函数（destructor）：

**构造函数的名称必须和class名称相同，不指定返回类型，可以被重载。**
可以像定义一般成员函数那样定义构造函数，也可以使用成员初始化列表。
```C++
#include <iostream>
using namespace std;

class TriangularArr {
public:
    TriangularArr();
    TriangularArr(int len, int begin_pos = 1);
    TriangularArr(const TriangularArr &arr);        // 自定义成员逐一初始化的方式

private:
    int _length;
    int _begin_pos;
    int _next;
};

TriangularArr::TriangularArr() {
    _length = 1;
    _begin_pos = 1;
    _next = 0;
}

TriangularArr::TriangularArr(int len, int begin_pos) {
    _length = len > 0? len: 1;
    _begin_pos = begin_pos > 0? begin_pos: 1;
    _next = _begin_pos - 1;
}

// 使用成员初始化列表定义构造函数
TriangularArr::TriangularArr(const TriangularArr &arr): 
_length(arr._length), _begin_pos(arr._begin_pos), _next(arr._next - 1) 
{}
```
初始化一个类实例的时候会自动调用对应的构造函数。
```C++
TriangularArr t;            // 正确，调用第一个constructor
TriangularArr t2();         // 错误
TriangularArr t3 = 8;       // 正确，调用第二个
TriangularArr t4(8, 3);     // 正确，调用第三个
```

析构函数是用户自定义的一个类成员，可以提供，也可以不提供。一旦一个class提供了析构函数，当其实例结束生命时，会自动调用析构函数善后。
**析构函数的名称是类名称前加上`~`，没有返回值，也没有参数。**
```C++
#include <iostream>
using namespace std;

class Matrix {
public:
    // 通过构造函数和析构函数，让类自动完成了堆内存的分配和管理。
    // 类的使用者无需操心这些内容
    Matrix(int row, int col): _row(row), _col(col) {
        _mat_ptr = new double[row * col];
    }
    
    ~Matrix() {
        delete []_mat_ptr;
    }

private:
    int _row, _col;
    double *_mat_ptr;
};
```
析构函数并不总是必要的。


3、成员逐一初始化（default memberwise copy）：

对于上面这个`Matrix`类，当执行
```C++
Matrix mat(4, 4);
Matrix mat2 = mat;          // 将一个class object赋值给另一个
```
时，编译器会逐一复制`mat`实例的各个成员给`mat2`。这里执行了`mat2._mat_ptr = mat._mat_ptr`，仅仅是指针的赋值运算，而没有重新分配内存空间。这是严重的bug。
因此，我们应当定义相应的构造函数来更正成员逐一初始化的方式。
```C++
#include <iostream>
using namespace std;

class Matrix {
public:
    Matrix(int row, int col): _row(row), _col(col) {
        _mat_ptr = new double[row * col];
    }
    
    // 该构造函数传入的参数是const ThisClassName &，这里的引用符号表示直接作用在调用者身上
    Matrix(const Matrix &mat): _row(mat._row), _col(mat._col) {
        // 在堆上分配空间并依次赋值
        int elem_cnt = _row * _col;
        _mat_ptr = new double[elem_cnt];
        for (int i = 0; i < elem_cnt; i++) {
            _mat_ptr[i] = mat._mat_ptr[i];
        }
    }

    ~Matrix() {
        delete []_mat_ptr;
    }

private:
    int _row, _col;
    double *_mat_ptr;
};
```

4、使用`const`关键字表明成员函数不会改变类对象实例：
```C++
#include <iostream>
#include <vector>
using namespace std;

class TriangularArr {
public:
    TriangularArr();
    TriangularArr(int len, int begin_pos = 1);
    TriangularArr(const TriangularArr &arr);        // 成员逐一初始化

    // 下面三个函数都在参数列表后面加上了const修饰符，表明它们不会更改其调用者（即某个该类的实例对象）
    int length() const {
        return _length;
    }
    int begin_pos() const {
        return _begin_pos;
    }
    int elem(int pos) const;

    bool next(int &val);
    void next_reset() {
        _next = _begin_pos - 1;
    }

private:
    int _length;
    int _begin_pos;
    int _next;
    // 这里声明了静态数据成员（该类的全体实例共享的变量，只需要维护一块内存区域即可），因此我们需要在类的外部（程序代码文件中）提供其清楚的定义（或初始化）
    static vector<int> _elems;
    static const int _buf_size = 1024;          // 对于这种静态常量数据成员，此处虽然是声明，但是也可以直接初始化
};

// trian是一个const reference参数，因此编译器必须保证train在sum()中不会被修改
// 也就是说，需要让begin_pos()，length()和elem()函数不改变trian的内容
// 通过为这三个函数添加const修饰符，来让编译器检查并要求开发者正确实现这三个函数
int sum(const TriangularArr &trian) {
    int begin_pos = trian.begin_pos();
    int length = trian.length();
    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += trian.elem(begin_pos + i);
    }
    return sum;
}

TriangularArr::TriangularArr() {
    _length = 1;
    _begin_pos = 1;
    _next = 0;
}

TriangularArr::TriangularArr(int len, int begin_pos) {
    _length = len > 0? len: 1;
    _begin_pos = begin_pos > 0? begin_pos: 1;
    _next = _begin_pos - 1;
}

TriangularArr::TriangularArr(const TriangularArr &arr):
        _length(arr._length), _begin_pos(arr._begin_pos), _next(arr._next - 1)
{}

int TriangularArr::elem(int pos) const {
    return _elems[pos - 1];
}

// 在类成员函数中访问静态数据成员和访问一般的非静态数据成员一样
bool TriangularArr::next(int &val) {
    if (_next < _begin_pos + _length - 1) {
        val = _elems[_next++];
        return true;
    }
    return false;
}
```

下面这个例子中，虽然语法层面正确，但是这里的const无法给出任何保证。
```C++
class ValClass {
public:
    ValClass(const SomeClass &v): _value(v) {}
    // 这里的函数声明int value() const，而非const int value() const，相当于把类实例的私有变量_value开放了出去，
    // 允许其他程序修改。因此，这里的const等于没有。然而，这里并不会报错。
    // 注意，这里的返回值是SomeClass &，这是什么意思？其实和返回指针一样，是为了返回特定的“那个对象”以便在外部蹂躏它，而不是返回一个长得一样的另一个东西
    SomeClass& value() const {
        return _value;
    }

private:
    SomeClass _value;
};
```

解决方案是根据const与否进行重载。
```C++
class ValClass {
public:
    ValClass(const SomeClass &v): _value(v) {}
    const SomeClass& value() const {
        return _value;
    }
    SomeClass& value() {
        return _value;
    }

private:
    SomeClass _value;
};

void example(const SomeClass *c1, SomeClass &c2) {
    c1->value();        // 调用const版本
    c2.value();         // 调用non-const版本
}
```

5、copy constructor和this指针：
```C++
#include <iostream>
#include <vector>
using namespace std;

class TriangularArr {
public:
    TriangularArr();
    TriangularArr(int len, int begin_pos = 8);
    TriangularArr(const TriangularArr &arr);
    TriangularArr& copy(const TriangularArr &arr);

private:
    int _length;
    int _begin_pos;
    int _next;
};


TriangularArr::TriangularArr() {
    _length = 1;
    _begin_pos = 1;
    _next = 0;
}

TriangularArr::TriangularArr(int len, int begin_pos) {
    _length = len > 0? len: 1;
    _begin_pos = begin_pos > 0? begin_pos: 1;
    _next = _begin_pos - 1;
}

TriangularArr::TriangularArr(const TriangularArr &arr):
        _length(arr._length), _begin_pos(arr._begin_pos), _next(arr._next - 1)
{}

// 返回值带有&，是为了返回调用者本身而不是复制出一个一模一样的东西出来
TriangularArr& TriangularArr::copy(const TriangularArr &arr) {
    // 是一个指针，指向调用者本身
    if (this != &arr) {
        _length = arr._length;
        _begin_pos = arr._begin_pos;
        _next = arr._next;
    }
    return *this;
}
```
调用copy constructor的方式为
```C++
TriangularArr t1, t2(3, 8);
t1.copy(t2);                    // 被编译器自动翻译成copy(&t1, t2)，注意t1带有引用符号&
```

**注意区分copy constructor和memberwise copy**。后者在使用assignment operator时被调用，前者本质上是一个构造函数，在一个对象被创建出来的时候使用。
下面以`Matrix`的例子对二者进行比较：
```C++
#include <iostream>
using namespace std;

class Matrix {
public:
    Matrix(int row, int col): _row(row), _col(col) {
        _mat_ptr = new double[row * col];
    }
    
    // copy constructor
    Matrix(const Matrix &mat): _row(mat._row), _col(mat._col) {
        int elem_cnt = _row * _col;
        _mat_ptr = new double[elem_cnt];
        for (int i = 0; i < elem_cnt; i++) {
            _mat_ptr[i] = mat._mat_ptr[i];
        }
    }
    
    // copy assignment operator
    Matrix& operator=(const Matrix &mat) {
        if (this != &mat) {
            _row = mat._row;
            _col = mat._col;
            int elem_cnt = _row * _col;
            delete []_mat_ptr;
            _mat_ptr = new double[elem_cnt];
            for (int i = 0; i < elem_cnt; i++) {
                _mat_ptr[i] = mat._mat_ptr[i];
            }
        }
        return *this;
    }

    ~Matrix() {
        delete []_mat_ptr;
    }

private:
    int _row, _col;
    double *_mat_ptr;
};
```

6、使用静态数据成员和静态成员函数：
```C++
#include <iostream>
#include <vector>
using namespace std;

class TriangularArr {
public:
    TriangularArr();
    TriangularArr(int len, int begin_pos = 1);
    TriangularArr(const TriangularArr &arr);
    int length() const { return _length; }
    int begin_pos() const { return _begin_pos; }
    int elem(int pos) const;
    bool next(int &val);
    void next_reset() { _next = _begin_pos - 1; }

    // 声明静态成员函数（只有当该函数不访问任何非静态成员的时候，才可以被声明为static）
    static bool is_elem(int value);
    static void gen_elems_to_value(int value);

private:
    int _length;
    int _begin_pos;
    int _next;
    // 这里声明了静态数据成员（该类的全体实例共享的变量，只需要维护一块内存区域即可），因此我们需要在程序代码文件中提供其清楚的定义（或初始化）
    static vector<int> _elems;
    static const int _buf_size = 1024;          // 对于这种静态常量数据成员，此处虽然是声明，但是也可以直接初始化
};

TriangularArr::TriangularArr() {
    _length = 1;
    _begin_pos = 1;
    _next = 0;
}

TriangularArr::TriangularArr(int len, int begin_pos) {
    _length = len > 0? len: 1;
    _begin_pos = begin_pos > 0? begin_pos: 1;
    _next = _begin_pos - 1;
}

TriangularArr::TriangularArr(const TriangularArr &arr):
        _length(arr._length), _begin_pos(arr._begin_pos), _next(arr._next - 1)
{}

int TriangularArr::elem(int pos) const {
    return _elems[pos - 1];
}

// 在类成员函数中访问静态数据成员和访问一般的非静态数据成员一样
bool TriangularArr::next(int &val) {
    if (_next < _begin_pos + _length - 1) {
        val = _elems[_next++];
        return true;
    }
    return false;
}

// 在程序文件中定义函数内容时不可再重复带上static关键字
bool TriangularArr::is_elem(int value) {
    if (_elems.empty() || _elems[_elems.size() - 1] < value) gen_elems_to_value(value);
    vector<int>::iterator found_it, end_it = _elems.end();
    found_it = find(_elems.begin(), _elems.end(), value);
    return found_it != end_it;
}

void TriangularArr::gen_elems_to_value(int value) {
    // do sth...
}

int main() {
    int val;
    cin >> val;
    // 直接通过类名调用静态成员函数，不依赖于任何该类的实例
    bool res = TriangularArr::is_elem(val);
    // do sth...
}
```

7、实现iterator class对运算符进行重载：

如果我们想要通过iterator的方式操作类的实例，像下面这样，需要补充什么内容？
```C++
TriangularArr tri(1, 8);
TriangularArr::iterator it = tri.begin(), it_end = tri.end();
while (it != it_end) {
    cout << *it << " ";
    it++;
}
```
**这意味着我们要为`TriangularArr::iterator`提供运算符的重载。**

下面给出了**为一个类定义作用在其上的迭代器**的完整步骤：
```C++
#include <iostream>
#include <vector>
using namespace std;

class TriangularArrIter {
public:
    TriangularArrIter(int idx): _index(idx) {}
    // 重载运算符，成员函数的命名规则为"operator OP"，其中OP指具体的运算符
    // 前三个成员函数的返回值类型为boo，第四个的返回值类型为TriangularArrIter&，第五个的返回值类型为TriangularArrIter
    bool operator==(const TriangularArrIter&) const;
    bool operator!=(const TriangularArrIter&) const;
    int operator*() const;
    TriangularArrIter& operator++();        // 重载前置++
    TriangularArrIter operator++(int);      // 重载后置++

private:
    void check_integrity() const;
    // _index是本类维护的索引值，用于索引TriangularArr中_elems中的元素
    // 因此，本类需具备访问TriangularArr的成员的权限（使用friend机制）
    int _index;
};

// 通常以inline func的形式定义运算符重载成员函数（放在头文件中）
inline bool TriangularArrIter::operator==(const TriangularArrIter &arr) const {
    return _index == arr._index;
}

inline bool TriangularArrIter::operator!=(const TriangularArrIter &arr) const {
    // !=和==性质相反，用后者实现前者
    return !(*this == arr);
}

// 到目前为止，都是给出了member function这样的定义方式
inline int TriangularArrIter::operator*() const {
    check_integrity();
    return TriangularArr::_elems[_index];
}

// 下面给出了int operator*() const的非成员函数的定义方式
inline int operator*(const TriangularArrIter &arr) {
    // 这是非成员函数的定义方式，因此不具备访问non-public member的权力（注意，类TriangularArrIter的public member是可以被本函数直接访问的，无需使用friend机制）
    // 如果check_integrity()是一个private成员函数，则无法被arr调用
    // 但是这里我们依然调用了，这是因为用到了friend机制（下一条给出，这里先不分析）
    arr.check_integrity();
    return TriangularArr::_elems[_index];
}

// 重载前置++
inline TriangularArrIter& TriangularArrIter::operator++() {
    ++_index;
    check_integrity();
    return *this;
}

// 重载后置++（编译器自动为其产生一个int参数0），这个用不到的参数之所以出现在这里是为了避免破坏重载规则（即参数列表必须独一无二）
inline TriangularArrIter TriangularArrIter::operator++(int) {
    TriangularArrIter tmp = *this;
    ++_index;
    check_integrity();
    return tmp;
}

inline void TriangularArrIter::check_integrity() const {
    if (_index >= TriangularArr::_buf_size) throw iterator_overflow();
    if (_index >= TriangularArr::_elems.size()) TriangularArr::gen_elements(_index + 1);
}


// 修正TriangularArr，使其可以通过迭代器的方式被访问
// #include "TriangularArrIter.h"
class TriangularArr {
public:
    // 使用嵌套类型屏蔽具体的迭代器TriangularArrIter的实现细节，使用者仅需要用iterator来定义作用于TriangularArr上的迭代器即可
    typedef TriangularArrIter iterator;
    TriangularArrIter begin() const { return TriangularArrIter(_begin_pos); }
    TriangularArrIter end() const { return TriangularArrIter(_begin_pos + _length); }
    
private:
    int _begin_pos;
    int _length;
    // ...
};

int main() {
    TriangularArr tri;
    // 使用class scope运算符::指引编译器找到TriangularArr的迭代器的定义
    TriangularArr::iterator it = tri.begin();
}
```

8、使用friend机制允许自身的non-public member被外部访问：

观察注释部分：
```C++
#include <iostream>
#include <vector>
using namespace std;

class TriangularArrIter {
public:
    TriangularArrIter(int idx): _index(idx) {}
    bool operator==(const TriangularArrIter&) const;
    bool operator!=(const TriangularArrIter&) const;
    int operator*() const;
    TriangularArrIter& operator++();
    TriangularArrIter operator++(int);
    void check_integrity() const;

private:
    int _index;
};

inline bool TriangularArrIter::operator==(const TriangularArrIter &arr) const {
    return _index == arr._index;
}

inline bool TriangularArrIter::operator!=(const TriangularArrIter &arr) const {
    return !(*this == arr);
}

inline TriangularArrIter& TriangularArrIter::operator++() {
    ++_index;
    check_integrity();
    return *this;
}

inline TriangularArrIter TriangularArrIter::operator++(int) {
    TriangularArrIter tmp = *this;
    ++_index;
    check_integrity();
    return tmp;
}


class TriangularArr {
public:
    typedef TriangularArrIter iterator;
    TriangularArrIter begin() const { return TriangularArrIter(_begin_pos); }
    TriangularArrIter end() const { return TriangularArrIter(_begin_pos + _length); }

    // 将下面这些非成员函数声明为自己的friend，因为它们访问了自己的non-public member
    // 如果自己提供了non-public member的公开访问方式，如：static int elem_size() { return _elem.size(); }
    // 则friend关键字就不需要了
    friend int TriangularArrIter::operator*() const;
    friend void TriangularArrIter::check_integrity() const;
    // friend class TriangularArrIter;      直接将这个类声明为自己的friend，这个类就可以肆意访问自己的non-public member

private:
    int _length;
    int _begin_pos;
    int _next;
    static vector<int> _elems;
    static const int _buf_size = 1024;
};

inline int TriangularArrIter::operator*() const {
    check_integrity();
    return TriangularArr::_elems[_index];
}

inline void TriangularArrIter::check_integrity() const {
    if (_index >= TriangularArr::_buf_size) throw iterator_overflow();
    if (_index >= TriangularArr::_elems.size()) TriangularArr::gen_elements(_index + 1);
}
```

9、实现一个function object：

在第三章，我们讲过使用STL中的函数对象来实现运算符的重载。例如如下：
```C++
#include <iostream>
#include <vector>
#include <functional>           // 包含本头文件
#include <algorithm>
using namespace std;

int main() {
    int arr[] = {1, 4, 3, 56, 7, 89, 90, -8, 91, 100};
    vector<int> v(arr, arr + 10);
    for (auto it = v.begin(); it != v.end(); it++) cout << *it << " ";
    cout << endl;
    // 类是greater<int>，通过调用构造函数greater<int>()产生该类的一个实例对象传入，目的是使得v按照降序排列
    sort(v.begin(), v.end(), greater<int>());
    for (auto it = v.begin(); it != v.end(); it++) cout << *it << " ";
    cout << endl;
}
```

此外，在习题3-2中我们实现了一个简易的`LessThan`函数对象用于实现升序排列：
```C++
class LessThan {
public:
    bool operator() (const string &s1, const string &s2) {
        return s1.size() < s2.size();
    }
};
```

现在，我们给出一个更全面的函数对象的实现：
```C++
#include <iostream>
#include <vector>
using namespace std;

class LessThan {
public:
    // 通过基值进行初始化。并实现对其的读取和写入操作
    LessThan(int val): _val(val) {}
    int read_val() { return _val; }
    void write_val(int val) { _val = val; }
    bool operator()(int val) const;
    
private:
    int _val;
};

bool LessThan::operator()(int val) const {
    return val < _val;
}

// 下面这个函数对用了函数对象LessThan
void print_less_than(const vector<int> &v, int comp, ostream &os = cout) {
    LessThan lt(comp);
    auto it = v.begin(), end_it = v.end();
    os << "Elements less than " << lt.read_val() << endl;
    // 循环条件执行的操作：
    // 将v[it]的元素和lt内部的基值进行比较，如果小于，则返回的it不为end_it
    while ((it = find_if(it, end_it, lt)) != end_it) {
        os << *it << " ";
        it++;
    }
}

// 下面这个函数对用了函数对象LessThan
int count_less_than(const vector<int> &v, int comp) {
    LessThan lt(comp);
    int res = 0;
    for (int i = 0; i < v.size(); i++) {
        // lt(v[i])调用bool operator()(int val)函数
        if (lt(v[i])) res++;
    }
    return res;
}
```

10、重载iostream运算符：
```C++
// 重载iostream运算符（按照指定格式读入和写出）
// 重载cout起到的是to_string的效果，最后返回cout以便可以串接多个output运算符
// 不将cout和cin写成成员函数是因为，作为一个成员函数，其左操作数必须是本类的对象，因此该函数的调用将变成：tri << cout;
// 这显然是不合乎习惯的。
ostream& operator<<(ostream &os, const TriangularArr &arr) {
    os << "(" << arr.length() << ")" << endl;
    return os;
}

istream& operator>>(istream &is, TriangularArr &arr) {
    int len;
    is >> len;
    arr.set_length(len);
    return is;
}
```

11、定义并使用指向类成员函数的指针：
```C++
#include <iostream>
#include <vector>
using namespace std;

class num_sequence {
public:
    // 将pm声明为一个指针，指向num_sequence的成员函数，该成员函数的返回类型是void，且仅有一个参数，类型为int。
    // void (num_sequence::*pm)(int);
    // void (num_sequence::*pm)(int) = 0则是声明的时候同时初始化为nullptr。
    // 下面这种方式使用了typedef将该类型的指针化名为PtrType，这样就可以PtrType pm = 0直接声明一个该类型的函数指针。
    typedef void (num_sequence::*PtrType)(int);

    // _ptr可以指向本类中的、下面的任意函数。这些函数计算不同类型数列的数值并存放到对应的vector中
    void fib(int);
    void pell(int);
    void lucas(int);
    void triangular(int);
    void square(int);
    void pentagonal(int);

    void use_ptr() {
        // 使用取址运算符和class scope运算符获得成员函数的地址
        _ptr = &num_sequence::fib;
    }
    
    int elem(int pos);                      // 产生当前_ptr所指向的方法产生pos位置的元素并存入_elem

private:
    PtrType _ptr;                           // 指向成员函数的指针
    vector<int> *_elem;                     // 指向目前所用的数列存放的vector
    static const int num_seq = 7;
    static PtrType func_tbl[num_seq];       // 直接将六个满足条件的成员函数的地址存放在静态数组中
    static vector<vector<int>> seq;         // 这是为了避免重复计算每个数列的元素，使用静态变量存放每一个数列的各个元素
};

// 提供静态数据成员的定义
const int num_sequence::num_seq;                    // 已在声明时指定，无需再指定
vector<vector<int>> num_sequence::seq(num_seq);
num_sequence::PtrType num_sequence::func_tbl[num_seq] = {
        nullptr, &num_sequence::fib, &num_sequence::pell, &num_sequence::lucas,
        &num_sequence::triangular, &num_sequence::square, &num_sequence::pentagonal
};

int num_sequence::elem(int pos) {
    if (pos > _elem->size()) {
        (this->*_ptr)(pos);
    }
    return (*_elem)[pos - 1];
}

int main() {
    // 使用指向成员函数的指针
    num_sequence ns;
    num_sequence *pns = &ns;
    num_sequence::PtrType pm = &num_sequence::fib;      // 定义指向成员函数的指针
    int pos =0 ;
    (ns.*pm)(pos);                                      // 指针pm必须通过同一类的对象（这里便是ns和pns）实例加以调用
    (pns->*pm)(pos);
}
```