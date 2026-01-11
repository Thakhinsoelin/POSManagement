#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "productmodel.h"

class DatabaseManager
{
public:
    DatabaseManager();
    bool connectToDatabase();

    //This is here for sole purpose of testing
    void setupFirstTime();

    QSqlDatabase* getDB() {
        return &db;
    }
    static void saveProduct(const productModel &product);
    static void deleteProduct(const productModel &product);
    static void updateProduct(const productModel& product, QString currentName);
private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
