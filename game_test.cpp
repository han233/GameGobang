#include "game_test.h"
#include <QRandomGenerator>
#include <iostream>
#include <stdlib.h>
#include <QMetaType>


Game_Test::Game_Test(QObject *parent) : QObject(parent)
{
    //qRegisterMetaType<std::shared_ptr<Game_Test>>("std::shared_ptr<Game_Test>");


}

void Game_Test::Game_TestOthello()
{
    int x = QRandomGenerator::global()->bounded(14);
    int y = QRandomGenerator::global()->bounded(14);

    emit TestOthello(x,y);
}
