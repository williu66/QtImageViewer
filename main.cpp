#include "qtimagedemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtImageDemo w;
    w.show();
    return a.exec();
}
