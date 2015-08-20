#include "bondinfo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BondInfo w;
    w.show();

    return a.exec();
}
