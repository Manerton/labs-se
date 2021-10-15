#ifndef FILMTABLEWIDGET_H
#define FILMTABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include "FilmRepository.h"

class AboutFilmWidget;

namespace Ui {
class FilmTableWidget;
}

class FilmTableWidget : public QTableWidget
{
    Q_OBJECT

    enum TableColumns : uint8_t
    {
        id = 0,
        name = 1,
        date = 2,
        seen = 3,
        liked = 4
    };

    using FilmInfo = FilmRepository::FilmInfo;

public:

    explicit FilmTableWidget(QWidget *parent);
    ~FilmTableWidget();

    void setDb(FilmRepository *db);
    void addFilm(FilmInfo info);
    void clearTable();

private slots:
    void on_tableWidget_itemActivated(QTableWidgetItem *item);
    void setSeen(AboutFilmWidget *widget);
    void setLiked(AboutFilmWidget *widget);

private:
    Ui::FilmTableWidget *ui;
    FilmRepository *db;
};

#endif // FILMTABLEWIDGET_H
