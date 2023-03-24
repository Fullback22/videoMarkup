#include "videoMarkup.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    videoMarkup w;
    w.show();
    return a.exec();
}
