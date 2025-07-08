#include "game_othello.h"
#include "ui_game_othello.h"

#include <iostream>
#include <QPair>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QFile>
#include <QMessageBox>

#include "Common.h"

#include "game_issave.h"

Game_Othello::Game_Othello(QWidget *parent) :
    Game_Widget(parent),
    ui(new Ui::Game_Othello),
    test(std::make_shared<Game_Test>())
{
    ui->setupUi(this);
    init();
    connect(test.get(),&Game_Test::TestOthello,this,&Game_Othello::TestOthelloSlot);

    QFile qss("E:/QtProjects/gobang/GameOthello.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();
}

Game_Othello::~Game_Othello()
{
    delete ui;
}

void Game_Othello::init()
{
    for(int i = 0;i < Game_OthelloSize;i++)
    {
      std::vector<std::shared_ptr<Game_Piece>> tem;
      for(int j = 0;j < Game_OthelloSize;j++)
      {
          std::shared_ptr<Game_Piece> newPiece(new Game_Piece());
          newPiece->setFlag(PieceCorlor::blank);
          newPiece->setParent(this);
          tem.push_back(newPiece);
      }
      OthelloPieces.push_back(tem);
    }

    int mid = Game_OthelloSize / 2;
    setPiece(mid,mid,PieceCorlor::black);
    setPiece(mid-1,mid-1,PieceCorlor::black);
    setPiece(mid-1,mid,PieceCorlor::white);
    setPiece(mid,mid-1,PieceCorlor::white);

    CurrColor = PieceCorlor::black;
    // 初始化各色棋子数量
    for(size_t i = 0;i < ColorName.size() - 1 ;i++)
    {
      EachPieceCount.push_back(2);
    }

    if(CurrColor == PieceCorlor::black)
    {
        ui->labelBlackPic->setFrameShape(QFrame::Shape::Box);
        ui->labelWhitePic->setFrameShape(QFrame::NoFrame);
    }
    else
    {
        ui->labelWhitePic->setFrameShape(QFrame::Shape::Box);
        ui->labelBlackPic->setFrameShape(QFrame::NoFrame);
    }

    LoadName = "";
}

void Game_Othello::setPiece(int index_x,int index_y,PieceCorlor color)
{
    int posX = index_x * Game_PieceSize + 25;
    int posY = index_y * Game_PieceSize + 25;
    OthelloPieces[index_x][index_y]->setFlag(color);
    OthelloPieces[index_x][index_y]->setColor(ColorName[color]);
    EachPieceCount[CurrColor - 1]++;
    OthelloPieces[index_x][index_y]->move(posX,posY);
    OthelloPieces[index_x][index_y]->show();
}

void Game_Othello::mousePressEvent(QMouseEvent *event)
{
    if(!IsGameEnded && event->buttons() == Qt::LeftButton)
    {
        std::vector<QPair<int,int>> XY;

        // 相对位置-标题栏-棋子半径
        int x = cursor().pos().x() - this->pos().x() - 25;
        int y = cursor().pos().y() - this->pos().y() - HeaderWidth - 25;
        // 坐标位置/棋子直径
        int index_x = x / Game_PieceSize;
        int index_y = y / Game_PieceSize;

        // 点击已放置棋子的位置，进行提示
        if(OthelloPieces[index_x][index_y]->getFlag() != PieceCorlor::blank)
        {
            //std::cout<<QString("setted!").toStdString()<<std::endl;
            return;
        }
        // 判断棋子放置在当前空位是否可以产生翻转
        if(!GameJudgeValid(CurrColor,index_x,index_y,XY))
        {
            //std::cout<<QString("Invalid!").toStdString()<<std::endl;
            return;
        }

        // 放置棋子
        setPiece(index_x,index_y,CurrColor);

        // 翻转
        for(size_t index = 0;index < XY.size();index++)
        {
            OthelloPieces[XY[index].first][XY[index].second]->setColor(ColorName[CurrColor]);
            OthelloPieces[XY[index].first][XY[index].second]->setFlag(CurrColor);
            EachPieceCount[CurrColor-1]++;
            //std::cout<<"revert "<<ColorName[CurrColor].toStdString()<<"++"<<std::endl;
            EachPieceCount[(!static_cast<int>(CurrColor-1))]--;
            //std::cout<<"revert "<<ColorName[(!static_cast<int>(CurrColor-1))+1].toStdString()<<"--"<<std::endl;
        }

        PieceCorlor nextColor = static_cast<PieceCorlor>(!static_cast<int>(CurrColor-1) + 1);

        ui->labelBlackCount->setText(QString::number(EachPieceCount[static_cast<int>(PieceCorlor::black)-1]));
        ui->labelWhiteCount->setText(QString::number(EachPieceCount[static_cast<int>(PieceCorlor::white)-1]));

        IsGameEnded = GameJudgeEnd();
        if(!IsGameEnded && GameJudgeContinue(nextColor))
        {
            CurrColor = nextColor;
        }
        else if(!IsGameEnded && GameJudgeContinue(CurrColor))
        {
            std::cout<<ColorName[nextColor].toStdString()<<"is no way!"<<ColorName[CurrColor].toStdString()<<"to be continue."<<std::endl;
        }
        else if(!IsGameEnded)
        {
            IsGameEnded = true;
            std::cout<<"Both of "<<ColorName[nextColor].toStdString()<<" and "<<ColorName[CurrColor].toStdString()<<" are no way!"<<std::endl;
        }
        else
        {
            // IsGameEnded
        }

        if(CurrColor == PieceCorlor::black)
        {
            ui->labelBlackPic->setFrameShape(QFrame::Shape::Box);
            ui->labelWhitePic->setFrameShape(QFrame::NoFrame);
        }
        else
        {
            ui->labelWhitePic->setFrameShape(QFrame::Shape::Box);
            ui->labelBlackPic->setFrameShape(QFrame::NoFrame);
        }
    }
}

void Game_Othello::closeEvent(QCloseEvent *event)
{
    std::shared_ptr<Game_IsSave> isSaveDia = std::make_shared<Game_IsSave>();
    bool isSave = isSaveDia->exec();

    boost::posix_time::ptime t(boost::posix_time::second_clock::universal_time());
    std::string currTime = to_simple_string(t);

    boost::property_tree::ptree root;
    boost::property_tree::ptree piecesNode;
    boost::property_tree::ptree whitePiecesNode;
    boost::property_tree::ptree blackPiecesNode;

    if(isSave)
    {
        std::string filename = "E:/QtProjects/gobang/GameOthello.json";
        boost::property_tree::read_json(filename, root);

        for(size_t x = 0;x < OthelloPieces.size();x++)
        {
            for(size_t y = 0;y < OthelloPieces[x].size();y++)
            {
                PieceCorlor color = OthelloPieces[x][y]->getFlag();
                if(color != PieceCorlor::blank)
                {
                    boost::property_tree::ptree coordinate;
                    if(color == PieceCorlor::white)
                    {
                        std::string cor = std::to_string(x) + "," + std::to_string(y);
                        coordinate.put_value(cor);
                        //coordinate.put_value(std::make_pair(x,y));
                        whitePiecesNode.push_back(std::make_pair("",coordinate));
                    }
                    else
                    {
                        std::string cor = std::to_string(x) + "," + std::to_string(y);
                        coordinate.put_value(cor);
                        //coordinate.put_value(std::make_pair(x,y));
                        blackPiecesNode.push_back(std::make_pair("",coordinate));
                    }
                }
            }

        }

        piecesNode.add_child("white",whitePiecesNode);
        piecesNode.add_child("black",blackPiecesNode);

        if(LoadName != "")
            root.put_child(LoadName,piecesNode);
        else
            root.add_child(currTime, piecesNode);

        std::ofstream json_file("E:/QtProjects/gobang/GameOthello.json");
        if (json_file.is_open())
        {
            boost::property_tree::write_json(filename, root);
            json_file.close();
            std::cout << "JSON文件写入成功!" << std::endl;
         }
        else
        {
            std::cerr << "无法打开文件进行写入!" << std::endl;
        }
    }

     QWidget::closeEvent(event);
}

void Game_Othello::TestOthelloSlot(int index_x,int index_y)
{
    if(!IsGameEnded)
    {
        //qDebug()<<"\033[31mTest\033[0m";
        std::vector<QPair<int,int>> XY;

        int piece_x = index_x * Game_PieceSize + 25;
        int piece_y = index_y * Game_PieceSize + 25;

        // 点击已放置棋子的位置，进行提示
        if(OthelloPieces[index_x][index_y]->getFlag() != PieceCorlor::blank)
        {
            //std::cout<<QString("setted!").toStdString()<<std::endl;
            return;
        }
        // 判断棋子放置在当前空位是否可以产生翻转
        if(!GameJudgeValid(CurrColor,index_x,index_y,XY))
        {
            //std::cout<<QString("Invalid!").toStdString()<<std::endl;
            return;
        }
        QThread::msleep(100);
        disconnect(test.get(),&Game_Test::TestOthello,this,&Game_Othello::TestOthelloSlot);
        //std::cout<<"disconnect"<<std::endl;
        //std::cout<<"test set "<<ColorName[CurrColor].toStdString()<<" <"<<index_x<<" , "<<index_y<<">"<<std::endl;
        // 放置棋子
        OthelloPieces[index_x][index_y]->setColor(ColorName[CurrColor]);
        OthelloPieces[index_x][index_y]->setFlag(CurrColor);
        //std::cout<<"set   "<<ColorName[CurrColor].toStdString()<<"++"<<std::endl;
        EachPieceCount[CurrColor - 1]++;
        // 设置正确的父类
        OthelloPieces[index_x][index_y]->move(piece_x,piece_y);
        OthelloPieces[index_x][index_y]->show();

        // 翻转
        for(size_t index = 0;index < XY.size();index++)
        {
            OthelloPieces[XY[index].first][XY[index].second]->setColor(ColorName[CurrColor]);
            OthelloPieces[XY[index].first][XY[index].second]->setFlag(CurrColor);
            EachPieceCount[CurrColor - 1]++;
            //std::cout<<"revert "<<ColorName[CurrColor].toStdString()<<"++"<<std::endl;
            EachPieceCount[(!static_cast<int>(CurrColor-1))]--;
            //std::cout<<"revert "<<ColorName[(!static_cast<int>(CurrColor-1))+1].toStdString()<<"--"<<std::endl;
        }
        this->repaint();
        PieceCorlor nextColor = static_cast<PieceCorlor>(!static_cast<int>(CurrColor-1) + 1);
        //std::cout<<"currcolor="<<ColorName[CurrColor].toStdString()<<"  nextColor ="<<ColorName[nextColor].toStdString()<<std::endl;

        ui->labelBlackCount->setText(QString::number(EachPieceCount[static_cast<int>(PieceCorlor::black)-1]));
        ui->labelWhiteCount->setText(QString::number(EachPieceCount[static_cast<int>(PieceCorlor::white)-1]));

        IsGameEnded = GameJudgeEnd();
        if(!IsGameEnded && GameJudgeContinue(nextColor))
        {
            CurrColor = nextColor;
        }
        else if(!IsGameEnded && GameJudgeContinue(CurrColor))
        {
            std::cout<<ColorName[nextColor].toStdString()<<"is no way!"<<ColorName[CurrColor].toStdString()<<"to be continue."<<std::endl;
        }
        else if(!IsGameEnded)
        {
            IsGameEnded = true;
            std::cout<<"Both of "<<ColorName[nextColor].toStdString()<<" and "<<ColorName[CurrColor].toStdString()<<" are no way!"<<std::endl;
        }
        else
        {

        }
        connect(test.get(),&Game_Test::TestOthello,this,&Game_Othello::TestOthelloSlot);
        //std::cout<<"connect"<<std::endl;

        if(CurrColor == PieceCorlor::black)
        {
            ui->labelBlackPic->setFrameShape(QFrame::Shape::Box);
            ui->labelWhitePic->setFrameShape(QFrame::NoFrame);
        }
        else
        {
            ui->labelWhitePic->setFrameShape(QFrame::Shape::Box);
            ui->labelBlackPic->setFrameShape(QFrame::NoFrame);
        }
        on_GameSelfTest_clicked();
    }

}

bool Game_Othello::GameJudgeValid(PieceCorlor currColor,int currX,int currY,std::vector<QPair<int,int>> &XY)
{
    std::vector<QPair<int,int>> tmp;
    for(size_t index = 0;index < DirectionVector.size();index++)
    {
        tmp.clear();
        for(int i = currX + DirectionVector[index].first,j = currY + DirectionVector[index].second;
            i >= 0 && i < Game_OthelloSize && j >= 0 && j < Game_OthelloSize;
            i += DirectionVector[index].first,j += DirectionVector[index].second)
        {
            if(OthelloPieces[i][j] ->getFlag() != currColor &&
               OthelloPieces[i][j] ->getFlag() != PieceCorlor::blank)
            {
                // 添加可以被转换的棋子
                QPair<int,int> pair(i,j);
                tmp.push_back(pair);
            }
            else if(OthelloPieces[i][j]->getFlag() == PieceCorlor::blank)
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
        //std::cout<<"not end!"<<std::endl;
        return false;
    }
    else
    {
        int BlackCount = EachPieceCount[PieceCorlor::black -1];
        int WhiteCount = EachPieceCount[PieceCorlor::white -1];

        // 清空
        ui->labelWhitePic->setFrameShape(QFrame::NoFrame);
        ui->labelBlackPic->setFrameShape(QFrame::NoFrame);
        for(size_t indexX = 0;indexX < Game_OthelloSize; indexX++)
        {
            for(size_t indexY = 0;indexY < Game_OthelloSize; indexY++)
            {
                OthelloPieces[indexX][indexY]->setColor(ColorName[PieceCorlor::blank]);
                OthelloPieces[indexX][indexY]->setFlag(PieceCorlor::blank);
            }
        }

        //结算画面
        int mincount = WhiteCount < BlackCount ? WhiteCount : BlackCount;
        int maxcount = WhiteCount > BlackCount ? WhiteCount : BlackCount;
        PieceCorlor winCorlor = WhiteCount > BlackCount ? PieceCorlor::white : PieceCorlor::black;

        int winIndexX,winIndexY;
        for(size_t indexX = 0;indexX < Game_OthelloSize && (maxcount != 0 || mincount != 0); indexX++)
        {
            for(size_t indexY = 0;indexY < Game_OthelloSize && (maxcount != 0 || mincount != 0); indexY++)
            {
                QThread::msleep(100);
                winIndexX = WhiteCount < BlackCount ? indexX : Game_OthelloSize - indexX - 1;
                winIndexY = WhiteCount < BlackCount ? indexY : Game_OthelloSize - indexY - 1;

                if(maxcount)
                {
                    if(mincount)
                    {
                        OthelloPieces[indexX][indexY]->setColor(ColorName[PieceCorlor::black]);
                        OthelloPieces[indexX][indexY]->setFlag(PieceCorlor::black);
                        OthelloPieces[Game_OthelloSize - indexX - 1][Game_OthelloSize - indexY - 1]->setColor(ColorName[PieceCorlor::white]);
                        OthelloPieces[Game_OthelloSize - indexX - 1][Game_OthelloSize - indexY - 1]->setFlag(PieceCorlor::white);
                        mincount--;
                    }

                    OthelloPieces[winIndexX][winIndexY]->setColor(ColorName[winCorlor]);
                    OthelloPieces[winIndexX][winIndexY]->setFlag(winCorlor);
                    maxcount--;
                }
                this->repaint();
            }
        }
        QString winText = ColorName[winCorlor] + " WIN!";
        std::cout<<winText.toStdString()<<std::endl;

        QPixmap winPic;
        QString picPath = "E:/QtProjects/gobang/" + ColorName[winCorlor] + ".png";
        winPic.load(picPath);
        QMessageBox winBox(this);
        winBox.setWindowTitle("Notice");
        winBox.setText("WIN!");
        winBox.setStandardButtons(QMessageBox::Ok);
        winBox.setIconPixmap(winPic);
        winBox.exec();
        return true;
    }
}

bool Game_Othello::GameJudgeContinue(PieceCorlor nextColor)
{
    for(int i = 0;i < Game_OthelloSize;i++)
    {
      for(int j = 0;j < Game_OthelloSize;j++)
      {
          if(OthelloPieces[i][j]->getFlag() == PieceCorlor::blank)
          {
              std::vector<QPair<int,int>> XY;
              if(GameJudgeValid(nextColor,i,j,XY) != false)
              {
                  return true;
              }
          }
      }
    }
    return false;
}

void Game_Othello::on_GameSelfTest_clicked()
{
    while(!IsGameEnded)
        test->Game_TestOthello();
}

void Game_Othello::loadGame(boost::property_tree::ptree loadContext)
{
    IsLoading = true;
    for(auto i : loadContext)
    {
        std::cout<<i.first<<std::endl;
        if(i.first == "white")
        {
            boost::property_tree::ptree whitePieces = i.second;
            for(auto Piece : whitePieces)
            {
                boost::property_tree::ptree index = Piece.second;
                std::string cordinate = index.get<std::string>("");
                std::vector<std::string> corXY;
                splitCordinate(cordinate,',',corXY);
                setPiece(std::stoi(corXY[0]),std::stoi(corXY[1]),PieceCorlor::white);
            }
        }
        else
        {
            boost::property_tree::ptree blackPieces = i.second;
            for(auto Piece : blackPieces)
            {
                boost::property_tree::ptree index = Piece.second;
                std::string cordinate = index.get<std::string>("");
                std::vector<std::string> corXY;
                splitCordinate(cordinate,',',corXY);
                setPiece(std::stoi(corXY[0]),std::stoi(corXY[1]),PieceCorlor::black);
            }
        }
    }
    IsLoading = false;
}

void Game_Othello::setLoadName(std::string name)
{
    LoadName = name;
}
