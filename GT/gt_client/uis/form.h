#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QApplication>

class Form : public QWidget
{
        Q_OBJECT

    public:

        explicit Form(QWidget *parent = 0);

        void closeEvent(QCloseEvent *);

    public slots:

        virtual void disable();

        virtual void enable();

        virtual void show();

        virtual void hide();
};

#endif // FORM_H
