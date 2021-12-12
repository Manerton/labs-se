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
    , invertedALPHABET(prepareInvertedAlphabet())
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

Widget::CharToIntTable Widget::prepareInvertedAlphabet() const
{
    CharToIntTable result;

    const auto m = ALPHABET.size();
    for (qsizetype i = 0; i < m; ++i)
    {
        result[ALPHABET[i]] = i;
    }

    return result;
}

Widget::Text Widget::readFromFile(const QString &filename)
{
    Text text;

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        text = file.readAll();
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

Widget::Text Widget::crypt(const Text &text)
{
    const auto n = text.size();

    QVector<qsizetype> X(n);

    // Преобразуем исходный текст в множество X (состоящее из чисел).
    transform(text.begin(), text.end(), X.begin(),
                   [this](QChar c) { return invertedALPHABET[c]; });

    // Подготовим генератор случайных чисел.
    random_device rd;
    mt19937 mt(rd());

    const auto m = ALPHABET.size();
    uniform_int_distribution<qsizetype> dist(0, qsizetype(m) - 1);

    // Получим последовательность K (от 0 до n-1) элементы которой,
    // это случайные равномерно распределенные числа от 0 до m-1.
    QVector<qsizetype> K(n);
    generate(K.begin(), K.end(), [&](){  return dist(mt); } );

    // Запоминаем ключ, чтобы потом можно было расшифровать.
    latestKey = K;

    // Выполним подстановку Цезаря.
    QVector<qsizetype> Y(n);
    for (qsizetype i = 0; i < n; ++i)
    {
        Y[i] = (X[i] + K[i]) % m;
    }

    Text res(n, ' ');
    // Преобразуем числа Y в буквы алфавита и получим шифротекст с ключом K.
    transform(Y.begin(), Y.end(), res.begin(),
                   [this](qsizetype j) { return ALPHABET[j]; });

    return res;
}

Widget::Text Widget::decrypt(const Text &text)
{
    const auto n = text.size();

    QVector<qsizetype> Y(n);

    // Преобразуем шифротекст в множество Y (состоящее из чисел).
    transform(text.begin(), text.end(), Y.begin(),
                   [this](QChar c) { return invertedALPHABET[c]; });

    const auto m = ALPHABET.size();
    // Выполним обратную подстановку Цезаря.
    QVector<qsizetype> X(n);
    for (qsizetype i = 0; i < n; ++i)
    {
        X[i] = (Y[i] - latestKey[i]) % m;

        if (X[i] < 0)
        {
            X[i] += m;
        }
    }

    Text res(n, ' ');
    // Преобразуем числа X в буквы алфавита и получим расшифрованный текст.
    transform(X.begin(), X.end(), res.begin(),
                   [this](qsizetype j) { return ALPHABET[j]; });

    return res;
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
