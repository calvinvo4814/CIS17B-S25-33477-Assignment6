#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QListWidget>
#include <QString>
#include <QMessageBox>
#include <unordered_map>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "ui_mainwindow.h"

// Custom exception for handling duplicate item insertions
class DuplicateItemException : public std::runtime_error {
public:
    explicit DuplicateItemException(const std::string& msg) : std::runtime_error(msg) {}
};

// Custom exception for handling lookups/removals of missing items
class ItemNotFoundException : public std::runtime_error {
public:
    explicit ItemNotFoundException(const std::string& msg) : std::runtime_error(msg) {}
};

// Represents a stored inventory item
class StoredItem {
private:
    QString id;
    QString description;
    QString location;

public:
    StoredItem(QString id, QString desc, QString loc)
        : id(std::move(id)), description(std::move(desc)), location(std::move(loc)) {}

    QString getId() const { return id; }
    QString getDescription() const { return description; }
    QString getLocation() const { return location; }

    QString toString() const {
        return id + " - " + description + " at " + location;
    }
};

// Manages the storage system backend logic
class StorageManager {
private:
    std::unordered_map<QString, std::shared_ptr<StoredItem>> itemById;           // ID lookup
    std::map<QString, std::shared_ptr<StoredItem>> itemByDescription;            // Sorted by description

public:
    void addItem(const std::shared_ptr<StoredItem>& item);
    std::shared_ptr<StoredItem> findById(const QString& id) const;
    void removeItem(const QString& id);
    std::vector<std::shared_ptr<StoredItem>> listItemsByDescription() const;
};

// Main application window
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddItemClicked();     // Slot for adding a new item
    void onFindItemClicked();    // Slot for finding an item by ID
    void onRemoveItemClicked();  // Slot for removing an item by ID

private:
    StorageManager manager;      // Backend logic
    Ui::MainWindow *ui;          // UI pointer from .ui file

    void refreshList();          // Updates the list view with sorted items
};

#endif // MAINWINDOW_H
