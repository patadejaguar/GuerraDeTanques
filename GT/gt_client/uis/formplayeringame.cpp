#include "formplayeringame.h"
#include "ui_formplayeringame.h"

FormPlayerInGame::FormPlayerInGame(QString nick, QString game, QChar team, bool editable, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::FormPlayerInGame)
{
    _ui->setupUi(this);
    _ui->_pb_team->setVisible(editable);
    _ui->_pb_exit->setVisible(editable);
    _ui->_l_team->setVisible(!editable);

    _ui->_l_nick->setText(nick);
    _game = game;

    changeTeam(team);
}

FormPlayerInGame::~FormPlayerInGame()
{
    delete _ui;
}

QString FormPlayerInGame::getNick()
{
    return _ui->_l_nick->text();
}

void FormPlayerInGame::changeTeam(QChar team)
{
    if(team == 'G')
    {
        _ui->_pb_team->setIcon(QIcon(":/stylesheet/icon_green_flag.png"));
        _ui->_l_team->setPixmap(QPixmap(":/stylesheet/icon_green_flag.png"));
    }
    else
    {
        _ui->_pb_team->setIcon(QIcon(":/stylesheet/icon_red_flag.png"));
        _ui->_l_team->setPixmap(QPixmap(":/stylesheet/icon_red_flag.png"));
    }
}

void FormPlayerInGame::on__pb_exit_clicked()
{
//    ConnectionTcp::getInstance()->sendLeaveGame(ui->_l_nick->text(), _game);
}

void FormPlayerInGame::on__pb_team_clicked()
{
//    ConnectionTcp::getInstance()->sendChangeTeam(ui->_l_nick->text(), _game);
}
