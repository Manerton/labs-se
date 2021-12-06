#include "Widget.h"
#include "./ui_Widget.h"

#include <cmath>
#include <algorithm>
#include <random>

#include <QFileDialog>
#include <QMessageBox>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

Widget::Text Widget::readFromFile(const QString &filename)
{
    Text buffer;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        buffer = QString::fromUtf8(file.readAll());
    }
    else
    {
        QMessageBox::warning(this, "Ошибка!", "Не удалось открыть файл " + filename);
    }
    file.close();

    return buffer;
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
        file.write(text.toUtf8());
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

Widget::Gamma Widget::generateGamma(const uint16_t Y0, const uint16_t Y1) const
{
    Gamma Y;
    Y[0] = Y0;
    Y[1] = Y1;

    size_t N = Y.size();

    for (size_t i = 2; i < N; ++i)
    {
        Y[i] = (Y[i-1] + Y[i-2]) % m;
    }

    return Y;
}

Widget::Text Widget::crypt(const Text &text) const
{
    mt19937 mt (mt19937::result_type(time(nullptr)));

    Text res;
    res.reserve(text.size());

    uint16_t Y0 = this->initY0;
    uint16_t Y1 = this->initY1;

    size_t i = 0;
    Gamma Y;

    // Проходимся по всему тексту
    for (QChar c : text)
    {
        // Если читаем новый блок текста (из 8 символов)
        if (i == 0)
        {
            // Генерируем новую гамму
            Y = generateGamma(Y0, Y1);
        }

        // Складываем по модулю 2 символ и гамму шифра.
        res.push_back( QChar(c.unicode() ^ Y[i]) );

        // Инкрементируем счётчик (по модулю 8).
        i = (i + 1) % this->blockSize;

        // Если после инкрементирования счетчик равен нулю,
        // значит нужно поместить в Y0 и Y1: 7 и 8 элемент предыдущей гаммы (т.е 2 последних)
        // чтобы сгененрировать новую гамму для нового блока текста.
        if (i == 0)
        {
            Y0 = Y[this->blockSize-2];
            Y1 = Y[this->blockSize-1];
        }
    }

    return res;
}

Widget::Text Widget::decrypt(const Text &text) const
{
    return crypt(text);
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
        this->saveToFile(this->crypt(text), this->outFileName);
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
