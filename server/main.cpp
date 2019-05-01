#include "servwindow.h"
#include <QCoreApplication>
#include <iostream>
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    std::cout << "test" << std::endl;
    servWindow win;
   // win.show();

    return app.exec();
}
