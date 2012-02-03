#include <QtGui/QApplication>
#include "qtcurrconv.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qtCurrconv w;
    w.show();

    return a.exec();
}
