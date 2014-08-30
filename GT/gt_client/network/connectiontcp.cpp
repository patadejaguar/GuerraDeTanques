#include "connectiontcp.h"
#include <QCryptographicHash>

ConnectionTcp *ConnectionTcp::_instance = NULL;

ConnectionTcp* ConnectionTcp::instance()
{
    if(_instance == NULL)
        _instance = new ConnectionTcp();
    return _instance;
}

ConnectionTcp::ConnectionTcp():TcpClient()
{
    qRegisterMetaType<QList<UserProperties> >("QList<UserProperties>");
    qRegisterMetaType<UserProperties>("UserProperties");
}

void ConnectionTcp::sendMissingsResources(QStringList terrains, QStringList tanks, QStringList objects)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_MISSING_RESOURCES << terrains << tanks << objects;
    sendData(data_to_write);
}

void ConnectionTcp::sendRegisterUser(QString nick, QString password, QByteArray avatar)
{
    QString password_hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha3_512).toBase64();

    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_REGISTER_USER << nick << password_hash << avatar;
    sendData(data_to_write);
}

void ConnectionTcp::sendEditUser(QString nick, QString current_password, QString new_password, QByteArray new_avatar)
{
    QString current_password_hash = QCryptographicHash::hash(current_password.toUtf8(), QCryptographicHash::Sha3_512).toBase64();
    QString new_password_hash = QCryptographicHash::hash(new_password.toUtf8(), QCryptographicHash::Sha3_512).toBase64();

    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_EDIT_USER << nick << current_password_hash << new_password_hash << new_avatar;
    sendData(data_to_write);
}

void ConnectionTcp::sendLoginUser(QString nick, QString password)
{
    QString password_hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha3_512).toBase64();

    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_LOGIN_USER << nick << password_hash;
    sendData(data_to_write);
}

void ConnectionTcp::parseData(QDataStream &stream_input)
{
    quint8 command;
    stream_input>>command;
    switch (command)
    {
        case TCP_CONNECTED_DATA:
            parseConnectedData(stream_input);
        break;
        case TCP_MISSING_RESOURCES:
            parseMissingResources(stream_input);
        break;
        case TCP_REGISTER_USER_SUCCESSFUL:
            parseRegisterUserSuccessful(stream_input);
        break;
        case TCP_REGISTER_USER_ERROR:
            parseRegisterUserError(stream_input);
        break;
        case TCP_EDIT_USER_SUCCESSFUL:
            parseEditUserSuccessful(stream_input);
        break;
        case TCP_EDIT_USER_ERROR:
            parseEditUserError(stream_input);
        break;
        case TCP_LOGIN_USER_SUCCESSFUL:
            parseLoginSucessful(stream_input);
        break;
        case TCP_LOGIN_USER_ERROR:
            parseLoginError(stream_input);
        break;
        case TCP_LOGOUT_USER:
            parseLogoutUser(stream_input);
        break;
        case TCP_MESSAGE:
            parseMessage(stream_input);
        break;
        case TCP_LOGIN_USER:
            parseLoginUser(stream_input);
        break;

        //juego
        case TCP_CREATE_GAME:
            parseCreateGame(stream_input);
        break;
        case TCP_CLOSE_GAME:
            parseCloseGame(stream_input);
        break;
        case TCP_REQUEST_CREATED_GAMES:
            parseRequestCreatedGames(stream_input);
        break;
        case TCP_PLAYER_JOINED_GAME:
            parsePlayerJoinedGame(stream_input);
        break;
        case TCP_PLAYER_LEAVED_GAME:
            parsePlayerLeavedGame(stream_input);
        break;
        case TCP_GAME_STARTED:
            parseGameStarted(stream_input);
        break;
    }
}

void ConnectionTcp::parseConnectedData(QDataStream &stream_input)
{
    QStringList terrains, tanks, objects;
    stream_input >> terrains >> tanks >> objects;
    emit notifyConnectedData(terrains, tanks, objects);
}

void ConnectionTcp::parseMissingResources(QDataStream &stream_input)
{
    int terrains, tanks, objects;

    stream_input >> terrains;
    while(terrains--)
    {
        QString file_name;
        QByteArray data;
        stream_input >> file_name >> data;
        QFile file(DIR_TERRAINS.absoluteFilePath(file_name));
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(data);
            file.close();
        }
    }

    stream_input >> tanks;
    while(tanks--)
    {
        QString file_name;
        QByteArray data;
        stream_input >> file_name >> data;
        QFile file(DIR_TANKS.absoluteFilePath(file_name));
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(data);
            file.close();
        }
    }

    stream_input >> objects;
    while(objects--)
    {
        QString file_name;
        QByteArray data;
        stream_input >> file_name >> data;
        QFile file(DIR_OBJECTS.absoluteFilePath(file_name));
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(data);
            file.close();
        }
    }
    emit notifyResourcesAreReady();
}

void ConnectionTcp::parseRegisterUserSuccessful(QDataStream &stream_input)
{
    QString nick;
    stream_input>>nick;
    emit notifyRegisterSuccessful(nick);
}

void ConnectionTcp::parseRegisterUserError(QDataStream &stream_input)
{
    QString m_error;
    stream_input>>m_error;
    emit notifyError(m_error, false);
}

void ConnectionTcp::parseEditUserSuccessful(QDataStream &stream_input)
{
    QString nick;
    stream_input>>nick;
    emit notifyEditSuccessful(nick);
}

void ConnectionTcp::parseEditUserError(QDataStream &stream_input)
{
    QString m_error;
    stream_input>>m_error;
    emit notifyError(m_error, false);
}

void ConnectionTcp::parseLoginSucessful(QDataStream &stream_input)
{
    QByteArray data;
    stream_input>>data;
    QDataStream stream(&data, QIODevice::ReadOnly);

    stream >> _user_properties;

    QList<UserProperties> users_login;
    while(!stream.atEnd())
    {
        UserProperties user;
        stream >> user;
        users_login.append(user);
    }
    emit notifyLoginSuccessful(_user_properties, users_login);

}

void ConnectionTcp::parseLoginError(QDataStream &stream_input)
{
    QString m_error;
    stream_input>>m_error;
    emit notifyError(m_error, false);
}

void ConnectionTcp::parseMessage(QDataStream &stream_input)
{
    QString message;
    stream_input>>message;
    emit notifyMessageReceived(message);
}

void ConnectionTcp::parseLoginUser(QDataStream &stream_input)
{
    QByteArray data;
    stream_input>>data;
    QDataStream stream(&data, QIODevice::ReadOnly);
    UserProperties user;
    stream >> user;
    emit notifyLoginUser(user);
}

void ConnectionTcp::parseLogoutUser(QDataStream &stream_input)
{
    UserProperties user;
    stream_input >> user;

    if(_user_properties._nick == user._nick)
        _user_properties.clear();

    emit notifyLogoutUser(user);
}

void ConnectionTcp::parseCreateGame(QDataStream &stream_input)
{
    UserProperties user;
    int index_of_terrain;
    QString ip;
    stream_input >> user >> index_of_terrain >> ip;
    emit notifyCreateGame(user, index_of_terrain, ip);
}

void ConnectionTcp::parseCloseGame(QDataStream &stream_input)
{
    QList<UserProperties> users;
    stream_input >> users;
    emit notifyCloseGame(users);
}

void ConnectionTcp::parseRequestCreatedGames(QDataStream &stream_input)
{
    QByteArray data;
    stream_input>>data;
    emit notifyRequestCreatedGames(data);
}

void ConnectionTcp::parsePlayerJoinedGame(QDataStream &stream_input)
{
    UserProperties user;
    stream_input >> user;
    emit notifyPlayerJoinedGame(user);
}

void ConnectionTcp::parsePlayerLeavedGame(QDataStream &stream_input)
{
    QList<UserProperties> users_prop;
    stream_input >> users_prop;
    emit notifyPlayerLeavedGame(users_prop);
}

void ConnectionTcp::parseGameStarted(QDataStream &stream_input)
{
    UserProperties creator;
    stream_input >> creator;
    emit notifyGameStarted(creator);
}

void ConnectionTcp::sendLogoutUser()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_LOGOUT_USER;
    sendData(data_to_write);
}

void ConnectionTcp::sendMessage(QString message)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_MESSAGE << message;
    sendData(data_to_write);
}

void ConnectionTcp::sendCreateGame(int index_of_terrain)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_CREATE_GAME << index_of_terrain;
    sendData(data_to_write);
}

void ConnectionTcp::sendCloseGame()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_CLOSE_GAME;
    sendData(data_to_write);
}

void ConnectionTcp::sendRequestCreatedGames()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_REQUEST_CREATED_GAMES;
    sendData(data_to_write);
}

void ConnectionTcp::sendJoinGame(QString creator)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_PLAYER_JOINED_GAME << creator;
    sendData(data_to_write);
}

void ConnectionTcp::sendLeaveGame()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_PLAYER_LEAVED_GAME;
    sendData(data_to_write);
}

void ConnectionTcp::sendGameStarted()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_GAME_STARTED;
    sendData(data_to_write);
}

void ConnectionTcp::sendGameOver(int team_win, QMap< PlayerData, QMap< PlayerData, KillsDeaths > > results)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_GAME_OVER << team_win << results;
    sendData(data_to_write);
}
