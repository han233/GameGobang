#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gobang = new Game_Gobang();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gobang;
}


void MainWindow::on_Button_NewGame_clicked()
{
    gobang->show();
}
