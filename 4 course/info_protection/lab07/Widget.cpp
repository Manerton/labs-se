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
    this->prepareProbabilityTable();
    this->prepareKeys();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::prepareKeys()
{
    // Генерируем массив из 1000 чисел.
    const uint16_t numberSize = 1000;
    vector<uint16_t> numbers(numberSize);
    iota(numbers.begin(), numbers.end(), 0);

    // Перемешиваем его.
    mt19937 mt (mt19937::result_type(time(nullptr)));
    shuffle(numbers.begin(), numbers.end(), mt);

    // Настраиваем таблицу.
    ui->tableWidget->horizontalHeader()->setMinimumSectionSize(headerSectionWidth);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(headerSectionWidth);
    ui->tableWidget->setRowCount(maxRowCount);

    auto numberIt = numbers.begin();

    // Для каждой буквы алфавита сгенерируем числа.
    auto columnCount = ALPHABET.length();
    for (int i = 0; i < columnCount; ++i)
    {
        QChar c = ALPHABET[i];
        ui->tableWidget->insertColumn(i);
        ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString(c)));

        // Список строк в соответствии с вероятностью появления символа c.
        auto rowCount = probabilityTable[c];

        for (int j = 0; j < rowCount; ++j)
        {
            // Сохраняем омофон в таблицу.
            auto homophone = QString::number(*numberIt++).rightJustified(3, '0');
            ui->tableWidget->setItem(j, i, new QTableWidgetItem(homophone));

            // Сохраним в специальных контейнерах соответствие между буквой и омофоном.
            // Омофоны по букве.
            homophonesBySymbol[c].push_back(homophone);
            // И буква по омофону.
            symbolByHomophone[homophone] = c;
        }
    }
}

void Widget::prepareProbabilityTable()
{
    probabilityTable[' '] = 175;
    probabilityTable[L'О'] = 90;
    probabilityTable[L'Е'] = 72;
    probabilityTable[L'А'] = 62;
    probabilityTable[L'И'] = 62;
    probabilityTable[L'Н'] = 53;
    probabilityTable[L'Т'] = 53;
    probabilityTable[L'С'] = 45;
    probabilityTable[L'Р'] = 40;
    probabilityTable[L'В'] = 38;
    probabilityTable[L'Л'] = 35;
    probabilityTable[L'К'] = 28;
    probabilityTable[L'М'] = 26;
    probabilityTable[L'Д'] = 25;
    probabilityTable[L'П'] = 23;
    probabilityTable[L'У'] = 21;
    probabilityTable[L'Я'] = 18;
    probabilityTable[L'Ы'] = 16;
    probabilityTable[L'З'] = 16;
    probabilityTable[L'Ъ'] = 14;
    probabilityTable[L'Б'] = 14;
    probabilityTable[L'Г'] = 13;
    probabilityTable[L'Ч'] = 12;
    probabilityTable[L'Й'] = 10;
    probabilityTable[L'Х'] = 9;
    probabilityTable[L'Ж'] = 7;
    probabilityTable[L'Ю'] = 6;
    probabilityTable[L'Ш'] = 6;
    probabilityTable[L'Ц'] = 4;
    probabilityTable[L'Щ'] = 3;
    probabilityTable[L'Э'] = 3;
    probabilityTable[L'Ф'] = 2;
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

Widget::Text Widget::crypt(const Text &text) const
{
    mt19937 mt (mt19937::result_type(time(nullptr)));

    Text res;
    res.reserve(text.size());

    for (const auto &origLine : text)
    {
        QString newLine;
        newLine.reserve(origLine.length() * 3);

        for (QChar c : origLine)
        {

            auto homophones = homophonesBySymbol[ c.isUpper() ? c : c.toUpper() ];
            uniform_int_distribution<uint16_t> dist(0, uint16_t(homophones.size()-1));
            newLine.push_back( homophones[dist(mt)] );
        }

        res.push_back( newLine );
    }

    return res;
}

Widget::Text Widget::decrypt(const Text &text) const
{
    Text res;
    res.reserve(text.size());

    for (const auto &origLine : text)
    {
        QString newLine;
        newLine.reserve(origLine.length() / 3);

        auto len = origLine.length();
        for (qsizetype i = 0; i < len; i += 3)
        {
            auto homophone = origLine.mid(i, 3);
            auto decryptedChar = symbolByHomophone[homophone];
            newLine.push_back( decryptedChar );
        }

        res.push_back( newLine );
    }

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
