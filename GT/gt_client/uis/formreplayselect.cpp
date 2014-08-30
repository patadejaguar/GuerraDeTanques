#include "formreplayselect.h"
#include "ui_formreplayselect.h"

#include "forms.h"

FormReplaySelect::FormReplaySelect(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormReplaySelect)
{
    _ui->setupUi(this);

    _model = new QStringListModel(this);

    GameListDelegate *game_list_delegate = new GameListDelegate(this);
    _ui->_lv_games->setItemDelegate(game_list_delegate);
}

FormReplaySelect::~FormReplaySelect()
{
    delete _ui;
}

void FormReplaySelect::show()
{
    QStringList replays = ResourceManager::instance()->replayList();

    _model->setStringList(replays);
    _ui->_lv_games->setModel(_model);

    if(_model->index(0,0).isValid())
    {
        _ui->_pb_accept->setEnabled(true);
        _ui->_lv_games->setCurrentIndex(_model->index(0,0));
    }

    Form::show();
}

void FormReplaySelect::on__pb_cancel_clicked()
{
    hide();
    Forms::instance()->_form_principal->show();
}


void FormReplaySelect::on__pb_accept_clicked()
{
    hide();
    Forms::instance()->_form_replays_view->show(_ui->_lv_games->currentIndex().data().toString());
}
