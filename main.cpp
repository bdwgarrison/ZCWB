#include "mainwindow.h"
#include "woscrollread.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    /*
    zwr::WOScrollRead* prog = new zwr::WOScrollRead;

    QMenuBar *menu = new QMenuBar( prog);
    QMenu* optionsMenu = new QMenu("Options");
    optionsMenu->addMenu(new QMenu("option1"));
    menu->addMenu( optionsMenu);

    w.setCentralWidget(prog);
    */
    w.show();

    return a.exec();
}
