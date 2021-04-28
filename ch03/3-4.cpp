#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
using namespace std;

class even_elem {
public:
    bool operator()(int elem) {
        return elem % 2? false: true;
    }
};

int main() {
    ifstream in_file("data");
    if (!in_file) {
        cerr << "Cannot open file" << endl;
        exit(-1);
    }
    vector<int> res;
    istream_iterator<int> in(in_file), eos;
    copy(in, eos, back_inserter(res));

    auto division = partition(res.begin(), res.end(), even_elem());

    ofstream even_file("even"), odd_file("odd");
    if (!even_file || !odd_file) {
        cerr << "Cannot create file" << endl;
        exit(-1);
    }
    ostream_iterator<int> even_iter(even_file, "\n"), odd_iter(odd_file, " ");
    copy(res.begin(), division, even_iter);
    copy(division, res.end(), odd_iter);
}
