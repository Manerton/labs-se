#include "FilmTableWidget.h"
#include "ui_FilmTableWidget.h"

#include "AboutFilmWidget.h"

FilmTableWidget::FilmTableWidget(QWidget *parent) :
    QTableWidget(parent),
    ui(new Ui::FilmTableWidget),
    db(nullptr)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setColumnHidden(0, true);
    ui->tableWidget->setHorizontalHeaderLabels({"Номер", "Название фильма", "Дата выхода", "Просмотрен", "Нравится"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

FilmTableWidget::~FilmTableWidget()
{
    delete ui;
}

void FilmTableWidget::setDb(FilmRepository* _db)
{
    this->db = _db;
}

void FilmTableWidget::addFilm(FilmInfo info)
{
    const auto row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, TableColumns::id, new QTableWidgetItem(QString::number(info.id)));
    ui->tableWidget->setItem(row, TableColumns::name, new QTableWidgetItem(info.name));
    ui->tableWidget->setItem(row, TableColumns::date, new QTableWidgetItem(info.date.toString()));

    QString isSeenTxt = "Нет";
    if (info.seen)
    {
        isSeenTxt = "Да";
    }

    QString isLikedTxt = "Нет";
    if (info.liked)
    {
        isLikedTxt = "Да";
    }

    const auto seenItem = new QTableWidgetItem(isSeenTxt);
    const auto likedItem = new QTableWidgetItem(isLikedTxt);

    ui->tableWidget->setItem(row, TableColumns::seen, seenItem);
    ui->tableWidget->setItem(row, TableColumns::liked, likedItem);
}

void FilmTableWidget::clearTable()
{
    ui->tableWidget->setRowCount(0);
}

void FilmTableWidget::on_tableWidget_itemActivated(QTableWidgetItem *item)
{
    const int row = item->row();
    const int id = ui->tableWidget->item(row, TableColumns::id)->text().toInt();

    FilmInfo info = db->getFilmInfo(id);
    auto aboutFilm = new AboutFilmWidget(row, info);

    this->connect(aboutFilm, &AboutFilmWidget::pressSeen, this, &FilmTableWidget::setSeen);
    this->connect(aboutFilm, &AboutFilmWidget::pressLike, this, &FilmTableWidget::setLiked);

    aboutFilm->show();
}

void FilmTableWidget::setSeen(AboutFilmWidget *widget)
{
    const auto row = widget->getRow();
    const auto id = widget->getId();
    const bool seen = widget->getSeen();

    auto item = ui->tableWidget->item(row, TableColumns::seen);
    db->pressSeen(id, seen);

    if (seen)
    {
        item->setText("Да");
    }
    else
    {
        item->setText("Нет");
    }

}

void FilmTableWidget::setLiked(AboutFilmWidget *widget)
{
    const auto row = widget->getRow();
    const auto id = widget->getId();
    const bool liked = widget->getLiked();

    auto item = ui->tableWidget->item(row, TableColumns::liked);
    db->pressLike(id, liked);

    if (liked)
    {
        item->setText("Да");
    }
    else
    {
        item->setText("Нет");
    }
}

