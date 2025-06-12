#ifndef GAME_OTHELLO_H
#define GAME_OTHELLO_H

#define Game_OthelloSize 14

#include <QWidget>
#include <QMouseEvent>
#include <vector>

#include "game_piece.h"

namespace Ui {
class Game_Othello;
}

class Game_Othello : public QWidget
{
    Q_OBJECT

public:
    explicit Game_Othello(QWidget *parent = nullptr);
    ~Game_Othello();

    void init();
    void setPiece(PieceFlag pieceFlag,int pieceIndexX,int pieceIndexY);
    void ProceEveryDir(const int currX,const int currY,const PieceFlag currFlag,std::vector<QPair<int,int>> &XY);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    bool GameJudge(PieceFlag currFlag,int currX,int currY,std::vector<QPair<int,int>> &XY);

private:
    Ui::Game_Othello *ui;
    std::vector<std::vector<Game_Piece*>> OthelloWidget;
    PieceFlag CurrColor;
    std::vector<QString> PieceColor;
    std::vector<int> EachPieceCount;
};

#endif // GAME_OTHELLO_H
