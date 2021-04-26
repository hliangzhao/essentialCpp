## Chapter 3 面向过程的编程风格

1、标准模版库（STL）主要由两类组件：
+ **容器**。包括vector、list、set、map等；
+ 操作这些容器的**泛型算法**，包括`find()`、`sort()`、`merge()`、`repalce()`等。
牢记容器及相关泛型算法可以大大提高编程效率。

2、容器氛围顺序性容器和关联性容器。前者包含vector和list，主要在其上进行迭代操作；map和set属于后者，可以让我们快速查找容器中的元素值。

3、抽象出指针的算术运算，节省代码量：

以下部分有三个问题层层递进组成。实现一个查找函数满足如下需求：
+ 问题一：给定一个存储整数的vector以及一个整数值。如果该值在vector内则返回vector内指向该值的指针，否则返回空指针。
+ 问题二：如果存储数值的是数组，并且不要求数值类型是int，如何操作？
+ 问题三：如何再次改进使得find同时支持数组和vector？
```C++
#include <iostream>
#include <vector>
using namespace std;

const int *my_find(const vector<int> &vec, int value);
template <typename elemType> const elemType *my_find(const elemType *arr, int size, const elemType &value);
template <typename elemType> const elemType *my_find(const elemType *first, const elemType *last, const elemType &value);
template <typename elemType> inline const elemType *my_begin(const vector<elemType> &vec);
template <typename elemType> inline const elemType *my_end(const vector<elemType> &vec);

int main() {
    int arr[] = {1, 9, 8, 0, 4};
    // arr + 5: 最后一个元素的下一个地址。他扮演着"标兵"的角色
    const int *ptr = my_find(arr, arr + 5, arr[2]);
    cout << ptr <<": " << *ptr << endl;

    vector<int> vec(arr, arr + 5);
    const int *ptr2 = my_find(my_begin(vec), my_end(vec), vec[2]);
    cout << ptr2 <<": " << *ptr2 << endl;
}

// 问题一：给定一个存储整数的vector以及一个整数值。如果该值在vector内则返回vector内指向该值的指针，否则返回空指针。
const int *my_find(const vector<int> &vec, int value) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == value) return &vec[i];
    }
    return nullptr;
}

// 问题二：如果存储数值的是数组，并且不要求数值类型是int，如何操作？
// 方法1：传入数组首地址和数组大小
template <typename elemType> const elemType *my_find(const elemType *arr, int size, const elemType &value) {
    if (!arr || size < 1) return nullptr;
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) return &arr[i];
    }
    return nullptr;
}
// 方法2：传入数组首尾地址
template <typename elemType> const elemType *my_find(const elemType *first, const elemType *last, const elemType &value) {
    if (!first || !last) return nullptr;
    for (; first != last; first++) {
        if (*first == value) return first;
    }
    return nullptr;
}

// 问题三：如何再次改进使得find同时支持数组和vector？
// 要让问题二-方法2的函数支持vector，需要考虑到一下特殊情形：vector为空。需要补充两个判空的函数：
template <typename elemType> inline const elemType *my_begin(const vector<elemType> &vec) {
    return vec.empty()? nullptr: &vec[0];
}
template <typename elemType> inline const elemType *my_end(const vector<elemType> &vec) {
    return vec.empty()? nullptr: &vec[vec.size()];
}
```
现在，函数`template <typename elemType> const elemType *my_find(const elemType *first, const elemType *last, const elemType &value)`能够同时支持数组和vector了，可否让其再支持list？

单纯通过上述指针运算是无法实现的，这是因为list本质是一个双向链表，前后元素的地址不一定相邻。指针运算只有在元素被分配到连续的一块区域时才有效。
解决这个问题的办法是在底层指针的行位置上提供一层抽象。这就是**泛型指针iterator**的作用。

4、泛型指针：

先来看看STL中iterator的用法。
```C++
// c是一个containerType<elemType>的容器实例
containerType<elemType>::iterator iter = c.begin();             // 可读可写
containerType<elemType>::const_iterator iter = c.begin();       // 只能读取，不能写入
```

使用iterator重写`my_find()`用以同时支持数组、vector和list。
```C++
#include <iostream>
#include <vector>
#include <list>
using namespace std;

template <typename iterType, typename elemType> iterType my_find(iterType first, iterType last, const elemType &value);

int main() {
    int arr[] = {1, 9, 8, 0, 4};
    vector<int> vec(arr, arr + 5);
    list<int> l(arr, arr + 5);
    int *p = my_find(arr, arr + 5, arr[2]);
    cout << p << ": " << *p << endl;
    vector<int>::iterator it1;
    it1 = my_find(vec.begin(), vec.end(), vec[2]);
    if (it1 != vec.end()) {
        cout << *it1 << endl;
    }
    list<int>::iterator it2;
    it2 = my_find(l.begin(), l.end(), arr[2]);
    if (it2 != l.end()) {
        cout << *it2 << endl;
    }
}

template <typename iterType, typename elemType> iterType my_find(iterType first, iterType last, const elemType &value) {
    for (; first != last; first++) {
        if (value == *first) return first;
    }
    return last;
}
```

5、泛型算法可大致划分如下几类（TODO：补充常用泛型算法）。：
+ **搜索**：`find()`、`count()`、`adjcent_find()`、`finf_if()`、`binary_search()`等；
+ **排序**及**次序整理**：`merge()`、`reverse()`、`rotate()`、`sort(0`、`partition()`、`random_shuffle()`等；
+ **复制**，**删除**与**替换**：`copy()`、`remove()`、`replace()`、`swap()`、`unique()`等；
+ **关系**：`equal()`、`includes()`、`mismatch()`等；
+ **生成与质变**：`fill()`、`for_each()`、`generate()`、`transform()`等；
+ **数值操作**：`accumulate()`、`partial_sum()`、`inner_product()`、`adjcent_difference()`等；
+ **集合操作**：`set_union()`、`set_difference()`等。


6、所有容器类以及string类都具备如下通用操作：
+ `==`和`!=`
+ 赋值`=`
+ 判空`empty()`
+ 大小`size()`
+ 清空`clear()`
+ 迭代器`begin()`和`end()`
+ 插入与删除`insert(pos, value)`和`erase(pos)`

7、使用顺序性容器：

