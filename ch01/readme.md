## Chapter 1 C++编程基础

1、数据的输入输出，并非C++程序语言本身定义的一部分，而是由C++的一套**面向对象的类层次体系**提供支持，并作为C++标准库的一员。

2、class的定义一般非为两部分，头文件和程序代码文件。
+ **头文件**用来声明该class所提供的各种操作行为；
+ **程序代码文件**则包含了这些操作行为的具体实现（定义）。

3、**命名空间**是一种将库名称封装起来的办法，从而避免命名冲突。`using namespace ns`是将命名空间`ns`内的定义的名称曝光的最简方法。

4、定义对象需要为该对象命名并赋予数据类型。数据类型决定了对象所能持有的数值范围以及占用多少内存空间。内置的数据类型包括**布尔值、整数、浮点数和字符**。我们也可以通过`struct`和`class`定义自己的数据类型。

5、从“自定义类也是数据类型”的角度来理解对象的初始化：`int x = 0;`和`int x(0)`都是合法的初始化方法，前者沿袭自C语言，当对象属于内置类型或者可以**通过单一值加以初始化**的时候合法；后者被称为构造函数语法，主要用于对象需要多个初值才能成功初始化的场合。例如：
```C++
#include <complex>
complex<double> p(0, 7);
```
本例中`complex`是一个**模版类**，直到我们使用该类的时候才需要真正决定数据类型（本例是`double`）。

6、定义数组时，数组的大小必须是常量表达式，即不需要在运行时通过求值才能确定的表达式（但是g++有扩展支持这个特性）。**变长数组实现是分配在栈上的，如果数组大小很大，可能会爆栈，因此使用定长数组更稳妥。**
```C++
const int len = 100;        // 如果此处没有const关键字，那么其实是变长数组，因为其值总可以在运行时被改变。
int arr[len];
```
相比于数组，使用模版类vector更好。
```C++
#include <vector>
vector<int> vec(len);       // 允许变长，甚至直接vector<int> vec;也是对的（即不指定长度）
```

初始化数组的时候允许不指定长度，编译器可自行计算大小：
```C++
int arr[] = {1, 2, 3, 4};
```
用一个数组来初始化vector：
```C++
vector<int> vec(arr, arr + arr_len);
```

7、一个不指向任何对象的指针，地址值为0，也被称为null指针。初始化null指针的方法为`int *ptr = 0`。提领指针的时候，表达式通常这样写：`if (ptr && ...)`防止提领一个空指针而报错。

8、使用标准库提供的伪随机数生成器：
```C++
#include <cstdlib>
srand(some_int_value);          // 输入伪随机数生成器种子
out = rand() % max_value;       // rand()返回[0, 最大int值]之间的一个整数，通过取模运算限定输出范围
```

9、文件读写基本操作：

（1）写文件：
```C++
#include <iostream>
#include <fstream>      // 包含此头文件
using namespace std;

int main() {
    // 打开一个可供输出的文件（create），使用追加模式
    // 注意第一个参数的类型是const char *。string转char *可以通过str.c_str()来完成
    ofstream outfile("data.txt", ios_base::app);
    if (!outfile) {
        cerr << "Can't open file" << endl;
        exit(-1);
    } else {
        // 写入内容到文件
        // endl属于库提供的操纵符，他们的作用是在iostream上执行一些操作。
        // endl：在当前iostream后追加一个换行符，并清空输出缓冲区
        outfile << "abc" << ' ' << endl;
        outfile << "def" << ' ' << endl;
    }
}
```

（2）读文件：
```C++
#include <iostream>
#include <fstream>
using namespace std;

int main() {
    // 打开一个可供读取的文件
    ifstream infile("data.txt");
    if (!infile) {
        cerr << "Can't open file" << endl;
        exit(-1);
    } else {
        // data.txt中一行中有一个string，两个整数
        string a;
        int b, c;
        // 依次从文本中读取一行的内容，一旦抵达文件末尾，infile >> a将返回false，因此while循环中止
        while (infile >> a) {
            infile >> b >> c;
            cout << a << ' ' << b << ' ' << c << endl;
        }
    }
}
```
可以发现，操作文件句柄与操作标准输入输出流`cin`和`cout`没有任何区别，只不过使用它们之前要明确指出该句柄要操作的文件名称以及操作的模式。

（3）同时读写：声明`fstream iofile("data.txt", ios_base::in|ios_base::app)`。因为使用了追加写模式，所以文件位置会被定位在末尾。如果不更正文件位置，读文件是会直接“文件结束”，因此读之前需要使用`iofile.seekg(0)`重新定位至文件起始位置。


10、string对象和C-style字符数组之间有两个主要差异：
+ string对象会动态地随着字符串长度而增加其存储空间，C-style字符数组只能分配固定空间，并**期望**这个空间可以容纳对应的字符串；
+ C-style字符数组不记录自身有效长度，需要遍历数组直到遇到`\0`字符（`int strlen(const char *)`的工作原理）。string对象使用`str.size()`获得其自身的长度。
```C++
#include <iomanip>
const int name_size = 128;
char username[name_size];
// 用操纵符setw保证读入字符个数不会超过127
cin >> setw(name_size) >> username;
```

11、读入一行不定长数据的方法：直接将读入操作放在while循环内，做编程题时，到文件末尾会自动退出while循环。

（1）使用数组：
```C++
const int max_size = 128;
int arr[max_size];
int v, i = 0;
while(cin >> v && i < max_size) {
    arr[i++] = v;
}
```

（2）使用vector：
```C++
vector<int> vec;
int v;
while(cin >> v) {
    vec.push_back(v);
}
```
