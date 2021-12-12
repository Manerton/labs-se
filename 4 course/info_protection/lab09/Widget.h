#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QListWidget>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget final : public QWidget
{
    Q_OBJECT

    using Text = QString;

    /// Таблица для преобразования буквы в число.
    /// Для обратного преобразования см. поле ALPHABET.
    using CharToIntTable = QMap<QChar, qsizetype>;

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() final;

private slots:
    void on_pushButton_openOrigFile_clicked();
    void on_pushButton_saveOutFile_clicked();
    void on_pushButton_crypt_clicked();
    void on_pushButton_decrypt_clicked();
    void on_lineEdit_outFilePath_textEdited(const QString &arg1);
    void on_lineEdit_origFilePath_textEdited(const QString &arg1);

private:
// методы
    /// Подготовить таблицу для преобразования буквы в число.
    CharToIntTable prepareInvertedAlphabet() const;
    /// Прочитать текст из файла.
    Text readFromFile(const QString &filename);
    /// Зашифровать.
    Text crypt(const Text &text);
    /// Расшифровать.
    Text decrypt(const Text &text);
    /// Сохранить текст в файл.
    void saveToFile(const Text &text, const QString &filename);
    /// Открыть файл блокнотом в системе.
    void openFileOnSystem(const QString &filename) const;

// поля
    Ui::Widget *ui;
    /// Исходный алфавит для шифрования.
    /// Можно использовать для преобразования числа в букву.
    const QString ALPHABET = "АБВГДЕЖЗИКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ ";
    /// Можно использовать для преобразования буквы в число.
    const CharToIntTable invertedALPHABET;
    /// Название исходного файла.
    QString origFileName;
    /// Название выходного файла.
    QString outFileName;
    /// Последний ключ для расшифровки.
    QVector<qsizetype> latestKey;
};
#endif // WIDGET_H
