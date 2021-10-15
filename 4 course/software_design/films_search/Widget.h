#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "FilmRepository.h"

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
    void on_dateEdit_year_dateChanged(const QDate &date);

    void on_comboBox_genre_currentIndexChanged(int index);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::Widget *ui;
    FilmRepository *db;

    void getGenreList();
    void getNowPlaying();
    void getFilmsByYear(int year);
    void getFilmsByGenre(FilmRepository::Genre genre);
};
#endif // WIDGET_H
