#include "fadelabeleffect.h"

FadeLabelEffect::FadeLabelEffect(QString text, QList<FadeLabelEffect *> *list, QWidget *parent) :
    QLabel(text, parent)
{
    _opacity = 255;

    setFont(QFont("Revue BT",8));
    setStyleSheet("background-color: rgba(0, 0, 0, 200);"
                  "border:1px solid;"
                  "border-radius:4px;"
                  "padding-left:2px;");

    _animation = new QPropertyAnimation(this, "_opacity");
    _animation->setStartValue(255);
    _animation->setEndValue(0);
    _animation->setDuration(300);

    setWordWrap(true);

    QTimer::singleShot(7000, _animation, SLOT(start()));

    connect(_animation, &QPropertyAnimation::finished, this, &FadeLabelEffect::deleteLater);
    _list = list;
    _list->append(this);
}

FadeLabelEffect::~FadeLabelEffect()
{
    delete _animation;
    _list->removeAll(this);
}

int FadeLabelEffect::opacity() const
{
    return _opacity;
}

void FadeLabelEffect::setOpacity(int opacity)
{
    if(opacity < 200 && opacity < _opacity)
        setStyleSheet(QString("background-color: rgba(0, 0, 0, %1);"
                      "border:1px solid;"
                      "border-color:rgba(0,0,0,%1);"
                      "border-radius:4px;"
                      "padding-left:2px;").arg(opacity));

    QString old_alpha = QString("#%1").arg(QString::number(_opacity,16));
    if(old_alpha.size()==2)
        old_alpha.insert(1,'0');

    _opacity = opacity;

    QString new_alpha = QString("#%1").arg(QString::number(_opacity,16));
    if(new_alpha.size()==2)
        new_alpha.insert(1,'0');

    setText(text().replace(old_alpha, new_alpha));
}

