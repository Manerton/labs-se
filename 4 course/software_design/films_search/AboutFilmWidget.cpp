#include "AboutFilmWidget.h"
#include "ui_AboutFilmWidget.h"

AboutFilmWidget::AboutFilmWidget(int _row, FilmInfo _info) :
    QWidget(nullptr),
    ui(new Ui::AboutFilmWidget),
    info(_info),
    row(_row)
{
    ui->setupUi(this);
    ui->lineEdit_name->setText(info.name);
    ui->lineEdit_date->setText(info.date.toString());
    ui->textEdit_desc->setText(info.desc);
    ui->checkBox_seen->setChecked(info.seen);
    ui->checkBox_liked->setChecked(info.liked);
}

AboutFilmWidget::~AboutFilmWidget()
{
    delete ui;
}

int AboutFilmWidget::getRow() const
{
    return this->row;
}

int AboutFilmWidget::getId() const
{
    return this->info.id;
}

bool AboutFilmWidget::getSeen() const
{
    return this->info.seen;
}

bool AboutFilmWidget::getLiked() const
{
    return this->info.liked;
}

void AboutFilmWidget::on_pushButton_save_clicked()
{
    const bool isSeen = ui->checkBox_seen->isChecked();
    const bool isLiked = ui->checkBox_liked->isChecked();

    if (isSeen != info.seen)
    {
        this->info.seen = isSeen;
        emit pressSeen(this);
    }

    if (isLiked != info.liked)
    {
        this->info.liked = isLiked;
        emit pressLike(this);
    }
}

