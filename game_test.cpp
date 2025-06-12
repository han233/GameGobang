#include "game_test.h"
#include <QRandomGenerator>
#include <iostream>

Game_Test::Game_Test(QObject *parent) : QObject(parent)
{

}

void Game_Test::Game_TestOthello()
{

    int x = QRandomGenerator::global()->bounded(14);
    int y = QRandomGenerator::global()->bounded(14);
    //for(int x = 0;x<14;x++)
    {
        //for(int y = 0;y<14;y++)
        {
            std::cout<<"emit"<<"("<<x<<","<<y<<")"<<std::endl;
            emit TestOthello(x,y);
        }
    }

}
