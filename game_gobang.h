#ifndef GAME_GOBANG_H
#define GAME_GOBANG_H

#define Game_GobangSize 15

#include <QWidget>
#include <QMouseEvent>
#include <vector>

#include "game_piece.h"

namespace Ui {
class Game_Gobang;
}

class Game_Gobang : public QWidget
{
    Q_OBJECT

public:
    explicit Game_Gobang(QWidget *parent = nullptr);
    ~Game_Gobang();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    bool GameJudge(int currFlag,int currX,int currY);

private:
    Ui::Game_Gobang *ui;
    std::vector<std::vector<int>> gobang_flag;
    bool flag;
    std::vector<QString> PieceColor;
};

#endif // GAME_GOBANG_H
