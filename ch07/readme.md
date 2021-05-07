## Chapter 7 异常处理

1、C++的异常处理机制有两个主要部分：**异常的鉴定与发出**，以及**异常的处理方式**。异常出现后，程序的执行便被暂停。与此同时，
异常处理机制开始搜索程序中有能力处理这一异常的地点。

2、本质上，异常是某种对象，它们通常属于特定的异常类（有自己的继承体系）。

（1）抛出异常：
在第四章，我们曾经抛出过如下的异常：
```C++
#include <iostream>
using namespace std;

class iterator_overflow {
public:
    iterator_overflow(int idx, int max): _index(idx), _max(max) {}
    int index() { return _index; }
    int max() { return _max; }
    void what_happened(ostream &os = cerr) {
        os << "Internal error: current index " << _index << " exceeds maximum bound: " << _max << endl;
    }

private:
    int _index;
    int _max;
};
```
抛出异常的代码：
```C++
if (_indedx >= _max_size) {
    // 通过throw表达式，直接调用类iterator_overflow对应的constructor
    throw iterator_overflow(_index, _max_size);
}
```

（2）捕获异常：
使用单条或者一串`catch`子句来补货被抛出的异常对象。`catch·子句由三部分组成：
* 关键字`catch`;
* 小括号内的一个**类型**或者**对象**；
* 打括号内的一组语句（用于处理异常）。

在通过了所有的`catch`子句之后，由正常程序重新接手。
```C++
bool func() {
    bool status = true;
    // ...
    catch (int errno) {
        log_msg(err_msgs[errno]);
        status = false;
    }
    catch (const char *str) {
        log_msg(str);
        status = false;
    }
    // 捕获到了一个iterator_overflow对象，然后调用该对象的成员函数打印到日志中
    catch (iterator_overflow &iof) {
        iof.what_happend(log_file);
        status = false;
    }
    return status;
}
```

（3）重新抛出异常：
如果当前捕获了异常的子句无法处理该异常，可以再次抛出。
```C++
catch (iterator_overflow &iof) {
        iof.what_happend(log_file);
        // 再次抛出异常时，只需要留下一个throw关键字即可。
        throw;
    }
}
```

（4）使用`catch(...)`一次性抛出所有异常：
```C++
catch (...) {
    // ...
}
```

（5）用一个例子来解释异常的触发与处理：
假设`_index`大于`Triangular::_max_size`，发生了什么？异常处理机制首先开始查看异常在何处抛出，是否位于`try`语句内？
然后发现没有，于是在`check_integrity()`的调用端继续查找类型吻合的`catch`语句。仍然没有找到，于是在`operator*()`的调用端继续寻找。
然后发现的确位于`try`块内，对比抛出的异常与`catch`语句中的异常是否相同，发现相同，然后进入`catch`块进行处理。

处理完毕之后，正常的程序执行**被执行的`catch`语句**下方的第一行语句，也就是`status = false`。
```C++
bool has_elem(Triangular_iterator first, Triangular_iterator last, int elem) {
    bool status = true;
    try {
        while (first != last) {
            if (*first == elem) return status;
            first++;
        }
    }
    // 只会捕获iterator_overflow这一种异常
    catch (iterator_overflow &iof) {
        log_msg(iof.what_happened());
    }
    status = false;
    return status;
}

// *first调用了本重载函数
int Triangular_iterator::operator*() {
    check_integrity();
    return _elems[_index];
}

// 上述重载函数调用了如下成员函数check_integrity()
void Triangular_iterator::check_integrity() {
    if (_index > Triangular::_max_size) {
        throw iterator_overflow(_index, Triangular::_max_size);
    }
}
```

总结以下：当函数的`try`块发生了某个异常，但没有相应的`catch`子句将它捕获，那么此函数便会被暂停，由异常处理机制接管，
沿着“函数调用链”一路回溯，搜寻符合条件的`catch`语句，一旦找到符合的，进入对应的`catch`块处理，完毕后将控制权交还给正常程序。

3、继承自标准异常：
标准库定义了一套异常类体系，根部是名为`exception`的抽象基类。`exception`声明了一个`what()`虚函数，会返回`const char *`，用于描述被抛出的异常的信息。
我们应当将自主编写的异常类继承自`exception`基类。这样做的好处是：当我们以`exception`来捕获异常时名，我们自己写的异常也可以被捕获到，并通过多态机制显示处真正的错误类型。
```C++
catch (const exception &ex) {
    cerr << ex.what() << endl;
}
```

按照这个策略重写前文的`iterator_overflow`：
```C++
#include <iostream>
#include <sstream>
#include <string>
#include <exception>            // 包含本头文件
using namespace std;

class iterator_overflow: public exception {
public:
    iterator_overflow(int idx, int max): _index(idx), _max(max) {}
    int index() { return _index; }
    int max() { return _max; }
    // override这个函数
    const char * what() const;

private:
    int _index;
    int _max;
};

const char * iterator_overflow:: what() const {
    ostringstream ex_msg;
    static string msg;
    // 将信息写到内存中的ostringstream对象中
    ex_msg << "Internal error: current index " << _index << " exceeds maximum bound: " << _max;
    // 从ostringstream对象中取得string
    msg = ex_msg.str();
    return msg.c_str();
}
```