#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <game_gobang.h>
#include <game_othello.h>
#include "game_savedgame.h"
#include <stdlib.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void ClearState(QPushButton * currbutton);

private slots:
    void on_Button_GameGobang_clicked();
    void on_Button_GameOthello_clicked();

    void on_Button_NewGame_clicked();

    void on_Button_Continue_clicked();

private:
    Ui::MainWindow*                 ui;
    QPushButton*                    currButton;
    std::shared_ptr<QWidget>        currWidget;
    std::shared_ptr<Game_Othello>   othello;
    std::shared_ptr<Game_Gobang>    gobang;
    std::shared_ptr<Game_SavedGame> savedGame;
    std::vector<QPushButton*>       allGameButton;
    std::map<QPushButton*,std::shared_ptr<QWidget>> buttonToWidget;
};
#endif // MAINWINDOW_H
