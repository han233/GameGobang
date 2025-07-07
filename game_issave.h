#ifndef GAME_ISSAVE_H
#define GAME_ISSAVE_H

#include <QDialog>

namespace Ui {
class Game_IsSave;
}

class Game_IsSave : public QDialog
{
    Q_OBJECT

public:
    explicit Game_IsSave(QWidget *parent = nullptr);
    ~Game_IsSave();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Game_IsSave *ui;
};

#endif // GAME_ISSAVE_H
