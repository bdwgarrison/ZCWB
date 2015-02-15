#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "woscrollread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ZeCoat WorkOrder Browser");
    zwr::WOScrollRead* prog = new zwr::WOScrollRead;

    menuBar = new QMenuBar( prog);
    QMenu* macroOptions = new QMenu("Options");
    QAction* conjoinAction = new QAction(tr("&Conjoin Pressure and Temperature Plots"), this);
    QAction* overlapAction = new QAction(tr("&Overlap Notes with Pressure Plot"), this);
    //optionsMenu->addMenu(new QMenu("option1"));
    macroOptions->addAction(conjoinAction);
    macroOptions->addAction(overlapAction);
    connect(conjoinAction,SIGNAL(triggered()),prog,SLOT(conjoinGraphs()));
    connect(overlapAction,SIGNAL(triggered()),prog,SLOT(overlapNotesWithGraphs()));
    menuBar->addMenu( macroOptions);

    particularOptions = new QMenu("Particular Options");
    menuBar->addMenu( particularOptions);

    ///add dynamic menu from scrollRead, connected and updating depending on selected tab
    connect(prog, SIGNAL(sendMenuItem(QMenu*&)), this, SLOT(updateParticularWindow(QMenu*&)));

    this->setCentralWidget(prog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateParticularWindow(QMenu*& inMenu)
{
    menuBar->removeAction( particularOptions->menuAction());
    particularOptions = inMenu;
    particularOptions->update();
    menuBar->addMenu( particularOptions);
    //menuBar->addMenu(particularOptions);
    menuBar->update();
}

