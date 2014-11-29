#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include "seamcarving.h"
#include <ctime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //drawArea= new DrawWidget(this);

    //this->setCentralWidget(drawArea);
    this->unifiedTitleAndToolBarOnMac();
    ui->menuBar->setNativeMenuBar(false);


    sc=0;

    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openAction()));
    connect(ui->actionSave_As,SIGNAL(triggered()), SLOT(saveAsAction()));
    connect(ui->actionRemove_Seam,SIGNAL(triggered()),SLOT(removeSeamAction()));
    connect(ui->actionShow_Energy_Distribution, SIGNAL(triggered()), &ed_view, SLOT(show()));
    connect(this, SIGNAL(sendEnergyDest(QImage&)), &ed_view, SLOT(receiveEnergyDist(QImage&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openAction(){
    QFileDialog fd(this, "Open Image", "", "Image(*.jpg *.bmp *.tif *.png)");
    fd.setFileMode(QFileDialog::ExistingFile);
    if(fd.exec()){
        image = QImage(fd.selectedFiles()[0]);
        sc = new SeamCarving(image);
        qDebug() << "Path: " << fd.selectedFiles()[0];
        this->resize(image.size());
        ui->ImageViewer->setPixmap(QPixmap::fromImage(image));
        //emit sendEnergyDest(sc.energyDist);
    }
}

void MainWindow::saveAsAction()
{
    sc->getImage().save(QFileDialog::getSaveFileName(this,"Save Image"));
}

void MainWindow::removeSeamAction()
{
    sc->removeSeamV();
    emit sendEnergyDest(sc->energyDist);
    ui->ImageViewer->setPixmap(QPixmap::fromImage(sc->getImage()));
}

void MainWindow::resizeEvent(QResizeEvent *event){
    if(sc!=0)
    {
        int deltaWidth = event->oldSize().width()- event->size().width();
        qDebug()<<"Delta: "<<deltaWidth;
        if (deltaWidth > 0){
            for (int i = 0; i < deltaWidth; i++){
                std::clock_t t = std::clock();
                sc->removeSeamV();
                t = std::clock() - t;
                qDebug()<<"TIME: "<<((float)t)/CLOCKS_PER_SEC;
            }
        }

        //image = sc->getImage();
        ui->ImageViewer->setPixmap(QPixmap::fromImage(sc->getImage()));

        emit sendEnergyDest(sc->energyDist);
    }
    event->accept();
}
