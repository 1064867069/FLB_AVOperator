#include "flb_avoperator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FLB_AVOperator w;
    w.show();
    return a.exec();
}
