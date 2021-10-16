#include "Widget.h"
#include "./ui_Widget.h"

#include <cmath>
#include <algorithm>

#include <QFileDialog>
#include <QMessageBox>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , keyBar(new QTabBar())
{
    ui->setupUi(this);
    this->prepareKeyBar(this->countInGroup);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::prepareKeyBar(int count)
{
    for (int i = 0; i < count; ++i)
    {
        this->keyBar->addTab(QString::number(i+1));
        this->keyBar->setTabData(i, i);
    }
    keyBar->setMovable(true);
    keyBar->setDrawBase(false);
    ui->horizontalLayout_keys->addWidget(keyBar);
}

QVector<QString> Widget::readSplittedStrFromFile(const QString &filename)
{
    QVector<QString> resStr;

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QString buffer = file.readAll();

        // делим входную последовательность на группы по countInGroup символов
        const qsizetype N = buffer.length();
        for (qsizetype i = 0; i < N; i += this->countInGroup)
        {
            resStr.emplace_back(buffer.mid(i, this->countInGroup));
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка!", "Не удалось открыть файл " + filename);
    }
    file.close();

    return resStr;
}

void Widget::on_pushButton_openOrigFile_clicked()
{
    this->origFileName = QFileDialog::getOpenFileName(this, "Открыть файл", "");

    if (this->origFileName.length() > 0)
    {
        ui->lineEdit_origFilePath->setText(this->origFileName);
    }
}

void Widget::on_pushButton_saveOutFile_clicked()
{
    this->outFileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "");

    if (this->outFileName.length() > 0)
    {
        // указываем путь
        ui->lineEdit_outFilePath->setText(this->outFileName);
    }
}

void Widget::saveToFile(const QString &outStr, const QString &filename)
{
    if (filename.length() <= 0)
    {
        return;
    }

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(outStr.toUtf8());
    }
    else
    {
        QMessageBox::warning(this, "Ошибка!", "Не удалось сохранить файл " + filename);
    }
    file.close();
}

void Widget::openFile(const QString &filename)
{

#ifdef WIN32
    std::wstring s = L"start \"\" \"" + filename.toStdWString() + L"\"";
    _wsystem(s.c_str());
#else
    auto s = "open \"" + filename.toStdString() + "\"";
    system(s.c_str());
#endif

}

QString Widget::crypt(bool crypt, const QVector<QString> &splittedStr)
{
    QString outStr = "";
    outStr.reserve(splittedStr.size() * this->countInGroup);

    /// массив с индексами для перестановки элементов
    std::array<int, countInGroup> indexArr {};

    // вытягиваем значения из keyBar
    for (int i = 0; i < this->countInGroup; ++i)
    {
        indexArr[size_t(i)] = this->keyBar->tabData(int(i)).value<int>();
    }

    // для каждой группы
    for (auto &str : splittedStr)
    {
        const qsizetype N = str.length();

        const auto toOutStr = [&](const std::array<int, countInGroup>& arr)
        {
            QString buffer(N, ' ');

            for (qsizetype i = 0; i < N; ++i)
            {
                if (crypt)
                {
                    buffer[i] = str[arr[size_t(i)]];
                }
                else
                {
                    buffer[arr[size_t(i)]] = str[i];
                }
            }

            outStr.append(buffer);
        };

        // если в группе символов их меньше, чем countInGroup (4)
        // удаляем из массива с индексами для перестановки лишние ключи
        if (N != this->countInGroup)
        {
            auto copyIndexArr = indexArr;
            std::remove_if( copyIndexArr.begin(), copyIndexArr.end(), [&](int j){ return j >= N; } );

            toOutStr(copyIndexArr);
        }
        else
        {
            toOutStr(indexArr);
        }
    }
    return outStr;
}

QString Widget::decrypt(const QVector<QString> &splittedStr)
{
    return this->crypt(false, splittedStr);
}

void Widget::on_pushButton_crypt_clicked()
{
    const auto splittedStr = this->readSplittedStrFromFile(this->origFileName);
    this->saveToFile(this->crypt(true, splittedStr), this->outFileName);
    this->openFile(this->outFileName);
}

void Widget::on_pushButton_decrypt_clicked()
{
    const auto splittedStr = this->readSplittedStrFromFile(this->origFileName);
    this->saveToFile(this->decrypt(splittedStr), this->outFileName);
    this->openFile(this->outFileName);
}

void Widget::on_lineEdit_outFilePath_textEdited(const QString &arg1)
{
    this->outFileName = arg1;
}

void Widget::on_lineEdit_origFilePath_textEdited(const QString &arg1)
{
    this->origFileName = arg1;
}

