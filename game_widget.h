#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QWidget>

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Ui {
class Game_Widget;
}

class Game_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Game_Widget(QWidget *parent = nullptr);
    ~Game_Widget();

    virtual void init() = 0;
    virtual void loadGame(boost::property_tree::ptree loadContext) = 0;
    void splitCordinate(std::string str,char split,std::vector<std::string> &cor);

private:
    Ui::Game_Widget *ui;
};

#endif // GAME_WIDGET_H
