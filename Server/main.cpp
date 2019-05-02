#include "servwindow.h"
#include <QCoreApplication>
#include <iostream>
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    servWindow win;

    return app.exec();
}
