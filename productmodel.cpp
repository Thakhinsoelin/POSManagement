#include "productmodel.h"

productModel::productModel(int id, QString name, QByteArray imageBuffer, int price)
{
    setFields(id, name, imageBuffer, price);
}

void productModel::setFields(int id, QString name, QByteArray imageBuffer, int price)
{
    this->id = id;
    this->name = name;
    this->imageBuffer = imageBuffer;
    this->price = price;

    thumbnail.loadFromData(imageBuffer);
}
