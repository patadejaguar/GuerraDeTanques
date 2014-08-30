#ifndef FORMTANKSELECTOR_H
#define FORMTANKSELECTOR_H

#include <QPushButton>

#include "form.h"
#include "itank.h"
#include "gameglobals.h"

namespace Ui {
    class FormTankSelector;
}

class FormTankSelector : public Form
{
        Q_OBJECT

    public:
        explicit                FormTankSelector(QWidget *parent = 0);

        ~FormTankSelector();

    public slots:

        void                    show();

    protected:

        bool                    eventFilter(QObject *object, QEvent *event);

    protected slots:

        void                    updateListView();

        void                    updatePreview();

    private slots:

        void                    on__pb_cancel_clicked();

        void                    on__pb_accept_clicked();

        void                    on__pb_left_clicked();

        void                    on__pb_right_clicked();

    private:
        Ui::FormTankSelector *  _ui;

        QString                 _html;

        int                     _page_index;

        int                     _tank_index;

        QList<QPushButton*>     _buttons;
};

#endif // FORMTANKSELECTOR_H
