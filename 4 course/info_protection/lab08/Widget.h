#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QListWidget>
#include <vector>
#include <array>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

/// Шифрование методом гаммирования (аддитивный генератор ПСЧ).
class Widget : public QWidget
{
    Q_OBJECT

    /// Размер блока текста.
    static constexpr uint16_t blockSize = 8;

    using Text = QString;
    using Gamma = std::array<uint16_t, blockSize>;
public:
    explicit Widget(QWidget *parent = nullptr);
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
    /// Прочитать текст из файла.
    Text readFromFile(const QString &filename);
    /// Зашифровать.
    Text crypt(const Text &text) const;
    /// Расшифровать.
    Text decrypt(const Text &text) const;
    /// Сохранить текст в файл.
    void saveToFile(const Text &text, const QString &filename);
    /// Открыть файл блокнотом в системе.
    void openFileOnSystem(const QString &filename) const;
    /// Сгенерировать гамму шифра.
    Gamma generateGamma(const uint16_t Y0, const uint16_t Y1) const;

// поля
    Ui::Widget *ui;
    /// Модуль.
    const uint16_t m = 4096;
    /// Первое начальное порождающее число.
    const uint16_t initY0 = 4003;
    /// Второе начальное порождающее число.
    const uint16_t initY1 = 59;
    /// Название исходного файла.
    QString origFileName;
    /// Название выходного файла.
    QString outFileName;
};
#endif // WIDGET_H
