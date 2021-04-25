#include <iostream>
#include <vector>
using namespace std;

bool is_size_ok(int size);
const vector<int> *fib(int size);
inline bool find_elem(int pos, int &elem);

int main() {
    int pos, elem;
    char ch;
    bool more = true;
    while (more) {
        cin >> pos;
        if (find_elem(pos, elem)) {
            cout << elem << elem << endl;
        } else {
            cout << "Error" << endl;
        }
        cout << "Try again?" << endl;
        cin >> ch;
        if (ch != 'y' && ch != 'Y') {
            more = false;
        }
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
