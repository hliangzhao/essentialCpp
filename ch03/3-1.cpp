#include <iostream>
#include <map>
#include <algorithm>
#include <fstream>
#include <set>
using namespace std;

void init_exclusion_set(set<string> &exs);
void process_file(map<string, int> &word_cnt, const set<string> &exs, ifstream &in_file);
void user_query(const map<string, int> &word_cnt);
void display(const map<string, int> &word_cnt, ofstream &os);

int main() {
    ifstream in_file("data.txt");
    ofstream out_file("res.txt");
    if (!in_file || !out_file) {
        cerr << "Cannot open" << endl;
    }
    set<string> exs;
    init_exclusion_set(exs);
    map<string, int> word_cnt;
    process_file(word_cnt, exs, in_file);
    display(word_cnt, out_file);
    user_query(word_cnt);
}

void init_exclusion_set(set<string> &exs) {
    static string _words[] = {"the", "and", "but", "then", "are", "been", "can", "a", "have", 
                             "could", "for", "of", "have", "had", "when", "where", "would"};
    exs.insert(_words, _words + 17);
}

void process_file(map<string, int> &word_cnt, const set<string> &exs, ifstream &in_file) {
    string word;
    while (in_file >> word) {
        if (exs.count(word)) continue;
        word_cnt[word]++;
    }
}

void user_query(const map<string, int> &word_cnt) {
    string search_word;
    cin >> search_word;
    map<string, int>::const_iterator it;
    if ((it = word_cnt.find(search_word)) != word_cnt.end()) {
        cout << it->first << " " << it->second << endl;
    } else {
        cout << "Not found" << endl;
    }
}

void display(const map<string, int> &word_cnt, ofstream &os) {
    auto it = word_cnt.begin(), it_end = word_cnt.end();
    while (it != it_end) {
        os << it->first << " " << it->second << endl;
    }
    os << endl;
}
