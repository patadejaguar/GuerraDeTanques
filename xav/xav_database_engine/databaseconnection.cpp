#include "databaseconnection.h"

DataBaseConnection::DataBaseConnection(QString data_base_name, QString data_base_type)
{
    _data_base_name = data_base_name;
    _data_base_type = data_base_type;
}

bool DataBaseConnection::open()
{
    _data_base = QSqlDatabase::addDatabase(_data_base_type);
    if(!_data_base.isValid())
    {
        emit error(QString("El tipo \"%1\" no está disponible").arg(_data_base_type));
        return false;
    }
    _data_base.setDatabaseName(_data_base_name);
    if(!_data_base.open())
    {
        emit error(QString("No es posible abrir la base de datos \"%1\"").arg(_data_base_name));
        return false;
    }
    emit connected();
    return true;
}

void DataBaseConnection::closeConnection()
{
    _data_base.close();
    emit disconnected();
}

QString DataBaseConnection::getDataBaseType() const
{
    return _data_base_type;
}

void DataBaseConnection::setDataBaseType(const QString &data_base_type)
{
    _data_base_type = data_base_type;
}

bool DataBaseConnection::executeQuery(QString string_query)
{
    QSqlQuery query(_data_base);
    return query.exec(string_query);
}

QSqlDatabase DataBaseConnection::getDataBase()
{
    return _data_base;
}

QString DataBaseConnection::getDataBaseName() const
{
    return _data_base_name;
}

void DataBaseConnection::setDataBaseName(const QString &data_base_name)
{
    _data_base_name = data_base_name;
}

