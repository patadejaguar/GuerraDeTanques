#include "formcreateuseraccount.h"
#include "ui_formcreateuseraccount.h"
#include "forms.h"

FormCreateUserAccount::FormCreateUserAccount(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormCreateUserAccount)
{
    _ui->setupUi(this);    
}

FormCreateUserAccount::~FormCreateUserAccount()
{
    delete _ui;
}

void FormCreateUserAccount::show()
{
    Form::show();
    _ui->_le_user->setStyleSheet("color: red;"
                                "border-color:red;");
    _ui->_le_password->setStyleSheet("color: red;"
                                "border-color:red;");
    _ui->_le_confirm_password->setStyleSheet("color: red;"
                                "border-color:red;");
    _ui->_le_user->clear();
    _ui->_le_password->clear();
    _ui->_le_confirm_password->clear();
    _ui->_l_avatar->clear();
    _avatar = QPixmap(":/icons/avatar.jpg");
    _ui->_l_avatar->setPixmap(_avatar);
    _ui->_pb_accept->setEnabled(false);
    _ui->_le_user->setFocus();
}

void FormCreateUserAccount::updateAvatar(QPixmap avatar)
{
    enable();
    _avatar = avatar;
    _ui->_l_avatar->setPixmap(_avatar);
    if(_avatar.isNull())
        _ui->_l_avatar->setText(tr("Avatar"));
}

void FormCreateUserAccount::on__le_user_textEdited(const QString &arg1)
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

void FormCreateUserAccount::on__le_password_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);

    if(_ui->_le_password->text() == "")
        _ui->_le_password->setStyleSheet("color: red;"
                                    "border-color:red;");
    else
        _ui->_le_password->setStyleSheet("color: white;"
                                    "border-color:white;");

    if(_ui->_le_password->text() != _ui->_le_confirm_password->text())
        _ui->_le_confirm_password->setStyleSheet("color: red;"
                                    "border-color:red;");
    else
        _ui->_le_confirm_password->setStyleSheet("color: white;"
                                    "border-color:white;");

    checkAccept();
}

void FormCreateUserAccount::on__le_confirm_password_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);

    if(_ui->_le_confirm_password->text() == "" || _ui->_le_password->text() != _ui->_le_confirm_password->text())
        _ui->_le_confirm_password->setStyleSheet("color: red;"
                                    "border-color:red;");
    else
        _ui->_le_confirm_password->setStyleSheet("color: white;"
                                    "border-color:white;");

    checkAccept();
}

void FormCreateUserAccount::on__pb_accept_clicked()
{
    QByteArray data;
    QBuffer buffer(&data);

    const QPixmap *pixmap = _ui->_l_avatar->pixmap();
    QPixmap scaled_pixmap = pixmap->scaled(50,50,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    if(!scaled_pixmap.isNull())
        scaled_pixmap.save(&buffer,"JPG");

    ConnectionTcp::instance()->sendRegisterUser(_ui->_le_user->text(), _ui->_le_password->text(), data);
}

void FormCreateUserAccount::on__pb_examine_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_create_user_account->disable();
    forms->_form_load_avatar->show();
}

void FormCreateUserAccount::checkAccept()
{
    _ui->_pb_accept->setEnabled(_ui->_le_user->text() != "" &&
            _ui->_le_password->text() != "" &&
            _ui->_le_password->text() == _ui->_le_confirm_password->text());
}

void FormCreateUserAccount::on__pb_cancel_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_create_user_account->hide();
    forms->_form_connected_to_server->show();
}
