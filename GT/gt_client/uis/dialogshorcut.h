#ifndef DIALOGSHORCUT_H
#define DIALOGSHORCUT_H

#include <QDialog>
#include <QKeyEvent>
#include <QMouseEvent>

namespace Ui {
    class DialogShorcut;
}

class DialogShorcut : public QDialog
{
        Q_OBJECT

    public:

        explicit DialogShorcut(QWidget *parent = 0);
        ~DialogShorcut();

    signals:

        void                notifyKey(int);

    protected:

        void                keyPressEvent(QKeyEvent *event);
        void                mousePressEvent(QMouseEvent* event);

    private:
        Ui::DialogShorcut *ui;
};

#endif // DIALOGSHORCUT_H
