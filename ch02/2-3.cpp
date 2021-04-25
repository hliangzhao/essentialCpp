#include <iostream>
#include <string>
#include <vector>
using namespace std;

extern void real_calc_elems(vector<int> &vec, int);
inline bool calc_elements(vector<int> &vec, int pos);
void display(vector<int> &vec, const string &title, ostream &os = cout);

int main() {
    vector<int> res;
    const string title("Pentagonal numeric series");
    if (calc_elements(res, 0)) {
        display(res, title);
    }
    if (calc_elements(res, 14)) {
        display(res, title);
    }
    if (calc_elements(res, 120)) {
        display(res, title);
    }
}

bool calc_elements(vector<int> &vec, int pos) {
    if (pos <= 0 || pos > 64) {
        cerr << "Invalid position!" << endl;
        return false;
    }
    if (vec.size() < pos) {
        real_calc_elems(vec, pos);
    }
    return true;
}

void real_calc_elems(vector<int> &vec, int pos) {
    for (int i = vec.size() + 1; i <= pos; i++) {
        vec.push_back(i * (3*i - 1) / 2);
    }
}

void display(vector<int> &vec, const string &title, ostream &os) {
    os << '\n' << title << "\n\t";
    for (int i = 0; i < vec.size(); i++) {
        os << vec[i] << " ";
    }
    os << endl;
}

