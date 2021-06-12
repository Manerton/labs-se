#ifndef SMSCONFIRMDIALOG_H
#define SMSCONFIRMDIALOG_H

#include <QDialog>

namespace Ui {
    class SmsConfirmDialog;
}

class SmsConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SmsConfirmDialog(QWidget *parent = nullptr);
    ~SmsConfirmDialog();

    bool isConfirmed() const;
    void clear();
private slots:
    void on_buttonBox_accepted();

private:
    Ui::SmsConfirmDialog *ui;

    bool confirmed = false;
};

#endif // SMSCONFIRMDIALOG_H
