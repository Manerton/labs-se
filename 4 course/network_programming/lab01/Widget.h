#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkReply>

#include <memory>

class QNetworkAccessManager;
class QHostInfo;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_pushButton_request_clicked();
    void on_pushButton_dnsRequest_clicked();

    /// Считываем данные из HTTP ответа.
    void httpFinished();

    /// Получаем ответ на запрос DNS
    void lookedUpDns(const QHostInfo &host);

private:
    Ui::Widget *ui;
    QNetworkAccessManager *networkManager;
    std::unique_ptr<QNetworkReply> response = nullptr;
};
#endif // WIDGET_H
