#include <iostream>
#include <vector>
#include <string>
using namespace std;

typedef string elemType;

// 抽象基类（接口的集合）
class Stack {
public:
    virtual ~Stack() {}

    virtual bool pop(elemType &) = 0;
    virtual bool push(const elemType &) = 0;
    virtual bool peek(int, elemType &) = 0;

    virtual int top() const = 0;
    virtual int size() const = 0;

    virtual bool empty() const = 0;
    virtual bool full() const = 0;
    virtual void print(ostream &os = cout) const = 0;
};

ostream& operator<<(ostream &os, const Stack &rhs) {
    rhs.print(os);
    return os;
}

class FIFOStack: public Stack {
public:
    explicit FIFOStack(int cap = 0): _top(0) {
        if (cap) _stack.reserve(cap);
    }

    bool pop(elemType &) override;
    bool push(const elemType &) override;
    bool peek(int, elemType &) override { return false; }

    int top() const override { return _top; }
    int size() const override { return int(_stack.size()); }
    bool empty() const override { return !_top; }
    bool full() const override { return size() >= _stack.max_size(); }

    void print(ostream &os = cout) const override;

private:
    vector<elemType> _stack;
    int _top;
};

class PeekBackStack: public Stack {
public:
    explicit PeekBackStack(int cap = 0): _top(0) {
        if (cap) _stack.reserve(cap);
    }

    bool pop(elemType &) override;
    bool push(const elemType &) override;
    bool peek(int, elemType &) override;

    int top() const override { return _top; }
    int size() const override { return int(_stack.size()); }
    bool empty() const override { return !_top; }
    bool full() const override { return size() >= _stack.max_size(); }

    void print(ostream &os = cout) const;

private:
    vector<elemType> _stack;
    int _top;
};


// 程序代码文件
bool FIFOStack::pop(elemType &elem) {
    if (empty()) return false;
    elem = _stack[--_top];
    _stack.pop_back();
    return true;
}

bool FIFOStack::push(const elemType &elem) {
    if (full()) return false;
    _stack.push_back(elem);
    _top++;
    return true;
}

void FIFOStack::print(ostream &os) const {
    auto r_it = _stack.rbegin(), r_end = _stack.rend();
    os << "\n\t";
    while (r_it != r_end) {
        os << *r_it++ << " ";
    }
    os << endl;
}

bool PeekBackStack::pop(elemType &elem) {
    if (empty()) return false;
    elem = _stack[--_top];
    _stack.pop_back();
    return true;
}

bool PeekBackStack::push(const elemType &elem) {
    if (full()) return false;
    _stack.push_back(elem);
    _top++;
    return true;
}

bool PeekBackStack::peek(int index, elemType &elem) {
    if (empty()) return false;
    if (index < 0 || index >= size()) return false;
    elem = _stack[index];
    return true;
}

void PeekBackStack::print(ostream &os) const {
    auto r_it = _stack.rbegin(), r_end = _stack.rend();
    os << "\n\t";
    while (r_it != r_end) {
        os << *r_it++ << " ";
    }
    os << endl;
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

