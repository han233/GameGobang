#ifndef GAME_GOBANG_H
#define GAME_GOBANG_H

#include <QWidget>
#include <QMouseEvent>
#include <vector>
#include <memory>

#include "game_piece.h"
#include "game_widget.h"

namespace Ui {
class Game_Gobang;
}

class Game_Gobang : public Game_Widget
{
    Q_OBJECT

public:
    explicit Game_Gobang(QWidget *parent = nullptr);
    ~Game_Gobang();

    virtual void init() override ;
    virtual void loadGame(boost::property_tree::ptree loadContext) override ;

    void setPiece(int index_x,int index_y,PieceCorlor color);
    void setLoadName(std::string name);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    bool GameJudgeEnd(PieceCorlor currCorlor,int currX,int currY);

private:
    Ui::Game_Gobang *ui;
    bool IsLoading;
    PieceCorlor CurrColor;
    std::string LoadName;
    std::vector<std::vector<std::shared_ptr<Game_Piece>>> GobangPieces;    // 棋盘上每个位置的棋子，右上角为(0,0)
};

#endif // GAME_GOBANG_H
