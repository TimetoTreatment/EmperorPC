#include "MainForm.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainForm mainForm;

    mainForm.show();

    return app.exec();
}
