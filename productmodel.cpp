#include "productmodel.h"

productModel::productModel(int id, QString name, int amount, QByteArray imageBuffer, int price)
{
    setFields(id, name, amount, imageBuffer, price);
}

void productModel::setFields(int id, QString name, int amount, QByteArray imageBuffer, int price)
{
    this->id = id;
    this->name = name;
    this->imageBuffer = imageBuffer;
    this->price = price;
    this->amount = amount;
    thumbnail.loadFromData(imageBuffer);
}
