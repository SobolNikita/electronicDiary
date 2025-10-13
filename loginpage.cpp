#include "loginpage.h"
#include "ui_loginpage.h"
#include "mainwindow.h"
#include "authmanager.h"

#include <QMessageBox>
#include <QUuid>
#include <QSqlQuery>
#include <QSettings>

LoginPage::LoginPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::on_btnLogin_clicked()
{
    QString username = ui->leEmail->text();
    QString password = ui->lePassword->text();
    bool remember = ui->cbRemember->isChecked();

    if (AuthManager::login(username, password)) {

        QString token = QUuid::createUuid().toString();

        QSqlQuery query;
        query.prepare("UPDATE users SET token = :token WHERE username = :username");
        query.bindValue(":token", token);
        query.bindValue(":username", username);
        query.exec();

        QSettings settings("EduDiary", "Login");
        settings.setValue("username", username);
        settings.setValue("auth_token", token);
        settings.setValue("remember_me", remember);

        MainWindow *mainWin = new MainWindow();
        mainWin->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Ошибка входа", "Неверный логин или пароль");
    }
}

