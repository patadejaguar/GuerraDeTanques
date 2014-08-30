#include "formjoingame.h"
#include "ui_formjoingame.h"
#include "forms.h"

FormJoinGame::FormJoinGame(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormJoinGame)
{
    _ui->setupUi(this);
    _model = new QStringListModel(this);
    GameListDelegate *game_list_delegate = new GameListDelegate(this);
    _ui->_lv_games->setItemDelegate(game_list_delegate);
    _ui->_lv_games->installEventFilter(this);

    connect(ConnectionTcp::instance(), &ConnectionTcp::notifyLogoutUser, this, &FormJoinGame::logoutUser);
    connect(ConnectionTcp::instance(), &ConnectionTcp::notifyCreateGame, this, &FormJoinGame::createGame);
    connect(ConnectionTcp::instance(), &ConnectionTcp::notifyCloseGame, this, &FormJoinGame::closeGame);
    connect(ConnectionTcp::instance(), &ConnectionTcp::notifyGameStarted, this, &FormJoinGame::gameStarted);

    connect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyGameFull, this, &FormJoinGame::gameFull);
    connect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyGameReadyToStart, this, &FormJoinGame::gameReadyToStart);
    connect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyGameIsStarted, this, &FormJoinGame::gameIsStarted);
}

FormJoinGame::~FormJoinGame()
{
    delete _ui;
}

void FormJoinGame::logoutUser(UserProperties user)
{
    bool j = true;
    for(int i = 0;j && i< _games_data.size(); ++i)
        if(_games_data.at(i)._creator == user)
        {
            _games_data.removeAt(i);
            j = false;
            updateListOfGames();
        }
}

void FormJoinGame::createGame(UserProperties user, int index_of_terrain, QString ip)
{
    GameData game_data;
    game_data._creator = user;
    game_data._index_of_terrain = index_of_terrain;
    game_data._ip = ip;
    _games_data.append(game_data);

    updateListOfGames();
}

void FormJoinGame::updateData(QByteArray data)
{
    _games_data.clear();
    QDataStream stream(&data,QIODevice::ReadOnly);
    while(!stream.atEnd())
    {
        GameData game;
        stream>>game._creator;
        stream>>game._index_of_terrain;
        stream>>game._ip;
        _games_data.append(game);
    }
}

void FormJoinGame::updateListOfGames()
{
    QStringList games;

    for(int i = 0;i< _games_data.size(); ++i)
        games.append(QString("Juego de [%1]").arg(_games_data.at(i)._creator._nick));

    _model->setStringList(games);
    _ui->_lv_games->setModel(_model);

    if(_model->index(0,0).isValid())
        _ui->_lv_games->setCurrentIndex(_model->index(0,0));
}

void FormJoinGame::show()
{
    disconnect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyError, this, &FormJoinGame::gameJoinError);
    connect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyError, this, &FormJoinGame::gameJoinError);
    Form::show();
}

void FormJoinGame::hide()
{
    disconnect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyError, this, &FormJoinGame::gameJoinError);
    Form::hide();
}

void FormJoinGame::show(QByteArray data)
{
    clearLabels();

    updateData(data);

    updateListOfGames();

    show();
}

bool FormJoinGame::eventFilter(QObject *object, QEvent *event)
{
    _ui->_pb_accept->setEnabled(_ui->_lv_games->currentIndex().isValid());
    if(_ui->_lv_games->currentIndex().isValid())
    {
        preview(_ui->_lv_games->currentIndex());
        return true;
    }
    return QObject::eventFilter(object, event);
}

void FormJoinGame::closeGame(QList<UserProperties> users)
{
    for(int i = 0; i< _games_data.size(); ++i)
        if(users.contains(_games_data.at(i)._creator))
        {
            _games_data.removeAt(i);
            clearLabels();
            updateListOfGames();
            i = _games_data.size();
        }
}

void FormJoinGame::gameStarted(UserProperties creator)
{
    for(int i = 0; i< _games_data.size(); ++i)
        if(creator == _games_data.at(i)._creator)
        {
            _games_data.removeAt(i);
            clearLabels();
            updateListOfGames();
            i = _games_data.size();
        }
}

void FormJoinGame::gameFull()
{
    Forms* forms = Forms::instance();

    forms->_form_join_game->disable();
    ((FormDialogMessage*)(forms->_form_message))->showError("Juego","El juego está completo.", false);
}

void FormJoinGame::gameJoinError(QString error, bool critical)
{
    Q_UNUSED(critical);

    Forms* forms = Forms::instance();

    forms->_form_join_game->disable();
    ((FormDialogMessage*)(forms->_form_message))->showError("Juego",error, false);
}

void FormJoinGame::gameReadyToStart()
{
    Forms* forms = Forms::instance();

    forms->_form_join_game->disable();
    ((FormDialogMessage*)(forms->_form_message))->showError("Juego","El juego está listo para comenzar", false);
}

void FormJoinGame::gameIsStarted()
{
    Forms* forms = Forms::instance();

    forms->_form_join_game->disable();
    ((FormDialogMessage*)(forms->_form_message))->showError("Juego","El juego ya empezó.", false);
}

void FormJoinGame::on__pb_cancel_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_join_game->hide();
    forms->_form_chat->show();
}

void FormJoinGame::on__pb_accept_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_join_game->disable();
    TcpGameConnectionClient::instance()->connectToHost(QHostAddress(_games_data.at(_ui->_lv_games->currentIndex().row())._ip), TCP_GAME_SERVER_PORT);
}

void FormJoinGame::preview(QModelIndex index)
{
    TerrainInfo terrain_info = ResourceManager::instance()->terrainsInfo().at(_games_data.at(index.row())._index_of_terrain);

    _ui->_l_terrain_text->setText(terrain_info._name);
    _ui->_l_terrain->setPixmap(terrain_info._image);
    _ui->_l_counter_green->setText(QString::number(terrain_info._green_count));
    _ui->_l_counter_red->setText(QString::number(terrain_info._red_count));
}

void FormJoinGame::clearLabels()
{
    _ui->_l_terrain_text->clear();
    _ui->_l_terrain->setPixmap(QPixmap());
    _ui->_pb_accept->setEnabled(false);
    _ui->_l_counter_green->clear();
    _ui->_l_counter_red->clear();
}
