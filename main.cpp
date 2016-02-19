#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main() {
    ifstream file("123.txt");
    ostringstream str;
    ostringstream stro;
    char key = 'C';
    char c;
    while (file.get(c)) {
        cout << c;
        str << (char)(c^key);
        stro << (char)(key^(c^key));
    }
    cout << endl << str.str() << endl;
    cout << stro.str() << endl;
    return 0;
}