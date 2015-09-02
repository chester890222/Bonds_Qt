#include "bondinfo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug() << Q_FUNC_INFO;
    QApplication a(argc, argv);
    BondInfo w;
    w.show();

    return a.exec();
}
