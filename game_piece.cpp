#include "game_piece.h"
#include "ui_game_piece.h"

Game_Piece::Game_Piece(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game_Piece)
{
    ui->setupUi(this);
}

Game_Piece::~Game_Piece()
{
    delete ui;
}
