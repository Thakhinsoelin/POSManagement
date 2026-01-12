#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H
#include "productmodel.h"
#include <QDialog>

namespace Ui {
class AddItemDialog;
}

class AddItemDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode { AddMode, EditMode };
    explicit AddItemDialog(QWidget *parent = nullptr, Mode mode = AddMode);
    void writeProductID(QString productID);
    void writeProductPrice(QString price);
    void writeProductName(QString name);
    void writePixmap(QByteArray array);
    void writeProductAmount(QString amount);

    QString getProductID();
    QString getProductPrice();
    QString getProductName();
    QString getProductAmount();
    QByteArray getPixmap();

    ~AddItemDialog();

private slots:
    void on_btnSelectImage_clicked();

    void on_addBtn_accepted();

private:
    Ui::AddItemDialog *ui;
    QString selectedImagePath;
    productModel m_model;
    Mode currentMode;
    // UserData m_data;

    QByteArray tempImageData;
};

#endif // ADDITEMDIALOG_H
