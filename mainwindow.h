#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "drawwidget.h"
#include <QResizeEvent>
#include <QComboBox>
#include "seamcarving.h"
#include "energydistview.h"
#include "gradientenergy.h"

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

signals:

    void sendEnergyDest(QImage &i);
    void sendEnergy(QImage &i);
private slots:
    void openAction();
    void saveAsAction();
    void removeSeamAction();
    void resetMask();
    void selectKernel(int);

    void resizeVerticaly();
    void resizeHorizontaly();

private:
    SeamCarving* sc;
    EnergyDistView ed_view;
    EnergyDistView ed_gradient;
    QComboBox* kernelbox;


    Ui::MainWindow *ui;
    DrawWidget *drawArea;
    GradientEnergy* grad;

    QImage image;
};

#endif // MAINWINDOW_H
