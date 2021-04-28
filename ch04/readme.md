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
    TriangularArr(int len = 1, int begin_pos = 1);
    TriangularArr(const TriangularArr &arr);        // 这其实是copy constructor，用于改变成员逐一初始化的默认行为

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


3、自定义copy constructor

对于上面这个`Matrix`类，当执行
```C++
Matrix mat(4, 4);
Matrix mat2 = mat;
```
时，编译器会逐一复制`mat`实例的各个成员给`mat2`。这里执行了`mat2._mat_ptr = mat._mat_ptr`，仅仅是指针的赋值运算，而没有重新分配内存空间。这是严重的bug。
因此，我们应当定义copy constructor来更正成员逐初始化的方式。
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
    
    // 自定义copy constructor，传入的参数是const ThisClassName &
    Matrix(const Matrix &mat): _row(mat._row), _col(mat._col) {
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
    TriangularArr(int len = 1, int begin_pos = 1);
    TriangularArr(const TriangularArr &arr);        // 这其实是copy constructor，用于改变成员逐一初始化的默认行为

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
    // 这里是用了静态数据成员（该类的全体实例共享的变量，只需要维护一块内存区域即可）
    static vector<int> _elems;
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

// 使用成员初始化列表定义构造函数
TriangularArr::TriangularArr(const TriangularArr &arr):
        _length(arr._length), _begin_pos(arr._begin_pos), _next(arr._next - 1)
{}

int TriangularArr::elem(int pos) const {
    return _elems[pos - 1];
}

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
    SomeClass value() const {
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
    const SomeClass value() const {
        return _value;
    }
    SomeClass value() {
        return _value;
    }

private:
    SomeClass _value;
};

void example(cosnt SomeClass *c1, SomeClass &c2) {
    c1->value();        // 调用const版本
    c2.value();         // 调用non-const版本
}
```

