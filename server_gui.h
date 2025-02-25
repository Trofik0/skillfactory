#ifndef CLIENT_GUI_H
#define CLIENT_GUI_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QString>
#include "database.h"

class ClientGUI : public QMainWindow {
    Q_OBJECT

public:
    ClientGUI(QWidget *parent = nullptr);
    
private slots:
    void fetchMessages();
    void fetchUsers();

private:
    Database db;
    QPushButton *fetchMessagesButton;
    QPushButton *fetchUsersButton;
    void displayMessages(const QStringList &messages);
    void displayUsers(const QStringList &users);
};
