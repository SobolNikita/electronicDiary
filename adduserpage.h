#ifndef ADDUSERPAGE_H
#define ADDUSERPAGE_H

#include <QDialog>

namespace Ui {
class addUserPage;
}

class addUserPage : public QDialog
{
    Q_OBJECT

public:
    explicit addUserPage(QWidget *parent = nullptr);
    ~addUserPage();

private slots:
    void onRadioButtonToggled(bool checked);

    void on_pbCancelUser_clicked();

    void on_pbAddUser_clicked();

private:
    Ui::addUserPage *ui;
};

#endif // ADDUSERPAGE_H
