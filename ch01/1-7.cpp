#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

// 通过标准输入读取多行文本，排序后并写入新的文本。
int main() {
    ifstream infile("data.txt");
    if (!infile) {
        cerr << "Cannot open file" << endl;
    }
    ofstream outfile("sorted.txt");
    if (!outfile) {
        cerr << "Cannot open file" << endl;
    }
    string word;
    vector<string> text;
    while (infile >> word) {
        text.push_back(word);
    }
    
    cout << "Unsorted words:"<< endl;
    for (int i = 0; i < text.size(); i++) {
        cout << text[i] << " " << endl;
    }
    cout << endl;

    // 按照字典序排序
    sort(text.begin(), text.end());

    outfile << "Sorted words:"<< endl;
    for (int i = 0; i < text.size(); i++) {
        outfile << text[i] << " " << endl;
    }
    outfile << endl;

    return 0;
}
