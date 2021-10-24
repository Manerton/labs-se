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
    this->prepareKeys();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::prepareKeys()
{
    auto length = ALPHABET.length();
    // первый символ алфавита убираем из списка возможных ключей
    for (qsizetype i = 1; i < length; ++i)
    {
        QChar c = ALPHABET[i];
        ui->comboBox_key->addItem(c);
    }
}

Widget::Text Widget::readFromFile(const QString &filename)
{
    Text text;

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        while (!file.atEnd())
        {
            QString buffer = file.readLine();

            text.push_back(buffer);
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка!", "Не удалось открыть файл " + filename);
    }
    file.close();

    return text;
}

void Widget::saveToFile(const Text &text, const QString &filename)
{
    if (filename.length() <= 0)
    {
        return;
    }

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        for (auto &line : text)
        {
            file.write(line.toUtf8());
            file.write("\n");
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка!", "Не удалось сохранить файл " + filename);
    }
    file.close();
}

void Widget::openFileOnSystem(const QString &filename) const
{

#ifdef WIN32
    std::wstring s = L"start \"\" \"" + filename.toStdWString() + L"\"";
    _wsystem(s.c_str());
#else
    auto s = "open \"" + filename.toStdString() + "\"";
    system(s.c_str());
#endif

}

Widget::CryptTable Widget::getCryptedAlphabet(qsizetype key, bool crypted) const
{
    CryptTable cryptedAlphabet;

    auto alphabet_len = this->ALPHABET.length();
    for (qsizetype i = 0; i < alphabet_len; ++i)
    {
        auto cryptedSymbolKey = key + i;

        if (cryptedSymbolKey >= alphabet_len)
        {
            cryptedSymbolKey -= alphabet_len;
        }

        QChar cryptedSymbol = ALPHABET[cryptedSymbolKey];

        if (crypted)
        {
            cryptedAlphabet[ALPHABET[i]] = cryptedSymbol;
        }
        else
        {
            cryptedAlphabet[cryptedSymbol] = ALPHABET[i];
        }
    }

    return cryptedAlphabet;
}

Widget::Text Widget::crypt(bool crypt, const Text &text) const
{
    QChar keyChar = ui->comboBox_key->currentText().front();
    auto keyIter = std::find(ALPHABET.begin(), ALPHABET.end(), keyChar);
    qsizetype keyIndex = std::distance(ALPHABET.begin(), keyIter);

    auto cryptedAlphabet = getCryptedAlphabet(keyIndex, crypt);

    Text res;
    res.reserve(text.size());

    for (const auto &origLine : text)
    {
        QString newLine;
        newLine.reserve(origLine.length());

        for (QChar c : origLine)
        {
            QChar newChar;
            newChar = cryptedAlphabet[ c.isUpper() ? c : c.toUpper() ];
            newLine.push_back( c.isUpper() ? newChar : newChar.toLower() );
        }

        res.push_back(newLine);
    }

    return res;
}

Widget::Text Widget::decrypt(const Text &text) const
{
    return this->crypt(false, text);
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

void Widget::on_pushButton_crypt_clicked()
{
    const auto text = this->readFromFile(this->origFileName);
    if (text.length())
    {
        this->saveToFile(this->crypt(true, text), this->outFileName);
        this->openFileOnSystem(this->outFileName);
    }
}

void Widget::on_pushButton_decrypt_clicked()
{
    const auto text = this->readFromFile(this->origFileName);
    if (text.length())
    {
        this->saveToFile(this->decrypt(text), this->outFileName);
        this->openFileOnSystem(this->outFileName);
    }
}

void Widget::on_lineEdit_outFilePath_textEdited(const QString &arg1)
{
    this->outFileName = arg1;
}

void Widget::on_lineEdit_origFilePath_textEdited(const QString &arg1)
{
    this->origFileName = arg1;
}
