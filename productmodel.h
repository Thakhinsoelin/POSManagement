#ifndef PRODUCTMODEL_H
#define PRODUCTMODEL_H
#include <QString>
#include <QByteArray>
#include <QPixmap>

class productModel
{
    int id;

    QString name;
    QByteArray imageBuffer;
    int price;
    int amount;
    QPixmap thumbnail;
public:
    int db_id;
    productModel(int id, QString name, int amount, QByteArray imageBuffer, int price);

    // Getters
    int getId() const { return id; }
    int getAmount() const {return amount;}
    QString getName() const { return name; }
    double getPrice() const { return price; }
    QByteArray getImageBuffer() const {return imageBuffer;}
    QPixmap getThumbnail() const { return thumbnail; }

    // Setters
    void setFields(int id, QString name, int amount, QByteArray imageBuffer, int price);
};

#endif // PRODUCTMODEL_H
