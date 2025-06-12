#ifndef GAME_OTHELLO_H
#define GAME_OTHELLO_H

#define Game_OthelloSize 14
#define Game_AllOthelloSize 14*14

#include <QWidget>
#include <QMouseEvent>
#include <vector>

#include "game_piece.h"
#include "game_test.h"

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

    bool GameJudgeValid(PieceFlag currFlag,int currX,int currY,std::vector<QPair<int,int>> &XY);
    bool GameJudgeContinue(PieceFlag nextflag);
    bool GameJudgeEnd();

protected slots:
    void TestOthelloSlot(int index_x,int index_y);

private slots:
    void on_GameSelfTest_clicked();

private:
    Ui::Game_Othello *ui;
    std::vector<std::vector<Game_Piece*>> OthelloWidget;
    PieceFlag CurrColor;
    std::vector<QString> PieceColor;
    std::vector<int> EachPieceCount;
    Game_Test *test;
    bool GameEnded = false;
};

#endif // GAME_OTHELLO_H
