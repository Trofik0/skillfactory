#include "client_gui.h"
#include <QApplication>
#include <QTextEdit>

ClientGUI::ClientGUI(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Client Application");
    resize(400, 300);
    
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    fetchMessagesButton = new QPushButton("Fetch Messages", this);
    fetchUsersButton = new QPushButton("Fetch Users", this);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(fetchMessagesButton);
    layout->addWidget(fetchUsersButton);

    connect(fetchMessagesButton, &QPushButton::clicked, this, &ClientGUI::fetchMessages);
    connect(fetchUsersButton, &QPushButton::clicked, this, &ClientGUI::fetchUsers);

    // Инициализируем соединение с базой
    if (!db.connectToDatabase("localhost", "yourdbname", "username", "password")) {
        QMessageBox::critical(this, "Connection Failed", "Could not connect to the database.");
    }
}

void ClientGUI::fetchMessages() {
    QStringList messages = db.getMessages();
    displayMessages(messages);
}

void ClientGUI::fetchUsers() {
    QStringList users = db.getUsers();
    displayUsers(users);
}

void ClientGUI::displayMessages(const QStringList &messages) {
    QTextEdit *textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(messages.join("\n"));
    textEdit->setWindowTitle("Messages");
    textEdit->show();
}

void ClientGUI::displayUsers(const QStringList &users) {
    QTextEdit *textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(users.join("\n"));
    textEdit->setWindowTitle("Users");
    textEdit->show();
}
