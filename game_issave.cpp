#include "game_issave.h"
#include "ui_game_issave.h"


Game_IsSave::Game_IsSave(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Game_IsSave)
{
    ui->setupUi(this);
}

Game_IsSave::~Game_IsSave()
{
    delete ui;
}

void Game_IsSave::on_buttonBox_accepted()
{

}
