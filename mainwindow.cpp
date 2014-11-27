#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    drawArea= new DrawWidget(this);
    this->setCentralWidget(drawArea);

    //this->unifiedTitleAndToolBarOnMac();
    //ui->menuBar->setNativeMenuBar(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
