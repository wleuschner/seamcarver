#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include "seamcarving.h"
#include "gradientenergy.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    drawArea = new DrawWidget(this);

    this->setCentralWidget(drawArea);
    this->unifiedTitleAndToolBarOnMac();
    ui->menuBar->setNativeMenuBar(false);


    sc=0;

    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openAction()));
    connect(ui->actionSave_As,SIGNAL(triggered()), SLOT(saveAsAction()));
    connect(ui->actionRemove_Seam,SIGNAL(triggered()),SLOT(removeSeamAction()));
    connect(ui->actionShow_Energy_Distribution, SIGNAL(triggered()), &ed_view, SLOT(show()));
    connect(ui->actionShow_Gradients,SIGNAL(triggered()),&ed_gradient,SLOT(show()));
    connect(this, SIGNAL(sendEnergyDest(QImage&)), &ed_view, SLOT(receiveEnergyDist(QImage&)));
    connect(this, SIGNAL(sendEnergy(QImage&)),&ed_gradient,SLOT(receiveEnergyDist(QImage&)));
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
        drawArea->setBackgroundImage(image);
        grad = new GradientEnergy(image);
        sc = new SeamCarving(image, grad);
        sc->setMask(drawArea->getImage());
        qDebug() << "Path: " << fd.selectedFiles()[0];
        this->resize(image.size().width(),image.size().height()+30);
        drawArea->setBackgroundImage(sc->getImage());
        //ui->ImageViewer->setPixmap(QPixmap::fromImage(image));
        //emit sendEnergyDest(sc.energyDist);
    }
}

void MainWindow::saveAsAction()
{
    sc->getImage().save(QFileDialog::getSaveFileName(this,"Save Image"));
}

void MainWindow::removeSeamAction()
{
    sc->removeSeamH();
    emit sendEnergyDest(sc->energyDist);
    QImage eplot = grad->getEnergyPlot();
    emit sendEnergy(eplot);
    drawArea->resize(drawArea->size().width(),drawArea->size().height()-1);
    drawArea->setBackgroundImage(sc->getImage());
    drawArea->update();
    //ui->ImageViewer->setPixmap(QPixmap::fromImage(sc->getImage()));
}

void MainWindow::resizeEvent(QResizeEvent *event){
    if(sc!=0)
    {
        int deltaWidth = sc->width - event->size().width();
        int deltaHeight = sc->height - event->size().height();
        qDebug()<<"Delta: "<<deltaWidth;
        if (deltaWidth > 0){
            for (int i = 0; i < deltaWidth; i++){
                std::clock_t t = std::clock();
                sc->removeSeamV();
                t = std::clock() - t;
                //qDebug()<<"TIME: "<<((float)t)/CLOCKS_PER_SEC;
            }
        }

        if(deltaWidth < 0){
            /*
            //find seams delta times
            std::vector< std::vector<int> > remove_seams(abs(deltaWidth));
            for (int i = 0; i < abs(deltaWidth); i++){
                sc->findSeamV();
                remove_seams[i] = sc->getSeam();
            }
            //order seams
            std::vector< std::vector<int> > ordered_seams(image.height());
            for(int y = 0; y < image.height(); y++){
                std::vector<int> row(abs(deltaWidth));
                for (int i = 0; i < abs(deltaWidth); i++){
                    row[i] = remove_seams[i][y];
                }
                std::sort(row.begin(),row.end());
                ordered_seams[y] = row;
            }
            //insert seams
            image.copy(0,0,image.width()-deltaWidth,image.height());
            for (int i = 0; i < ordered_seams.size(); i++){
                sc->insertSeam(ordered_seams[i], i);
            }
            //energyfunction reset grey image
            grad->greyTones();
            */
        }

        if(deltaHeight>0)
        {
            for(int i = 0; i < deltaHeight; i++)
            {
                sc->removeSeamH();
            }
        }

        drawArea->setBackgroundImage(sc->getImage());
        drawArea->update();
        //ui->ImageViewer->setPixmap(QPixmap::fromImage(sc->getImage()));
        QImage temp = grad->getGX();
        emit sendEnergyDest(*(drawArea->getImage()));
        QImage eplot = grad->getEnergyPlot();
        emit sendEnergy(eplot);
    }
    event->accept();
}
