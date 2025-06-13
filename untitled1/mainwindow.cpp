#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect button signals to corresponding slots
    connect(ui->addItemButton, &QPushButton::clicked, this, &MainWindow::onAddItemClicked);
    connect(ui->findItemButton, &QPushButton::clicked, this, &MainWindow::onFindItemClicked);
    connect(ui->removeItemButton, &QPushButton::clicked, this, &MainWindow::onRemoveItemClicked);

    // Menu actions
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionAbout, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "About", "Dante Heredia\nCIS17B - Congo GUI Assignment");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Handles adding a new item to the inventory
void MainWindow::onAddItemClicked()
{
    QString id = ui->idInput->text().trimmed();
    QString desc = ui->descInput->text().trimmed();
    QString loc = ui->locInput->text().trimmed();

    if (id.isEmpty() || desc.isEmpty() || loc.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled.");
        return;
    }

    auto item = std::make_shared<StoredItem>(id, desc, loc);

    try {
        manager.addItem(item);
        refreshList();
        ui->idInput->clear();
        ui->descInput->clear();
        ui->locInput->clear();
    } catch (const DuplicateItemException &e) {
        QMessageBox::warning(this, "Duplicate ID", e.what());
    }
}

// Finds and displays an item by ID
void MainWindow::onFindItemClicked()
{
    QString id = ui->findIdInput->text().trimmed();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter an ID to find.");
        return;
    }

    try {
        auto item = manager.findById(id);
        QMessageBox::information(this, "Item Found", item->toString());
    } catch (const ItemNotFoundException &e) {
        QMessageBox::warning(this, "Not Found", e.what());
    }
}

// Removes an item by ID
void MainWindow::onRemoveItemClicked()
{
    QString id = ui->removeIdInput->text().trimmed();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter an ID to remove.");
        return;
    }

    try {
        manager.removeItem(id);
        refreshList();
    } catch (const ItemNotFoundException &e) {
        QMessageBox::warning(this, "Not Found", e.what());
    }
}

// Refreshes the item list display
void MainWindow::refreshList()
{
    ui->itemList->clear();
    auto items = manager.listItemsByDescription();
    for (const auto &item : items) {
        ui->itemList->addItem(item->toString());
    }
}

