#include <iostream>
#include <map>
#include <vector>
#include <fstream>
using namespace std;

void get_population(ifstream &in_file, map<string, vector<string>> &families);
void display(const map<string, vector<string>> &families, ostream &os = cout);
void query(const string &fam_name, const map<string, vector<string>> &families);

int main() {
    map<string, vector<string>> families;
    ifstream in_file("data");
    if (!in_file) {
        cerr << "Cannot open file" << endl;
        exit(-1);
    }
    get_population(in_file, families);

    display(families);
    cout << endl;

    string fam_name;
    while (true) {
        cout << "Enter a family name to query (q to quit): ";
        cin >> fam_name;
        if (fam_name == "q") break;
        query(fam_name, families);
    }
}

void get_population(ifstream &in_file, map<string, vector<string>> &families) {
    string line;
    while (getline(in_file, line)) {
        string fam_name;
        vector<string> children;
        // use two pointers to get each substring (family name or child name)
        string::size_type pos = 0, prev_pos = 0;
        int name_num = 0;
        while ((pos = line.find_first_of(' ', pos)) != string::npos) {
            string::size_type end_pos = pos - prev_pos;
            if (!prev_pos) {
                // the first string, ie., the family name
                fam_name = line.substr(prev_pos, end_pos);
                name_num++;
            } else {
                // the child name
                children.push_back(line.substr(prev_pos, end_pos));
                name_num++;
            }
            // update the search string range
            prev_pos = ++pos;
        }
        if (name_num == 0) {
            fam_name = line.substr(prev_pos, pos - prev_pos);
        } else {
            children.push_back(line.substr(prev_pos, pos - prev_pos));
        }
        if (!families.count(fam_name)) {
            families[fam_name] = children;
        } else {
            cerr << "We have already have this family name but it appeared again!" << endl;
            exit(-1);
        }
    }
}

void display(const map<string, vector<string>> &families, ostream &os) {
    auto it = families.begin(), it_end = families.end();
    while (it != it_end) {
        os << "Family " << it->first << ": ";
        if (it->second.empty()) {
            os << "no children" << endl;
        } else {
            auto vec_it = it->second.begin(), vec_it_end = it->second.end();
            while (vec_it != vec_it_end) {
                os << *vec_it << " ";
                vec_it++;
            }
            os << endl;
        }
        it++;
    }
}

void query(const string &fam_name, const map<string, vector<string>> &families) {
    auto it = families.find(fam_name);
    if (it == families.end()) {
        cout << "Do not have data of this family name" << endl;
        return;
    }
    if (it->second.empty()) {
        cout << "This family has no child" << endl;
    } else {
        cout << "This family has " << it->second.size() << " children, their name: ";
        auto vec_it = it->second.begin(), vec_it_end = it->second.end();
        while (vec_it != vec_it_end) {
            cout << *vec_it << " ";
            vec_it++;
        }
        cout << endl;
    }
}
