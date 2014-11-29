#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "drawwidget.h"
#include <QResizeEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void openAction();
    void removeSeamAction();

private:

    Ui::MainWindow *ui;
    DrawWidget *drawArea;

    QImage image;
};

#endif // MAINWINDOW_H
