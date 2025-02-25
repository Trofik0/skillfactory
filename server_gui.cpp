#include "server_gui.h"

ServerGUI::ServerGUI(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Server Control Panel");
    resize(800, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
 messagesView = new QTableView(this);
    usersView = new QTableView(this);
    banButton = new QPushButton("Ban User", this);
    disconnectButton = new QPushButton("Disconnect User", this);
 
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(messagesView);
    layout->addWidget(usersView);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(banButton);
    buttonLayout->addWidget(disconnectButton);
    layout->addLayout(buttonLayout);

    // Инициализация моделей
    messagesModel = new QStandardItemModel(this);
    usersModel = new QStandardItemModel(this);
    messagesView->setModel(messagesModel);
    usersView->setModel(usersModel);

    connect(banButton, &QPushButton::clicked, this, &ServerGUI::banUser);
    connect(disconnectButton, &QPushButton::clicked, this, &ServerGUI::disconnectUser);

    if (db.connectToDatabase("localhost", "yourdbname", "username", "password")) {
        loadMessages();
        loadUsers();
    } else {
        QMessageBox::critical(this, "Connection Failed", "Could not connect to the database.");
    }
}

void ServerGUI::loadMessages() {
    messagesModel->clear();
    QStringList messages = db.getMessages();
    for (const QString &message : messages) {
        messagesModel->appendRow(new QStandardItem(message));
    }
}

void ServerGUI::loadUsers() {
    usersModel->clear();
    QStringList users = db.getUsers();
    for (const QString &user : users) {
        usersModel->appendRow(new QStandardItem(user));
    }
}

void ServerGUI::banUser() {
    bool ok;
    int userId = QInputDialog::getInt(this, "Ban User", "Enter User ID to ban:", 0, 0, 1000, 1, &ok);
    if (ok) {
        db.banUser(userId);
        QMessageBox::information(this, "Success", "User has been banned.");
        loadUsers(); // Обновляем список пользователей
    }
}

void ServerGUI::disconnectUser() {
    bool ok;
    int userId = QInputDialog::getInt(this, "Disconnect User", "Enter User ID to disconnect:", 0, 0, 1000, 1, &ok);
    if (ok) {
        db.disconnectUser(userId);
        QMessageBox::information(this, "Success", "User has been disconnected.");
        loadUsers(); // Обновляем список пользователей
    }
}
