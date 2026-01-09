#include "productcard.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>

ProductCard::ProductCard(const productModel &product, QWidget *parent) : QFrame(parent) {
    // 1. Style the rectangle
    this->setFixedSize(200, 250);
    this->setStyleSheet("ProductCard { border: 1px solid #ddd; border-radius: 10px; background: white; } "
                        "ProductCard:hover { background-color: #f9f9f9; border-color: #3498db; }");

    QVBoxLayout *layout = new QVBoxLayout(this);

    // 2. Image (Top)
    QLabel *imgLabel = new QLabel();
    imgLabel->setPixmap(product.getThumbnail().scaled(180, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imgLabel->setAlignment(Qt::AlignCenter);

    // 3. Name & Price (Bottom)
    QLabel *nameLabel = new QLabel(product.getName());
    nameLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    QLabel *priceLabel = new QLabel(QString::number(product.getPrice()) + " MMK");
    priceLabel->setStyleSheet("color: #27ae60; font-size: 13px;");

    layout->addWidget(imgLabel);
    layout->addWidget(nameLabel);
    layout->addWidget(priceLabel);
}
