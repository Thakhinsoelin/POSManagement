#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "additemdialog.h"
#include "productmanager.h"
#include "productmodel.h"
#include "productcard.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QMessageBox>

QString readFile(const QString &fileName) {
    QFile file(fileName);

    // Attempt to open the file in ReadOnly and Text mode
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for reading:" << fileName;
        return QString();
    }

    // Read everything and convert to QString
    QString content = file.readAll();

    file.close();
    return content;
}

MainWindow::MainWindow(UserData &userData, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->m_userData = userData;
    QFile file(":/style.qss");
    if(file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        QString style = in.readAll();

        this->setStyleSheet(style);
        file.close();
    } else {
        qInfo() << "Could not open file. " << "\n";
    }
    ui->stackedWidget->setCurrentIndex(0);

    QWidget *container = ui->CurrentItems->widget();
    QGridLayout *gridLayout = new QGridLayout(container);
    container->setLayout(gridLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    QString user = ui->nameInput->text();
    QString pass = ui->passInput->text();
    QSqlQuery query(QSqlDatabase::database(this->m_userData.db.databaseName()));
    query.prepare("SELECT password FROM users WHERE username = :user");
    query.bindValue(":user", user);
    if (query.exec() && query.next()) {
        QString dbPassword = query.value(0).toString();
        qInfo() << dbPassword << "\n";
        qInfo() << pass << "\n";
        if (dbPassword == pass) {
            // QMessageBox::warning(this, "Login success", "nigger"); // Login success!
            ui->stackedWidget->setCurrentIndex(1);
            loadDashboardProducts();
        } else {
            QMessageBox::warning(this, "Error", "Incorrect password.");
        }
    } else {
        QMessageBox::warning(this, "Error", "User not found.");
    }
}


void MainWindow::on_addNewItem_clicked()
{
    AddItemDialog dialog(this->m_userData,this);
    dialog.setWindowTitle("Add a new item");

    if(dialog.exec() == QDialog::Accepted) {
        qDebug() << "User clicked OK\n";
    } else {
        qDebug() << "User cancelled";
    }
}

void MainWindow::loadDashboardProducts()
{
    QWidget *container = ui->CurrentItems->widget();

    // Use a FlowLayout (or a Grid)
    // If you don't have a FlowLayout class, a QGridLayout works well:
    QGridLayout *gridLayout = new QGridLayout(container);
    container->setLayout(gridLayout);

    // 1. Clear existing items
    QLayoutItem *item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // 2. Fetch from database
    productManager manager(this->m_userData);
    QList<productModel> products = manager.getAllProducts();

    int columnCount = 3; // How many rectangles per row
    for (int i = 0; i < products.size(); ++i) {
        ProductCard *card = new ProductCard(products[i]);

        int row = i / columnCount;
        int col = i % columnCount;
        gridLayout->addWidget(card, row, col);
    }

    // 3. Add a spacer at the bottom so cards don't stretch weirdly
    gridLayout->setRowStretch(gridLayout->rowCount(), 1);
}

