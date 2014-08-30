#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "xav_database_engine_global.h"

class XAV_DATABASE_ENGINESHARED_EXPORT DataBaseConnection: public QObject
{
        Q_OBJECT

    public:
        DataBaseConnection(QString data_base_name, QString data_base_type);

        QString         getDataBaseName() const;
        void            setDataBaseName(const QString &data_base_name);

        QString         getDataBaseType() const;
        void            setDataBaseType(const QString &data_base_type);

        bool            executeQuery(QString string_query);
        QSqlDatabase    getDataBase();

    public slots:
        bool            open();
        void            closeConnection();

    signals:
        void            error(QString error);
        void            connected();
        void            disconnected();

    protected:
        QString         _data_base_name;
        QString         _data_base_type;
        QSqlDatabase    _data_base;
};

#endif // DATABASECONNECTION_H
