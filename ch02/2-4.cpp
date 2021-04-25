#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool check(int pos);
const vector<int> *pentagonal_series(int pos);
bool pentagonal_elem(int pos, int &elem);

int main() {
    int elem;
    const string title("Pentagonal numeric series");
    if (pentagonal_elem(8, elem)) {
        cout << elem << endl;
    }
    if (pentagonal_elem(14, elem)) {
        cout << elem << endl;
    }
    if (pentagonal_elem(120, elem)) {
        cout << elem << endl;
    }
}

bool check(int pos) {
    return (pos <= 0 || pos > 64)? false: true;
}

const vector<int> *pentagonal_series(int pos) {
    static vector<int> elems;
    if (check(pos) && pos > elems.size()) {
        for (int i = elems.size() + 1; i <= pos; i++) {
            elems.push_back(i * (3*i - 1) / 2);
        }
    }
    return &elems;
}

bool pentagonal_elem(int pos, int &elem) {
    if (!check(pos)) {
        cerr << "Invalid position!" << endl;
        elem = 0;
        return false;
    }
    const vector<int> *res = pentagonal_series(pos);
    elem = (*res)[pos - 1];
    return true;
}

