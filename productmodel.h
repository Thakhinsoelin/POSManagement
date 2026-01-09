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
    QPixmap thumbnail;
public:
    productModel(int id, QString name, QByteArray imageBuffer, int price);

    // Getters
    int getId() const { return id; }
    QString getName() const { return name; }
    double getPrice() const { return price; }
    QByteArray getImageBuffer() const {return imageBuffer;}
    QPixmap getThumbnail() const { return thumbnail; }

    // Setters
    void setFields(int id, QString name, QByteArray imageBuffer, int price);
};

#endif // PRODUCTMODEL_H
