#ifndef FORMINITGAME_H
#define FORMINITGAME_H

#include "form.h"

namespace Ui {
    class FormInitGame;
}

class FormInitGame : public Form
{
        Q_OBJECT

    public:
        explicit FormInitGame(QWidget *parent = 0);
        ~FormInitGame();

    public slots:
        void setPixmap(QPixmap pixmap);
        void setText(QString text);

    private:
        Ui::FormInitGame *ui;
};

#endif // FORMINITGAME_H
