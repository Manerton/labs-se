#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QListWidget>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

    using Text = QList<QString>;

    /// таблица шифра цезаря,
    /// где ключом является - буква из оригинального текста,
    /// а значением - заменяющая буква для шифрования
    using CryptTable = QMap<QChar, QChar>;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_openOrigFile_clicked();
    void on_pushButton_saveOutFile_clicked();
    void on_pushButton_crypt_clicked();
    void on_pushButton_decrypt_clicked();
    void on_lineEdit_outFilePath_textEdited(const QString &arg1);
    void on_lineEdit_origFilePath_textEdited(const QString &arg1);

private:
// методы
    /// подготовить кнопки для выбора ключа
    void prepareKeys();
    /// прочитать текст из файла
    Text readFromFile(const QString &filename);
    /// если crypted = true, то получить таблицу для шифрования по ключу
    /// иначе получить таблицу для расшифровки по ключу
    CryptTable getCryptedAlphabet(qsizetype key, bool crypted) const;
    /// зашифровать
    Text crypt(bool crypt, const Text &text) const;
    /// расшифровать
    Text decrypt(const Text &text) const;
    /// сохранить текст в файл
    void saveToFile(const Text &text, const QString &filename);
    /// открыть файл блокнотом в системе
    void openFileOnSystem(const QString &filename) const;

// поля
    Ui::Widget *ui;
    QTabBar *keyBar = nullptr;
    /// исходный алфавит для шифрования
    const QString ALPHABET = "АБВГДЕЖЗИКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ ";
    /// название исходного файла
    QString origFileName;
    /// название выходного файла
    QString outFileName;
};
#endif // WIDGET_H
