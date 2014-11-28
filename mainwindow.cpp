#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include "seamcarving.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //drawArea= new DrawWidget(this);

    //this->setCentralWidget(drawArea);
    this->unifiedTitleAndToolBarOnMac();
    ui->menuBar->setNativeMenuBar(false);
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openAction()));
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
        qDebug() << "Path: " << fd.selectedFiles()[0];
        ui->ImageViewer->setPixmap(QPixmap::fromImage(image));
        SeamCarving sc(image);
        ui->ImageViewer->setPixmap(QPixmap::fromImage(sc.getGX()));
    }
}
