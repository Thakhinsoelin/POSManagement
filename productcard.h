#ifndef PRODUCTCARD_H
#define PRODUCTCARD_H
#include <QFrame>
#include "productmodel.h"

class ProductCard : public QFrame
{
    Q_OBJECT
public:
    void contextMenuEvent(QContextMenuEvent* event) override;
    explicit ProductCard(productModel &product, QWidget *parent = nullptr);

private:
    productModel model;
protected:
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void clicked(const productModel &product);
    void productChanged(); // Signal to notify parent to reload
};

#endif // PRODUCTCARD_H
