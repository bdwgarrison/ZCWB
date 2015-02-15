#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateParticularWindow(QMenu *&inMenu);
private:
    Ui::MainWindow *ui;

protected:
    QMenuBar* menuBar;
    QMenu* macroOptions;
    QMenu* particularOptions;
};

#endif // MAINWINDOW_H
