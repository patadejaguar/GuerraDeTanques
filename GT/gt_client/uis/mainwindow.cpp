#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    //play the music
    _music = new Music(this);
    _music->setMedia(QUrl("qrc:///music/music_2.mp3"));
    _music->play();

    Configuration::addObserver(_music);

    //register my meta types
    qRegisterMetaType< QDataStream* >("QDataStream*");
    qRegisterMetaType< UserProperties >("UserProperties");
    qRegisterMetaType< QList<UserProperties> >("QList<UserProperties>");
    qRegisterMetaType< PlayerAction >("PlayerAction");
    qRegisterMetaType< Actions >("Actions");
    qRegisterMetaType< PlayerData >("PlayerData");
    qRegisterMetaType< QList<PlayerData> >("QList<PlayerData>");
    qRegisterMetaType< QList<int> >("QList<int>");
    qRegisterMetaType< KillsDeaths >("KillsDeaths");
    qRegisterMetaType< QMap< PlayerData, QMap< PlayerData, KillsDeaths > > >("QMap< PlayerData, QMap< PlayerData, KillsDeaths > >");

    //init configuration
    Configuration::load();
    updateScreenResolution();

    //create cursor
    createAndSetCursor();

    //create screens
    createScreens();

    //create connections
    createConnection();

    //update screen resolution
    updateScreenResolution();
}

MainWindow::~MainWindow()
{
    delete _ui;
    _music->stop();
    delete _music;
}

void MainWindow::createScreens()
{
    QSize default_resolution = SCREEN_RESOLUTION;

    Forms* forms = Forms::instance();

    forms->_form_principal = new FormPrincipal(_ui->_container);
    forms->_form_principal->setGeometry(0,0,default_resolution.width(),default_resolution.height());

    forms->_form_replays_select = new FormReplaySelect(_ui->_container);
    forms->_form_replays_select->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_replays_select->hide();

    forms->_form_replays_view = new FormReplayView(_ui->_container);
    forms->_form_replays_view->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_replays_view->hide();

    forms->_form_configuration = new FormConfiguration(_ui->_container);
    forms->_form_configuration->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_configuration->hide();

    forms->_form_controls = new FormControls(_ui->_container);
    forms->_form_controls->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_controls->hide();

    forms->_form_sound = new FormSound(_ui->_container);
    forms->_form_sound->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_sound->hide();

    forms->_form_network = new FormNetwork(_ui->_container);
    forms->_form_network->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_network->hide();

    forms->_form_help = new FormHelp(_ui->_container);
    forms->_form_help->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_help->hide();

    forms->_form_credits = new FormCredits(_ui->_container);
    forms->_form_credits->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_credits->hide();

    forms->_form_connected_to_server = new FormConnectedToServer(_ui->_container);
    forms->_form_connected_to_server->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_connected_to_server->hide();

    forms->_form_create_user_account = new FormCreateUserAccount(_ui->_container);
    forms->_form_create_user_account->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_create_user_account->hide();

    forms->_form_edit_user_account = new FormEditUserAccount(_ui->_container);
    forms->_form_edit_user_account->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_edit_user_account->hide();

    forms->_form_chat = new FormChat(_ui->_container);
    forms->_form_chat->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_chat->hide();

    forms->_form_create_game = new FormCreateGame(_ui->_container);
    forms->_form_create_game->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_create_game->hide();

    forms->_form_created_game = new FormCreatedGame(_ui->_container);
    forms->_form_created_game->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_created_game->hide();

    forms->_form_join_game = new FormJoinGame(_ui->_container);
    forms->_form_join_game->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_join_game->hide();

    forms->_form_load_avatar = new FormLoadAvatar(_ui->_container);
    forms->_form_load_avatar->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_load_avatar->hide();

    forms->_form_tank_selector = new FormTankSelector(_ui->_container);
    forms->_form_tank_selector->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_tank_selector->hide();

    forms->_form_game = new FormGame(_ui->_container);
    forms->_form_game->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_game->hide();

    forms->_form_init_game = new FormInitGame(_ui->_container);
    forms->_form_init_game->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_init_game->hide();

    forms->_form_result = new FormResults(_ui->_container);
    forms->_form_result->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_result->hide();

    forms->_form_message = new FormDialogMessage(_ui->_container);
    forms->_form_message->setGeometry(0,0,default_resolution.width(),default_resolution.height());
    forms->_form_message->hide();
}

void MainWindow::createConnection()
{
    ConnectionTcp *connection_tcp = ConnectionTcp::instance();
    connect(connection_tcp, &ConnectionTcp::notifyError, this, &MainWindow::connectionError);
    connect(connection_tcp, &ConnectionTcp::notifyRegisterSuccessful, this, &MainWindow::connectionRegistered);
    connect(connection_tcp, &ConnectionTcp::notifyEditSuccessful, this, &MainWindow::connectionEdited);
    connect(connection_tcp, &ConnectionTcp::notifyLoginSuccessful, this, &MainWindow::connectionLogin);

    ResourceManager* resource_manager = ResourceManager::instance();
    connect(resource_manager, &ResourceManager::notifyResourceAreReady, this, &MainWindow::resourceManagerReady);
}

void MainWindow::updateScreenResolution()
{
//    Forms* forms = Forms::instance();

//    forms->_form_principal->setEnabled(false);
    QSize resolution = SCREEN_RESOLUTION;

#ifdef WIN32
//    DEVMODE devmode;
//    memset(&devmode,0,sizeof(DEVMODE));

//    devmode.dmSize=sizeof(DEVMODE);
//    devmode.dmPelsWidth=resolution.width();
//    devmode.dmPelsHeight=resolution.height();
//    devmode.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
//    ChangeDisplaySettings(&devmode,CDS_FULLSCREEN);
#endif

    _ui->_container->setMaximumSize(resolution);
    setMinimumSize(resolution);
    setMaximumSize(resolution);

//    forms->_form_principal->setEnabled(true);
}

void MainWindow::connectionError(QString error, bool critical)
{
    Forms* forms = Forms::instance();

    ((FormDialogMessage*)(forms->_form_message))->showError("Error",error, critical);
    forms->_form_connected_to_server->disable();
    forms->_form_create_user_account->disable();
    forms->_form_edit_user_account->disable();
    forms->_form_chat->disable();
    forms->_form_create_game->disable();
    forms->_form_created_game->disable();
    forms->_form_join_game->disable();
    forms->_form_tank_selector->disable();
    forms->_form_load_avatar->disable();
    forms->_form_game->disable();
    forms->_form_init_game->disable();
    forms->_form_result->disable();
}

void MainWindow::connectionRegistered(QString user)
{
    Forms* forms = Forms::instance();

    forms->_form_create_user_account->hide();    
    ((FormConnectedToServer*)(forms->_form_connected_to_server))->show(user);
}

void MainWindow::connectionEdited(QString user)
{
    Forms* forms = Forms::instance();

    forms->_form_edit_user_account->hide();
    ((FormConnectedToServer*)(forms->_form_connected_to_server))->show(user);
}

void MainWindow::connectionLogin(UserProperties user_properties, QList<UserProperties> users_login)
{
    QSettings settings("game_config", QSettings::IniFormat);
    settings.setValue("last_nick_login", user_properties._nick);

    Forms* forms = Forms::instance();

    forms->_form_connected_to_server->hide();
    ((FormChat*)(forms->_form_chat))->show(user_properties, users_login);
}

void MainWindow::createAndSetCursor()
{
    QCursor cursor = QCursor(QPixmap(":/cursor/cursor.png"), 8, 2);
    setCursor(cursor);
}

void MainWindow::resourceManagerReady()
{
    Forms* forms = Forms::instance();

    forms->_form_principal->hide();
    forms->_form_message->hide();
    forms->_form_connected_to_server->show();
}
