#include "game_othello.h"
#include "ui_game_othello.h"

#include <iostream>
#include <QPair>
#include <QDebug>

#include "Common.h"

extern QString ColorName[];
extern std::vector<QPair<int,int>> DirectionVector;

Game_Othello::Game_Othello(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game_Othello),
    PieceColor({"White","Black"})
{
    ui->setupUi(this);
    init();
}

Game_Othello::~Game_Othello()
{
    delete ui;
}

void Game_Othello::init()
{
    for(int i = 0;i < Game_OthelloSize;i++)
    {
      std::vector<Game_Piece*> tem;
      for(int j = 0;j < Game_OthelloSize;j++)
      {
          Game_Piece * newPiece = new Game_Piece();
          newPiece->setFlag(PieceFlag::blank);
          newPiece->setParent(this);
          tem.push_back(newPiece);
      }
      OthelloWidget.push_back(tem);
    }

    int mid = Game_OthelloSize / 2;
    setPiece(PieceFlag::black,mid,mid);
    setPiece(PieceFlag::black,mid-1,mid-1);
    setPiece(PieceFlag::white,mid-1,mid);
    setPiece(PieceFlag::white,mid,mid-1);

    CurrColor = PieceFlag::black;
    for(size_t i = 0;i < PieceColor.size();i++)
    {
      EachPieceCount.push_back(0);
    }
}

void Game_Othello::setPiece(PieceFlag pieceFlag,int pieceIndexX,int pieceIndexY)
{
    int posX = pieceIndexX * Game_PieceSize + 25;
    int posY = pieceIndexY * Game_PieceSize + 25;
    OthelloWidget[pieceIndexX][pieceIndexY]->setFlag(pieceFlag);
    OthelloWidget[pieceIndexX][pieceIndexY]->setColor(PieceColor[pieceFlag - 1]);
    OthelloWidget[pieceIndexX][pieceIndexY]->move(posX,posY);
    OthelloWidget[pieceIndexX][pieceIndexY]->show();
}

void Game_Othello::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        qDebug()<<"\033[31mPressed\033[0m";
        std::vector<QPair<int,int>> XY;

        int x = cursor().pos().x() - this->pos().x() - 25;
        int y = cursor().pos().y() - this->pos().y() - HeaderWidth - 25;
        std::cout<<"("<<x<<","<<y<<std::endl;

        int index_x = x / Game_PieceSize;
        int index_y = y / Game_PieceSize;
        int piece_x = index_x * Game_PieceSize + 25;
        int piece_y = index_y * Game_PieceSize + 25;
        if(OthelloWidget[index_x][index_y]->getFlag() != PieceFlag::blank)
        {
            std::cout<<QString("setted!").toStdString()<<std::endl;
            return;
        }
        if(!GameJudge(CurrColor,index_x,index_y,XY))
        {
            std::cout<<QString("Invalid!").toStdString()<<std::endl;
            return;
        }

        OthelloWidget[index_x][index_y]->setFlag(CurrColor);
        for(size_t index = 0;index < XY.size();index++)
        {
            OthelloWidget[XY[index].first][XY[index].second]->setColor(ColorName[CurrColor - 1]);
            OthelloWidget[XY[index].first][XY[index].second]->setFlag(CurrColor);
        }
        if(CurrColor == PieceFlag::black)
        {
            OthelloWidget[index_x][index_y]->setStyleSheet("background-image: url(E:/QtProjects/gobang/black.png);background-position:center;background-repeat:no-repeat;");
            EachPieceCount[CurrColor-1]++;
            CurrColor = PieceFlag::white;

        }
        else
        {
            OthelloWidget[index_x][index_y]->setStyleSheet("background-image: url(E:/QtProjects/gobang/white.png);background-position:center;background-repeat:no-repeat;");
            EachPieceCount[CurrColor-1]++;
            CurrColor = PieceFlag::black;
        }
        // 设置正确的父类
        OthelloWidget[index_x][index_y]->move(piece_x,piece_y);
        OthelloWidget[index_x][index_y]->show();
    }
}

bool Game_Othello::GameJudge(PieceFlag currFlag,int currX,int currY,std::vector<QPair<int,int>> &XY)
{
    //up
    /*
    std::vector<QPair<int,int>> tmp;
    for(int i = currY - 1;i >= 0;i--)
    {
        if(OthelloWidget[currX][i]->getFlag() != currFlag &&
           OthelloWidget[currX][i]->getFlag() != PieceFlag::blank)
        {
            std::cout<<"up add("<<currX<<","<<i<<")"<<std::endl;
            QPair<int,int> pair(currX,i);
            tmp.push_back(pair);
        }
        else if(OthelloWidget[currX][i]->getFlag() == PieceFlag::blank)
        {
            std::cout<<"up blank break("<<currX<<","<<i<<")"<<std::endl;
            tmp.clear();
            break;
        }
        else
        {
            for(size_t index = 0;index < tmp.size();index++)
            {
                std::cout<<"up final add<"<<tmp[index].first<<","<<tmp[index].second<<">"<<std::endl;
                XY.push_back(tmp[index]);
            }
            break;
        }
    }

    //down
    tmp.clear();
    for(int i = currY + 1;i < Game_OthelloSize;i++)
    {
        if(OthelloWidget[currX][i]->getFlag() != currFlag &&
           OthelloWidget[currX][i]->getFlag() != PieceFlag::blank)
        {
            std::cout<<"down add("<<currX<<","<<i<<")"<<std::endl;
            QPair<int,int> pair(currX,i);
            tmp.push_back(pair);
        }
        else if(OthelloWidget[currX][i]->getFlag() == PieceFlag::blank)
        {
            std::cout<<"down blank break("<<currX<<","<<i<<")"<<std::endl;
            tmp.clear();
            break;
        }
        else
        {
            for(size_t index = 0;index < tmp.size();index++)
            {
                std::cout<<"down final add<"<<tmp[index].first<<","<<tmp[index].second<<">"<<std::endl;
                XY.push_back(tmp[index]);
            }
            break;
        }
    }

    //left
    tmp.clear();
    for(int i = currX - 1;i >= 0;i--)
    {
        if(OthelloWidget[i][currY]->getFlag() != currFlag &&
           OthelloWidget[i][currY]->getFlag() != PieceFlag::blank)
        {
            std::cout<<"left add("<<currX<<","<<i<<")"<<std::endl;
            QPair<int,int> pair(i,currY);
            tmp.push_back(pair);
        }
        else if(OthelloWidget[i][currY]->getFlag() == PieceFlag::blank)
        {
            std::cout<<"left break("<<currX<<","<<i<<")"<<std::endl;
            tmp.clear();
            break;
        }
        else
        {
            for(size_t index = 0;index < tmp.size();index++)
            {
                std::cout<<"left final add<"<<tmp[index].first<<","<<tmp[index].second<<">"<<std::endl;
                XY.push_back(tmp[index]);
            }
            break;
        }
    }

    //right
    tmp.clear();
    for(int i = currX + 1;i < Game_OthelloSize;i++)
    {
        if(OthelloWidget[i][currY]->getFlag() != currFlag &&
           OthelloWidget[i][currY]->getFlag() != PieceFlag::blank)
        {
            std::cout<<"right add("<<currX<<","<<i<<")"<<std::endl;
            QPair<int,int> pair(i,currY);
            tmp.push_back(pair);
        }
        else if(OthelloWidget[i][currY]->getFlag() == PieceFlag::blank)
        {
            std::cout<<"right break("<<currX<<","<<i<<")"<<std::endl;
            tmp.clear();
            break;
        }
        else
        {
            for(size_t index = 0;index < tmp.size();index++)
            {
                std::cout<<"right final add<"<<tmp[index].first<<","<<tmp[index].second<<">"<<std::endl;
                XY.push_back(tmp[index]);
            }
            break;
        }
    }

    //leftup
    tmp.clear();
    for(int i = currX - 1,j = currY - 1;i >= 0 && j >= 0;i--,j--)
    {
        if(OthelloWidget[i][j] ->getFlag() != currFlag &&
           OthelloWidget[i][j]->getFlag() != PieceFlag::blank)
        {
            std::cout<<"leftup add("<<currX<<","<<i<<")"<<std::endl;
            QPair<int,int> pair(i,j);
            tmp.push_back(pair);
        }
        else if(OthelloWidget[i][j]->getFlag() == PieceFlag::blank)
        {
            std::cout<<"leftup break("<<currX<<","<<i<<")"<<std::endl;
            tmp.clear();
            break;
        }
        else
        {
            for(size_t index = 0;index < tmp.size();index++)
            {
                std::cout<<"leftup final add<"<<tmp[index].first<<","<<tmp[index].second<<">"<<std::endl;
                XY.push_back(tmp[index]);
            }
            break;
        }
    }

    //rightdown
    tmp.clear();
    for(int i = currX + 1,j = currY +1;i < Game_OthelloSize && j < Game_OthelloSize;i++,j++)
    {
        if(OthelloWidget[i][j] ->getFlag() != currFlag &&
           OthelloWidget[i][j] ->getFlag() != PieceFlag::blank)
        {
            std::cout<<"rightdown add("<<currX<<","<<i<<")"<<std::endl;
            QPair<int,int> pair(i,j);
            tmp.push_back(pair);
        }
        else if(OthelloWidget[i][j]->getFlag() == PieceFlag::blank)
        {
            std::cout<<"rightdown break("<<currX<<","<<i<<")"<<std::endl;
            tmp.clear();
            break;
        }
        else
        {
            for(size_t index = 0;index < tmp.size();index++)
            {
                std::cout<<"rightdown final add<"<<tmp[index].first<<","<<tmp[index].second<<">"<<std::endl;
                XY.push_back(tmp[index]);
            }
            break;
        }
    }

    //rightup
    tmp.clear();
    for(int i = currX + 1,j = currY - 1;i < Game_OthelloSize && j >= 0;i++,j--)
    {
        if(OthelloWidget[i][j] ->getFlag() != currFlag &&
           OthelloWidget[i][j] ->getFlag() != PieceFlag::blank)
        {
            std::cout<<"rightup add("<<currX<<","<<i<<")"<<std::endl;
            QPair<int,int> pair(i,j);
            tmp.push_back(pair);
        }
        else if(OthelloWidget[i][j]->getFlag() == PieceFlag::blank)
        {
            std::cout<<"rightup break("<<currX<<","<<i<<")"<<std::endl;
            tmp.clear();
            break;
        }
        else
        {
            for(size_t index = 0;index < tmp.size();index++)
            {
                std::cout<<"rightup final add<"<<tmp[index].first<<","<<tmp[index].second<<">"<<std::endl;
                XY.push_back(tmp[index]);
            }
            break;
        }
    }
    //leftdown
    tmp.clear();
    for(int i = currX -1,j = currY + 1;i >= 0 && j < Game_OthelloSize;i--,j++)
    {
        if(OthelloWidget[i][j] ->getFlag() != currFlag &&
           OthelloWidget[i][j] ->getFlag() != PieceFlag::blank)
        {
            std::cout<<"leftdown add("<<currX<<","<<i<<")"<<std::endl;
            QPair<int,int> pair(i,j);
            tmp.push_back(pair);
        }
        else if(OthelloWidget[i][j]->getFlag() == PieceFlag::blank)
        {
            std::cout<<"leftdown break("<<currX<<","<<i<<")"<<std::endl;
            tmp.clear();
            break;
        }
        else
        {
            for(size_t index = 0;index < tmp.size();index++)
            {
                std::cout<<"leftdown final add<"<<tmp[index].first<<","<<tmp[index].second<<">"<<std::endl;
                XY.push_back(tmp[index]);
            }
            break;
        }
    }
    */

    ProceEveryDir(currX,currY,currFlag,XY);
    std::cout<<XY.size()<<std::endl;
    return XY.size();
}

void Game_Othello::ProceEveryDir(const int currX,const int currY,const PieceFlag currFlag,std::vector<QPair<int,int>> &XY)
{
    std::vector<QPair<int,int>> tmp;
    for(size_t index = 0;index < DirectionVector.size();index++)
    {
        tmp.clear();
        for(int i = currX + DirectionVector[index].first,j = currY + DirectionVector[index].second;
            i >= 0 && i < Game_OthelloSize && i >= 0 && j < Game_OthelloSize;
            i += DirectionVector[index].first,j += DirectionVector[index].second)
        {
            if(OthelloWidget[i][j] ->getFlag() != currFlag &&
               OthelloWidget[i][j] ->getFlag() != PieceFlag::blank)
            {
                std::cout<<"leftdown add("<<currX<<","<<i<<")"<<std::endl;
                QPair<int,int> pair(i,j);
                tmp.push_back(pair);
            }
            else if(OthelloWidget[i][j]->getFlag() == PieceFlag::blank)
            {
                std::cout<<"leftdown break("<<currX<<","<<i<<")"<<std::endl;
                tmp.clear();
                break;
            }
            else
            {
                for(size_t index = 0;index < tmp.size();index++)
                {
                    std::cout<<"leftdown final add<"<<tmp[index].first<<","<<tmp[index].second<<">"<<std::endl;
                    XY.push_back(tmp[index]);
                }
                break;
            }
        }
    }
}
