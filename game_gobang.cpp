#include "game_gobang.h"
#include "ui_game_gobang.h"

#include "Common.h"

#include <iostream>

extern QString ColorName[];

Game_Gobang::Game_Gobang(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game_Gobang),
    PieceColor({"White","Black"})
{
    ui->setupUi(this);
    for(int i = 0;i < Game_GobangSize;i++)
    {
        std::vector<int> tem;
        for(int j = 0;j < Game_GobangSize;j++)
        {
            tem.push_back(0);
        }
        gobang_flag.push_back(tem);
    }

    flag = Game_ColorFlag;

}

Game_Gobang::~Game_Gobang()
{
    delete ui;
}

void Game_Gobang::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {        
        int x = cursor().pos().x() - this->pos().x();
        int y = cursor().pos().y() - this->pos().y() - HeaderWidth;

        int index_x = x / Game_PieceSize;
        int index_y = y / Game_PieceSize;
        int piece_x = index_x * Game_PieceSize;
        int piece_y = index_y * Game_PieceSize;
        if(gobang_flag[index_x][index_y] != 0)
        {
            std::cout<<"warn!"<<std::endl;
            return;
        }

        Game_Piece * newPiece = new Game_Piece();
        gobang_flag[index_x][index_y] = flag + 1;
        if(flag)
        {
            newPiece->setStyleSheet("background-image: url(E:/QtProjects/gobang/black.png);background-position:center;background-repeat:no-repeat;");
            flag = !flag;
        }
        else
        {
            newPiece->setStyleSheet("background-image: url(E:/QtProjects/gobang/white.png);background-position:center;background-repeat:no-repeat;");
            flag = !flag;
        }
        // 设置正确的父类
        newPiece->setParent(this);
        newPiece->move(piece_x,piece_y);
        newPiece->show();

        GameJudge(gobang_flag[index_x][index_y],index_x,index_y);
    }
}

bool Game_Gobang::GameJudge(int currFlag,int currX,int currY)
{
    //up
    int up = 0;
    for(int i = currY - 1;i > currY - Game_JudgeSize&&i > 0;i--)
    {
        if(gobang_flag[currX][i] == currFlag)
        {
            up++;
        }
        else
            break;
    }
    //down
    int down = 0;
    for(int i = currY + 1;i > currY + Game_JudgeSize&&i < Game_GobangSize;i++)
    {
        if(gobang_flag[currX][i] == currFlag)
        {
            down++;
        }
        else
            break;
    }
    if(up+down>=4)
    {
        std::cout<<PieceColor[currFlag-1].toStdString()<<"win"<<std::endl;
        return true;
    }

    //left
    int left = 0;
    for(int i = currX - 1;i > currX - Game_JudgeSize&&i > 0;i--)
    {
        if(gobang_flag[i][currY] == currFlag)
        {
            left++;
        }
        else
            break;
    }
    //right
    int right = 0;
    for(int i = currX + 1;i > currX + Game_JudgeSize&&i < Game_GobangSize;i++)
    {
        if(gobang_flag[i][currY] == currFlag)
        {
            right++;
        }
        else
            break;
    }
    if(left+right>=4)
    {
        std::cout<<PieceColor[currFlag-1].toStdString()<<"win"<<std::endl;
        return true;
    }

    //leftup
    int leftup = 0;
    for(int i = currX - 1,j = currY - 1;i > currX - Game_JudgeSize&&i > 0&&j > currY - Game_JudgeSize&&j > 0;i--,j--)
    {
        if(gobang_flag[i][j] == currFlag)
        {
            leftup++;
        }
        else
            break;
    }
    //rightdown
    int rightdown = 0;
    for(int i = currX + 1,j = currY +1;i > currX + Game_JudgeSize&&i < Game_GobangSize&&j > currX + Game_JudgeSize&&j < Game_GobangSize;i++,j++)
    {
        if(gobang_flag[i][j] == currFlag)
        {
            rightdown++;
        }
        else
            break;
    }
    if(leftup+rightdown>=4)
    {
        std::cout<<PieceColor[currFlag-1].toStdString()<<"win"<<std::endl;
        return true;
    }

    //rightup
    int rightup = 0;
    for(int i = currX + 1,j = currY - 1;i < currX + Game_JudgeSize&&i < Game_GobangSize&&j > currY - Game_JudgeSize&&j > 0;i++,j--)
    {
        if(gobang_flag[i][j] == currFlag)
        {
            rightup++;
        }
        else
            break;
    }
    //leftdown
    int leftdown = 0;
    for(int i = currX -1,j = currY + 1;i > currX - Game_JudgeSize&&i > 0&&j > currY + Game_JudgeSize&&j < Game_GobangSize;i--,j++)
    {
        if(gobang_flag[i][j] == currFlag)
        {
            leftdown++;
        }
        else
            break;
    }
    if(rightup+leftdown>=4)
    {
        std::cout<<PieceColor[currFlag-1].toStdString()<<"win"<<std::endl;
        return true;
    }
}
