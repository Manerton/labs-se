#include <QFile>
#include "teoria.h"
#include "loader.h"
#include <QMessageBox>
#include "ui_teoria.h"

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

    if (!QFile::exists("files\\dictionary.txt"))
    {
        QMessageBox::warning(this,"Ошибка","Не удалось открыть файл files\\dictionary.txt");
    } else ui->dictionary->setSource(QUrl("files\\dictionary.txt"));
    ui->textBrowser->setAlignment(Qt::AlignJustify); // -- выравнивание текста по ширине -- //
    ui->textBrowser->setOpenLinks(false); // -- запретить изменения окна с теории при нажатии на ссылки -- //


}

void Teoria::on_textBrowser_anchorClicked(const QUrl &arg1)
{
    if (arg1.fileName() == "dictionary")
    {
        ui->dictionary->scrollToAnchor(arg1.fragment());    // -- переход к якорю в словаре, при нажатии на ссылку в тексте теории -- //
    }
}

Teoria::~Teoria()
{
    delete ui;
}

void Teoria::on_pushButton_clicked()
{
    emit return_to_menu();
}
