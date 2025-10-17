#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "addUserPage.h"
#include "loginpage.h"
#include "profilepage.h"

#include <QMenu>
#include <QSettings>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);



    QToolButton *btnAccount = ui->btnAccount;

    QMenu *menu = new QMenu(btnAccount);
    menu->addAction("Профиль", this, SLOT(onProfileClicked()));
    menu->addAction("Настройки", this, SLOT(onSettingsClicked()));
    menu->addSeparator();
    menu->addAction("Выйти", this, SLOT(onLogoutClicked()));

    btnAccount->setMenu(menu);

    QSqlQuery query;
    QSettings settings("EduDiary", "Login");

    const QString& username = settings.value("username").toString();

    query.prepare("SELECT role FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec()) {
        if (query.next()) {
            QString role = query.value(0).toString();
            if (role != "admin"){
                QWidget *button = ui->pbAddUser;
                QLayout *layout = ui->widget->layout();
                if (layout && button) {
                    layout->removeWidget(button);
                    delete button;
                    ui->pbAddUser = nullptr;
                }

            }
        } else {
            qDebug() << "Пользователь не найден";
        }
    } else {
        qDebug() << "Ошибка выполнения запроса";
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onProfileClicked()
{
    profilePage *pp = new profilePage(this);
    pp->exec();
}

void MainWindow::onSettingsClicked()
{
    qDebug() << "Настройки открыты";
}

void MainWindow::onLogoutClicked()
{
    QSettings settings("EduDiary", "Login");
    settings.remove("auth_token");
    settings.setValue("remember_me", false);

    LoginPage *login = new LoginPage();
    login->show();

    this->close();
}

void MainWindow::on_pbAddUser_clicked()
{
    addUserPage *up = new addUserPage(this);
    up->exec();
}

