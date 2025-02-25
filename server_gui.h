#ifndef SERVER_GUI_H
#define SERVER_GUI_H

#include <QMainWindow>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QStandardItemModel>
#include "database.h"

class ServerGUI : public QMainWindow {
    Q_OBJECT

public:
    ServerGUI(QWidget *parent = nullptr);
    void loadMessages();
    void loadUsers();

private slots:
    void banUser();
    void disconnectUser();

private:
    QTableView *messagesView;
    QTableView *usersView;
    Database db;
    QStandardItemModel *messagesModel;
    QStandardItemModel *usersModel;
    QPushButton *banButton;
    QPushButton *disconnectButton;
};

#endif // SERVER_GUI_H
