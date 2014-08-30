#include "formcreategame.h"
#include "ui_formcreategame.h"
#include "forms.h"
#include "resourcemanager.h"

FormCreateGame::FormCreateGame(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormCreateGame)
{
    _ui->setupUi(this);

    _model = new QStringListModel(this);

    TerrainListDelegate *terrain_list_delegate = new TerrainListDelegate(this);
    _ui->_lv_terrains->setItemDelegate(terrain_list_delegate);
    _ui->_lv_terrains->installEventFilter(this);
}

FormCreateGame::~FormCreateGame()
{
    delete _ui;
}

void FormCreateGame::show()
{
    disconnect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyConnected, this, &FormCreateGame::connectionClientConnected);
    connect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyConnected, this, &FormCreateGame::connectionClientConnected);

    QStringList terrains = ResourceManager::instance()->terrainList();

    _model->setStringList(terrains);
    _ui->_lv_terrains->setModel(_model);

    _ui->_l_terrain->clear();
    _ui->_l_counter_green->clear();
    _ui->_l_counter_red->clear();

    if(_model->index(0,0).isValid())
        _ui->_lv_terrains->setCurrentIndex(_model->index(0,0));

    Form::show();
}

void FormCreateGame::hide()
{
    disconnect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyConnected, this, &FormCreateGame::connectionClientConnected);
    Form::hide();
}

void FormCreateGame::on__pb_cancel_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_create_game->hide();
    forms->_form_chat->show();
}

void FormCreateGame::preview(QModelIndex model_index)
{
    _index_of_current_terrain = model_index.row();

    TerrainInfo terrain_info = ResourceManager::instance()->terrainsInfo().at(_index_of_current_terrain);

    _counter_of_players = terrain_info._green_count + terrain_info._red_count;

    _ui->_l_terrain->setPixmap(terrain_info._image);
    _ui->_l_counter_green->setText(QString::number(terrain_info._green_count));
    _ui->_l_counter_red->setText(QString::number(terrain_info._red_count));
}

void FormCreateGame::connectionClientConnected()
{
    ConnectionTcp::instance()->sendCreateGame(_index_of_current_terrain);
}

void FormCreateGame::on__pb_accept_clicked()
{
    Forms* forms = Forms::instance();

    if(TcpGameServer::instance()->open(_index_of_current_terrain, _counter_of_players))
    {
        forms->_form_create_game->disable();
        TcpGameConnectionClient::instance()->connectToHost(QHostAddress("127.0.0.1"), TCP_GAME_SERVER_PORT);
    }
    else
    {
        ((FormDialogMessage*)(forms->_form_message))->showError("Servidor de juego", QString("Error al crear el juego. Es posible que el puerto %1 esté en uso.").arg(TCP_GAME_SERVER_PORT), false);
    }
}

bool FormCreateGame::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::Timer)
    {
        _ui->_pb_accept->setEnabled(_ui->_lv_terrains->currentIndex().isValid());
        if(_ui->_lv_terrains->currentIndex().isValid())
        {
            preview(_ui->_lv_terrains->currentIndex());
            return true;
        }            
    }
    return QObject::eventFilter(object, event);
}
