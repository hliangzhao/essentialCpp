#include <iostream>
#include <string>
using namespace std;

int main() {
    string username;
    cin >> username;
    switch (username.size()) {
        case 0:
            cout << "No name entered" << endl;
            break;
        case 1:
            cout << "User with only 1-char length" << endl;
            break;
        default:
            cout << "Hello, " << username << endl;
    }

    return 0;
}
