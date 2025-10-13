#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "loginpage.h"

#include <QMenu>
#include <QSettings>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onProfileClicked()
{
    qDebug() << "Профиль открыт";
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
