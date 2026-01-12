#include "databasemanager.h"

DatabaseManager::DatabaseManager() {
    connectToDatabase();
}

bool DatabaseManager::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("hotel_system.db");
    if (!db.open()) {
        qDebug() << "Error: connection with the database failed:" << db.lastError() << "\n";
        return false;
    }
    qDebug() << "Database connected successfully\n";
    return true;
}

void DatabaseManager::setupFirstTime()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "username TEXT UNIQUE, "
               "password TEXT)");

    query.exec("CREATE TABLE IF NOT EXISTS items ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "product_id INTEGER, "
               "name TEXT, "
               "amount INTEGER, "
               "image_data BLOB, "
               "price INTEGER)"
               );

    // Optional: Add a default admin if table is empty
    query.exec("INSERT OR IGNORE INTO users (username, password) VALUES ('admin', 'password123')");
}

void DatabaseManager::saveProduct(const productModel &product)
{
    QSqlQuery query;
    query.prepare("INSERT INTO items (product_id, name, amount, image_data, price) VALUES (:prodid, :name, :amount, :image, :price)");

    query.bindValue(":prodid", product.getId());
    qDebug() << product.getId()<< "\n";
    query.bindValue(":name", product.getName());
    qDebug() << product.getName() << "\n";
    query.bindValue(":amount", product.getAmount());
    query.bindValue(":price", product.getPrice());
    qDebug() << product.getPrice() << "\n";
    // This sends the full, uncompressed binary data to the database
    query.bindValue(":image", product.getImageBuffer());

    if(!query.exec()) {
        qDebug() << "SQL Error: save product" << query.lastError().text();
    }

}

void DatabaseManager::deleteProduct(const productModel& product) {
    QSqlQuery query;
    query.prepare("DELETE FROM items WHERE product_id = :id");
    query.bindValue(":id", product.getId());

    if(!query.exec()) {
        qDebug() << "SQL Error:" << query.lastError().text();
    }
}

void DatabaseManager::updateProduct(const productModel& product, QString currentName) {
    QSqlQuery query;
    query.prepare("UPDATE items SET product_id = :prodid, name = :name, amount = :amount, image_data = :image, price = :price "
                  "WHERE name = :currentName");
    query.bindValue(":prodid", product.getId());
    query.bindValue(":name", product.getName());
    query.bindValue(":amount", product.getAmount());
    query.bindValue(":image", product.getImageBuffer());
    query.bindValue(":price", product.getPrice());
    query.bindValue(":currentName", currentName);
    if(!query.exec()) {
        qDebug() << "SQL Error:" << query.lastError().text();
    } else {
        qDebug() << "SQL update success.";
    }

}
