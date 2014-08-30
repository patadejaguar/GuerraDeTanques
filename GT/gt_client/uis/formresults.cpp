#include "formresults.h"
#include "ui_formresults.h"
#include "forms.h"

FormResults::FormResults(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormResults)
{
    _ui->setupUi(this);
}

FormResults::~FormResults()
{
    delete _ui;
}

void FormResults::show(int team_win, QList<PlayerInformation> players_information, QMap<PlayerData, QMap<PlayerData, KillsDeaths> > result, TerrainInfo terrain_info, QList<Actions> actions)
{
    _terrain_info = terrain_info;
    _actions = actions;
    _players_information = players_information;
    _ui->_pb_save_replay->show();

    _ui->_l_win->setPixmap(team_win == 1 ? QPixmap(":/stylesheet/green_win.png") : QPixmap(":/stylesheet/red_win.png"));
    _ui->_l_team_color->setPixmap(team_win == 1 ? QPixmap(":/slots/green_team.png") : QPixmap(":/slots/red_team.png"));

    for(int i = 1; i <= 6; ++i)
        findChild<QWidget*>(QString("_w_player_results_%1").arg(i))->hide();

    int index = 1;
    foreach(PlayerInformation player_information, players_information)
    {
        QWidget* widget_data = findChild<QWidget*>(QString("_w_player_results_%1").arg(index));
        widget_data->show();

        QLabel* l_nick = widget_data->findChild<QLabel*>(QString("_l_nick_%1").arg(index));
        l_nick->setStyleSheet(QString("color:%1").arg(player_information._team==1?COLOR_GREEN:COLOR_RED));
        l_nick->setText(player_information._nick);
        widget_data->findChild<QLabel*>(QString("_l_avatar_%1").arg(index))->setPixmap(player_information._avatar);
        widget_data->findChild<QLabel*>(QString("_l_rank_%1").arg(index))->setPixmap(QString(":/gt/ranks/rank_%1.png").arg(player_information._rank));

        int kills = 0;
        int deaths = 0;

        PlayerData pd;
        pd._nick = player_information._nick;
        QMap<PlayerData, KillsDeaths> data = result.value(pd);
        QList<PlayerData> keys = data.keys();
        int index_key = 1;

        for(int i = 1; i <= 3; ++i)
            widget_data->findChild<QWidget*>(QString("_w_other_player_%1").arg((index-1)*3+i))->hide();

        foreach(PlayerData key, keys)
        {
            int my_index = (index-1)*3+index_key;
            widget_data->findChild<QWidget*>(QString("_w_other_player_%1").arg(my_index))->show();
            widget_data->findChild<QLabel*>(QString("_l_other_nick_%1").arg(my_index))->setText(key._nick);
            widget_data->findChild<QLabel*>(QString("_l_other_avatar_%1").arg(my_index))->setPixmap(key._avatar);

            KillsDeaths kills_deaths = data.value(key);
            kills   += kills_deaths._kills;
            deaths  += kills_deaths._deaths;

            widget_data->findChild<QLabel*>(QString("_l_other_kills_%1").arg(my_index))->setText(QString("%1/%2").arg(kills_deaths._kills).arg(kills_deaths._deaths));

            index_key++;
        }

        widget_data->findChild<QLabel*>(QString("_l_kills_%1").arg(index))->setText(QString("%1/%2").arg(kills).arg(deaths));

        index++;
    }

    Form::show();
}


void FormResults::on__pb_chat_clicked()
{
    Forms::instance()->_form_chat->show();
    Forms::instance()->_form_game->hide();
    hide();
}

void FormResults::on__pb_save_replay_clicked()
{
    _ui->_pb_save_replay->hide();

    QFile file(QString("%1\\gt-%2.rpl").arg(DIR_REPLAYS.absolutePath()).arg(QDateTime::currentDateTime().toString("[yyyy-MM-dd][hh-mm-ss]")));
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);
    stream << _terrain_info << RandomNumberMannager::_shema << _players_information << true << _actions;
    file.close();
}
