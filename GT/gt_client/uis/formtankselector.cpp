#include "formtankselector.h"
#include "ui_formtankselector.h"
#include "forms.h"

FormTankSelector::FormTankSelector(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormTankSelector)
{
    _ui->setupUi(this);

    _ui->_l_foreground_skill_1->installEventFilter(this);
    _ui->_l_foreground_skill_2->installEventFilter(this);
    _ui->_l_foreground_skill_3->installEventFilter(this);
    _ui->_l_foreground->installEventFilter(this);

    _html = "<html><head><style type=\"text/css\">"
            "body {"
            "background-color: #000;"
            "}"
            "span{"
            "color:white;"
            "}"
            ".y{"
            "color: " + COLOR_YELOW + ";"
            "}"
            ".yl{"
            "color: " + COLOR_YELOW_LIGHT + ";"
            "}"
            ".g{"
            "color: " + COLOR_GREEN + ";"
            "}"
            ".gl{"
            "color: " + COLOR_GREEN_LEMON + ";"
            "}"
            ".gll{"
            "color: " + COLOR_GREEN_LEMON_LIGHT + ";"
            "}"
            ".r{"
            "color: " + COLOR_RED + ";"
            "}"
            ".p{"
            "color: " + COLOR_PURPLE + ";"
            "}"
            ".b{"
            "color: " + COLOR_BLUE + ";"
            "}"
            ".bl{"
            "color: " + COLOR_BLUE_LIGHT + ";"
            "}"
            ".o{"
            "color: " + COLOR_ORANGE + ";"
            "}"
            ".od{"
            "color: " + COLOR_ORANGE_DARK + ";"
            "}"
            ".w{"
            "color: " + COLOR_WHITE + ";"
            "}"
            "</style></head><body> %1 </body></html>";
}

FormTankSelector::~FormTankSelector()
{
    delete _ui;
}

void FormTankSelector::show()
{
    _page_index = 1;
    _tank_index = -1;

    updateListView();

    updatePreview();

    _ui->_pb_accept->setEnabled(!ResourceManager::instance()->tankFactories().isEmpty());

    Form::show();
}

bool FormTankSelector::eventFilter(QObject *object, QEvent *event)
{
    if(object == _ui->_l_foreground_skill_1 && event->type() == QEvent::MouseMove)
    {
        if(_tank_index >= 0)
            _ui->_l_information->setText(_html.arg(ResourceManager::instance()->tankFactories().at(_tank_index)->tankInfo()._skill_info_1._details));
        return true;
    }
    else if(object == _ui->_l_foreground_skill_2 && event->type() == QEvent::MouseMove)
    {
        if(_tank_index >= 0)
            _ui->_l_information->setText(_html.arg(ResourceManager::instance()->tankFactories().at(_tank_index)->tankInfo()._skill_info_2._details));
        return true;
    }
    else if(object == _ui->_l_foreground_skill_3 && event->type() == QEvent::MouseMove)
    {
        if(_tank_index >= 0)
            _ui->_l_information->setText(_html.arg(ResourceManager::instance()->tankFactories().at(_tank_index)->tankInfo()._skill_info_3._details));
        return true;
    }
    else if(object == _ui->_l_foreground && event->type() == QEvent::MouseMove)
    {
        if(_tank_index >= 0)
            _ui->_l_information->setText(_html.arg(ResourceManager::instance()->tankFactories().at(_tank_index)->tankInfo()._description));
        return true;
    }
    return false;
}

void FormTankSelector::updateListView()
{
    QList<ITankFactory* > tanks_factories = ResourceManager::instance()->tankFactories();

    foreach(QPushButton* button, _buttons)
        _ui->widget_2->layout()->removeWidget(button);

    qDeleteAll(_buttons);
    _buttons.clear();

    int visible_items = 5;
    int begin = (_page_index - 1) * visible_items;
    int end = _page_index * visible_items;

    _ui->_pb_left->setEnabled(begin >= visible_items);
    _ui->_pb_right->setEnabled(end < tanks_factories.size()+1);

    if(_page_index == 1)
    {
        QPushButton* button = new QPushButton(QIcon(QPixmap(":/icons/random_tank.png")),"");
        button->setIconSize(QSize(36,36));
        button->setMaximumSize(40,40);
        button->setMinimumSize(40,40);
        button->setObjectName(QString("_pb_preview_-1"));
        connect(button, &QPushButton::clicked, this, &FormTankSelector::updatePreview);
        _buttons.append(button);
        _ui->widget_2->layout()->addWidget(button);
        --end;
    }
    else
    {
        --begin;
    }

    for(int i = begin; i < end && i < tanks_factories.size(); ++i)
    {
        ITankFactory* factory = tanks_factories.at(i);
        QPushButton* button = new QPushButton(QIcon(QPixmap(factory->tankInfo()._preview)),"");
        button->setIconSize(QSize(36,36));
        button->setMaximumSize(40,40);
        button->setMinimumSize(40,40);
        button->setObjectName(QString("_pb_preview_%1").arg(i));
        connect(button, &QPushButton::clicked, this, &FormTankSelector::updatePreview);
        _buttons.append(button);
        _ui->widget_2->layout()->addWidget(button);
    }
}

void FormTankSelector::updatePreview()
{
    QPushButton* button = (QPushButton*)sender();
    if(button)
        _tank_index = button->objectName().split("_").at(3).toInt();

    if(_tank_index >= 0)
    {
        QList<ITankFactory* > tanks_factories = ResourceManager::instance()->tankFactories();

        ITankFactory* factory = tanks_factories.at(_tank_index);
        _ui->_l_preview_tank->setPixmap(factory->tankInfo()._preview);
        _ui->_l_name_tank->setText(factory->tankInfo()._name);
        _ui->_l_live->setText(QString("%1+%2").arg(factory->tankInfo()._live).arg(factory->tankInfo()._inc_max_live));
        _ui->_l_mana->setText(QString("%1+%2").arg(factory->tankInfo()._mana).arg(factory->tankInfo()._inc_max_mana));
        _ui->_l_live_regeneration->setText(QString("%1+%2").arg(factory->tankInfo()._live_regeneration).arg(factory->tankInfo()._inc_live_regeneration));
        _ui->_l_mana_regeneration->setText(QString("%1+%2").arg(factory->tankInfo()._mana_regeneration).arg(factory->tankInfo()._inc_mana_regeneration));
        _ui->_l_armor->setText(QString("%1+%2").arg(factory->tankInfo()._armor).arg(factory->tankInfo()._inc_armor));
        _ui->_l_speed->setText(QString("%1+%2").arg(factory->tankInfo()._speed).arg(factory->tankInfo()._inc_speed));

        _ui->_l_preview_skill_1->setPixmap(factory->tankInfo()._skill_info_1._preview);
        _ui->_l_preview_skill_1->setToolTip(factory->tankInfo()._skill_info_1._name);

        _ui->_l_preview_skill_2->setPixmap(factory->tankInfo()._skill_info_2._preview);
        _ui->_l_preview_skill_2->setToolTip(factory->tankInfo()._skill_info_2._name);

        _ui->_l_preview_skill_3->setPixmap(factory->tankInfo()._skill_info_3._preview);
        _ui->_l_preview_skill_3->setToolTip(factory->tankInfo()._skill_info_3._name);

        _ui->_l_information->setText(_html.arg(factory->tankInfo()._description));
    }
    else
    {
        _ui->_l_preview_tank->setPixmap(QPixmap(":/icons/random_tank.png"));
        _ui->_l_name_tank->setText("Aleatorio");
        _ui->_l_live->setText(QString("%1+%2").arg("?").arg("?"));
        _ui->_l_mana->setText(QString("%1+%2").arg("?").arg("?"));
        _ui->_l_live_regeneration->setText(QString("%1+%2").arg("?").arg("?"));
        _ui->_l_mana_regeneration->setText(QString("%1+%2").arg("?").arg("?"));
        _ui->_l_armor->setText(QString("%1+%2").arg("?").arg("?"));
        _ui->_l_speed->setText(QString("%1+%2").arg("?").arg("?"));

        _ui->_l_preview_skill_1->setPixmap(QPixmap(":/icons/random_skill.png"));
        _ui->_l_preview_skill_1->setToolTip("?");

        _ui->_l_preview_skill_2->setPixmap(QPixmap(":/icons/random_skill.png"));
        _ui->_l_preview_skill_2->setToolTip("?");

        _ui->_l_preview_skill_3->setPixmap(QPixmap(":/icons/random_skill.png"));
        _ui->_l_preview_skill_3->setToolTip("?");

        _ui->_l_information->setText(_html.arg("Al comenzar el partido se selecciona un tanque aleatorio."));
    }
}

void FormTankSelector::on__pb_cancel_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_tank_selector->hide();
    forms->_form_created_game->enable();
}

void FormTankSelector::on__pb_accept_clicked()
{
    TcpGameConnectionClient::instance()->sendChangeTank(_tank_index);

    Forms* forms = Forms::instance();

    forms->_form_tank_selector->hide();
    forms->_form_created_game->enable();
}

void FormTankSelector::on__pb_left_clicked()
{
    --_page_index;
    updateListView();
}

void FormTankSelector::on__pb_right_clicked()
{
    ++_page_index;
    updateListView();
}
