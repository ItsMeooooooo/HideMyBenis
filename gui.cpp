#include "benisgui.h"
#include <QApplication>
#include <stdexcept>
#include "sodium.h"

int main(int argc, char *argv[])
{
    if(sodium_init() == -1)
    {
        throw std::runtime_error("ERROR: libsodium could not be initialized!");
        return 1;
    }
    QApplication a(argc, argv);
    BenisGUI w;
    w.show();

    return a.exec();
}
