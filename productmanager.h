#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H
#include <QList>
#include <QSqlQuery>
#include <qsqlerror.h>
#include "productmodel.h"
#include "UserData.h"
class productManager
{
    UserData m_userdata;
public:
    productManager(UserData& userdata);
    // Load everything from the database into a list of Product objects
    QList<productModel> getAllProducts() {
        QList<productModel> list;
        QSqlQuery query(QSqlDatabase::database(this->m_userdata.db.databaseName()));
        query.prepare("SELECT id, name, image_data, price FROM items");
        // QSqlQuery query("SELECT id, name, price, image_data FROM items");

        if (!query.exec()) {
            qWarning() << "Query failed:" << query.lastError().text();
            return list;
        }
        while ( query.next()) {
            list.append(productModel(
                query.value(0).toInt(),
                query.value(1).toString(),
                query.value(2).toByteArray(),
                query.value(3).toInt()
                ));
        }
        return list;
    }
};

#endif // PRODUCTMANAGER_H
