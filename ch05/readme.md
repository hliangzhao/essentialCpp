## Chapter 5 面向对象编程风格

1、面向对象编程的两个最主要特征是**继承**和**多态**。前者使得我们得以将一群相关的类组织起来，
让我们得以分享其间的共通数据和操作行为。后者让我们以一种类型无关的方式来操作类对象，并赋予我们更多的弹性来加入或移除任何特定类。

继承基值定义了父子关系。父类定义了所有子类共通的公有接口和私有实现。每个子类都可以增加或覆盖继承而来的东西，以实现其自身独特的行为。
在C++中，父类被称为基类，子类被称为派生类。

使用抽象基类的指针或引用来操纵其公共接口，实际执行起来时需要等到运行时才能确定指针或引用所指的实际对象的类型。

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
    // 加上关键字virtual，令该成员函数的解析在运行时进行
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
    // 被声明为protected的所有成员都可以被派生类直接访问，初派生类之外，无法被直接访问
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
可以发现构造函数和析构函数在派生类中的调用顺序正好相反。

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
    if (size <= 0 || size > _max_size) return false;
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
接下来，我们将用继承的方式来实现多态。

4、实现多态：

