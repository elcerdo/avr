#include <QApplication>
#include "MainWindow.h"

int main(int argc,char *argv[]) {
    QApplication app(argc,argv);
    QApplication::setOrganizationName("elgatocorp");
    QApplication::setApplicationName("autonomeserial");

    MainWindow main;
    main.show();

    return app.exec();
}


