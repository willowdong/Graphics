#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionRect,&QAction::triggered,ui->label,[=](){
        ui->label->drawRect=1;
        ui->label->getPoint=0;
        ui->label->drawPoint=0;
    });
    connect(ui->actionPoly,&QAction::triggered,ui->label,[=](){
        ui->label->drawRect=1;
        ui->label->getPoint=1;
        ui->label->drawPoint=0;
    });
    connect(ui->actionPolyD,&QAction::triggered,ui->label,[=](){
        ui->label->drawRect=1;
        ui->label->getPoint=0;
        ui->label->drawPoint=1;
        repaint();
    });
    connect(ui->actionCurve,&QAction::triggered,ui->label,[=](){
        ui->label->drawCurve=1;
        ui->label->curve();
        repaint();
    });
    connect(ui->actionNew,&QAction::triggered,ui->label,[=](){
        ui->label->fresh();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

