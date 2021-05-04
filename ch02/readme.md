## Chapter 2 面向过程的编程风格

1、使用标准库`cstdlib`提供的`exit(-1)`函数中止程序。更好的方式是使用**异常处理**，后面会讲到。

2、使用模版类查询某个数据类型的最大值/最小值：
```C++
#include <iostream>
#include <limits>
using namespace std;

int main() {
    // 直接调用模版类的静态成员函数
    cout << numeric_limits<int>::max() << endl;
    cout << numeric_limits<double>::min() << endl;

    return 0;
}
```

3、当我们调用一个函数的时候，会在内存区域建立一块特殊区域（程序堆栈），这块区域提供了每个函数参数的存储空间和函数所定义的每个对象的内存空间，这些对象被称为“局部对象”。一旦函数完成，这块内存区域就会被释放掉（从堆栈中被pop出来）。因此，如果我们撰写了一个不带有返回值的参数，就需要认真思考这个函数是否真正改变了传入的参数。

这就引入了**传值**与**传址**两种参数传递方式。C++中函数只能有最多一个返回值，因此善用传址很有必要。
下面给出了一个例子：
```C++
#include <iostream>
#include <vector>
using namespace std;

void display(const vector<int> &vec);
void swap1(int v1, int v2);
void swap2(int &v1, int &v2);
void bubble_sort(vector<int> &vec);

int main() {
    int arr[8] = {8, 34, 3, 13, 5, 6, 8, 90};
    vector<int> vec(arr, arr + 8);
    bubble_sort(vec);
    display(vec);

    return 0;
}

// 加上const是为了方便阅读：表明本函数不会对输入的vec做任何更改
void display(const vector<int> &vec) {
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

// 传值：vec并未被修改
void swap1(int v1, int v2) {
    int tmp = v1;
    v1 = v2;
    v2 = tmp;
}

// 传址：函数体不变，但是vec中的两个元素被更改了
void swap2(int &v1, int &v2) {
    int tmp = v1;
    v1 = v2;
    v2 = tmp;
}

void bubble_sort(vector<int> &vec) {
    for (int i = 0; i < vec.size(); i++) {
        for (int j = i + 1; j < vec.size(); j++) {
            if (vec[j] < vec[i]) {
                swap2(vec[i], vec[j]);
            }
        }
    }
}
```
引用不同于指针，它必须代表某一个变量，无法被置为0。

4、**在一个函数的末尾返回局部对象的地址会导致意想不到的效果。**
一旦函数完成，这块内存区域就会被释放掉（local scope），而传值则不会出现错误。
一种解决方案是使用全局变量，但是，一般不建议重度使用全局作用域变量，因为这样会打破函数之间的独立性，降低代码的安全性。

下面有一个例子：
```C++
#include <iostream>
using namespace std;

// 返回了局部变量（数组M）的地址
float* Array(float *A,int length) {
    float M[5];
    for (int i = 0; i < length; i++)
        M[i] = A[i];
    return M;
}

int main() {
    float A[5] = {1.75, 0.25, 0, 0.75, 2.5};
    float *M = Array(A, 5);
    for (int i = 0; i < 5; i++)
        cout << M[i] << endl;
    cout << endl;
}
```
此时的输出为
```C++
1.75
4.59149e-41
2.59681e-30
1.4013e-45
2.59681e-30
```
结果只输出了数组首地址对应的数组值，其他数值都未输出。这是因为在函数中定义的数组`M`在函数执行完后已经被系统释放掉了（local scope）。
一种解决办法是声明全局变量（file scope），但是过多的全局变量会增强函数之间的耦合性，不推荐。实际上，不论为对象分配的内存是file scope还是local scope，
他们都是由编译系统自动管理的。第三种存储期方式为**动态范围（dynamic extent）**，由用户通过`new`操作在堆上分配。
```C++
int *p;
p = new int;            // 如果不提供初值，则不初始化
p = new int(1024);      // 从heap分配并初始化
p_arr = new int[20];    // 无法从heap分配数组的同时为其元素赋值
```
通过`delete`释放分配的空间（如果不手动delete，则该对象永远不会被释放，这被称为**内存泄漏**）：
```C++
delete p;               // 无需通过if(!p)来检查是否为空指针，编译器自行操作
delete []p_arr;         // 释放一个数组的空间占用的方法
```
基于此，重写`Array`函数如下：
```C++
// 本函数虽然在被调用者内部创建了指针M，但是M所指向的区域是在堆上进行分配的，因此调用返回时M所指向的区域仍然存在。
float* Array(float *A, int length) {
    float *M = new float[5];
    for (int i = 0; i < length; i++)
           M[i] = A[i];
    return M;
```

当然，使用**静态布局对象**也可以解决问题（后文给出）：
```C++
const float* Array(float *A,int length) {
    static float M[5];  //Array函数内部数组
    for (int i = 0; i < length; i++)
        M[i] = A[i];
    return M;
}
```

5、设定函数默认参数：
```C++
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// 在函数声明处指定默认参数值（放在头文件），程序代码文件中的函数的定义处无需再重复指定默认参数。
void display(const vector<int> &vec, ostream & = cout);
void swap2(int &v1, int &v2);
void bubble_sort(vector<int> &vec, ofstream *ofs = nullptr);

int main() {
    int arr[8] = {8, 34, 3, 13, 5, 6, 8, 90};
    vector<int> vec(arr, arr + 8);
    ofstream ofs("data.txt");
    if (!ofs) {
        cerr << "Error!" << endl;
        exit(-1);
    }
    bubble_sort(vec, &ofs);
    display(vec);

    return 0;
}

// 程序代码文件中的函数的定义处无需再重复指定默认参数
void display(const vector<int> &vec, ostream &os) {
    for (int i = 0; i < vec.size(); i++) {
        os << vec[i] << " ";
    }
    os << endl;
}

void swap2(int &v1, int &v2) {
    int tmp = v1;
    v1 = v2;
    v2 = tmp;
}

void bubble_sort(vector<int> &vec, ofstream *ofs) {
    for (int i = 0; i < vec.size(); i++) {
        for (int j = i + 1; j < vec.size(); j++) {
            if (vec[j] < vec[i]) {
                if (ofs != 0) {
                    (*ofs) << "About to call swap between " << vec[i] << " and " << vec[j] << endl;
                }
                swap2(vec[i], vec[j]);
            }
        }
    }
}
```

6、使用**静态局部对象**提高代码性能：
以斐波那契数列的计算为例，当我们计算`fib(10)`的时候，其实已经把`fib(1)`到`fib(9)`的值全部计算出来了。
然而，`fib(1)`到`fib(9)`的值会随着函数的调用结束而被遗忘，下次仍需要重新计算。如何保存已经计算出来的值的结果？

方法是使用静态局部对象。该类型对象所在空间在不同的函数调用过程中，**仍然持续存在**。
```C++
#include <iostream>
#include <vector>
using namespace std;

const vector<int> *fib(int size);

int main() {
    const vector<int> res = *fib(10);
    for (int i = 0; i < res.size(); i++) {
        cout << res[i] << endl;
    }
}

// 该函数返回的是一个指向特定内存区域首地址的指针，const关键字保证该指针在函数调用中不会被修改。
// 因为容器vector实际上是通过new在堆上创建的，因此这里返回的是指向堆内存空间的特定位置的指针。
// 这块特定区域内存储的数值被修改了，但是指针指向的区域首地址不变。
const vector<int> *fib(int size) {
    const int max_size = 1024;
    static vector<int> elems;
    if (size <= 0 || size > max_size) {
        cout << "invalid size"  << endl;
        return nullptr;
    }
    // 计算index为[elems.size()， size]区间的元素值
    for (unsigned i = elems.size(); i < size; i++) {
        if (i == 0 || i == 1) {
            elems.push_back(1);
        } else {
            elems.push_back(elems[i-1] + elems[i-2]);
        }
    }
    // 返回静态局部变量的首地址
    return &elems;
}
```
这里用`const`修饰函数的返回值是值得思考的（见注释部分）。被`const`修饰的对象都受到强制保护，可以预防意外的变动，能提高程序的健壮性。下面对`const`的使用做个梳理。

（1）定义`const`常量。这是最常用的使用方式，表示该常量对象无法再被修改。

（2）用`const`修饰函数的参数。对于非内部数据类型的输入参数，应该将“值传递”的方式改为“`const`引用传递”，目的是提高效率。
例如将`void Func(A a)`改为`void Func(const A &a)`。
对于内部数据类型的输入参数，不要将“值传递”的方式改为“`const`引用传递”。否则既达不到提高效率的目的，又降低了函数的可理解性。
例如`void Func(int x)`不应该改为`void Func(const int &x)`。这是因为内部数据类型的参数不存在构造、析构的过程，
而复制也非常快，“值传递”和“引用传递”的效率几乎相当。除了值传递和引用传递，传入参数若是指针，则`const`可起到防止意外改动该指针的作用。例如`void StrCpy(char *strDestination, const char *strSource)`。

（3）用`const`修饰函数的返回值。如果给以“指针传递”方式的函数返回值加`const`修饰，那么函数返回值（即指针）的内容不能被修改，
返回值只能被赋给加`const`修饰的同类型指针。上面静态局部对象的例子正是这样。

如果不使用静态局部变量，直接按照如下方式写代码：
```C++
#include <iostream>
#include <vector>
using namespace std;

vector<int> fib(int size);

int main() {
    vector<int> res = fib(10);
    for (int i = 0; i < res.size(); i++) {
        cout << res[i] << endl;
    }
}

vector<int> fib(int size) {
    const int max_size = 1024;
    static vector<int> elems;
    if (size <= 0 || size > max_size) {
        cout << "invalid size\n";
    }
    for (unsigned i = elems.size(); i < size; i++) {
        if (i == 0 || i == 1) {
            elems.push_back(1);
        } else {
            elems.push_back(elems[i-1] + elems[i-2]);
        }
    }
    return elems;
}
```
虽然我们返回了一个在函数内创建的对象，但是这并不会报错。具体原因这里再解释一遍。这是因为，该对象`elems`是一个容器，STL中容器都是通过`new`操作在堆上分配的，
它们不在file scope或local scope的管辖范围内。注意，我们使用静态局部对象的原因是**对象所在空间即使在不同的函数调用过程中，仍然持续存在**。
因为这里返回的不是特定的被创建的对象的首地址，因此，每次调用`fib()`函数，都相当于又在堆上分配了一块新的内存区域，这就导致了内存资源的浪费。

7、使用inline改善代码性能：
将函数声明为inline，表示要求编译器在该函数内的每个调用点上将被调用者的内容展开——**将调用操作改为以一份函数代码副本代替**。inline对编译器而言**不是强制性的**。
```C++
#include <iostream>
#include <vector>
using namespace std;

bool is_size_ok(int size);
const vector<int> *fib(int size);
inline bool find_elem(int pos, int &elem);

int main() {
    int elem;
    if (find_elem(10, elem)) {
        cout << elem << endl;
    }
}

bool is_size_ok(int size) {
    const int max_size = 1024;
    if (size <= 0 || size > max_size) {
        cout << "invalid size\n";
        return false;
    }
    return true;
}

const vector<int> *fib(int size) {
    static vector<int> elems;
    if (!is_size_ok(size)) {
        return nullptr;
    }
    for (unsigned i = elems.size(); i < size; i++) {
        if (i == 0 || i == 1) {
            elems.push_back(1);
        } else {
            elems.push_back(elems[i-1] + elems[i-2]);
        }
    }
    return &elems;
}

// 把三个函数的内容写入find_elem函数，但是维持三个独立的运算单元。
// 适合声明为inline的函数和find_elem一样，体积小、计算简单但是常常被调用。
inline bool find_elem(int pos, int &elem) {
    const vector<int> *res = fib(pos);
    if (!res) {
        elem = 0;
        return false;
    } else {
        elem = (*res)[pos - 1];
        return true;
    }
}
```
一般而言，函数的声明放在头文件，但是定义放在程序代码文件。但是，inline函数的定义常常被放在头文件中（见本文档第9条）。

8、从重载函数到模版函数

（1）**函数重载**：对于具有相同函数名的函数，通过**参数列表**来区分具体应该调用那一个函数。注意，函数的返回类型不足以将函数重载。
```C++
#include <iostream>
using namespace std;

void display(char ch);
void display(const string &s);
void display(const string &s, int i);

int main() {
    char ch = 'A';
    int i = 0;
    string s = "abc";
    display(ch);
    display(s);
    display(s, i);
}

void display(char ch) {
    cout << ch << endl;
}

void display(const string &s) {
    cout << s << endl;
}

void display(const string &s, int i) {
    cout << i << " " << s << endl;
}
```

（2）对于如下的函数实现需求：
```C++
void display(const string &s, const vector<int> &);
void display(const string &s, const vector<double> &);
void display(const string &s, const vector<string> &);
```
如果我们只需要定义一份函数内容，而不是将一份代码复制三份再针对每一份做一些微小的修改，就可以节省时间，避免犯错。
因此，需要一种机制**将单一函数的内容和希望达到的效果绑定起来**，这就是**函数模版**。函数模版扮演者“处方”一样的角色，通过它可以产生无数的函数。
```C++
#include <iostream>
#include <vector>
using namespace std;

// elemType是一个暂时放置类型的占位符
template <typename elemType> void display(const string &, const vector<elemType> &);

int main() {
    string s = "abc";
    vector<int> intVec;
    intVec.push_back(0);
    intVec.push_back(1);
    display(s, intVec);

    vector<string> strVec;
    strVec.push_back("str1");
    strVec.push_back("str2");
    display(s, strVec);
}

template <typename elemType> void display(const string &s, const vector<elemType> &vec) {
    cout << s << ": ";
    for (int i = 0; i < vec.size(); i++) {
        // cout << vec[i] << " ";
        elemType e = vec[i];
        cout << e << " ";
    }
    cout << endl;
}
```

（3）使用函数指针：

回到斐波那契数列inline函数的例子。该例子中，`const vector<int> *fib(int size)`返回了一个斐波那契数列，
`inline bool find_elem(int pos, int &elem)`则输出斐波那契数列的某个位置的数值。在inline函数中，只有一行调用了`fib()`函数，现在想要让`find_elem()`支持别的数列，如何写最少的代码？

——使用**函数指针**。
```C++
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

bool is_size_ok(int size);
// 下面是那个函数都使用静态局部变量并返回常指针以提供代码性能。
const vector<double> *fib_seq(int size);
const vector<double> *square_seq(int size);
const vector<double> *circle_seq(int size);

// const vector<double> * func(int)就是上述三个函数的声明方式。将func替换为(*seq_ptr)是因为后者是一个指向函数func的指针，
// 该指针通过直接赋予待传入的函数名来赋值。
bool find_elem(int pos, double &elem, const vector<double> * (*seq_ptr)(int));
// 设定枚举类型seq_types（seq_types是一个数据类型，而不是一个变量），花括号内的是枚举常量，只能以标识符的形式表示，不能是整型、字符型等文字常量。
// 枚举常量分别默认赋值0，1，2，可用于数组的索引。区分枚举类型、枚举常量和枚举变量！
enum seq_types {fib_seq_id, square_seq_id, circle_seq_id};

int main() {
    // 定义一个由函数指针组成的数组，这种数组的声明方式和上面无异，唯一区别是加上了[]表示这是一个数组
    const vector<double> * (*seq_arr[])(int) = {fib_seq, square_seq, circle_seq};
    // 依次输出每一种数列前三个元素
    for (auto & ptr : seq_arr) {
        double elem;
        for (int j = 1; j < 4; j++) {
            if (find_elem(j, elem, ptr)) {
                cout << elem << " ";
            }
        }
        cout << endl;
    }

    double elem;
    for (int j = 1; j < 4; j++) {
        // // 只关心斐波那契数列的输出，用enum变量的
        if (find_elem(j, elem, seq_arr[fib_seq_id])) {
            cout << elem << " ";
        }
    }

}

bool find_elem(int pos, double &elem, const vector<double> * (*seq_ptr)(int)) {
    const vector<double> *seq = seq_ptr(pos);
    if (seq_ptr == nullptr) {
        elem = 0;
        return false;
    }
    elem = (*seq)[pos - 1];
    return true;
}

const vector<double> *fib_seq(int size) {
    static vector<double> elems;
    if (!is_size_ok(size)) {
        return nullptr;
    }
    for (unsigned i = elems.size(); i < size; i++) {
        if (i == 0 || i == 1) {
            elems.push_back(1);
        } else {
            elems.push_back(elems[i-1] + elems[i-2]);
        }
    }
    return &elems;
}

const vector<double> *square_seq(int size) {
    static vector<double> elems;
    if (!is_size_ok(size)) {
        return nullptr;
    }
    for (unsigned i = elems.size(); i < size; i++) {
        elems.push_back(M_PI * i * i);
    }
    return &elems;
}

const vector<double> *circle_seq(int size) {
    static vector<double> elems;
    if (!is_size_ok(size)) {
        return nullptr;
    }
    for (unsigned i = elems.size(); i < size; i++) {
        elems.push_back(i * i);
    }
    return &elems;
}

bool is_size_ok(int size) {
    const int max_size = 1024;
    if (size <= 0 || size > max_size) {
        cout << "invalid size\n";
        return false;
    }
    return true;
}
```

9、**对于一般的函数，定义只能有一份，但是可以有多份声明**。因为一个头文件可能被多个代码文件包含，所以不建议将函数体定义在头文件中，
否则就违背“函数的定义只能有一份”这个要求了。
但是，**inline函数应该定义在头文件中**——这是因为在每一个调用点上，编译器都需要取得该inline函数的定义，放在头文件并被多个程序文件包含反而是合理的。

10、一个在file scope内定义的对象，如果可能被多个文件访问，那么应该被声明于头文件中（使用`extern`），然后让使用该变量的程序文件包含该头文件。
这是因为，如果我们只在单一的某个程序文件中声明它，别的程序文件是看不到的，因而是无法访问的。当然，我们也可以在每一个程序文件都用`extern`声明该变量，但是这显然不够简洁。
```C++
// 这是某个头文件
const int seq_cnt = 3;          // const object和inline函数一样，是一个例外，不需要加上extern，因为它可以被多次定义
extern const vector<double> * (*seq_arr[seq_cnt])(int);             // 这不是一个const object，而是一个指向const object的指针，因而需要带上extern关键字
extern enum seq_types {fib_seq_id, square_seq_id, circle_seq_id};   // 使用extern声明一个全局枚举变量
```

11、如果一个头文件是标准的或项目专属的头文件，则以**尖括号**将文件名括住，这样编译器会在默认的某些文件目录下寻找该头文件；
如果文件名由**双引号**括住，则被认为是用户提供的头文件，编译器会首先在项目文件目录下寻找。
