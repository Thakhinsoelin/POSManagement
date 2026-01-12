#include "additemdialog.h"
#include "ui_additemdialog.h"
#include "productmodel.h"
#include <QFileDialog>
#include <math.h>
#include <QSqlQuery>
#include "databasemanager.h"

AddItemDialog::AddItemDialog( QWidget *parent, Mode mode)
    : QDialog(parent)
    , currentMode(mode)
    , m_model(0, "", 0, nullptr, 0)
    , ui(new Ui::AddItemDialog)
{
    ui->setupUi(this);
}

void AddItemDialog::writeProductID(QString productID)
{
    this->ui->productID->setText(productID);
}

void AddItemDialog::writeProductPrice(QString price)
{
    this->ui->productPrice->setText(price);
}

void AddItemDialog::writeProductName(QString name)
{
    this->ui->productName->setText(name);
}

void AddItemDialog::writePixmap(QByteArray array)
{
    QPixmap pixmap;
    if(pixmap.loadFromData(array)) {
        int w = 120;
        int h = 120;
        this->ui->lblImagePreview->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));
    } else {
        qDebug() << "Pixmap conversion failed";
    }

}

void AddItemDialog::writeProductAmount(QString amount)
{
    this->ui->productAmount->setText(amount);
}

QString AddItemDialog::getProductID()
{
    return this->ui->productID->text();
}

QString AddItemDialog::getProductPrice()
{
    return this->ui->productPrice->text();
}

QString AddItemDialog::getProductName()
{
    return this->ui->productName->text();
}

QString AddItemDialog::getProductAmount()
{
    return this->ui->productAmount->text();
}

QByteArray AddItemDialog::getPixmap()
{
    return this->tempImageData;
}

AddItemDialog::~AddItemDialog()
{
    delete ui;
}

void AddItemDialog::on_btnSelectImage_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Select Image"), "",
                                                    tr("Images (*.png *.jpg *.jpeg *.bmp *.avif)"));
    if(!filename.isEmpty()) {
        selectedImagePath = filename;
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly)) {
            this->tempImageData = file.readAll();
            file.close();

            QPixmap pix(filename);

            int w = 120;//ui->lblImagePreview->width();
            int h = 120;//ui->lblImagePreview->height();
            ui->lblImagePreview->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        }

    } else {
        this->tempImageData = nullptr;
    }
}


void AddItemDialog::on_addBtn_accepted()
{
    if(currentMode == AddMode) {
        int id = atoi( ui->productID->text().toStdString().c_str());
        QString name = ui->productName->text();
        int price = atoi(ui->productPrice->text().toStdString().c_str());
        int amount = atoi(ui->productAmount->text().toStdString().c_str());
        this->m_model.setFields(id, name, amount, tempImageData, price);

        DatabaseManager::saveProduct(this->m_model);
    }
}

