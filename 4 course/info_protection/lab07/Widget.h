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

    /// Таблица вероятностей букв (количество строк для каждой буквы алфавита).
    using ProbabilityTable = QMap<QChar, int>;

    /// Таблица с омофонами (для каждой буквы алфавита список омофонов).
    using HomophonesBySymbolMap = QMap<QChar, std::vector<QString>>;

    /// Таблица с буквами (для каждого омофона соответствующая буква алфавита).
    using SymbolByHomophoneMap = QMap<QString, QChar>;
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
    /// Подготовить кнопки для выбора ключа.
    void prepareKeys();
    /// Подготовить таблицу с вероятностями букв.
    void prepareProbabilityTable();
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

// поля
    Ui::Widget *ui;
    /// Ширина столбца таблицы.
    static constexpr int headerSectionWidth = 35;
    /// Количество строк таблицы.
    static constexpr int maxRowCount = 175;
    /// Исходный алфавит для шифрования.
    const QString ALPHABET = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ ";
    /// Таблица вероятностей букв (количество строк для каждой буквы алфавита).
    ProbabilityTable probabilityTable;
    /// Омофоны по букве.
    HomophonesBySymbolMap homophonesBySymbol;
    /// Буква по омофону.
    SymbolByHomophoneMap symbolByHomophone;
    /// Название исходного файла.
    QString origFileName;
    /// Название выходного файла.
    QString outFileName;
};
#endif // WIDGET_H
