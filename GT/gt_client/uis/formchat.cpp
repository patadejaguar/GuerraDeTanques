#include "formchat.h"
#include "ui_formchat.h"
#include "forms.h"

FormChat::FormChat(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormChat)
{
    _ui->setupUi(this);

    _user_list_model = new UserListModel(this);
    _ui->_lv_users->setModel(_user_list_model);
    UserListDelegate *user_list_delegate = new UserListDelegate(this);
    _ui->_lv_users->setItemDelegate(user_list_delegate);
    installEventFilter(this);

    connect(ConnectionTcp::instance(), &ConnectionTcp::notifyLoginUser, this, &FormChat::addUserToList);
    connect(ConnectionTcp::instance(), &ConnectionTcp::notifyLogoutUser, this, &FormChat::logoutUser);
    connect(ConnectionTcp::instance(), &ConnectionTcp::notifyMessageReceived, this, &FormChat::showMessageReceived);

    connect(ConnectionTcp::instance(), &ConnectionTcp::notifyCreateGame, this, &FormChat::createGame);
    connect(ConnectionTcp::instance(), &ConnectionTcp::notifyCloseGame, this, &FormChat::closeGame);
    connect(ConnectionTcp::instance(), &ConnectionTcp::notifyRequestCreatedGames, this, &FormChat::requestCreatedGames);
    connect(ConnectionTcp::instance(), &ConnectionTcp::notifyPlayerJoinedGame, this, &FormChat::playerJoinedGame);
    connect(ConnectionTcp::instance(), &ConnectionTcp::notifyPlayerLeavedGame, this, &FormChat::playerLeavedGame);

}

FormChat::~FormChat()
{
    delete _ui;
    delete _user_list_model;
}

void FormChat::show(UserProperties user_properties, QList<UserProperties> users_login)
{
    _ui->_le_message->clear();
    _ui->_te_chat->clear();

    _user_list_model->clear();
    _user_list_model->addUserLogin(user_properties);
    _user_list_model->addUsersLogin(users_login);

    _ui->_l_nick->setText(user_properties._nick);
    _ui->_l_avatar->setPixmap(user_properties._avatar);
    _ui->_l_rank->setPixmap(QPixmap(QString(":/gt/ranks/rank_%1.png").arg(user_properties.rank())));
    _ui->_l_wins->setText(QString::number(user_properties._wins));
    _ui->_l_losses->setText(QString::number(user_properties._losses));
    _ui->_le_message->setFocus();

    Form::show();
}

void FormChat::addUserToList(UserProperties user_login)
{
    _user_list_model->addUserLogin(user_login);
}

void FormChat::logoutUser(UserProperties user)
{
    if(user._nick == _ui->_l_nick->text())
    {
        Forms* forms = Forms::instance();

        forms->_form_chat->hide();
        forms->_form_connected_to_server->show();
    }
    else
        _user_list_model->removeUser(user);
}

bool FormChat::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::KeyPress && !_ui->_le_message->hasFocus())
    {
        _ui->_le_message->setText(_ui->_le_message->text()+((QKeyEvent*)event)->text());
        _ui->_le_message->setFocus();
    }
    return Form::eventFilter(object, event);

}

void FormChat::showMessageReceived(QString message)
{
    _ui->_te_chat->append(message);
}

void FormChat::createGame(UserProperties user, int index_of_terrain, QString ip)
{
    Q_UNUSED(index_of_terrain);
    Q_UNUSED(ip);
    _user_list_model->removeUser(user);
}

void FormChat::closeGame(QList<UserProperties> users)
{
    _user_list_model->setUsersLogins(users);
    foreach(UserProperties user, users)
    {
        if(user._nick == _ui->_l_nick->text())
        {
            _ui->_l_wins->setText(QString::number(user._wins));
            _ui->_l_losses->setText(QString::number(user._losses));
            break;
        }
    }
}

void FormChat::requestCreatedGames(QByteArray data)
{
    Forms* forms = Forms::instance();

    forms->_form_chat->hide();
    ((FormJoinGame*)(forms->_form_join_game))->show(data);
}

void FormChat::playerJoinedGame(UserProperties user)
{
    _user_list_model->removeUser(user);
}

void FormChat::playerLeavedGame(QList<UserProperties> users_properties)
{
    _user_list_model->setUsersLogins(users_properties);

    foreach(UserProperties user, users_properties)
    {
        if(user._nick == _ui->_l_nick->text())
        {
            _ui->_l_wins->setText(QString::number(user._wins));
            _ui->_l_losses->setText(QString::number(user._losses));
            break;
        }
    }
}

void FormChat::on__pb_exit_clicked()
{
    ConnectionTcp::instance()->sendLogoutUser();
}

void FormChat::on__le_message_returnPressed()
{
    if(_ui->_le_message->text() != "")
    {
        ConnectionTcp::instance()->sendMessage(_ui->_le_message->text());
        _ui->_le_message->clear();
        _ui->_le_message->setFocus();
    }
}

void FormChat::on__pb_create_game_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_chat->hide();
    forms->_form_create_game->show();
}

void FormChat::show()
{
    _ui->_le_message->setFocus();
    Form::show();
}

void FormChat::on__pb_join_game_clicked()
{
    ConnectionTcp::instance()->sendRequestCreatedGames();

    Forms* forms = Forms::instance();

    forms->_form_chat->disable();
}

