#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
//    QButtonGroup *buttonGroup=new QButtonGroup(this);
//    buttonGroup->addButton(ui->checkBox);
//    buttonGroup->addButton(ui->checkBox_2);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
   if(arg1==Qt::Checked){
       ui->label->DDA=1;
   }
   if(arg1==Qt::Unchecked){
       ui->label->DDA=0;
   }
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1==Qt::Checked){
        ui->label->Bresen=1;
    }
    if(arg1==Qt::Unchecked){
        ui->label->Bresen=0;
    }
}
