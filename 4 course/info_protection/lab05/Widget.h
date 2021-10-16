#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

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
    void prepareKeyBar(int count);
    /// прочитать входную последовательность, которая разбита на группы по countInGroup (4) символа, из файла
    QVector<QString> readSplittedStrFromFile(const QString &filename);
    /// зашифровать
    QString crypt(bool crypt, const QVector<QString> &splittedStr);
    /// расшифровать
    QString decrypt(const QVector<QString> &splittedStr);
    /// сохранить текст в файл
    void saveToFile(const QString &outStr, const QString &filename);
    /// открыть файл блокнотом в системе
    void openFile(const QString &filename);

// поля
    Ui::Widget *ui;
    QTabBar *keyBar = nullptr;
    static constexpr int countInGroup = 4;
    /// название исходного файла
    QString origFileName;
    /// название выходного файла
    QString outFileName;
};
#endif // WIDGET_H
