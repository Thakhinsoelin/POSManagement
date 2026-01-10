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

    ui->CurrentItems->setWidgetResizable(false);

    if (ui->productsContainer) {
        // Ensure the container is allowed to grow but doesn't force a loop
        ui->productsContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        if (!ui->productsContainer->layout()) {
            QGridLayout *gridLayout = new QGridLayout(ui->productsContainer);
            gridLayout->setAlignment(Qt::AlignTop);
            // gridLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        }
    }

    ui->stackedWidget->setCurrentIndex(0);

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
            qDebug() << "Before loading products";

            loadDashboardProducts();
            qDebug() << "after loading products";

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
    qDebug() << "ENTER loadDashboardProducts";
    QGridLayout *layout = qobject_cast<QGridLayout*>(ui->productsContainer->layout());
    if (!layout) return;
    qDebug() << "Layout OK";
    // Use a temporary list to avoid deleting while iterating if needed
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->hide();
            widget->deleteLater();
        }
        delete item;
    }
     qDebug() << "Cleared layout";

    productManager manager(this->m_userData);
    QList<productModel> products = manager.getAllProducts();
    qDebug() << "Products fetched:" << products.size();
    int columnCount = 3;
    for (int i = 0; i < products.size(); ++i) {
        qDebug() << "Creating card" << i;
        ProductCard *card = new ProductCard(products[i]);
        qDebug() << "Card constructed" << i;
        // Set a minimum size for the card if it doesn't have one
        card->setMinimumSize(200, 150);
        layout->addWidget(card, i / columnCount, i % columnCount);

        qDebug() << "Card added" << i;
    }
    qDebug() << "Exit dashboard";
}
