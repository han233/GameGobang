#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <memory>

#include "game_issave.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile qss("E:/QtProjects/gobang/Mainwindow.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();

    gobang = std::make_shared<Game_Gobang>();
    othello = std::make_shared<Game_Othello>();
    savedGame = std::make_shared<Game_SavedGame>(this);

    allGameButton.push_back(ui->Button_GameGobang);
    allGameButton.push_back(ui->Button_GameOthello);

    buttonToWidget[ui->Button_GameGobang] = std::static_pointer_cast<QWidget>(gobang);
    buttonToWidget[ui->Button_GameOthello] = std::static_pointer_cast<QWidget>(othello);
    currButton = nullptr;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ClearState(QPushButton *currbutton)
{
    for(auto iterator = buttonToWidget.begin();iterator != buttonToWidget.end();iterator++)
    {
        if(iterator->first != currbutton)
        {
            QString name = iterator->first->parent()->objectName();
            QString widgetStyle = "QWidget#" + name + "{border:2px;border-style:solid;border-color:transparent;}";
            dynamic_cast<QWidget*>(iterator->first->parent())->setStyleSheet(widgetStyle);
            iterator->first->setAttribute(Qt::WA_Hover, true);
            iterator->first->setAttribute(Qt::WA_UnderMouse, false);
        }
        else
        {
            currButton = currbutton;
            QString name = currbutton->parent()->objectName();
            QString widgetStyle = "QWidget#" + name + "{border:2px;border-style:solid;border-color:#FF0000;}";
            dynamic_cast<QWidget*>(currbutton->parent())->setStyleSheet(widgetStyle);
            currbutton->setAttribute(Qt::WA_Hover, false);
            currbutton->setAttribute(Qt::WA_UnderMouse, false);
        }
    }
    //this->repaint();
}


void MainWindow::on_Button_GameGobang_clicked()
{
    ClearState(ui->Button_GameGobang);
    currWidget = std::make_shared<Game_Gobang>();
}

void MainWindow::on_Button_GameOthello_clicked()
{
    ClearState(ui->Button_GameOthello);
    currWidget = std::make_shared<Game_Othello>();
}

void MainWindow::on_Button_NewGame_clicked()
{
    currWidget->show();
}

void MainWindow::on_Button_Continue_clicked()
{
    std::string fileName;
    std::string name = currButton->objectName().split("_")[1].toStdString();
    fileName = "E:/QtProjects/gobang/" + name + ".json";

    savedGame->setReadFileName(fileName);
    bool isLoad = savedGame->exec();
    boost::property_tree::ptree loadContext = savedGame->getLoadNameContext();
    if(isLoad)
    {
        currWidget->loadGame(loadContext);
        currWidget->show();
    }
}
