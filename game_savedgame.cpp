#include "game_savedgame.h"
#include "ui_game_savedgame.h"

#include <QAction>
#include <iostream>

Game_SavedGame::Game_SavedGame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Game_SavedGame)
{
    ui->setupUi(this);
}

Game_SavedGame::~Game_SavedGame()
{
    delete ui;
}

void Game_SavedGame::setReadFileName(const std::string name)
{
    readFileName = name;
    boost::property_tree::ptree root;
    boost::property_tree::read_json(name, root);
    ui->Game_SavedList->clear();
    for(auto i : root)
    {
        //QAction* action = new QAction(QString::fromStdString(i.first));
        //ui->Game_SavedList->addAction(action);
        //ui->Game_SavedList->update();

        QListWidgetItem * item = new QListWidgetItem(QString::fromStdString(i.first));
        ui->Game_SavedList->addItem(item);
    }
}

void Game_SavedGame::on_buttonBox_accepted()
{
    QListWidgetItem * item = ui->Game_SavedList->currentItem();
    if(item!=nullptr)
    {
        QString gameSavedName = item->text();

        boost::property_tree::ptree root;
        boost::property_tree::read_json(readFileName, root);

        for(auto i : root)
        {
            if(i.first == gameSavedName.toStdString())
            {
                loadGameContext = i.second;
                break;
            }
        }
    }
    else
    {
        //请选择读档内容
    }
}

boost::property_tree::ptree Game_SavedGame::getLoadNameContext()
{
    return loadGameContext;
}
