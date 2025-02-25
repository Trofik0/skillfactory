
#include "database.h"

Database::Database(QObject *parent) : QObject(parent) { }

bool Database::connectToDatabase(const QString &host, const QString &dbName, const QString &user, const QString &password) {
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(password);
    if (!db.open()) {
        qDebug() << "Error: Unable to connect to database." << db.lastError().text();
        return false;
    }
    return true;
}

QStringList Database::getMessages() {
    QStringList messages;
    QSqlQuery query("SELECT content FROM messages");
    while (query.next()) {
        messages << query.value(0).toString();
    }
    return messages;
}

QStringList Database::getUsers() {
    QStringList users;
    QSqlQuery query("SELECT id, username FROM users");
    while (query.next()) {
        users << QString("%1: %2").arg(query.value(0).toString()).arg(query.value(1).toString());
    }
    return users;
}

void Database::banUser(int userId) {
    QSqlQuery query;
    query.prepare("UPDATE users SET banned = TRUE WHERE id = :id");
    query.bindValue(":id", userId);
    if (!query.exec()) {
        qDebug() << "Error banning user:" << query.lastError().text();
    }
}

void Database::disconnectUser(int userId) {
    QSqlQuery query;
    query.prepare("UPDATE users SET online = FALSE WHERE id = :id");
    query.bindValue(":id", userId);
    if (!query.exec()) {
        qDebug() << "Error disconnecting user:" << query.lastError().text();
    }
}
