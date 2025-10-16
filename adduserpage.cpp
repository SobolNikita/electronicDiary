#include "adduserpage.h"
#include "ui_adduserpage.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QCryptographicHash>

addUserPage::addUserPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addUserPage)
{
    ui->setupUi(this);
    connect(ui->rbAdmin, &QRadioButton::toggled, this, &addUserPage::onRadioButtonToggled);
    connect(ui->rbTeatcher, &QRadioButton::toggled, this, &addUserPage::onRadioButtonToggled);
    connect(ui->rbPupil, &QRadioButton::toggled, this, &addUserPage::onRadioButtonToggled);

    ui->rbAdmin->setChecked(true);

}

addUserPage::~addUserPage()
{
    delete ui;
}

void addUserPage::onRadioButtonToggled(bool checked)
{
    QRadioButton* senderButton = qobject_cast<QRadioButton*>(sender());
    if (checked && senderButton) {
        if(!ui->rbPupil->isChecked()){
            ui->leClassId->setDisabled(true);
            ui->leClassId->clear();
            ui->leClassId->setPlaceholderText("Задаётся только для ученика");
        }else{
            ui->leClassId->setDisabled(false);
            ui->leClassId->setPlaceholderText("");
        }
    }
}




void addUserPage::on_pbCancelUser_clicked()
{
    this->close();
    delete ui;
}


void addUserPage::on_pbAddUser_clicked()
{
    bool allCorrect = ui->leLogin->text().length() > 0;
    allCorrect &= ui->leFullName->text().length() > 0;
    allCorrect &= ui->lePassword->text().length() > 0;
    allCorrect &= !ui->rbPupil->isChecked() || ui->leClassId->text().length() > 0;

    if(allCorrect){
       // check user exists
        QSqlQuery query;

        const QString& username = ui->leLogin->text();

        query.prepare("SELECT role FROM users WHERE username = :username");
        query.bindValue(":username", username);

        if (query.exec()) {
            if (query.next()) {
                QMessageBox::warning(this, "Ошибка создания", "Логин уже занят");
                allCorrect = false;
            }
        } else {
            qDebug() << "Ошибка выполнения запроса";
            allCorrect = false;
        }

    }

    if(allCorrect){
        QByteArray passwordHash = QCryptographicHash::hash(ui->lePassword->text().toUtf8(), QCryptographicHash::Sha256).toHex();

        QSqlQuery insert;
        insert.prepare(R"(
            INSERT INTO users (username, password_hash, token, full_name, role, class_id)
            VALUES (:username, :password_hash, '', :full_name, :role, :class_id)
        )");
        QString role = "";
        if(ui->rbAdmin->isChecked())role = "admin";
        else if(ui->rbTeatcher->isChecked())role = "teatcher";
        else if(ui->rbPupil->isChecked()) role = "pupil";
        insert.bindValue(":username", ui->leLogin->text());
        insert.bindValue(":password_hash", passwordHash);
        insert.bindValue(":full_name", ui->leFullName->text());
        insert.bindValue(":role", role);
        insert.bindValue(":class_id", ui->leClassId->text());

        if (!insert.exec()) {
            qWarning() << "Ошибка добавления пользователя";
            return;
        } else {
            qDebug() << "Пользователь admin успешно создан.";
        }
        QMessageBox::information(this, "Успешно!", "Пользователь создан!");
    }else{
        QMessageBox::warning(this, "Ошибка создания", "Заполните все поля!");
    }
    on_pbCancelUser_clicked();
}

