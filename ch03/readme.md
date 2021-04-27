## Chapter 3 泛型编程风格

1、标准模版库（STL）主要由两类组件：
+ **容器**。包括vector、list、set、map等；
+ 操作这些容器的**泛型算法**，包括`find()`、`sort()`、`merge()`、`repalce()`等。
牢记容器及相关泛型算法可以大大提高编程效率。

容器分为顺序性容器和关联性容器。前者包含vector和list，主要在其上进行迭代操作；map和set属于后者，可以让我们快速查找容器中的元素值。

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

5、所有容器类以及string类都具备如下通用操作：
+ `==`和`!=`
+ 赋值`=`
+ 判空`empty()`
+ 大小`size()`
+ 清空`clear()`
+ 迭代器`begin()`和`end()`
+ 插入与删除`insert(pos, value)`和`erase(pos)`

6、使用顺序性容器：

最主要的顺序性容器是vector和list。
+ 对vector的内容进行随机访问（random access）效率很高，但是在任意位置进行插入和删除的效率很低；
+ list本质上是一个双向链表。对list的任意位置进行元素的插入和删除操作效率都很高，只需设定正确的back指针和front指针即可，但是对list进行随机访问效率很低。

上述特征表示，如果我们需要反复随机访问元素，应当使用vector；如果需要反复增删改元素，应该使用list。

另一种常用的顺序性容器是deque。在STL中，deque以连续内存空间存储元素，**对于首位元素的插入和删除操作效率很高**。STL中的queue以deque作为底部存储元素。

（1）顺序性容器的定义：
```C++
#include <iostream>
#include <vector>
#include <deque>
#include <list>
using namespace std;

int main() {
    // 产生空容器
    list<int> l;
    vector<int> v;
    deque<int> d;

    // 产生特定大小的容器，不手动设定初值，则每个元素以元素类型的默认值作为初值（int：0）
    list<int> l1(1024);             // 不设定初值
    vector<int> v1(1024, -1);       // 设定初值
    deque<int> d1(1024);
    
    // 通过一对iterator产生容器
    int arr[4] = {1, 2, 3, 4};
    list<int> l2(arr, arr + 4);
    vector<int> v2(arr, arr + 4);
    deque<int> d2(arr, arr + 4);
    
    // 根据已有容器产生新容器（整个复制）
    list <int> l3(l2);
}
```

（2）顺序性容器共有的操作：在尾部进行插入和删除的操作——`push_back()`和`pop_back()`。
此外，list和deque还提供了在首部进行插入和删除的操作`push_front()`和`pop_front()`（因为它们首尾都有指针）。

**注意：**`pop_back()`和`pop_front()`不会返回被弹出的元素，如果想要获得尾部和首部的元素，应当使用`back()`和`front()`。
```C++
#include <iostream>
#include <vector>
#include <deque>
#include <list>
using namespace std;

int main() {
    int arr[4] = {1, 2, 3, 4};
    list<int> l(arr, arr + 4);
    vector<int> v(arr, arr + 4);
    deque<int> d(arr, arr + 4);

    l.push_back(5); l.pop_back();
    v.push_back(5); v.pop_back();
    d.push_back(5); d.pop_back();

    l.push_front(0);
    d.push_front(0);

    for (list<int>::iterator it = l.begin(); it != l.end(); it++) cout << *it << " ";
    // 使用range-based for loop简化操作（C++11引入）
    for (int & it : v) cout << it << " ";
    for (deque<int>::iterator it = d.begin(); it != d.end(); it++) cout << *it << " ";
}
```

（3）`push_back()`和`push_front()`都属于特殊化的插入操作`insert()`。
```C++
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
using namespace std;

int main() {
    int arr[4] = {1, 2, 3, 4};
    vector<int> v(arr, arr + 4);

    // iterator insert(iterator position, elemType value)
    vector<int>::iterator it = v.begin() + 1;
    vector<int>::iterator new_it = v.insert(it, 5);
    cout << *new_it << endl;

    // void insert(iterator position, int count, elemType)
    string str("abc");
    vector<string> s;
    auto sIt = s.begin();           // C++11引入的新特性，自行判定迭代器的类
    s.insert(sIt, 5, str);

    //void insert(iterator1 position, iterator2 first, iterator2 last)
    int ia1[] = {1, 2, 3, 4, 5, 55, 89};
    int ia2[] = {8, 13, 21};
    list<int> elems(ia1, ia1 + 7);
    auto lIt = find(elems.begin(), elems.end(), 55);
    elems.insert(lIt, ia2, ia2 + 4);
}
```

（4）`pop_back()`和`pop_front()`都属于特殊化的删除操作`erase()`。
```C++
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
using namespace std;

int main() {
    int arr[4] = {1, 2, 3, 4};
    vector<int> v(arr, arr + 4);

    // iterator erase(iterator position)
    auto it = v.erase(v.begin());
    // 结合find删除特定元素，返回最后一个被删除元素的下一位置
    it = find(v.begin(), v.end(), 3);
    v.erase(it);
    
    // iterator erase(iterator first, iterator last)
    list<int> l;
    for (int i = 0; i < 10; i++) {
        l.push_back(i + 1);
    }
    auto it1 = find(l.begin(), l.end(), 5);
    auto it2 = find(l.begin(), l.end(), 8);
    // 返回最后一个被删除元素的下一位置
    it1 = l.erase(it1, it2);
}
```


7、泛型算法可大致划分如下几类（TODO：补充常用泛型算法的使用）。：
+ **搜索**：`find()`、`count()`、`adjcent_find()`、`finf_if()`、`binary_search()`等；
+ **排序及次序整理**：`merge()`、`reverse()`、`rotate()`、`sort(0`、`partition()`、`random_shuffle()`等；
+ **复制，删除与替换**：`copy()`、`remove()`、`replace()`、`swap()`、`unique()`等；
+ **关系**：`equal()`、`includes()`、`mismatch()`等；
+ **生成与质变**：`fill()`、`for_each()`、`generate()`、`transform()`等；
+ **数值操作**：`accumulate()`、`partial_sum()`、`inner_product()`、`adjcent_difference()`等；
+ **集合操作**：`set_union()`、`set_difference()`等。


8、如何实现一个泛型算法？接下来以一个层层递进例子进行阐述。

（1）**对于一个给定的vector，如何返回一个由该vector中小于给定数值的元素组成的新的vector？**

解决方案：
```C++
#include <iostream>
#include <vector>
using namespace std;

vector<int> filter(const vector<int> &vec, int filter_value) {
    vector<int> res;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] < filter_value) {
            res.push_back(vec[i]);
        }
    }
    return res;
}

int main() {
    vector<int> v;
    for (int i = 5; i <= 15; i++) {
        v.push_back(i);
    }
    vector<int> res = filter(v, 10);
    for (auto &it: res) {
        cout << it << " ";
    }
}
```

（2）**如果允许用户指定不同的比较操作（不限于小于），如何操作？**——传入函数指针。
```C++
#include <iostream>
#include <vector>
using namespace std;

vector<int> filter(const vector<int> &vec, int filter_value, bool (*pred)(int, int)) {
    vector<int> res;
    for (int i = 0; i < vec.size(); i++) {
        if (pred(vec[i], filter_value)) res.push_back(vec[i]);
    }
    return res;
}
bool less_than(int v1, int v2) {
    return v1 < v2? true: false;
}
bool greater_than(int v1, int v2) {
    return v1 > v2? true: false;
}
// 更多自定义比较运算...

int main() {
    vector<int> v;
    for (int i = 5; i <= 15; i++) {
        v.push_back(i);
    }
    vector<int> res = filter(v, 10, less_than);
    for (auto &it: res) {
        cout << it << " ";
    }
}
```

（3）**通过函数指针来调用传入的比较方法仍然有不可忽视的代价，还有没有别的方法？**——使用函数对象（function object）。
函数对象是类的实例对象，这些类对函数调用运算符做了重载操作，是的函数对象可以被当作一般函数来使用。

在下面的例子中，通过传入函数对象`greater<int>()`给`sort()`函数，使得`sort()`按照降序排列。
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
    sort(v.begin(), v.end(), greater<int>());           // 类是greater<int>，通过调用构造函数greater<int>()产生该类的一个实例对象传入
    for (auto it = v.begin(); it != v.end(); it++) cout << *it << " ";
    cout << endl;
}
```

然而，诸如`greater<>()`或`less<>()`等函数对象传入的参数是两个（二元判断式），而在我们的问题中，第二个参数需要被指定为`filter_value`，
我们可以使用绑定适配器（binder adapter）来对第二个参数进行绑定，从而将二元函数对象转变为一元函数对象。下面给出了结合函数对象和参数适配器的解决方案：
```C++
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

vector<int> filter(const vector<int> &vec, int val, less<int> &lt) {
    vector<int> res;
    auto it = vec.begin();
    // 对于传入的函数对象l（是类less<int>的一个实例），使用bind操作将该函数对象的第二个参数绑定val
    // 这里使用find_if()，因为find_if()第三个参数为Predicate，它在区间[first,end)中搜寻使一元判断式pred为true的第一个元素
    // 而find()第三个参数为待寻找的元素值
    while ((it = find_if(it, vec.end(), bind2nd(lt, val))) != vec.end()) {
        res.push_back(*it);
        it++;
    }
    return res;
}

int main() {
    int arr[] = {1, 4, 3, 56, 7, 89, 90, -8, 91, 100};
    vector<int> v(arr, arr + 10);
    for (auto it = v.begin(); it != v.end(); it++) cout << *it << " ";
    cout << endl;

    // 声明一个函数对象并传入
    less<int> lt;
    vector<int> res = filter(v, 50, lt);

    for (auto it = res.begin(); it != res.end(); it++) cout << *it << " ";
    cout << endl;
}
```

（4）**如果一开始给定不是vector，而是别的容器，要如何更改`filter()`使其泛化？**——这个简单，使用函数模版就可以了。
```C++
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

template <typename inIter, typename outIter, typename elemType, typename comp>
outIter filter(inIter first, inIter last, outIter at, const elemType &val, comp pred) {
    while ((first = find_if(first, last, bind2nd(pred, val))) != last) {
        *at++ = *first++;       // 存放满足要求的元素
    }
}

int main() {
    int arr[] = {1, 4, 3, 56, 7, 89, 90, -8, 91, 100};
    vector<int> v(arr, arr + 10);

    int arr2[10];
    vector<int> v2(10);
    filter(arr, arr + 10, arr2, 50, less<int>());
    filter(v.begin(), v.end(), v2.begin(), 50, greater<int>());

    // 因为arr2和v2的元素个数小于10，但我们无法知道具体有多少个，因此全部打印出来
    for (auto i: arr2) {
        cout << i << " ";           // 后面的元素未经初始化，可以是任意值
    }
    cout << endl;
    for (auto i : v2) {
        cout << i << " ";           // 后面的元素都是0
    }
    cout << endl;
}
```
至此，我们实现的函数和元素类型无关，无比较操作无关，约合容器类型无关了。可以说，我们实现了了一个泛型算法。
