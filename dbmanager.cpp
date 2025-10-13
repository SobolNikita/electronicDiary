#include "dbmanager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QDir>
#include <QCryptographicHash>

DBManager::DBManager() {}

bool DBManager::initDatabase() {
    QDir dir;
    if (!dir.exists("data")) {
        dir.mkpath("data");
    }


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data/edu_diary.sqlite");

    if (!db.open()) {
        qWarning() << "Не удалось открыть БД:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    QString createUsers = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password_hash TEXT NOT NULL,
            token TEXT,
            full_name TEXT,
            role TEXT NOT NULL,
            class_id INTEGER
        );
    )";

    if (!query.exec(createUsers)) {
        qWarning() << "Ошибка создания таблицы users:" << query.lastError().text();
        return false;
    }

    query.prepare("SELECT COUNT(*) FROM users WHERE username = 'admin'");
    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        if (count == 0) {
            QByteArray passwordHash = QCryptographicHash::hash(QByteArray("admin"), QCryptographicHash::Sha256).toHex();

            QSqlQuery insert;
            insert.prepare(R"(
                INSERT INTO users (username, password_hash, token, full_name, role, class_id)
                VALUES (:username, :password_hash, '', 'Администратор', 'admin', NULL)
            )");
            insert.bindValue(":username", "admin");
            insert.bindValue(":password_hash", passwordHash);

            if (!insert.exec()) {
                qWarning() << "Ошибка добавления пользователя admin:" << insert.lastError().text();
                return false;
            } else {
                qDebug() << "Пользователь admin успешно создан.";
            }
        }
    }

    return true;
}
