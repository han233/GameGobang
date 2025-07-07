#ifndef GAME_SAVEDGAME_H
#define GAME_SAVEDGAME_H

#include <QDialog>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Ui {
class Game_SavedGame;
}

class Game_SavedGame : public QDialog
{
    Q_OBJECT

public:
    explicit Game_SavedGame(QWidget *parent = nullptr);
    ~Game_SavedGame();

    void setReadFileName(const std::string name);
    boost::property_tree::ptree getLoadNameContext();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Game_SavedGame *ui;

    std::string readFileName;
    boost::property_tree::ptree loadGameContext;
};

#endif // GAME_SAVEDGAME_H
