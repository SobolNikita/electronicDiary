#include<QString>

#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

class AuthManager
{
public:
    AuthManager();
    static bool validateToken(QString &username, QString &token);
    static bool login(const QString& username, const QString& password);
};

#endif // AUTHMANAGER_H
