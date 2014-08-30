#include "formcreatedgame.h"
#include "ui_formcreatedgame.h"
#include "forms.h"

FormCreatedGame::FormCreatedGame(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormCreatedGame)
{
    _ui->setupUi(this);

    createSlots();

    _server_game = TcpGameServer::instance();

    _timer_for_request_latency = new QTimer();
    connect(_timer_for_request_latency, &QTimer::timeout, _server_game, &TcpGameServer::requestLatency);

    _client_game = TcpGameConnectionClient::instance();
    connect(_client_game, &TcpGameConnectionClient::notifyJoinedSuccessful, this, &FormCreatedGame::playerJoinedSuccessful);
    connect(_client_game, &TcpGameConnectionClient::notifyNewPlayerJoined, this, &FormCreatedGame::newPlayerJoinedGame);
    connect(_client_game, &TcpGameConnectionClient::notifyPlayerLeaveGame, this, &FormCreatedGame::playerLeaveGame);
    connect(_client_game, &TcpGameConnectionClient::notifyPlayersLatency, this, &FormCreatedGame::playersLatency);
    connect(_client_game, &TcpGameConnectionClient::notifyMessage, this, &FormCreatedGame::showMessage);
    connect(_client_game, &TcpGameConnectionClient::notifyChangeTeam, this, &FormCreatedGame::changeTeam);
    connect(_client_game, &TcpGameConnectionClient::notifyChangeTank, this, &FormCreatedGame::changeTank);
    connect(_client_game, &TcpGameConnectionClient::notifyPlayerReady, this, &FormCreatedGame::changePlayerReady);
    connect(_client_game, &TcpGameConnectionClient::notifyPrepareGame, this, &FormCreatedGame::prepareGame);

    installEventFilter(this);
}

FormCreatedGame::~FormCreatedGame()
{
    delete _ui;
    delete _timer_for_request_latency;
}

void FormCreatedGame::show()
{
    _ui->_pb_cancel->setEnabled(true);
    disconnect(_client_game, &TcpGameConnectionClient::notifyDisconnected, this, &FormCreatedGame::clientGameDisconnected);
    connect(_client_game, &TcpGameConnectionClient::notifyDisconnected, this, &FormCreatedGame::clientGameDisconnected);
    disconnect(_client_game, &TcpGameConnectionClient::notifyPlayerLeaveGame, this, &FormCreatedGame::playerLeaveGame);
    connect(_client_game, &TcpGameConnectionClient::notifyPlayerLeaveGame, this, &FormCreatedGame::playerLeaveGame);
    Form::show();
}

void FormCreatedGame::hide()
{
    disconnect(_client_game, &TcpGameConnectionClient::notifyDisconnected, this, &FormCreatedGame::clientGameDisconnected);
    disconnect(_client_game, &TcpGameConnectionClient::notifyPlayerLeaveGame, this, &FormCreatedGame::playerLeaveGame);
    Form::hide();
}

bool FormCreatedGame::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::Timer)
        _ui->_le_message->setFocus();
    else if(event->type() == QEvent::KeyPress && !_ui->_le_message->hasFocus())
    {
        _ui->_le_message->setText(_ui->_le_message->text()+((QKeyEvent*)event)->text());
        _ui->_le_message->setFocus();
    }
    return Form::eventFilter(object, event);
}

void FormCreatedGame::playerJoinedSuccessful(int index_of_terrain, QList<PlayerData> players_data, QList<int> random_numbers)
{
    RandomNumberMannager::setShema(random_numbers);

    Forms* forms = Forms::instance();

    forms->_form_join_game->hide();
    forms->_form_create_game->hide();

//    _ui->_pb_i_ready->setEnabled(false);
    _ui->_pb_i_ready->setChecked(false);
    _ui->_pb_begin->setEnabled(false);
    _ui->_pb_select_tank->setEnabled(true);

    _ui->_pb_begin->setVisible(TcpGameConnectionClient::instance()->isCreator());

    if(TcpGameConnectionClient::instance()->isCreator())
        _timer_for_request_latency->start(TCP_REQUEST_LATENCY_TIME);

    _players_data = players_data;

    _ui->_le_message->clear();
    _ui->_te_chat->clear();

    _index_of_terrain = index_of_terrain;
    TerrainInfo terrain_info = ResourceManager::instance()->terrainsInfo().at(_index_of_terrain);

    _green_counter = terrain_info._green_count;
    _red_counter = terrain_info._red_count;
    _counter_of_player = _green_counter + _red_counter;
    _presentation = terrain_info._image;

    _ui->_l_terrain_name->setText(terrain_info._name);
    _ui->_l_counter_green->setText(QString::number(terrain_info._green_count));
    _ui->_l_counter_red->setText(QString::number(terrain_info._red_count));
    _ui->_l_terrain->setPixmap(terrain_info._image);

    updateSlots();
    ConnectionTcp::instance()->sendJoinGame(_players_data.at(0)._nick);

    _ui->_le_message->setFocus();
    show();
}

void FormCreatedGame::newPlayerJoinedGame(PlayerData player_data)
{
    _players_data.append(player_data);
    updateSlots();
}

void FormCreatedGame::playerLeaveGame(PlayerData player_data)
{
    if(player_data == _players_data.at(0))
    {
        _players_data.clear();

        Forms* forms = Forms::instance();

        forms->_form_created_game->hide();
        forms->_form_chat->show();
    }
    else
    {
        _players_data.removeOne(player_data);
        updateSlots();
    }
}

void FormCreatedGame::playersLatency(QMap<int, int> players_latency)
{
    foreach(int index, players_latency.keys())
        _slots[index].setLatency(players_latency.value(index));
}

void FormCreatedGame::showMessage(QString message)
{
    _ui->_te_chat->append(message);
}

void FormCreatedGame::clientGameDisconnected()
{
    if(_client_game->isCreator())
        ConnectionTcp::instance()->sendCloseGame();
    else
        ConnectionTcp::instance()->sendLeaveGame();

    Forms* forms = Forms::instance();

    forms->_form_init_game->hide();
    forms->_form_chat->show();
    forms->_form_tank_selector->hide();
    hide();
}

void FormCreatedGame::closeGame(QString creator, QPixmap avatar, int rank)
{
    Q_UNUSED(avatar);
    Q_UNUSED(rank);

    if(creator == _slots.at(0)._l_nick->text())
    {
        Forms* forms = Forms::instance();

        forms->_form_created_game->hide();
        forms->_form_chat->show();
    }
}

void FormCreatedGame::changeTeam(int index, int team)
{
    _players_data[index]._team = team;
    _slots[index].setTeam(team);
}

void FormCreatedGame::changeTank(int index, int index_of_tank)
{
    _players_data[index]._index_of_tank = index_of_tank;
    _slots[index].setTank(index_of_tank);
    if(TcpGameConnectionClient::instance()->index() == index)
        _ui->_pb_i_ready->setEnabled(true);
}

void FormCreatedGame::changePlayerReady(int index, bool value)
{
    if(TcpGameConnectionClient::instance()->index() == index)
    {
        _ui->_pb_cancel->setEnabled(!value);
        _ui->_pb_select_tank->setEnabled(!value);
    }

    _players_data[index].setReady(value);
    _slots[index].setReady(value);

    if(TcpGameConnectionClient::instance()->isCreator())
    {
        _ui->_pb_begin->setEnabled(isReadyToStart());
        for(int i = 0; i < _players_data.size(); ++i)
            _slots[i]._pb_close->setEnabled(!_players_data[0]._ready);
    }

}

void FormCreatedGame::prepareGame()
{
    hide();
    Forms* forms = Forms::instance();
    dynamic_cast<FormGame*>(forms->_form_game)->show(_players_data, _index_of_terrain, _presentation);
}

void FormCreatedGame::cancel()
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;

    QPushButton *button_sender = (QPushButton*)sender_object;
    int index = button_sender->objectName().split("_pb_close_slot_").at(1).toInt();
    if( index == 1)
    {
        _timer_for_request_latency->stop();
        _server_game->close();
    }
    else
    {
        _client_game->sendPlayeLeaveGame(_players_data[index-1]._nick);
    }
}

void FormCreatedGame::on__le_message_returnPressed()
{
    if(_ui->_le_message->text() != "")
    {
        _client_game->sendMessage(_ui->_le_message->text());
        _ui->_le_message->clear();
        _ui->_le_message->setFocus();
    }
}

void FormCreatedGame::on__pb_cancel_clicked()
{
    if(TcpGameConnectionClient::instance()->isCreator())
    {
        _timer_for_request_latency->stop();
        _server_game->close();
    }
    else
    {
        _client_game->stop();
    }
}

void FormCreatedGame::createSlots()
{
    Slot slot_1;
    slot_1._l_avatar = _ui->_l_avatar_1;
    slot_1._l_rank = _ui->_l_rank_1;
    slot_1._l_nick = _ui->_l_nick_1;
    slot_1._l_tank = _ui->_l_tank_1;
    slot_1._l_team = _ui->_l_team_slot_1;
    slot_1._pb_team = _ui->_pb_team_slot_1;
    slot_1._l_latency = _ui->_l_latency_1;
    slot_1._pb_close = _ui->_pb_close_slot_1;
    slot_1.init();
    _slots.append(slot_1);

    Slot slot_2;
    slot_2._l_avatar = _ui->_l_avatar_2;
    slot_2._l_rank = _ui->_l_rank_2;
    slot_2._l_nick = _ui->_l_nick_2;
    slot_2._l_tank = _ui->_l_tank_2;
    slot_2._l_team = _ui->_l_team_slot_2;
    slot_2._pb_team = _ui->_pb_team_slot_2;
    slot_2._l_latency = _ui->_l_latency_2;
    slot_2._pb_close = _ui->_pb_close_slot_2;
    slot_2.init();
    _slots.append(slot_2);

    Slot slot_3;
    slot_3._l_avatar = _ui->_l_avatar_3;
    slot_3._l_rank = _ui->_l_rank_3;
    slot_3._l_nick = _ui->_l_nick_3;
    slot_3._l_tank = _ui->_l_tank_3;
    slot_3._l_team = _ui->_l_team_slot_3;
    slot_3._pb_team = _ui->_pb_team_slot_3;
    slot_3._l_latency = _ui->_l_latency_3;
    slot_3._pb_close = _ui->_pb_close_slot_3;
    slot_3.init();
    _slots.append(slot_3);

    Slot slot_4;
    slot_4._l_avatar = _ui->_l_avatar_4;
    slot_4._l_rank = _ui->_l_rank_4;
    slot_4._l_nick = _ui->_l_nick_4;
    slot_4._l_tank = _ui->_l_tank_4;
    slot_4._l_team = _ui->_l_team_slot_4;
    slot_4._pb_team = _ui->_pb_team_slot_4;
    slot_4._l_latency = _ui->_l_latency_4;
    slot_4._pb_close = _ui->_pb_close_slot_4;
    slot_4.init();
    _slots.append(slot_4);

    Slot slot_5;
    slot_5._l_avatar = _ui->_l_avatar_5;
    slot_5._l_rank = _ui->_l_rank_5;
    slot_5._l_nick = _ui->_l_nick_5;
    slot_5._l_tank = _ui->_l_tank_5;
    slot_5._l_team = _ui->_l_team_slot_5;
    slot_5._pb_team = _ui->_pb_team_slot_5;
    slot_5._l_latency = _ui->_l_latency_5;
    slot_5._pb_close = _ui->_pb_close_slot_5;
    slot_5.init();
    _slots.append(slot_5);

    Slot slot_6;
    slot_6._l_avatar = _ui->_l_avatar_6;
    slot_6._l_rank = _ui->_l_rank_6;
    slot_6._l_nick = _ui->_l_nick_6;
    slot_6._l_tank = _ui->_l_tank_6;
    slot_6._l_team = _ui->_l_team_slot_6;
    slot_6._pb_team = _ui->_pb_team_slot_6;
    slot_6._l_latency = _ui->_l_latency_6;
    slot_6._pb_close = _ui->_pb_close_slot_6;
    slot_6.init();
    _slots.append(slot_6);

    connect(slot_1._pb_team, &QPushButton::clicked, TcpGameConnectionClient::instance(), &TcpGameConnectionClient::sendChangeTeam);
    connect(slot_2._pb_team, &QPushButton::clicked, TcpGameConnectionClient::instance(), &TcpGameConnectionClient::sendChangeTeam);
    connect(slot_3._pb_team, &QPushButton::clicked, TcpGameConnectionClient::instance(), &TcpGameConnectionClient::sendChangeTeam);
    connect(slot_4._pb_team, &QPushButton::clicked, TcpGameConnectionClient::instance(), &TcpGameConnectionClient::sendChangeTeam);
    connect(slot_5._pb_team, &QPushButton::clicked, TcpGameConnectionClient::instance(), &TcpGameConnectionClient::sendChangeTeam);
    connect(slot_6._pb_team, &QPushButton::clicked, TcpGameConnectionClient::instance(), &TcpGameConnectionClient::sendChangeTeam);

    connect(slot_1._pb_close, &QPushButton::clicked, this, &FormCreatedGame::cancel);
    connect(slot_2._pb_close, &QPushButton::clicked, this, &FormCreatedGame::cancel);
    connect(slot_3._pb_close, &QPushButton::clicked, this, &FormCreatedGame::cancel);
    connect(slot_4._pb_close, &QPushButton::clicked, this, &FormCreatedGame::cancel);
    connect(slot_5._pb_close, &QPushButton::clicked, this, &FormCreatedGame::cancel);
    connect(slot_6._pb_close, &QPushButton::clicked, this, &FormCreatedGame::cancel);

}

void FormCreatedGame::updateSlots()
{
    for(int i = 0; i < _slots.size(); ++i)
        _slots[i].clear();

    int i = 0;
    foreach(PlayerData player_data, _players_data)
    {
        _slots[i].setNick(player_data._nick);       
        _slots[i].setAvatar(player_data._avatar);
        _slots[i].setRank(player_data._rank);
        _slots[i].setTeam(player_data._team);
        _slots[i].setTank(player_data._index_of_tank);
        _slots[i].setReady(player_data._ready);
        _slots[i]._ready = player_data._ready;
        TcpGameConnectionClient::instance()->isCreator() ? _slots[i].setSlotMode(SLOT_SERVER_MODE) : _slots[i].setSlotMode(SLOT_CLIENT_MODE);
        ++i;
    }

    _slots[TcpGameConnectionClient::instance()->index()].setMy(true);

    hideLastSlots(MAX_PLAYERS - _counter_of_player);
}

void FormCreatedGame::hideLastSlots(int cant)
{
    while(cant)
        _slots[MAX_PLAYERS - cant--].hide();
}

void FormCreatedGame::on__pb_select_tank_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_created_game->disable();
    forms->_form_tank_selector->show();
}

void FormCreatedGame::on__pb_close_slot_1_clicked()
{
    TcpGameConnectionClient::instance()->stop();
}

void FormCreatedGame::on__pb_i_ready_clicked(bool checked)
{
    TcpGameConnectionClient::instance()->sendReady(checked);
}

bool FormCreatedGame::isReadyToStart()
{
    int green_counter = 0;
    int red_counter = 0;
    foreach (PlayerData player_data, _players_data)
    {
        if(!player_data._ready)
            return false;
        player_data._team == 1 ? ++green_counter : ++red_counter;
    }

    return green_counter != 0 && red_counter != 0 && green_counter <= _green_counter && red_counter <= _red_counter;
}

void FormCreatedGame::on__pb_begin_clicked()
{
    disable();
    _timer_for_request_latency->stop();
    TcpGameConnectionClient::instance()->sendPrepareGame();
    ConnectionTcp::instance()->sendGameStarted();
}
