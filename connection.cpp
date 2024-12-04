#include "connection.h"
#include <QSqlDatabase>
#include <cstddef>

Connection::Connection(){}

bool Connection::createconnection()
{
    db=QSqlDatabase::addDatabase("QODBC");
    bool test=false;
    db.setDatabaseName("CPP_Projet");
    db.setUserName("selmi");
    db.setPassword("oracle");

    if(db.open()) test=true;
    return test;



}

void Connection::closeConnection(){db.close();}
