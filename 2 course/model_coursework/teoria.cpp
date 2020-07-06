#include <QFile>
#include "teoria.h"
#include "loader.h"
#include <QMessageBox>
#include <QDebug>
#include "ui_teoria.h"
#include <string>

Teoria::Teoria(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Teoria)
{
    ui->setupUi(this);
    // -- если файлов нет, то выдаем месседжбокс, иначе читаем файл и грузим в textBrowser -- //
    if (!QFile::exists("files\\text.txt"))
    {
        QMessageBox::warning(this,"Ошибка","Не удалось открыть файл files\\text.txt");
    } else ui->textBrowser->setSource(QUrl("files\\text.txt"));
    ui->textBrowser->setAlignment(Qt::AlignJustify); // -- выравнивание текста по ширине -- //
    ui->textBrowser->setOpenLinks(false); // -- запретить изменения окна с теории при нажатии на ссылки -- //
    fillContentTable();
}

Teoria::~Teoria()
{
    delete ui;
}

void Teoria::on_pushButton_clicked()
{
    emit return_to_menu();
}

void Teoria::fillContentTable()
{
    auto stdstr = loader::getText_from_file("files\\text.txt");
    QString str = QString::fromStdString(stdstr);
    int ix1 = 0,ix2 = 0,ix3 = 0;
    while (ix3 != -1)
    {
        ix1 = str.indexOf("<a id=", ix3);
        ix2 = str.indexOf(">",ix1);
        ix3 = str.indexOf("</a>",ix2);
        QString res;
        if (ix1 != -1 && ix2 != -1 && ix3 != -1)
        {
            for (int i = ix2+1; i < ix3; ++i)
            {
                res.push_back(str[i]);
            }
            ui->listWidget->addItem(res);
        }
    }
}


void Teoria::on_listWidget_clicked(const QModelIndex &index)
{
    int a = index.row()+1;
    ui->textBrowser->scrollToAnchor(std::to_string(a).c_str());
}
