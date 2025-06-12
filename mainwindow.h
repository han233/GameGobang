#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <game_gobang.h>
#include <game_othello.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Button_GameGobang_clicked();

    void on_Button_GameOthello_clicked();

private:
    Ui::MainWindow *ui;
    Game_Gobang* gobang;
    Game_Othello* othello;
};
#endif // MAINWINDOW_H
