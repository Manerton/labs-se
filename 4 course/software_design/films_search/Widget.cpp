#include "Widget.h"
#include "./ui_Widget.h"
#include "FilmTableWidget.h"
#include "FilmRepository.h"

using FilmInfo = FilmRepository::FilmInfo;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , db(new FilmRepository())
{
    ui->setupUi(this);

    ui->tableWidget_filmsNow->setDb(db);
    ui->tableWidget_filmsByYear->setDb(db);
    ui->tableWidget_filmsByGenre->setDb(db);

    for (auto &film : db->getNowPlaying())
    {
        ui->tableWidget_filmsNow->addFilm(film);
    }

    for (auto &film : db->getFilmsByYear(2021))
    {
        ui->tableWidget_filmsByYear->addFilm(film);
    }

    for (auto &film : db->getFilmsByGenre(FilmRepository::Genre::boevik))
    {
        ui->tableWidget_filmsByGenre->addFilm(film);
    }

    this->getGenreList();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::getGenreList()
{
    ui->comboBox_genre->addItems(db->getGenreList());
}

void Widget::on_dateEdit_year_dateChanged(const QDate &date)
{
    ui->tableWidget_filmsByYear->clearTable();

    for (auto &film : db->getFilmsByYear(date.year()))
    {
        ui->tableWidget_filmsByYear->addFilm(film);
    }
}


void Widget::on_comboBox_genre_currentIndexChanged(int index)
{
    ui->tableWidget_filmsByGenre->clearTable();

    for (auto &film : db->getFilmsByGenre(FilmRepository::Genre(index)))
    {
        ui->tableWidget_filmsByGenre->addFilm(film);
    }
}


void Widget::on_tabWidget_currentChanged(int index)
{
    if (index == 0)
    {
        ui->tableWidget_filmsNow->clearTable();

        for (auto &film : db->getNowPlaying())
        {
            ui->tableWidget_filmsNow->addFilm(film);
        }
    }
}

