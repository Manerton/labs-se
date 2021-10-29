#include "Widget.h"
#include "./ui_Widget.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHostInfo>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_request_clicked()
{
    const QUrl requestUrl = ui->lineEdit_request->text().trimmed();

    // создать объект запроса
    QNetworkRequest request(requestUrl);

    // получить ответ с сервера
    this->response.reset(networkManager->get(request));

    // асинхронно считываем данные из ответа
    connect(response.get(), &QNetworkReply::finished, this, &Widget::httpFinished);
}

void Widget::httpFinished()
{
    // текст из ответа
    auto responseText = this->response->readAll();
    qDebug() << responseText.size();
    ui->plainTextEdit_response->setPlainText(responseText.left(10000));

    // данные из запроса и ответа
    auto request = this->response->request();

    QStringList requestInfo;
    requestInfo.emplace_back("Целевой URL: " + request.url().toString());
    requestInfo.emplace_back("Код ответа: " + response->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
    requestInfo.emplace_back("Заголовки:");

    for (auto &header : response->rawHeaderPairs())
    {
        requestInfo.emplace_back(header.first + "\t" + header.second);
    }

    ui->plainTextEdit_serverInfo->clear();
    ui->plainTextEdit_serverInfo->setPlainText(requestInfo.join("\n"));
}

void Widget::lookedUpDns(const QHostInfo &host)
{
    if (host.error() != QHostInfo::NoError)
    {
        QMessageBox::critical(this, "DNS Lookup failed:", host.errorString());
        return;
    }

    const auto addressList = host.addresses();
    QString addressListString;
    for (auto &address : addressList)
    {
        addressListString.push_back(address.toString() + "\n");
    }
    ui->plainTextEdit_dnsResponse->setPlainText(addressListString);
}


void Widget::on_pushButton_dnsRequest_clicked()
{
    const QString dnsUrl = ui->lineEdit_dnsRequest->text().trimmed();
    ui->plainTextEdit_dnsResponse->clear();
    QHostInfo::lookupHost(dnsUrl, this, [&](const QHostInfo &host){ this->lookedUpDns(host); });
}

