#include "productcard.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QMenu>
#include <QContextMenuEvent>
#include <QCursor>
#include <string>
#include "additemdialog.h"
#include "databasemanager.h"
#include <assert.h>
void ProductCard::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    QAction* editAction = menu.addAction("Edit Product");
    QAction* deleteAction = menu.addAction("Delete this product");

    // Add an icon if you have them in your resources
    // editAction->setIcon(QIcon(":/icons/edit.png"));

    // Connect actions to logic
    connect(editAction, &QAction::triggered, this, [=]() {
        qDebug() << "Edit clicked for:" << this->model.getId(); // Or use a stored product ID
        AddItemDialog dialog(this->window(), AddItemDialog::Mode::EditMode);
        dialog.setWindowTitle("Edit item");
        dialog.writeProductID(QString::number(this->model.getId()));
        dialog.writeProductName(this->model.getName());
        dialog.writeProductAmount(QString::number(this->model.getAmount()));

        qDebug() << "Before pixmap writing: " << dialog.getPixmap();
        // qDebug() << "Before pixmap writing and " << dialog.getPixmap();
        dialog.writePixmap(this->model.getImageBuffer());
        qDebug() << "After pixmap writing: " << dialog.getPixmap();
        dialog.writeProductPrice(QString::number(this->model.getPrice()));

        // qDebug() << temp ;

        if(dialog.exec() == QDialog::Accepted) {
            QByteArray temp = dialog.getPixmap();
            if(temp == nullptr) {
                temp = this->model.getImageBuffer();
            }
            qDebug() << "User clicked OK\n";
            DatabaseManager::updateProduct(productModel(
                atoi(dialog.getProductID().toStdString().c_str()),
                dialog.getProductName(),
                atoi(dialog.getProductAmount().toStdString().c_str()),
                temp,
                atoi(dialog.getProductPrice().toStdString().c_str())), QString(this->model.getName()));
            emit productChanged();

        } else {
            qDebug() << "User cancelled";
        }
    });

    connect(deleteAction, &QAction::triggered, this, [=]() {
        DatabaseManager::deleteProduct(this->model);
        qDebug() << "Delete clicked";
        emit productChanged();
    });

    // Execute the menu at the cursor position
    menu.exec(event->globalPos());
};

ProductCard::ProductCard( productModel &product, QWidget *parent) : QFrame(parent),
    model(product)
{

    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    // 1. Style the rectangle
    this->setFixedSize(200, 300);
    this->setStyleSheet("ProductCard { border: 1px solid #ddd; border-radius: 10px; background: black; } "
                        "ProductCard:hover { background-color: #f9f9f9; border-color: #3498db; }");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setObjectName(QString::number(product.getId()));
    // qDebug() << product.getName();
    qDebug() << layout->objectName();
    // 2. Image (Top)
    QLabel *imgLabel = new QLabel();
    imgLabel->setPixmap(product.getThumbnail().scaled(180, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imgLabel->setAlignment(Qt::AlignCenter);

    // 3. Name & Price (Bottom)
    QLabel *nameLabel = new QLabel(product.getName());
    qDebug() << product.getName() ;
    nameLabel->setStyleSheet("color: #27ae60; font-weight: bold; font-size: 14px;");
    nameLabel->setWordWrap(true);
    nameLabel->setAlignment(Qt::AlignLeft);

    QLabel *amountLabel = new QLabel(QString::number(product.getAmount()) + " left");
    amountLabel->setStyleSheet("color: #27ae60; font-size: 13px;");

    QLabel *priceLabel = new QLabel(QString::number(product.getPrice()) + " MMK");
    priceLabel->setStyleSheet("color: #27ae60; font-size: 13px;");

    layout->addWidget(imgLabel);
    layout->addWidget(nameLabel);
    layout->addWidget(amountLabel);
    layout->addWidget(priceLabel);
}

void ProductCard::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        if(event->button() == Qt::LeftButton) {
            emit clicked(this->model);
        }
    }
    QFrame::mousePressEvent(event);
}
