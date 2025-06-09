#ifndef GAME_PIECE_H
#define GAME_PIECE_H

#include <QWidget>

namespace Ui {
class Game_Piece;
}

class Game_Piece : public QWidget
{
    Q_OBJECT

public:
    explicit Game_Piece(QWidget *parent = nullptr);
    ~Game_Piece();

private:
    Ui::Game_Piece *ui;
};

#endif // GAME_PIECE_H
