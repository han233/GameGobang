#ifndef GAME_OTHELLO_H
#define GAME_OTHELLO_H

#define Game_OthelloSize 14
#define Game_AllOthelloSize 14*14

#include <QWidget>
#include <QMouseEvent>
#include <vector>
#include <memory>

#include "game_piece.h"
#include "game_test.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

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
    void setPiece(PieceCorlor pieceColor,int pieceIndexX,int pieceIndexY);
    void ProceEveryDir(const int currX,const int currY,const PieceCorlor currFlag,std::vector<QPair<int,int>> &XY);
    void loadGame(boost::property_tree::ptree loadContext);

protected:
    void mousePressEvent(QMouseEvent *event) override;

    bool GameJudgeValid(PieceCorlor currColor,int currX,int currY,std::vector<QPair<int,int>> &XY);
    bool GameJudgeContinue(PieceCorlor nextColor);
    bool GameJudgeEnd();

protected slots:
    void TestOthelloSlot(int index_x,int index_y);

private slots:
    void on_GameSelfTest_clicked();

private:
    Ui::Game_Othello *ui;
    std::shared_ptr<Game_Test> test;                                        // 测试程序
    std::vector<std::vector<std::shared_ptr<Game_Piece>>> OthelloWidget;    // 棋盘上每个位置的棋子，右上角为(0,0)

    PieceCorlor CurrColor;              // 当前所下棋子颜色
    bool GameEnded = false;             // 游戏是否结束
    std::vector<int> EachPieceCount;    // 各色棋子的数量
};

#endif // GAME_OTHELLO_H
