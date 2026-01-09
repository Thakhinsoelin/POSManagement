#include "additemdialog.h"
#include "ui_additemdialog.h"
#include "productmodel.h"
#include <QFileDialog>
#include <math.h>
#include <QSqlQuery>
#include "databasemanager.h"

AddItemDialog::AddItemDialog(UserData& data, QWidget *parent)
    : QDialog(parent)
    , m_data(data)
    , m_model(0, "", nullptr, 0)
    , ui(new Ui::AddItemDialog)
{
    ui->setupUi(this);
}

AddItemDialog::~AddItemDialog()
{
    delete ui;
}

void AddItemDialog::on_btnSelectImage_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Select Image"), "",
                                                    tr("Images (*.png *.jpg *.jpeg *.bmp)"));
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

    }
}


void AddItemDialog::on_addBtn_accepted()
{
    int id = atoi( ui->productID->text().toStdString().c_str());
    QString name = ui->productName->text();
    int price = atoi(ui->productPrice->text().toStdString().c_str());
    this->m_model.setFields(id, name, tempImageData, price);

    DatabaseManager::saveProduct(this->m_model);
}

