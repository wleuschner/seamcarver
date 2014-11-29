#include "energydistview.h"
#include "ui_energydistview.h"


EnergyDistView::EnergyDistView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnergyDistView)
{
    ui->setupUi(this);
}

EnergyDistView::~EnergyDistView()
{
    delete ui;
}

void EnergyDistView::receiveEnergyDist(QImage &i){
    ui->ImageViewer->setPixmap(QPixmap::fromImage(i));
}
