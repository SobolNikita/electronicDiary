#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QDialog>

namespace Ui {
class profilePage;
}

class profilePage : public QDialog
{
    Q_OBJECT

public:
    explicit profilePage(QWidget *parent = nullptr);
    ~profilePage();

private:
    Ui::profilePage *ui;
};

#endif // PROFILEPAGE_H
