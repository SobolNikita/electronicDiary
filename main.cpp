#include "mainwindow.h"
#include "loginpage.h"

#include "authmanager.h"
#include "dbmanager.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!DBManager::initDatabase()){
        return 1;
    }

    MainWindow w;
    LoginPage l;

    QSettings settings("EduDiary", "Login");
    bool remember = settings.value("remember_me", false).toBool();

    if (remember) {
        QString username = settings.value("username").toString();
        QString token = settings.value("auth_token").toString();

        if (AuthManager::validateToken(username, token)) {
            w.show();
        } else {
            l.show();
        }
    } else {
        l.show();
    }

    return a.exec();
}
