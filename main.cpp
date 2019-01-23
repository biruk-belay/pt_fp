#include "pt_fp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pt_fp w;
    w.show();

    return a.exec();
}
