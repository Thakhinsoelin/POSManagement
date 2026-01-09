#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qsqldatabase.h>
#include "UserData.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(UserData &userData, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // void on_pushButton_clicked();

    void on_loginButton_clicked();

    void on_addNewItem_clicked();
    void loadDashboardProducts();
private:
    Ui::MainWindow *ui;
    UserData m_userData;
};
#endif // MAINWINDOW_H
