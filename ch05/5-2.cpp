#include <iostream>
#include <vector>
#include <string>
using namespace std;

typedef string elemType;

// 具象基类
class Stack {
public:
    explicit Stack(int cap = 0): _top(0) {
        if (cap) _stack.reserve(cap);
    }

    bool pop(elemType &);
    bool push(const elemType &) ;
    virtual bool peek(int, elemType &) { return false; }

    int top() const { return _top; }
    int size() const { return int(_stack.size()); }
    bool empty() const { return !_top; }
    bool full() const { return size() >= _stack.max_size(); }

    void print(ostream &os = cout) const;

protected:
    vector<elemType> _stack;
    int _top;
};

class PeekBackStack: public Stack {
public:
    explicit PeekBackStack(int cap = 0): _top(0) {
        if (cap) _stack.reserve(cap);
    }
    bool peek(int, elemType &);
};


// 程序代码文件
bool Stack::pop(elemType &elem) {
    if (empty()) return false;
    elem = _stack[--_top];
    _stack.pop_back();
    return true;
}

bool Stack::push(const elemType &elem) {
    if (full()) return false;
    _stack.push_back(elem);
    _top++;
    return true;
}

void Stack::print(ostream &os) const {
    auto r_it = _stack.rbegin(), r_end = _stack.rend();
    os << "\n\t";
    while (r_it != r_end) {
        os << *r_it++ << " ";
    }
    os << endl;
}

bool PeekBackStack::peek(int index, elemType &elem) {
    if (empty()) return false;
    if (index < 0 || index >= size()) return false;
    elem = _stack[index];
    return true;
}

void peek(Stack &s, int index) {
    cout << endl;
    string t;
    if (s.peek(index, t)) {
        cout << "peek: " << t;
    } else {
        cout << "peek failed!" << endl;
    }
}

