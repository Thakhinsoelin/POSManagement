#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H
#include "UserData.h"
#include "productmodel.h"
#include <QDialog>

namespace Ui {
class AddItemDialog;
}

class AddItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemDialog(UserData& data, QWidget *parent = nullptr);
    ~AddItemDialog();

private slots:
    void on_btnSelectImage_clicked();

    void on_addBtn_accepted();

private:
    Ui::AddItemDialog *ui;
    QString selectedImagePath;
    productModel m_model;
    UserData m_data;

    QByteArray tempImageData;
};

#endif // ADDITEMDIALOG_H
