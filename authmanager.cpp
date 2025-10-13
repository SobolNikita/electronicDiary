#include "authmanager.h"

#include <QSqlQuery>
#include <QCryptographicHash>

AuthManager::AuthManager() {}


bool AuthManager::validateToken(QString &username, QString &token){
    QSqlQuery query;
    query.prepare("SELECT token FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        QString storedToken = query.value(0).toString();
        return storedToken == token;
    }

    return false;
}

bool AuthManager::login(const QString& username, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT password_hash FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        return false;
    }

    QString storedHash = query.value(0).toString();

    QByteArray inputHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    return storedHash == inputHash;
}
