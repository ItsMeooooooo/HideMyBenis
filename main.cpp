#include <iostream>
#include <string.h>
#include "benis_crypt.h"

using namespace std;

void check_encrypt(int err_code) {
    switch(err_code){
        case 0:
            cout << "Success!" << endl;
            break;
        case -1:
            cout << "Error loading image!" << endl;
            break;
        case -2:
            cout << "Error loading file!" << endl;
            break;
        case -3:
            cout << "File too large!" << endl;
            break;
        case -4:
            cout << "Out of memory!" << endl;
            break;
        default:
            cout << "An unknown error occurred." << endl;
    }
    cout << "Press any key to continue...";
    cin.clear();
    fflush(stdin);
    cin.get();
}

void check_decrypt(int err_code) {
    switch(err_code){
        case 0:
            cout << "Success!" << endl;
            break;
        case -1:
            cout << "Error loading image!" << endl;
            break;
        case -2:
            cout << "Error saving file!" << endl;
            break;
        case -3:
            cout << "Authentication failed. Wrong password, data damaged or no data hidden." << endl;
            break;
        case -4:
            cout << "Out of memory!" << endl;
            break;
        default:
            cout << "An unknown error occurred." << endl;
    }
    cout << "Press any key to continue...";
    cin.clear();
    fflush(stdin);
    cin.get();
}

void manual_encrypt() {
    cin.clear();
    fflush(stdin);
    cout << "Path to image to be used as container: ";
    string picname;
    cin >> picname;
    cout << "Output PNG filename: ";
    string outname;
    cin >> outname;
    cout << "File to be hidden: ";
    string filename;
    cin >> filename;
    cout << "0: No password" << endl << "1: Password" << endl << "Anything else: no password" << endl << "Please select: ";
    string password;
    cin.clear();
    fflush(stdin);
    switch(cin.get()){
        case '0':
            password = "";
            break;
        case '1':
            cout  << "Enter password: ";
            cin.clear();
            fflush(stdin);
            cin >> password;
            break;
        default:
            password = "";
            break;
    }
    check_encrypt(HideMyBenis::encrypt(picname.c_str(), outname.c_str(), filename.c_str(), password.c_str()));
}

void manual_decrypt() {
    cin.clear();
    fflush(stdin);
    cout << "Path to image to extract from: ";
    string picname;
    cin >> picname;
    cout << "Output filename: ";
    string filename;
    cin >> filename;
    cout << "0: No password" << endl << "1: Password" << endl << "Anything else: no password" << endl << "Please select: ";
    string password;
    cin.clear();
    fflush(stdin);
    switch(cin.get()){
        case '0':
            password = "";
            break;
        case '1':
            cout  << "Enter password: ";
            cin.clear();
            fflush(stdin);
            cin >> password;
            break;
        default:
            password = "";
            break;
    }
    check_decrypt(HideMyBenis::decrypt(picname.c_str(), filename.c_str(), password.c_str()));
}

void manual_routine() {
    cout << "0: Encrypt" << endl << "1: Decrypt" << endl << "Anything else: quit" << endl << "Please select: ";
    cin.clear();
    fflush(stdin);
    switch (cin.get()) {
        case '0':
            manual_encrypt();
            break;
        case '1':
            manual_decrypt();
            break;
        default:
            return;
    }
}

void display_help() {
    cout << "Welcome to HideMyBenis. This is the Help." << endl << endl;
    cout << "In order to see this help, start the program with '-h' or '-help'." << endl;

    cout << "In order to encrypt, start the program with: 'a' 'b' 'c' 'd'." << endl;
    cout << "a: The path to the image where you want to hide data" << endl;
    cout << "b: The path where you want to save the output PNG" << endl;
    cout << "c: The path to the file you want to hide" << endl;
    cout << "d: The password for encryption. Use \"\" for no password" << endl << endl;

    cout << "In order to decrypt, start the program with: 'a' 'b' 'c'." << endl;
    cout << "a: The path to the image from which you want to extract data" << endl;
    cout << "b: The path where you want to save the output extracted data" << endl;
    cout << "c: The password for decryption. Use \"\" for no password" << endl << endl;
    cout << "Start with no arguments or a wrong number of arguments if you want to enter the values manually." << endl;
    cout << "Press any key to continue...";
    cin.clear();
    fflush(stdin);
    cin.get();
}

int main(int argc, char *argv[]) {
    if (sodium_init() == -1) {
        cout << "Error in Sodium. Terminating." << endl;
        return 1;
    }
    if (argc == 1) {
        manual_routine();
    }
    else if (strcmp(argv[1], "-h") || strcmp(argv[1], "-help")) {
        display_help();
    }
    else if (argc == 4) {
        return(HideMyBenis::decrypt(argv[1], argv[2], argv[3]));
    }
    else if (argc == 5) {
        return(HideMyBenis::encrypt(argv[1], argv[2], argv[3], argv[4]));
    }
    else {
        manual_routine();
    }
    return 0;
}