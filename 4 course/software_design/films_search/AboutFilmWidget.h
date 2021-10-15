#ifndef ABOUTFILMWIDGET_H
#define ABOUTFILMWIDGET_H

#include <QWidget>
#include "FilmTableWidget.h"
#include "FilmRepository.h"

namespace Ui {
class AboutFilmWidget;
}

class AboutFilmWidget : public QWidget
{
    Q_OBJECT

    using FilmInfo = FilmRepository::FilmInfo;
public:
    explicit AboutFilmWidget(int row, FilmInfo _info);
    ~AboutFilmWidget();

    int getRow() const;
    int getId() const;
    bool getSeen() const;
    bool getLiked() const;

signals:
    void pressSeen(AboutFilmWidget *widget);
    void pressLike(AboutFilmWidget *widget);

private slots:
    void on_pushButton_save_clicked();

private:
    Ui::AboutFilmWidget *ui;
    FilmInfo info;
    int row;
};

#endif // ABOUTFILMWIDGET_H
