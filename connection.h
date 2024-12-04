#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <cstddef>

class Connection
{
    QSqlDatabase db;

public :
    Connection();
    bool createconnection();
    void closeConnection();
};

#endif // CONNECTION_H
