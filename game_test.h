#ifndef GAME_TEST_H
#define GAME_TEST_H

#include <QObject>

class Game_Test : public QObject
{
    Q_OBJECT
public:
    explicit Game_Test(QObject *parent = nullptr);

    void Game_TestOthello();

signals:
    void TestOthello(int index_x,int index_y);
};

#endif // GAME_TEST_H
