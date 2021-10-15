#ifndef FILMREPOSITORY_H
#define FILMREPOSITORY_H

#include <list>
#include <QStringList>
#include <QDate>

class FilmRepository
{
public:
    struct FilmInfo
    {
        int id;
        QString name;
        QDate date;
        QString desc;
        int genre;
        bool seen;
        bool liked;
        bool nowPlaying;
    };

    enum Genre : int
    {
        boevik = 0,
        comedy = 1,
        drama = 2,
        comics = 3,
        horror = 4,
        triller = 5,
        action = 6,
        sport = 7
    };

    FilmRepository();
    QStringList getGenreList() const;
    std::list<FilmInfo> getNowPlaying() const;
    std::list<FilmInfo> getFilmsByYear(int year) const;
    std::list<FilmInfo> getFilmsByGenre(Genre genre) const;
    FilmInfo getFilmInfo(int id) const;
    void pressSeen(int id, bool seen);
    void pressLike(int id, bool liked);

private:
    std::map<int, FilmInfo> films;

};

#endif // FILMREPOSITORY_H
