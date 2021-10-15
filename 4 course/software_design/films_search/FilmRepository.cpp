#include "FilmRepository.h"

FilmRepository::FilmRepository()
{
    films[0] = {0, "Фильм0", QDate::fromString("12.10.2021", "dd.MM.yyyy"), "Описание0", Genre::boevik, false, false, true};
    films[1] = {1, "Фильм1", QDate::fromString("14.10.2021", "dd.MM.yyyy"), "Описание1", Genre::comedy, false, false, true};
    films[2] = {2, "Фильм2", QDate::fromString("11.11.2011", "dd.MM.yyyy"), "Описание2", Genre::horror, false, false, false};
    films[3] = {3, "Фильм3", QDate::fromString("15.10.2015", "dd.MM.yyyy"), "Описание3", Genre::action, false, false, false};
}

QStringList FilmRepository::getGenreList() const
{
    return {"Боевик", "Комедия", "Драма", "Комикс", "Ужасы", "Триллер", "Экшн", "Спорт"};
}

std::list<FilmRepository::FilmInfo> FilmRepository::getNowPlaying() const
{
    std::list<FilmInfo> res;

    for (auto &film : films)
    {
        if (film.second.nowPlaying)
        {
            res.push_back(film.second);
        }
    }

    return res;
}

std::list<FilmRepository::FilmInfo> FilmRepository::getFilmsByYear(int year) const
{
    std::list<FilmInfo> res;

    for (auto &film : films)
    {
        if (film.second.date.year() == year)
        {
            res.push_back(film.second);
        }
    }

    return res;
}

std::list<FilmRepository::FilmInfo> FilmRepository::getFilmsByGenre(Genre genre) const
{
    std::list<FilmInfo> res;

    for (auto &film : films)
    {
        if (film.second.genre == genre)
        {
            res.push_back(film.second);
        }
    }

    return res;
}

FilmRepository::FilmInfo FilmRepository::getFilmInfo(int id) const
{
    return films.at(id);
}

void FilmRepository::pressSeen(int id, bool seen)
{
    films[id].seen = seen;
}

void FilmRepository::pressLike(int id, bool liked)
{
    films[id].liked = liked;
}
