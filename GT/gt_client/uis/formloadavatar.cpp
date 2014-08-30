#include "formloadavatar.h"
#include "ui_formloadavatar.h"
#include "forms.h"

#include <QDebug>

FormLoadAvatar::FormLoadAvatar(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormLoadAvatar)
{
    _ui->setupUi(this);

    _dir_model = new QDirModel(this);
    _dir_model->setFilter(QDir::AllEntries|QDir::NoDotAndDotDot);
    _dir_model->setSorting(QDir::DirsFirst|QDir::Name);
    _proxy_model = new FilterProxyModel(this);
    _proxy_model->setFilterList(QStringList(".jpg"));
    _proxy_model->setSourceModel(_dir_model);
    _ui->_tv_dir_view->setModel(_proxy_model);
    _ui->_tv_dir_view->setColumnHidden(1,true);
    _ui->_tv_dir_view->setColumnHidden(2,true);
    _ui->_tv_dir_view->setColumnHidden(3,true);

    _ui->_tv_dir_view->installEventFilter(this);
}

FormLoadAvatar::~FormLoadAvatar()
{
    delete _ui;
}

void FormLoadAvatar::preview(QModelIndex model_index)
{
    _pixmap = QPixmap();

    QString file_name = _dir_model->filePath(_proxy_model->mapToSource(model_index));
    if(file_name.toUpper().endsWith(".JPG"))
    {
        QFile file(file_name);
        if(file.open(QIODevice::ReadOnly))
        {
            _pixmap.loadFromData(file.readAll(),"JPG");
        }
    }

    _ui->_l_avatar->setPixmap(_pixmap);

    if(_pixmap.isNull())
        _ui->_l_avatar->setText(tr("Avatar"));    

}

void FormLoadAvatar::on__pb_cancel_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_load_avatar->hide();
    forms->_form_create_user_account->enable();
    forms->_form_edit_user_account->enable();
}

void FormLoadAvatar::on__pb_accept_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_load_avatar->hide();
    ((FormCreateUserAccount*)(forms->_form_create_user_account))->updateAvatar(_pixmap);
    ((FormEditUserAccount*)(forms->_form_edit_user_account))->updateAvatar(_pixmap);
}


bool FormLoadAvatar::eventFilter(QObject *obj, QEvent *event)
{
    preview(_ui->_tv_dir_view->currentIndex());
    return QObject::eventFilter(obj,event);
}
