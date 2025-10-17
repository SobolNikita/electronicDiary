#include "profilepage.h"
#include "ui_profilepage.h"

#include <QSettings>
#include <QSqlQuery>

profilePage::profilePage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::profilePage)
{
    ui->setupUi(this);

    QSettings settings("EduDiary", "Login");
    const QString& username = settings.value("username").toString();
    int userId = 0;
    QString fullName = "-";
    QString role = "-";
    QString classId = "-";

    QSqlQuery query;
    query.prepare("SELECT id, role, full_name, class_id FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec()) {
        if (query.next()) {
            userId = query.value("id").toInt();
            fullName = query.value("full_name").toString();
            role = query.value("role").toString();
            if(query.value("class_id").toString().length() > 0)
                classId = query.value("class_id").toString();
        } else {
            qDebug() << "Пользователь не найден";
        }
    } else {
        qDebug() << "Ошибка выполнения запроса";
    }

    ui->lbUserId->setText("Идентификатор: " + QString::number(userId));
    ui->lbUserLogin->setText("Логин: " + username);
    ui->lbUserFullName->setText("Полное имя: " + fullName);
    ui->lbUserRole->setText("Роль: " + role);
    ui->lbUserClassId->setText("Класс: " + classId);
}

profilePage::~profilePage()
{
    delete ui;
}

