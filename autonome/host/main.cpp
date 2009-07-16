#include <QApplication>
#include "AutonomeWidget.h"

int main(int argc,char *argv[]) {
    QApplication app(argc,argv);

    AutonomeWidget main;
    main.show();

    return app.exec();
}


