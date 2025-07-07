#ifndef GAME_PIECE_H
#define GAME_PIECE_H

#include <QWidget>

enum PieceCorlor{
    blank,
    white,
    black
};
enum direction{
    up,
    down,
    left,
    right,
    leftup,
    rightdown,
    leftdown,
    rightup
};



namespace Ui {
class Game_Piece;
}

class Game_Piece : public QWidget
{
    Q_OBJECT

public:
    explicit Game_Piece(QWidget *parent = nullptr);
    ~Game_Piece();

    void setFlag(PieceCorlor flag);
    PieceCorlor getFlag();

    void setColor(QString color);

private:
    Ui::Game_Piece *ui;
    PieceCorlor pieceflag = PieceCorlor::blank;
};

#endif // GAME_PIECE_H
