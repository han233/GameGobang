#include "game_widget.h"
#include "ui_game_widget.h"

Game_Widget::Game_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game_Widget)
{
    ui->setupUi(this);
}

Game_Widget::~Game_Widget()
{
    delete ui;
}

void Game_Widget::splitCordinate(std::string str,char split,std::vector<std::string> &cor)
{
    std::stringstream ss;
    ss<<str;
    std::string tmp;
    while(getline(ss,tmp,split))
        cor.push_back(tmp);
}

