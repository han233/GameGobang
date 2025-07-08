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
#include "game_widget.h"

namespace Ui {
class Game_Othello;
}

class Game_Othello : public Game_Widget
{
    Q_OBJECT

public:
    explicit Game_Othello(QWidget *parent = nullptr);
    ~Game_Othello();

    virtual void init() override;
    virtual void loadGame(boost::property_tree::ptree loadContext) override;

    void setPiece(int index_x,int index_y,PieceCorlor color);
    void setLoadName(std::string name);
    void ProceEveryDir(const int currX,const int currY,const PieceCorlor currFlag,std::vector<QPair<int,int>> &XY);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    void setPiece(int index_x,int index_y);
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
    std::vector<std::vector<std::shared_ptr<Game_Piece>>> OthelloPieces;    // 棋盘上每个位置的棋子，右上角为(0,0)

    PieceCorlor CurrColor;              // 当前所下棋子颜色
    std::vector<int> EachPieceCount;    // 各色棋子的数量
    std::string LoadName;               // 读档名字
    bool IsLoading = false;             // 是否正在读档
    bool IsGameEnded = false;           // 游戏是否结束
    std::string LogFileName = "GameOthello";
};

#endif // GAME_OTHELLO_H
