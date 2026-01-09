#ifndef PRODUCTCARD_H
#define PRODUCTCARD_H
#include <QFrame>
#include "productmodel.h"

class ProductCard : public QFrame
{
    Q_OBJECT
public:
    explicit ProductCard(const productModel &product, QWidget *parent = nullptr);
};

#endif // PRODUCTCARD_H
