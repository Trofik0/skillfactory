#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QStringList>

class Database : public QObject {
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    bool connectToDatabase(const QString &host, const QString &dbName, const QString &user, const QString &password);
    QStringList getMessages(); // Получить сообщения
    QStringList getUsers();    // Получить пользователей
    void banUser(int userId);
    void disconnectUser(int userId);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
