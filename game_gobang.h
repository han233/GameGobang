#ifndef GAME_GOBANG_H
#define GAME_GOBANG_H

#include <QWidget>
#include <QMouseEvent>
#include <vector>
#include <memory>

#include "game_piece.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Ui {
class Game_Gobang;
}

class Game_Gobang : public QWidget
{
    Q_OBJECT

public:
    explicit Game_Gobang(QWidget *parent = nullptr);
    ~Game_Gobang();

    void init();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    bool GameJudgeEnd(PieceCorlor currCorlor,int currX,int currY);
    void loadGame(boost::property_tree::ptree loadContext);

private:
    Ui::Game_Gobang *ui;
    PieceCorlor CurrColor;
    std::string LoadName;
    std::vector<std::vector<std::shared_ptr<Game_Piece>>> GobangPieces;    // 棋盘上每个位置的棋子，右上角为(0,0)
};

#endif // GAME_GOBANG_H
