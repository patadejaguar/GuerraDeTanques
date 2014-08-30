#include "connection.h"
#include "server.h"

Connection::Connection(bool use_ssl, int socket_descriptor, QObject *parent):
    TcpConnectionServer(use_ssl, socket_descriptor, parent)
{
    _data_base_connection = ServerConfiguration::getNewDataBaseConnection();
    _login = false;
}

Connection::~Connection()
{
    delete _data_base_connection;
}

UserProperties Connection::userProperties() const
{
    return _user_properties;
}

bool Connection::updateUserProperties()
{
    QSqlQuery query(_data_base_connection->getDataBaseName());
    query.prepare("SELECT avatar, wins, losses FROM Player WHERE Player.nick=:nick AND password=:password");
    query.bindValue(":nick", _nick);
    query.bindValue(":password", _password);
    query.exec();

    if(query.next())
    {
        _user_properties._nick = _nick;
        QPixmap avatar;
        avatar.loadFromData(query.value(0).toByteArray());
        _user_properties._avatar = avatar;
        _user_properties._wins = query.value(1).toInt();
        _user_properties._losses = query.value(2).toInt();
    }
    else
    {
        sendLoginResponceError(tr("Los datos son incorrectos."));
        return false;
    }

    return true;
}

bool Connection::isLogin()
{
    return _login;
}

QString Connection::location() const
{
    return _location;
}

void Connection::setLocation(const QString &location)
{
    _location = location;
}

void Connection::logout()
{
    _user_properties.clear();
    _location.clear();
}

void Connection::sendConnectedData(QStringList terrains, QStringList tanks, QStringList objects)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_CONNECTED_DATA << terrains << tanks << objects;
    sendData(data_to_write);
}

void Connection::sendRegisterUserSuccessful(QString nick)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_REGISTER_USER_SUCCESSFUL << nick;
    sendData(data_to_write);
}

void Connection::sendRegisterUserError(QString error_message)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream<<TCP_REGISTER_USER_ERROR<<error_message;
    sendData(data_to_write);
}

void Connection::sendEditUserSuccessful(QString nick)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream<<TCP_EDIT_USER_SUCCESSFUL<<nick;
    sendData(data_to_write);
}

void Connection::sendEditUserError(QString error_message)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream<<TCP_EDIT_USER_ERROR<<error_message;
    sendData(data_to_write);
}

void Connection::sendLoginSuccessful(QByteArray data)
{    
    _login = true;
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream<<TCP_LOGIN_USER_SUCCESSFUL;
    stream<<data;
    sendData(data_to_write);
}

void Connection::sendLoginResponceError(QString error_message)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream<<TCP_LOGIN_USER_ERROR<<error_message;
    sendData(data_to_write);
}

void Connection::sendUserLogin(QByteArray data)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_LOGIN_USER << data;
    sendData(data_to_write);
}

void Connection::sendUserLogout(UserProperties user)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_LOGOUT_USER << user;
    sendData(data_to_write);
}

void Connection::sendMessage(QString message)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_MESSAGE;
    stream << message;
    sendData(data_to_write);
}

void Connection::sendCreateGame(UserProperties user, int index_of_terrain, QString ip)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_CREATE_GAME << user << index_of_terrain << ip;
    sendData(data_to_write);
}

void Connection::sendCloseGame(QList<UserProperties> users)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_CLOSE_GAME << users;
    sendData(data_to_write);
}

void Connection::sendCreatedGames(QByteArray data)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_REQUEST_CREATED_GAMES;
    stream << data;
    sendData(data_to_write);
}

void Connection::sendPlayerJoinedGame(UserProperties user)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_PLAYER_JOINED_GAME << user;
    sendData(data_to_write);
}

void Connection::sendPlayerLeavedGame(QList<UserProperties> users_properties)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_PLAYER_LEAVED_GAME << users_properties;
    sendData(data_to_write);
}

void Connection::sendGameStarted(UserProperties creator)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_GAME_STARTED << creator;
    sendData(data_to_write);
}

void Connection::parseData(QDataStream &stream_input)
{
    quint8 command;
    stream_input>>command;
    switch (command)
    {
        case TCP_MISSING_RESOURCES:
            parseMissingResources(stream_input);
        break;
        case TCP_REGISTER_USER:
            parseRegisterUser(stream_input);
        break;
        case TCP_EDIT_USER:
            parseEditUser(stream_input);
        break;
        case TCP_LOGIN_USER:
            parseLoginUser(stream_input);
        break;
        case TCP_LOGOUT_USER:
            parseLogoutUser();
        break;
        case TCP_MESSAGE:
            parseMessage(stream_input);
        break;

            //juego
        case TCP_CREATE_GAME:
            parseCreateGame(stream_input);
        break;
        case TCP_CLOSE_GAME:
            parseCloseGame();
        break;
        case TCP_REQUEST_CREATED_GAMES:
            emit notifyRequestCreatedGames();
        break;
        case TCP_PLAYER_JOINED_GAME:
            parseJoinedGame(stream_input);
        break;
        case TCP_PLAYER_LEAVED_GAME:
            emit notifyLeavedGame();
        break;
        case TCP_GAME_STARTED:
            emit notifyGameStarted();
        break;
        case TCP_GAME_OVER:
            parseGameOver(stream_input);
        break;
    }
}

void Connection::parseMissingResources(QDataStream &stream_input)
{
    QStringList terrains, tanks, objects;
    stream_input >> terrains >> tanks >> objects;

    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_MISSING_RESOURCES << terrains.size();

    foreach (QString file_name, terrains)
    {
        QFile file(DIR_TERRAINS.absoluteFilePath(file_name));
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray file_data;
            while(!file.atEnd())
                file_data.append(file.readAll());
            stream << file_name << file_data;
        }
    }

    stream << tanks.size();

    foreach (QString file_name, tanks)
    {
        QFile file(DIR_TANKS.absoluteFilePath(file_name));
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray file_data;
            while(!file.atEnd())
                file_data.append(file.readAll());
            stream << file_name << file_data;
        }
    }

    stream << objects.size();

    foreach (QString file_name, objects)
    {
        QFile file(DIR_OBJECTS.absoluteFilePath(file_name));
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray file_data;
            while(!file.atEnd())
                file_data.append(file.readAll());
            stream << file_name << file_data;
        }
    }

    sendData(data_to_write);
}

void Connection::parseRegisterUser(QDataStream &stream_input)
{
    QString nick;
    QString password;
    QByteArray avatar;
    stream_input>>nick;
    stream_input>>password;
    stream_input>>avatar;

    QSqlQuery query(_data_base_connection->getDataBase());
    query.prepare("INSERT INTO Player (nick, password, avatar) VALUES (:nick, :password, :avatar)");
    query.bindValue(":nick", nick);
    query.bindValue(":password", password);
    query.bindValue(":avatar", avatar, QSql::In|QSql::Binary);
    query.exec();

    if(query.lastError().isValid())
        sendRegisterUserError(tr("Es posible que ya exista un usuario con ese nombre."));
    else
        sendRegisterUserSuccessful(nick);
}

void Connection::parseEditUser(QDataStream &stream_input)
{
    QString nick;
    QString current_password;
    QString new_password;
    QByteArray avatar;
    stream_input>>nick;
    stream_input>>current_password;
    stream_input>>new_password;
    stream_input>>avatar;

    QSqlQuery select_query(_data_base_connection->getDataBase());
    select_query.prepare("SELECT nick FROM Player WHERE nick=:nick AND password=:current_password");
    select_query.bindValue(":nick", nick);
    select_query.bindValue(":current_password", current_password);
    select_query.exec();

    if(!select_query.next())
        sendEditUserError(tr("Es posible que el usuario no exista o los datos son incorrectos."));
    else
    {
        QSqlQuery query(_data_base_connection->getDataBase());
        query.prepare("UPDATE Player SET password=:new_password , avatar=:new_avatar WHERE nick=:nick AND password=:current_password");
        query.bindValue(":nick", nick);
        query.bindValue(":current_password", current_password);
        query.bindValue(":new_password", new_password);
        query.bindValue(":new_avatar", avatar, QSql::In|QSql::Binary);
        query.exec();
        if(query.lastError().isValid())
            sendEditUserError(tr("Ocurrió un error mientras se actualizaba el usuario. Intentelo otra vez."));
        else
            sendEditUserSuccessful(nick);
    }
}

void Connection::parseLoginUser(QDataStream &stream_input)
{
    stream_input>>_nick;
    stream_input>>_password;

    emit notifyRequestLoginUser(_nick);
}

void Connection::parseLogoutUser()
{
    emit notifyLogoutUser(_user_properties);
}

void Connection::parseMessage(QDataStream &stream_input)
{
    QString message;
    stream_input >> message;
    emit notifyMessageReceived(QString("%1: %2").arg(_user_properties._nick).arg(message));
}

void Connection::parseCreateGame(QDataStream &stream_input)
{
    int index_of_terrain;
    stream_input >> index_of_terrain;
    emit notifyCreateGame(index_of_terrain);
}

void Connection::parseCloseGame()
{
    emit notifyCloseGame();
}

void Connection::parseJoinedGame(QDataStream &stream_input)
{
    QString creator;
    stream_input >> creator;

    emit notifyJoinedGame(creator);
}

void Connection::parseGameOver(QDataStream &stream_input)
{
    int team_win;
    QMap< PlayerData, QMap< PlayerData, KillsDeaths > > result;
    stream_input >> team_win >> result;

    QSqlQuery query(_data_base_connection->getDataBase());

    QList<PlayerData> players = result.keys();
    foreach(PlayerData player, players)
    {
        if(player._team == team_win)
            query.prepare("UPDATE Player SET wins=(SELECT wins FROM Player WHERE nick=:nick)+1 WHERE nick=:nick");
        else
            query.prepare("UPDATE Player SET losses=(SELECT losses FROM Player WHERE nick=:nick)+1 WHERE nick=:nick");
        query.bindValue(":nick", player._nick);
        query.exec();

        QMap<PlayerData, KillsDeaths> data = result.value(player);

        QList<PlayerData> others_players = data.keys();
        foreach (PlayerData other_player, others_players)
        {
            KillsDeaths kd = data.value(other_player);

            query.prepare("INSERT INTO Player_vs_Player (id_player_kill, id_player_death, kills, destroy) VALUES ((SELECT id FROM Player WHERE Player.nick=:p1), (SELECT id FROM Player WHERE Player.nick=:p2), :kills, :destroy)");
            query.bindValue(":p1", player._nick);
            query.bindValue(":p2", other_player._nick);
            query.bindValue(":kills", kd._kills);
            query.bindValue(":destroy", int(kd._circle_of_power_destroyed));
            query.exec();
        }
    }

    emit notifyCloseGame();
}
