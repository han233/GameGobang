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
