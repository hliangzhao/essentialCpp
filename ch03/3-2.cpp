#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
using namespace std;

// 定义一个function object类
class LessThan {
public:
    bool operator() (const string &s1, const string &s2) {
        return s1.size() < s2.size();
    }
};

template <typename elemType> void display(const vector<elemType> &vec, ostream &os = cout, int len = 8) {
    auto it = vec.begin(), it_end = vec.end();
    int elem_cnt = 1;
    while (it != it_end) {
        os << *it++ << (!(elem_cnt++ % len) ? '\n': ' ');
    }
    os << endl;
}

int main() {
    ifstream in_file("data.txt");
    ofstream out_file("res.txt");
    if (!in_file || !out_file) {
        cerr << "Cannot open" << endl;
    }
    vector<string> text;
    string word;
    while (in_file >> word) {
        text.push_back(word);
    }
    sort(text.begin(), text.end(), LessThan());
    display(text, out_file);
}

