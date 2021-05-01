#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 头文件 ===
class Stack {
public:
    bool push(const string &);
    bool pop(string &elem);
    bool peek(string &elem);
    bool empty() const {
        return _stack.empty();
    }
    bool full() const {
        return _stack.size() == _stack.max_size();
    }
    int size() const {
        return (int)_stack.size();
    }
    bool find(const string &elem) const;
    int count(const string &elem) const;

private:
    vector<string> _stack;
};
// 头文件结束 ===


// 程序代码文件 ===
bool Stack::push(const string &elem) {
    if (full()) return false;
    _stack.push_back(elem);
    return true;
}

bool Stack::pop(string &elem) {
    if (empty()) return false;
    elem = _stack.back();
    _stack.pop_back();
    return true;
}

bool Stack::peek(string &elem) {
    if (empty()) return false;
    elem = _stack.back();
    return true;
}

bool Stack::find(const string &elem) const {
    return ::find(_stack.begin(), _stack.end(), elem) != _stack.end();
}

int Stack::count(const string &elem) const {
    return ::count(_stack.begin(), _stack.end(), elem);
}

int main() {
    Stack s;
    string str;
    while (cin >> str && !s.full()) {
        s.push(str);
    }
    if (s.empty()) {
        cout << "No strings read in" << endl;
        return 0;
    }
    s.peek(str);
    // 更多测试操作...
}
// 程序代码文件结束 ===

