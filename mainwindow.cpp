#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gobang = new Game_Gobang();
    othello = new Game_Othello();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gobang;
    delete othello;
}


void MainWindow::on_Button_GameGobang_clicked()
{
    gobang->show();
}

void MainWindow::on_Button_GameOthello_clicked()
{
    othello->show();
}
