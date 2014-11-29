#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "drawwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openAction();
    void removeSeamAction();

private:

    Ui::MainWindow *ui;
    DrawWidget *drawArea;

    QImage image;
};

#endif // MAINWINDOW_H
