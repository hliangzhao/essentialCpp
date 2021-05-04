## Chapter 5 面向对象编程风格

1、面向对象编程的两个最主要特征是**继承**和**多态**。前者使得我们得以将一群相关的类组织起来，
让我们得以分享其间的共通数据和操作行为。后者让我们以一种类型无关的方式来操作类对象，并赋予我们更多的弹性来加入或移除任何特定类。

继承机制定义了父子关系。父类定义了所有子类共通的公有接口和私有实现。每个子类都可以增加或覆盖继承而来的东西，以实现其自身独特的行为。
在C++中，父类被称为**基类**，子类被称为**派生类**。

使用抽象基类的**指针**或**引用**来操纵其公共接口，实际执行起来时需要等到运行时才能确定指针或引用所指的实际对象的类型。

2、通过以下实例理解继承机制：
```C++
#include <iostream>
using namespace std;

// 整个图书馆类体系中最根本的抽象基类
class LibMat {
public:
    // 默认情况下，成员函数的解析在编译时静态地进行
    LibMat() {
        cout << "LibMat::LibMat() default constructor!" << endl;
    }
    // 加上关键字virtual，令该成员函数的解析在运行时动态进行
    virtual ~LibMat() {
        cout << "LibMat::~LibMat() destructor!" << endl;
    }
    virtual void print() const {
        cout << "LibMat::print() -- I am a LibMat object!" << endl;
    }
};

// 基类可以通过public、protected和private三种方式继承而来，我们只讨论公有继承
// 相关讨论：https://www.cnblogs.com/qlwy/archive/2011/08/25/2153584.html
class Book: public LibMat {
public:
    Book(const string &title, const string &author):
    _title(title), _author(author) {
        cout << "Book::Book(" << _title << ", " << _author << ") constructor!" << endl;
    }
    // override
    virtual ~Book() {
        cout << "Book::~Book() destructor!" << endl;
    }
    // override
    virtual void print() const {
        cout << "Book::print() -- I am a Book object!" << endl;
        cout << "Title: " << _title << endl;
        cout << "Author: " << _author << endl;
    }
    // 下面两个都是access func，同java中的getter方法
    const string& title() const {
        return _title;
    }
    const string& author() const {
        return _author;
    }

protected:
    // 被声明为protected的所有成员都可以被派生类直接访问，除派生类之外，无法被直接访问
    string _title;
    string _author;
};

// 通过公有方式继承Book，Book的公有成员对本派生类仍是公有的，protected仍是protected
class AudioBook: public Book {
public:
    AudioBook(const string &title, const string &author, const string &narrator):
    Book(title, author), _narrator(narrator) {
        cout << "AudioBook::AudioBook(" << _title << ", " << _author << ", " << _narrator << ") constructor!" << endl;
    }
    virtual ~AudioBook() {
        cout << "AudioBook::~AudioBook() destructor!" << endl;
    }
    virtual void print() const {
        cout << "AudioBook::print() -- I am a AudioBook object!" << endl;
        // 基类Book的protected成员_title和_author对本派生类而言仍然是protected，可以被直接访问
        cout << "Title: " << _title << endl;
        // 基类Book的public成员函数author()对本派生类而言仍然是public，可以被直接访问
        cout << "Author: " << author() << endl;
        cout << "Narrator: " << _narrator << endl;
    }
    const string &narrator() const {
        return _narrator;
    }

protected:
    string _narrator;
};

void print(const LibMat &mat) {
    cout << "In global print(): about to print mat.print()" << endl;
    // 在运行时解析mat所代指的具体对象并调用对应对象的成员函数print()
    mat.print();
    cout << endl;
}

int main() {
    cout << "Creating a LibMat object" << endl;
    LibMat mat;
    print(mat);

    cout << "Creating a Book object" << endl;
    Book book("The Castle", "Franz Kafka");
    print(book);

    cout << "Creating a AudioBook object" << endl;
    AudioBook audioBook("Man Without Qualities", "Robert Musil", "Kenneth Mayer");
    print(audioBook);
}
```
该程序的输出为
```
Creating a LibMat object
LibMat::LibMat() default constructor!
In global print(): about to print mat.print()
LibMat::print() -- I am a LibMat object!

Creating a Book object
LibMat::LibMat() default constructor!
Book::Book(The Castle, Franz Kafka) constructor!
In global print(): about to print mat.print()
Book::print() -- I am a Book object!
Title: The Castle
Author: Franz Kafka

Creating a AudioBook object
LibMat::LibMat() default constructor!
Book::Book(Man Without Qualities, Robert Musil) constructor!
AudioBook::AudioBook(Man Without Qualities, Robert Musil, Kenneth Mayer) constructor!
In global print(): about to print mat.print()
AudioBook::print() -- I am a AudioBook object!
Title: Man Without Qualities
Author: Robert Musil
Narrator: Kenneth Mayer

AudioBook::~AudioBook() destructor!
Book::~Book() destructor!
LibMat::~LibMat() destructor!           // AudioBook析构完毕
Book::~Book() destructor!
LibMat::~LibMat() destructor!           // Book析构完毕
LibMat::~LibMat() destructor!           // LibMat析构完毕
```
**可以发现构造函数和析构函数在派生类中的调用顺序正好相反**。

如果按如下方式定义指针`b`：
```C++
LibMat *b = new Book("The Castle", "sFranz Kafkads");
print(*b);
```
则`b`实际指向的是一个Book类对象。

`Book *b = new LibMat`会报错：
```C++
error: cannot initialize a variable of type 'Book *' with an rvalue of type 'LibMat *'
```

3、通过之前的一个例子来模拟多态：

下面的代码中，`NumSequence`通过编程技巧实现了“六个不同数列调用同一个方法`elems()`却执行了不同操作”这个效果。显然，这种操作不具备扩展性，如果要添加一个新的数列，
那么许多成员函数都要重新定义。
```C++
#include <iostream>
#include <vector>
using namespace std;

class NumSequence {
public:
    typedef void (NumSequence::*SeqPtr)(int);               // 定义数据类型SeqPtr，用于定义指向成员函数的指针
    enum ns_type {
        ns_unset, ns_fib, ns_pell, ns_lucas,
        ns_triangular, ns_square, ns_pentagonal
    };
    // 如果这里不为静态变量分配堆内存，就会直接报错
    // 这是因为_seqs的类型是vector<vector<int>>，不能像单纯的使用vector<int>那样定义，必须要分配堆内存
    // 其实，constructor应当是给non-static数据成员初始化的时候用的，static成员函数在程序代码文件定义更恰当，下面给出了_seqs在程序代码文件中的定义。
//    NumSequence() {
//        _seqs = *new vector<vector<int>>(_num_seq);
        // 下面这四行是不必要的
        // auto it = _seqs.begin();
        // while (it != _seqs.end()) {
        //    *it = *new vector<int>;
        //    it++;
        // }
//    }
    static ns_type nstype(int);                             // 返回index对应的数列类型
    void set_seq(ns_type);                                  // 根据数列类型设置_ptr, _isa, _elem三个指针变量
    const string what_am_i();

    // 元素产生器
    void fib(int);
    void pell(int);
    void lucas(int);
    void triangular(int);
    void square(int);
    void pentagonal(int);

    int elem(int);
    bool is_size_ok(int) const;

    inline static int num_of_seqs() {
        return _num_seq;
    }

private:
    SeqPtr _ptr;                                            // 指向某个数列元素产生器
    ns_type _isa;                                           // 目前的数列类型
    vector<int> *_elem;                                     // 存放当前函数指针指向的数列vector的地址

    static const int _num_seq = 7;                          // 支持的数列种类数
    const int _max_size = 1024;                             // 单个数列的最大元素个数
    static SeqPtr _seq_pointers[_num_seq];                  // 全部可选的数列元素产生器组成的数列
    static vector<vector<int>> _seqs;                       // 真正存放各组数列vector的vector
};

// static成员需要在程序代码文件定义
const int NumSequence::_num_seq;
NumSequence::SeqPtr NumSequence::_seq_pointers[_num_seq] = {
        nullptr, &NumSequence::fib, &NumSequence::pell, &NumSequence::lucas,
        &NumSequence::triangular, &NumSequence::square, &NumSequence::pentagonal
};
// 如果不像上面那样在构造函数中分配堆内存，在定义静态成员时分配也可以
vector<vector<int>> NumSequence::_seqs = *new vector<vector<int>>(_num_seq);

NumSequence::ns_type NumSequence::nstype(int idx) {
    // 调用static_cast<>()将整数idx转换为对应的枚举项
    return idx <= 0 || idx >= _num_seq? ns_unset: static_cast<ns_type>(idx);
}

void NumSequence::set_seq(ns_type nst) {
    switch (nst) {
        default:
            cerr << "invalid type:setting to 0" << endl;
        case ns_unset:
            _ptr = nullptr;
            _elem = nullptr;
            _isa = ns_unset;
            break;
        case ns_fib: case ns_pell: case ns_lucas:
        case ns_triangular: case ns_square: case ns_pentagonal:
            _ptr = _seq_pointers[nst];
            _elem = &_seqs[nst];
            _isa = nst;
            break;
    }
}

const string NumSequence::what_am_i() {
    static string names[_num_seq] = {
        "notSet", "fib", "pell", "lucas", "triangular", "square", "pentagonal"
    };
    return names[_isa];
}

void NumSequence::fib(int pos) {
    if (!is_size_ok(pos)) {
        cerr << "Input pos not correct" << endl;
        exit(-1);
    }
    for (unsigned i = _elem->size(); i < pos; i++) {
        if (i == 0 || i == 1) {
            _elem->push_back(1);
        } else {
            _elem->push_back((*_elem)[i - 1] + (*_elem)[i - 2]);
        }
    }
}

void NumSequence::pell(int pos) {
    if (!is_size_ok(pos)) {
        cerr << "Input pos not correct" << endl;
        exit(-1);
    }
    for (unsigned i = _elem->size(); i < pos; i++) {
        _elem->push_back((int)i + 1);
    }
}

void NumSequence::lucas(int pos) {
    if (!is_size_ok(pos)) {
        cerr << "Input pos not correct" << endl;
        exit(-1);
    }
    for (unsigned i = _elem->size(); i < pos; i++) {
        _elem->push_back((int)i + 1);
    }
}

void NumSequence::triangular(int pos) {
    if (!is_size_ok(pos)) {
        cerr << "Input pos not correct" << endl;
        exit(-1);
    }
    for (unsigned i = _elem->size(); i < pos; i++) {
        _elem->push_back((int)i + 1);
    }
}

void NumSequence::square(int pos) {
    if (!is_size_ok(pos)) {
        cerr << "Input pos not correct" << endl;
        exit(-1);
    }
    for (unsigned i = _elem->size(); i < pos; i++) {
        _elem->push_back(((int)i + 1) * ((int)i + 1));
    }
}

void NumSequence::pentagonal(int pos) {
    if (!is_size_ok(pos)) {
        cerr << "Input pos not correct" << endl;
        exit(-1);
    }
    for (unsigned i = _elem->size(); i < pos; i++) {
        _elem->push_back(((int)i + 1) * ((int)i + 1));
    }
}

int NumSequence::elem(int pos) {
    // 如果现在计算的还不到pos则产生新的元素
    if (pos > _elem->size()) {
        (this->*_ptr)(pos);
    }
    return (*_elem)[pos];
}

bool NumSequence::is_size_ok(int size) const {
    if (size < 0 || size > _max_size) return false;
    return true;
}

int main() {
    NumSequence ns;
    int pos = 10;
    for (int idx = 1; idx < NumSequence::num_of_seqs(); idx++) {
        ns.set_seq(NumSequence::nstype(idx));
        ns.elem(pos);
    }
    for (int idx = 1; idx < NumSequence::num_of_seqs(); idx++) {
        ns.set_seq(NumSequence::nstype(idx));
        cout << ns.what_am_i() << ": ";
        for (int i = 0; i < pos; i++) {
            cout << ns.elem(i) << " ";
        }
        cout << endl;
    }
}
```
该程序的输出为
```
fib: 1 1 2 3 5 8 13 21 34 55 
pell: 1 2 3 4 5 6 7 8 9 10 
lucas: 1 2 3 4 5 6 7 8 9 10 
triangular: 1 2 3 4 5 6 7 8 9 10 
square: 1 4 9 16 25 36 49 64 81 100 
pentagonal: 1 4 9 16 25 36 49 64 81 100 
```
接下来，我们将用继承的方式来实现多态。

4、实现数列问题多态：

（1）第一步，定义一个抽象基类。定义抽象基类的第一个步骤是**找出所有子类共通的操作行为**。第二步，**找出哪些操作行为与类型相关**，它们应当被定义为虚函数，
各派生类然后再给出自己独特的实现。第三步，**确定每个操作行为的访问层级**（public、protected还是private），
相关的讨论见https://www.cnblogs.com/qlwy/archive/2011/08/25/2153584.html。

因此定义基类如下：
```C++
#include <iostream>
using namespace std;

class NumSequence {
public:
    // 作为基类，析构函数应该被声明为virtual，应该需要根据实际对象的类型选择调用哪一个析构函数。
    // 基类的destructor不应该被声明为纯虚函数，最好是处理方式是提供空白定义。
    virtual ~NumSequence() {};
    // 将虚函数赋值为0，意即将其设定为一个纯虚函数。
    // 纯虚函数意味着本函数对本类而言没有实际意义，只在派生类中才有意义。
    // 一个类如果有一个或多个纯虚函数，相当于本类的定义是不完整的，因此程序无法为其产生任何对象。
    // 这种类只能作为派生类的子对象使用，前提是这些派生类必须为所有虚函数提供确切的定义。
    virtual int elem(int pos) const = 0;
    virtual string what_am_i() const = 0;
    virtual ostream& print(ostream &os = cout) const = 0;
    // 显然，静态成员函数无法被声明为虚函数。这是因为静态成员函数访问的是静态数据，二者都是要在编译时就明确的。
    static int max_elems() { return _max_elems; }

protected:
    virtual void gen_elems(int pos) const = 0;
    static bool check_integrity(int pos) const;
    const static int _max_elems = 1024;
};

bool NumSequence::check_integrity(int pos) const {
    if (pos <= 0 || pos > _max_elems) {
        cerr << "Invalid position: " << pos << endl;
        return false;
    }
    return true;
}

ostream& operator<<(ostream &os, const NumSequence *ns) {
    return ns->print(os);
}
```

（2）第二步，定义派生类。

派生类由基类的non-static数据成员和派生类自身的non-static数据成员组成。
```C++
#include <iostream>
#include <vector>
using namespace std;

// 头文件
class NumSequence {
public:
    virtual ~NumSequence() {};
    virtual int elem(int pos) const = 0;
    virtual string what_am_i() const = 0;
    virtual ostream& print(ostream &os = cout) const = 0;
    static int max_elems() { return _max_elems; }

protected:
    virtual void gen_elems(int pos) const = 0;
    static bool check_integrity(int pos) ;
    const static int _max_elems = 1024;
};

class Fibonacci: public NumSequence {
public:
    Fibonacci(int len = 1, int begin_pos = 1): _length(len), _begin_pos(begin_pos) {}
    // 派生类内对该成员的任何使用，都会被解析为该派生类自身的那份成员，而非基类的那份成员，这就是override。
    virtual int elem(int pos) const;
    virtual string what_am_i() const {
        return "Fibonacci";
    }
    virtual ostream& print(ostream &os = cout) const;
    int length() const { return _length; }
    int begin_pos() const { return _begin_pos; }

private:
    virtual void gen_elems(int pos) const;
    int _length;
    int _begin_pos;
    static vector<int> _elems;
};

inline void display(ostream &os, const NumSequence &ns, int pos) {
    os << "The element at position " << pos << 
    " for the " << ns.what_am_i() << 
    " sequence is " << ns.elem(pos) << endl;
}


// 程序代码文件
vector<int> Fibonacci::_elems;

bool NumSequence::check_integrity(int pos) {
    if (pos <= 0 || pos > _max_elems) {
        cerr << "Invalid position: " << pos << endl;
        return false;
    }
    return true;
}

ostream& operator<<(ostream &os, const NumSequence &ns) {
    return ns.print(os);
}

int Fibonacci::elem(int pos) const {
    // 调用自基类继承而来的函数check_integrity()
    if (!check_integrity(pos)) return 0;
    // 下面的代码是Fibonacci::gen_elems()而非::gen_elems(pos)，这是为了跳过虚函数的机制，在编译时就完成解析，提高代码性能。
    if (pos > _elems.size()) Fibonacci::gen_elems(pos);
    return _elems[pos - 1];
}

ostream& Fibonacci::print(ostream &os) const {
    int elem_pos = _begin_pos - 1;
    int end_pos = elem_pos + _length;
    if (end_pos > _elems.size()) Fibonacci::gen_elems(end_pos);
    os << "(" << elem_pos << ", " << end_pos << "): ";
    while (elem_pos < end_pos) os << _elems[elem_pos++] << " ";
    os << endl;
    return os;
}

void Fibonacci::gen_elems(int pos) const {
    if (_elems.empty()) {
        _elems.push_back(1);
        _elems.push_back(1);
    }
    // 下面这个判断有点啰嗦了，是不必要的。
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
    Fibonacci fib;
    cout << "fib: begin at 1 for 1 element: \n" << fib << endl;

    Fibonacci fib2(16);
    cout << "fib2: begin at 1 for 16 element: \n" << fib2 << endl;

    Fibonacci fib3(8, 12);
    cout << "fib3: begin at 12 for 8 element: \n" << fib3 << endl;
}
```

5、基类不应该过分抽象：对于上面的例子，部分成员应当被提升值基类的层次，我们将新的类定义如下：
```C++
#include <iostream>
#include <vector>
using namespace std;

class NumSequence {
public:
    virtual ~NumSequence() {}
    virtual string what_am_i() const = 0;
    int elem(int pos) const;
    ostream& print(ostream &os = cout) const;

    int length() const { return _length; }
    int begin_pos() const { return _begin_pos; }
    static int max_elems() { return 1024; }

protected:
    virtual void gen_elems(int pos) const = 0;
    bool check_integrity(int pos, int size) const;
    // 这里将抽象基类的构造函数置为protected，这是因为本类是无法被实例化，那么直接杜绝其被外界访问的可能性可以降低写出bug的概率。
    NumSequence(int len, int begin_pos, vector<int> &re):
    _length(len), _begin_pos(begin_pos), ref_elems(re) {}

    // 这些数据成员被提升至基类的层次
    int _length;
    int _begin_pos;
    // 定义为引用，则无需再检查是否null，并且需要在constructor中通过参数化列表的形式被初始化，且无法再更改。
    // 因为引用不是指针，是无法被更改的。
    // 用于指向派生类中的数据成员_elem。
    vector<int> &ref_elems;
};

class Fibonacci: public NumSequence {
public:
    Fibonacci(int len = 1, int begin_pos = 1): NumSequence(len, begin_pos, _elems) {}
    virtual string what_am_i() const {
        return "Fibonacci";
    }

protected:
    virtual void gen_elems(int pos) const;
    static vector<int> _elems;
};
```
应当说明，所谓设计，必须来来回回借着程序猿的经验和用户的反馈演进。

6、派生类的初始化与复制行为分析：

```C++
#include <iostream>
#include <vector>
using namespace std;

class NumSequence {
public:
    virtual ~NumSequence() {}
    virtual string what_am_i() const = 0;
    int elem(int pos) const;
    ostream& print(ostream &os = cout) const;

    int length() const { return _length; }
    int begin_pos() const { return _begin_pos; }
    static int max_elems() { return 1024; }
    NumSequence& operator=(const NumSequence &rhs) {
        if (this != &rhs) {
            _length = rhs._length;
            _begin_pos = rhs._begin_pos;
            // 下面这一行不妥！因为引用无法被修改。
            // 但是不妨碍我们阐述如何"在派生类中使用基类的成员逐一初始化方法"。
            ref_elems = rhs.ref_elems;
        }
        return *this;
    }

protected:
    virtual void gen_elems(int pos) const = 0;
    bool check_integrity(int pos, int size) const;
    NumSequence(int len, int begin_pos, vector<int> &re):
    _length(len), _begin_pos(begin_pos), ref_elems(re) {}

    // 这些数据成员被提升至基类的层次
    int _length;
    int _begin_pos;
    // 定义为引用，则无需再检查是否null，并且需要在constructor中通过参数化列表的形式被初始化，且无法再更改。
    // 因为引用不是指针，是无法被更改的。
    // 用于指向派生类中的数据成员_elem。
    vector<int> &ref_elems;
};

class Fibonacci: public NumSequence {
public:
    Fibonacci(int len = 1, int begin_pos = 1): NumSequence(len, begin_pos, _elems) {}
    virtual string what_am_i() const {
        return "Fibonacci";
    }
    // 定义一个copy constructor。因为没有内存分配的需求，直接调用基类的、默认的成员逐一初始化方法即可。
    // 本copy constructor是无需定义的。
    Fibonacci(const Fibonacci &rhs): NumSequence(rhs) {}
    // 需要显著指出调用了基类的copy assignment operator。
    // 同样地，本copy assignment operator是无需定义的。
    Fibonacci& operator=(const Fibonacci &rhs) {
        if (this != &rhs) NumSequence::operator=(rhs);
        return *this;
    }

protected:
    virtual void gen_elems(int pos) const;
    static vector<int> _elems;
};
```

7、在派生类中定义虚函数：

在定义派生类的时候，我们必须决定，是覆盖基类中的虚函数，还是原封不动加以继承。如果继承了一个纯虚函数，则这个派生类也会被视为抽象类，
程序无法为期定义任何对象。如果决定覆盖基函数，那么**其函数原型必须完全符合基类所声明的函数原型，包括参数列表、返回类型和常量性**。
当然，有一种情况是特例。

详见注释：
```C++
#include <iostream>
#include <vector>
using namespace std;

class NumSequence {
public:
    virtual ~NumSequence() {}
    virtual string what_am_i() const = 0;
    virtual NumSequence *clone() = 0;

protected:
    NumSequence(int len, int begin_pos, vector<int> &re):
    _length(len), _begin_pos(begin_pos), ref_elems(re) {}

    int _length;
    int _begin_pos;
    vector<int> &ref_elems;
};

class Fibonacci: public NumSequence {
public:
    Fibonacci(int len = 1, int begin_pos = 1): NumSequence(len, begin_pos, _elems) {}
    // 只有当派生类所提供的函数和基类中的函数声明完全一致，才是override！
    // 这是对基类成员函数virtual string what_am_i() const的override
    virtual string what_am_i() const {
        return "Fibonacci";
    }
    // 这不是override
    virtual string what_am_i() {
        return "Fibonacci";
    }
    // 下面这个是一个例外：这是对基类成员函数virtual NumSequence *clone()的override
    // 当基类的虚函数返回的是某个基类形式（通常是引用或指针）时，派生类中的覆盖允许和基类中的成员函数有略微不同的声明。
    //这里不需要virtual关键字，因为我们不需要等到运行时才确定该调用哪一个clone()函数。
    Fibonacci *clone() {
        return new Fibonacci(*this);
    }
    

protected:
    static vector<int> _elems;
};
```

8、虚函数的静态解析：

有两种情况虚函数机制不会出现预期的行为：
+ **基类的constructor和destructor内；**
+ **使用的是基类的对象本身，而不是该对象的引用或指向该对象的指针。**

对于第一种情况，要知道，在构造派生类对象时，会率先调用基类的构造函数，如果在基类的构造函数中调用某个虚函数，由于此时派生类中的数据成员尚未初始化，
因此运行时并不会调用派生类中定义的、对应的虚函数，而是直接调用基类中的该虚函数。同样地，如果在基类的destructor中调用虚函数，
因为此时派生类的数据成员已经被销毁，因此调用的也是基类中定义的虚函数，而非派生类中的虚函数，否则就会有访问非法内存的风险。

对于第二种情况，当我们为基类声明一个实际对象的时候，实际上已经在程序堆栈中为该对象分配了足够的空间以容纳该对象。但如果稍后传入的是一个派生类对象，
那么就可能没有足够的空间来存放派生类中新定义的成员。因此，只会调用基类的虚函数实现以避免访问非法内存空间。

9、运行时的类型鉴定机制：

使用`typeid`在运行时获得对象真正指向的类型。
```C++
#include <iostream>
#include <vector>
#include <typeinfo>     // 包含本头文件
using namespace std;

class NumSequence {
public:
    virtual ~NumSequence() {}
    const string who_am_i() const {
        // typeid()返回了this实际指向的对象的信息，每一个继承了本类的多态类都有自己对应的type_info对象
        return typeid(*this).name();
    }

protected:
    NumSequence(int len, int begin_pos, string name, vector<int> &re):
    _length(len), _begin_pos(begin_pos), _name(name), ref_elems(re) {}

    int _length;
    int _begin_pos;
    string _name;
    vector<int> &ref_elems;
};

class Fibonacci: public NumSequence {
public:
    Fibonacci(int len = 1, int begin_pos = 1, string name = "Fibonacci"):
    NumSequence(len, begin_pos, name, _elems) {}

protected:
    static vector<int> _elems;
};

int main() {
    NumSequence *ns = new Fibonacci;
    // 静态无条件转换
    if (typeid(*ns) == typeid(Fibonacci)) {
        Fibonacci *fib = static_cast<Fibonacci*>(ns);
        // 调用fib的方法：fib.some_member_func()...
    }
    // 动态转换：运行时鉴定ns所指对象是否属于Fibonacci类
    if (Fibonacci *fib2 = dynamic_cast<Fibonacci*>(ns)) {
        // 调用fib的方法：fib.some_member_func()...
    }
}
```