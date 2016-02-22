#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG

#include <iostream>
#include "sodium.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <fstream>

using namespace std;

void encrypt() {
    cout <<
    "Hallo. Ich akzeptiere JPEGs und PNG-Bilder. Bitte nur Farbbilder ohne Transparenz, ansonsten mache ich ein Farbbild ohne Transparenz daraus. Auch aus Grauwertbildern." <<
    endl;

    unsigned char *data = NULL;
    int x, y, n;

    while (data == NULL) {
        cout << endl <<
        "Bitte gib den vollstaendigen relativen oder absoluten Pfad zum Bild in dem du etwas verstecken willst an, inklusive Dateiname und -endung: ";
        string path;
        cin >> path;
        const char *p = path.c_str();
        data = stbi_load(p, &x, &y, &n, 3);
        if (n == 4) {
            cout << "Haelst dich wohl fuer einen richtigen Alpha-Kevin?" << endl;
        }
        else if (n == 2) {
            cout << "Grau. Und Alpha. Ob du behindert bist?" << endl;
        }
        else if (n == 1) {
            cout << "Nachts sind alle Moesen grau." << endl;
        }
    }

    size_t storage = ((x * y - 10) * 3) / 8;
    cout << "Du kannst ca. " << storage / 1000000.0 << " MB in diesem Bild verstecken." << endl;

    string filestr;
    ifstream file;
    bool fail = true;
    size_t size = 0;
    while (fail) {
        cout << endl << "Zu versteckende Datei eingeben: ";
        cin >> filestr;
        file.open(filestr, ios::binary);
        if (file.good())
            fail = false;
        size = 0;
        file.seekg(0, file.end);
        size = file.tellg();
        file.seekg(0, file.beg);
        if (size > storage) {
            cout << "Datei zu gross! (" << size / 1000000.0 << "MB)" << endl;
            fail = true;
            file.close();
        }
    }
    unsigned char *vptr = data;
    for (int i = 29; i >= 0; i--) {
        *vptr = (((*vptr) >> 1) << 1) | ((size >> i) & 1);
        vptr++;
    }

    char choice;
    cout << endl << "Passwortverschluesseln?(J/N): ";
    cin >> choice;

    if (choice == 'J' || choice == 'j') {
        cout << endl << "Bitte gib das Passwort zum verschluesseln ein: ";
        string password;
        cin >> password;
        const char *pwd = password.c_str();
        int count = 0;
        while (size > 0) {
            char c;
            file.get(c);
            c = c ^ pwd[count];
            count++;
            if (count == strlen(pwd))
                count = 0;
            for (int i = 7; i >= 0; i--) {
                *vptr = (((*vptr) >> 1) << 1) | ((c >> i) & 1);
                vptr++;
            }
            size--;
        }
        file.close();
    }

    else {
        cout << "Verschluesselung der Hurensohn!";
        while (size > 0) {
            char c;
            file.get(c);
            for (int i = 7; i >= 0; i--) {
                *vptr = (((*vptr) >> 1) << 1) | ((c >> i) & 1);
                vptr++;
            }
            size--;
        }
        file.close();
    }
    cout << endl << "Bild speichern unter...: ";
    string saveme;
    cin >> saveme;
    stbi_write_png(saveme.c_str(), x, y, 3, data, 3 * x);
    stbi_image_free(data);
}

void decrypt() {
    unsigned char *data = NULL;
    int x, y, n;

    while (data == NULL) {
        cout << endl <<
        "Bitte gib den vollstaendigen relativen oder absoluten Pfad zum Bild, das du entschluesseln willst, an, inklusive Dateiname und -endung: ";
        string path;
        cin >> path;
        const char *p = path.c_str();
        data = stbi_load(p, &x, &y, &n, 3);
    }
    size_t storage = ((x * y - 10) * 3) / 8;

    unsigned char *vptr = data;
    size_t size = 0;
    for (int i = 29; i >= 0; i--) {
        size = size | (*vptr & 1) << i;
        vptr++;
    }
    if(size > storage){
        cout << endl << "Hier stimmt etwas nicht. Das Bild versteckt entweder keine Daten oder wurde auf irgendeine Art zerstoert. Druecke irgendeine Taste zum Beenden...";
        cin.get();
        return;
    }

    cout << endl << "Entschluesselte Datei speichern unter...: ";
    string saveme;
    cin >> saveme;
    ofstream out(saveme, ios::binary);

    char choice;
    cout << endl << "Passwortverschluesselt?(J/N): ";
    cin >> choice;

    if (choice == 'J' || choice == 'j') {
        cout << endl << "Bitte gib das Passwort zum entschluesseln ein: ";
        string password;
        cin >> password;
        const char *pwd = password.c_str();
        int count = 0;
        while (size > 0) {
            char c = 0;
            for (int i = 7; i >= 0; i--) {
                c = c | (*vptr & 1) << i;
                vptr++;
            }
            size--;
            c = pwd[count] ^ c;
            count++;
            if(count == strlen(pwd))
                count = 0;
            out << c;
        }
    }

    else {
        cout << "Verschluesselung der Hurensohn!";
        while (size > 0) {
            char c = 0;
            for (int i = 7; i >= 0; i--) {
                c = c | (*vptr & 1) << i;
                vptr++;
            }
            size--;
            out << c;
        }
    }

}

int main() {
    if (sodium_init() == -1) {
        cout << "Error in Sodium. Terminating." << endl;
        return 1;
    }
    char choice;
    cout << "Moechtest du entschluesseln oder verschluesseln?(E/V): ";
    cin >> choice;
    if (choice == 'e' || choice == 'E')
        decrypt();
    else if (choice == 'v' || choice == 'V')
        encrypt();
    return 0;
}