#include "game_gobang.h"
#include "ui_game_gobang.h"
#include "game_issave.h"

#include "Common.h"
#include <iostream>
#include <QMessageBox>
#include <QSettings>
#include <fstream>
#include <iostream>

#include <chrono>
#include <ctime>

namespace boost
{
    namespace property_tree
    {
        template<typename Ch, typename Traits>
        std::basic_ostream<Ch, Traits>& operator<<(std::basic_ostream<Ch, Traits> &s, const std::pair<size_t, size_t> &e)
        {
            s << e.first;
            s.put('|');
            s << e.second;
            return s;
        }

        template<typename Ch, typename Traits>
        std::basic_istream<Ch, Traits>& operator>>(std::basic_istream<Ch, Traits> &s, std::pair<size_t, size_t> &e)
        {
            s >> e.first;
            char unused;
            s >> unused;
            s >> e.second;
            return s;
        }
    }
}


#define Game_GobangSize 15

Game_Gobang::Game_Gobang(QWidget *parent) :
    Game_Widget(parent),
    ui(new Ui::Game_Gobang)
{
    ui->setupUi(this);
    init();

    QFile qss("E:/QtProjects/gobang/GameGobang.qss");
    qss.open(QFile::ReadOnly);
    this->setStyleSheet(qss.readAll());
    qss.close();
}

Game_Gobang::~Game_Gobang()
{
    delete ui;
}

void Game_Gobang::init()
{
    for(int i = 0;i < Game_GobangSize;i++)
    {
      std::vector<std::shared_ptr<Game_Piece>> tem;
      for(int j = 0;j < Game_GobangSize;j++)
      {
          std::shared_ptr<Game_Piece> newPiece(new Game_Piece());
          newPiece->setFlag(PieceCorlor::blank);
          newPiece->setParent(this);
          tem.push_back(newPiece);
      }
      GobangPieces.push_back(tem);
    }

    CurrColor = PieceCorlor::black;
    // 历史记录文件
    std::string fileName = "GobangLog.json";
    std::ifstream f(fileName) ;
    if(!f.good())
        std::ofstream outFile(fileName);

    LoadName = "";
}

void Game_Gobang::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {        
        int x = cursor().pos().x() - this->pos().x();
        int y = cursor().pos().y() - this->pos().y() - HeaderWidth;


        int index_x = x / Game_PieceSize;
        int index_y = y / Game_PieceSize;

        if(index_x < 0||index_y < 0)
        {
            std::cout<<"cursor  "<<cursor().pos().x()<<","<<cursor().pos().y()<<"  this  "<<this->pos().x()<<","<<this->pos().y()<<std::endl;
        }
        else
        {
            setPiece(index_x,index_y,CurrColor);
        }
    }
}

void Game_Gobang::closeEvent(QCloseEvent *event)
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
        std::string filename = "E:/QtProjects/gobang/GameGobang.json";
        boost::property_tree::read_json(filename, root);

        for(size_t x = 0;x < GobangPieces.size();x++)
        {
            for(size_t y = 0;y < GobangPieces[x].size();y++)
            {
                PieceCorlor color = GobangPieces[x][y]->getFlag();
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

        std::ofstream json_file("E:/QtProjects/gobang/GameGobang.json");
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

    /*int matrix[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

    boost::property_tree::ptree matrix_node;
    for (int i = 0; i < 3; i++)
    {
        boost::property_tree::ptree row;
        for (int j = 0; j < 3; j++)
        {
            // Create an unnamed value
            boost::property_tree::ptree cell;
            cell.put_value(matrix[i][j]);
            // Add the value to our row
            row.push_back(std::make_pair("", cell));
        }
        // Add the row to our matrix
        matrix_node.push_back(std::make_pair("", row));
    }
    // Add the node to the root
    root.add_child("matrix", matrix_node);
    int x = 0;
    for (boost::property_tree::ptree::value_type &row : root.get_child("matrix"))
    {
        int y = 0;
        for (boost::property_tree::ptree::value_type &cell : row.second)
        {
            matrix[x][y] = cell.second.get_value<int>();
            y++;
        }
        x++;
    }*/
}

bool Game_Gobang::GameJudgeEnd(PieceCorlor currCorlor,int currX,int currY)
{
    int direction = DirectionVector.size() / 2;
    for(int dir = 0;dir < direction;dir++)
    {
        int dir1 = 0,dir2 = 0;
        for(int i = currX + DirectionVector[dir*2].first,j = currY + DirectionVector[dir*2].second;
            i >= 0 && i < Game_GobangSize && j >= 0 && j < Game_GobangSize;
            i += DirectionVector[dir*2].first,j += DirectionVector[dir*2].second)
        {
            if(GobangPieces[i][j]->getFlag() == currCorlor)
            {
                dir1++;
            }
            else
                break;
        }

        for(int i = currX + DirectionVector[dir*2+1].first,j = currY + DirectionVector[dir*2+1].second;
            i >= 0 && i < Game_GobangSize && j >= 0 && j < Game_GobangSize;
            i += DirectionVector[dir*2+1].first,j += DirectionVector[dir*2+1].second)
        {
            if(GobangPieces[i][j]->getFlag() == currCorlor)
            {
                dir2++;
            }
            else
                break;
        }

        if(dir1 + dir2>=4)
        {
            std::cout<<ColorName[currCorlor].toStdString()<<"win"<<std::endl;

            QPixmap winPic;
            QString picPath = "E:/QtProjects/gobang/" + ColorName[currCorlor] + ".png";
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
    return false;
}
void Game_Gobang::setPiece(int index_x,int index_y,PieceCorlor color)
{
    int piece_x = index_x * Game_PieceSize;
    int piece_y = index_y * Game_PieceSize;

    if(GobangPieces[index_x][index_y]->getFlag() != PieceCorlor::blank)
    {
        std::cout<<"warn!"<<std::endl;
        return;
    }

    // 放置棋子
    GobangPieces[index_x][index_y]->setColor(ColorName[color]);
    GobangPieces[index_x][index_y]->setFlag(color);
    GobangPieces[index_x][index_y]->move(piece_x,piece_y);
    GobangPieces[index_x][index_y]->show();

    if(!IsLoading)
    {
        // 判断是否结束
        GameJudgeEnd(GobangPieces[index_x][index_y]->getFlag(),index_x,index_y);
        // 更新当前颜色
        CurrColor = static_cast<PieceCorlor>(!static_cast<int>(CurrColor-1) + 1);
    }
}

void Game_Gobang::loadGame(boost::property_tree::ptree loadContext)
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

void Game_Gobang::setLoadName(std::string name)
{
    LoadName = name;
}
