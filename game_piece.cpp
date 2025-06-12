#include "game_piece.h"
#include "ui_game_piece.h"


QString ColorName[3] = {"blank","white","black"};
std::vector<QPair<int,int>> DirectionVector = {QPair<int,int>(0,-1),QPair<int,int>(0,1),QPair<int,int>(-1,0),QPair<int,int>(1,0),
                                        QPair<int,int>(-1,-1),QPair<int,int>(1,1),QPair<int,int>(-1,1),QPair<int,int>(1,-1)};
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

void Game_Piece::setFlag(PieceFlag flag)
{
    pieceflag = flag;
}

PieceFlag Game_Piece::getFlag()
{
    return pieceflag;
}

void Game_Piece::setColor(QString color)
{
    QString style = "background-image: url(E:/QtProjects/gobang/" + color + ".png);background-position:center;background-repeat:no-repeat;";
    this->setStyleSheet(style);
}
