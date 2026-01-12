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

    ui->CurrentItems->setWidgetResizable(true);

    // Initialize the grid layout on the internal container
    if (ui->productsContainer) {
        QGridLayout *gridLayout = new QGridLayout(ui->productsContainer);
        // Align items to the top-left so they don't float if the list is short
        gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        gridLayout->setContentsMargins(10, 10, 10, 10);
        gridLayout->setSpacing(15);
    }
    loadDashboardProducts();
    // ui->stackedWidget->setCurrentIndex(0);

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
    AddItemDialog dialog(this);
    dialog.setWindowTitle("Add a new item");

    if(dialog.exec() == QDialog::Accepted) {
        qDebug() << "User clicked OK\n";
        loadDashboardProducts();
    } else {
        qDebug() << "User cancelled";
    }
}

void MainWindow::loadDashboardProducts()
{
    ui->productsContainer->setUpdatesEnabled(false);
    // Find the layout we created in the constructor
    QGridLayout *layout = qobject_cast<QGridLayout*>(ui->productsContainer->layout());
    if (!layout) return;

    // 1. Clear existing items safely
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }

    // 2. Fetch data from your manager
    productManager manager(this->m_userData);
    QList<productModel> products = manager.getAllProducts();

    // 3. Create and add a ProductCard for each item
    int columnCount = 7;
    for (int i = 0; i < products.size(); ++i) {
        ProductCard *card = new ProductCard(products[i]);

        // It's helpful to set a minimum size for cards to ensure they look uniform
        card->setMinimumSize(200, 250);

        int row = i / columnCount;
        int col = i % columnCount;
        connect(card, &ProductCard::productChanged, this, &MainWindow::loadDashboardProducts);
        layout->addWidget(card, row, col);
    }
    ui->productsContainer->setUpdatesEnabled(true);
}
