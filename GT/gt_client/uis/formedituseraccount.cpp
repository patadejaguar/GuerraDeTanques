#include "formedituseraccount.h"
#include "ui_formedituseraccount.h"
#include "connectiontcp.h"
#include "forms.h"

#include <QBuffer>

FormEditUserAccount::FormEditUserAccount(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormEditUserAccount)
{
    _ui->setupUi(this);
}

FormEditUserAccount::~FormEditUserAccount()
{
    delete _ui;
}

void FormEditUserAccount::show()
{
    Form::show();
    _ui->_le_user->setStyleSheet("color: red;"
                                "border-color:red");
    _ui->_le_new_password->setStyleSheet("color: red;"
                                "border-color:red");
    _ui->_le_current_password->setStyleSheet("color: red;"
                                "border-color:red");
    _ui->_le_confirm_password->setStyleSheet("color: red;"
                                "border-color:red");
    _ui->_le_user->clear();
    _ui->_le_current_password->clear();
    _ui->_le_new_password->clear();
    _ui->_le_confirm_password->clear();
    _ui->_l_avatar->clear();    
    _avatar = QPixmap(":/icons/avatar.jpg");
    _ui->_l_avatar->setPixmap(_avatar);
    _ui->_pb_accept->setEnabled(false);
    _ui->_le_user->setFocus();
}

void FormEditUserAccount::updateAvatar(QPixmap avatar)
{
    enable();
    _avatar = avatar;
    _ui->_l_avatar->setPixmap(_avatar);
    if(_avatar.isNull())
        _ui->_l_avatar->setText(tr("Avatar"));
}

void FormEditUserAccount::on__pb_cancel_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_edit_user_account->hide();
    forms->_form_connected_to_server->show();
}

void FormEditUserAccount::on__pb_examine_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_edit_user_account->disable();
    forms->_form_load_avatar->show();
}

void FormEditUserAccount::on__pb_accept_clicked()
{
    QByteArray data;
    QBuffer buffer(&data);

    const QPixmap *pixmap = _ui->_l_avatar->pixmap();
    QPixmap scaled_pixmap = pixmap->scaled(50,50,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    if(!scaled_pixmap.isNull())
        scaled_pixmap.save(&buffer,"JPG");

    ConnectionTcp::instance()->sendEditUser(_ui->_le_user->text(), _ui->_le_current_password->text(), _ui->_le_new_password->text(), data);
}

void FormEditUserAccount::on__le_user_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);

    if(_ui->_le_user->text() == "")
        _ui->_le_user->setStyleSheet("color: red;"
                                    "border-color:red;");
    else
        _ui->_le_user->setStyleSheet("color: white;"
                                    "border-color:white;");

    checkAccept();
}

void FormEditUserAccount::on__le_current_password_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);

    if(_ui->_le_current_password->text() == "")
        _ui->_le_current_password->setStyleSheet("color: red;"
                                    "border-color:red;");
    else
        _ui->_le_current_password->setStyleSheet("color: white;"
                                    "border-color:white;");

    checkAccept();
}

void FormEditUserAccount::on__le_new_password_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);

    if(_ui->_le_new_password->text() == "")
        _ui->_le_new_password->setStyleSheet("color: red;"
                                    "border-color:red;");
    else
        _ui->_le_new_password->setStyleSheet("color: white;"
                                    "border-color:white;");

    if(_ui->_le_new_password->text() != _ui->_le_confirm_password->text())
        _ui->_le_confirm_password->setStyleSheet("color: red;"
                                    "border-color:red;");
    else
        _ui->_le_confirm_password->setStyleSheet("color: white;"
                                    "border-color:white;");

    checkAccept();
}

void FormEditUserAccount::on__le_confirm_password_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);

    if(_ui->_le_confirm_password->text() == "" || _ui->_le_new_password->text() != _ui->_le_confirm_password->text())
        _ui->_le_confirm_password->setStyleSheet("color: red;"
                                    "border-color:red;");
    else
        _ui->_le_confirm_password->setStyleSheet("color: white;"
                                                "border-color:white;");

    checkAccept();
}

void FormEditUserAccount::checkAccept()
{
    _ui->_pb_accept->setEnabled(_ui->_le_user->text() != "" &&
            _ui->_le_current_password->text() != "" &&
            _ui->_le_new_password->text() != "" &&
            _ui->_le_new_password->text() == _ui->_le_confirm_password->text());
}
