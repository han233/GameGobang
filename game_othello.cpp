#include "game_othello.h"
#include "ui_game_othello.h"

#include <iostream>
#include <QPair>
#include <QDebug>
#include <QTimer>
#include <QThread>

#include "Common.h"

void DelayFun()
{
}

extern QString ColorName[];
extern std::vector<QPair<int,int>> DirectionVector;

Game_Othello::Game_Othello(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game_Othello),
    PieceColor({"White","Black"}),
    test(new Game_Test)
{
    ui->setupUi(this);
    init();
    connect(test,&Game_Test::TestOthello,this,&Game_Othello::TestOthelloSlot);
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
    // 初始化各色棋子数量
    for(size_t i = 0;i < PieceColor.size();i++)
    {
      EachPieceCount.push_back(2);
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

        // 相对位置-标题栏-棋子半径
        int x = cursor().pos().x() - this->pos().x() - 25;
        int y = cursor().pos().y() - this->pos().y() - HeaderWidth - 25;
        // 坐标位置/棋子直径
        int index_x = x / Game_PieceSize;
        int index_y = y / Game_PieceSize;
        int piece_x = index_x * Game_PieceSize + 25;
        int piece_y = index_y * Game_PieceSize + 25;

        // 点击已放置棋子的位置，进行提示
        if(OthelloWidget[index_x][index_y]->getFlag() != PieceFlag::blank)
        {
            std::cout<<QString("setted!").toStdString()<<std::endl;
            return;
        }
        // 判断棋子放置在当前空位是否可以产生翻转
        if(!GameJudgeValid(CurrColor,index_x,index_y,XY))
        {
            std::cout<<QString("Invalid!").toStdString()<<std::endl;
            return;
        }

        // 放置棋子
        OthelloWidget[index_x][index_y]->setColor(ColorName[CurrColor]);
        OthelloWidget[index_x][index_y]->setFlag(CurrColor);
        std::cout<<"set   "<<ColorName[CurrColor].toStdString()<<"++"<<std::endl;
        EachPieceCount[CurrColor-1]++;
        // 设置正确的父类
        OthelloWidget[index_x][index_y]->move(piece_x,piece_y);
        OthelloWidget[index_x][index_y]->show();

        // 翻转
        for(size_t index = 0;index < XY.size();index++)
        {
            OthelloWidget[XY[index].first][XY[index].second]->setColor(ColorName[CurrColor]);
            OthelloWidget[XY[index].first][XY[index].second]->setFlag(CurrColor);
            EachPieceCount[CurrColor-1]++;
            std::cout<<"revert "<<ColorName[CurrColor].toStdString()<<"++"<<std::endl;
            EachPieceCount[(!static_cast<int>(CurrColor-1))]--;
            std::cout<<"revert "<<ColorName[(!static_cast<int>(CurrColor-1))+1].toStdString()<<"--"<<std::endl;
        }

        CurrColor = static_cast<PieceFlag>(!static_cast<int>(CurrColor-1) + 1);

        ui->labelBlackCount->setText(QString::number(EachPieceCount[static_cast<int>(PieceFlag::black)-1]));
        ui->labelWhiteCount->setText(QString::number(EachPieceCount[static_cast<int>(PieceFlag::white)-1]));

        GameJudgeEnd();
    }
}

void Game_Othello::TestOthelloSlot(int index_x,int index_y)
{
    //if(!GameEnded)
    {
        //qDebug()<<"\033[31mTest\033[0m";
        std::vector<QPair<int,int>> XY;

        int piece_x = index_x * Game_PieceSize + 25;
        int piece_y = index_y * Game_PieceSize + 25;

        // 点击已放置棋子的位置，进行提示
        if(OthelloWidget[index_x][index_y]->getFlag() != PieceFlag::blank)
        {
            std::cout<<QString("setted!").toStdString()<<std::endl;
            return;
        }
        // 判断棋子放置在当前空位是否可以产生翻转
        if(!GameJudgeValid(CurrColor,index_x,index_y,XY))
        {
            std::cout<<QString("Invalid!").toStdString()<<std::endl;
            return;
        }
        QThread::msleep(100);
        disconnect(test,&Game_Test::TestOthello,this,&Game_Othello::TestOthelloSlot);
        //std::cout<<"disconnect"<<std::endl;
        std::cout<<"test set "<<"<"<<index_x<<" , "<<index_y<<">"<<std::endl;
        // 放置棋子
        OthelloWidget[index_x][index_y]->setColor(ColorName[CurrColor]);
        OthelloWidget[index_x][index_y]->setFlag(CurrColor);
        std::cout<<"set   "<<ColorName[CurrColor].toStdString()<<"++"<<std::endl;
        EachPieceCount[CurrColor-1]++;
        // 设置正确的父类
        OthelloWidget[index_x][index_y]->move(piece_x,piece_y);
        OthelloWidget[index_x][index_y]->show();

        // 翻转
        for(size_t index = 0;index < XY.size();index++)
        {
            OthelloWidget[XY[index].first][XY[index].second]->setColor(ColorName[CurrColor]);
            OthelloWidget[XY[index].first][XY[index].second]->setFlag(CurrColor);
            EachPieceCount[CurrColor-1]++;
            std::cout<<"revert "<<ColorName[CurrColor].toStdString()<<"++"<<std::endl;
            EachPieceCount[(!static_cast<int>(CurrColor-1))]--;
            std::cout<<"revert "<<ColorName[(!static_cast<int>(CurrColor-1))+1].toStdString()<<"--"<<std::endl;
        }
        this->repaint();
        PieceFlag nextColor = static_cast<PieceFlag>(!static_cast<int>(CurrColor-1) + 1);
        std::cout<<"currcolor="<<ColorName[CurrColor].toStdString()<<"  nextColor ="<<ColorName[nextColor].toStdString()<<std::endl;

        ui->labelBlackCount->setText(QString::number(EachPieceCount[static_cast<int>(PieceFlag::black)-1]));
        ui->labelWhiteCount->setText(QString::number(EachPieceCount[static_cast<int>(PieceFlag::white)-1]));

        GameEnded = GameJudgeEnd();
        if(GameJudgeContinue(nextColor))
        {
            CurrColor = nextColor;
        }
        else if(GameJudgeContinue(CurrColor))
        {
            std::cout<<ColorName[nextColor].toStdString()<<"is no way!"<<ColorName[CurrColor].toStdString()<<"to be continue."<<std::endl;
        }
        else
        {
            GameEnded = true;
            std::cout<<"Both of "<<ColorName[nextColor].toStdString()<<" and "<<ColorName[CurrColor].toStdString()<<" are no way!"<<std::endl;
        }
        connect(test,&Game_Test::TestOthello,this,&Game_Othello::TestOthelloSlot);
        //std::cout<<"connect"<<std::endl;
        on_GameSelfTest_clicked();
        //QTimer::singleShot(2000,DelayFun());
    }

}

bool Game_Othello::GameJudgeValid(PieceFlag currFlag,int currX,int currY,std::vector<QPair<int,int>> &XY)
{
    std::vector<QPair<int,int>> tmp;
    for(size_t index = 0;index < DirectionVector.size();index++)
    {
        tmp.clear();
        for(int i = currX + DirectionVector[index].first,j = currY + DirectionVector[index].second;
            i >= 0 && i < Game_OthelloSize && j >= 0 && j < Game_OthelloSize;
            i += DirectionVector[index].first,j += DirectionVector[index].second)
        {
            if(OthelloWidget[i][j] ->getFlag() != currFlag &&
               OthelloWidget[i][j] ->getFlag() != PieceFlag::blank)
            {
                // 添加可以被转换的棋子
                QPair<int,int> pair(i,j);
                tmp.push_back(pair);
            }
            else if(OthelloWidget[i][j]->getFlag() == PieceFlag::blank)
            {
                // 当遇到空白切还没遇到当前颜色的棋子时，break
                tmp.clear();
                break;
            }
            else
            {
                // 有可以被转换的棋子且遇到了对应当前颜色的棋子时，添加到XY中，break
                for(size_t index = 0;index < tmp.size();index++)
                {
                    XY.push_back(tmp[index]);
                }
                break;
            }
        }
    }

    return XY.size();
}

bool Game_Othello::GameJudgeEnd()
{
    size_t sum = 0;
    for(size_t index = 0;index < EachPieceCount.size(); index++)
    {
        sum += EachPieceCount[index];
    }

    if(sum < Game_AllOthelloSize)
    {
        std::cout<<"not end!"<<std::endl;
        return false;
    }
    else
    {

        std::cout<<"sum= "<<sum<<"  OthelloWidget.size()= "<<OthelloWidget.size()<<std::endl;
        std::cout<<"end!"<<std::endl;
        int BlackCount = EachPieceCount[PieceFlag::black -1];
        int WhiteCount = EachPieceCount[PieceFlag::white -1];
        for(size_t indexX = 0;indexX < Game_OthelloSize; indexX++)
        {
            for(size_t indexY = 0;indexY < Game_OthelloSize; indexY++)
            {
                OthelloWidget[indexX][indexY]->setColor(ColorName[PieceFlag::blank]);
                OthelloWidget[indexX][indexY]->setFlag(PieceFlag::blank);
            }
        }


        for(size_t indexX = 0, countBlack = BlackCount;indexX < Game_OthelloSize; indexX++)
        {
            for(size_t indexY = 0;indexY < Game_OthelloSize; indexY++)
            {
                if(countBlack)
                {
                    OthelloWidget[indexX][indexY]->setColor(ColorName[PieceFlag::black]);
                    OthelloWidget[indexX][indexY]->setFlag(PieceFlag::black);
                    countBlack--;
                }
                else
                {
                    OthelloWidget[indexX][indexY]->setColor(ColorName[PieceFlag::white]);
                    OthelloWidget[indexX][indexY]->setFlag(PieceFlag::white);
                }
            }
        }
        int win = BlackCount > WhiteCount ? 2 : 1;
        std::cout<<ColorName[win].toStdString()<<" WIN!"<<std::endl;
        return true;
    }
}

bool Game_Othello::GameJudgeContinue(PieceFlag nextflag)
{
    for(int i = 0;i < Game_OthelloSize;i++)
    {
      for(int j = 0;j < Game_OthelloSize;j++)
      {
          if(OthelloWidget[i][j]->getFlag() == PieceFlag::blank)
          {
              std::cout<<"GameJudgeContinue judge<"<<i<<","<<j<<">"<<std::endl;
              std::vector<QPair<int,int>> XY;
              if(GameJudgeValid(nextflag,i,j,XY) != false)
              {
                  std::cout<<"GameJudgeContinue"<<"  continue"<<std::endl;
                  return true;
              }
          }
      }
    }
    std::cout<<"GameJudgeContinue"<<"  no way"<<std::endl;
    return false;
}

void Game_Othello::on_GameSelfTest_clicked()
{
    std::cout<<"Clicked"<<std::endl;
    while(!GameEnded)
        test->Game_TestOthello();
}
