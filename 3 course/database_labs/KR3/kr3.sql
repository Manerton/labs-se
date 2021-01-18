@@ Задание 1:
// Автор
INSERT INTO "Автор" (name) VALUES ('Иванов И. И.');
INSERT INTO "Автор" (name) VALUES ('Петров П. П.');
INSERT INTO "Автор" (name) VALUES ('Сидоров С. С.');
INSERT INTO "Автор" (name) VALUES ('Кузнецов К. К.');
// Издательство
INSERT INTO "Издательство" (name) VALUES ('Питер');
INSERT INTO "Издательство" (name) VALUES ('Дрофа');
INSERT INTO "Издательство" (name) VALUES ('Феникс');
INSERT INTO "Издательство" (name) VALUES ('Финансы и статистика');
// Книга
INSERT INTO "Книга" (name, year_out) VALUES ('Основы программирования', '2003');
INSERT INTO "Книга" (name, year_out) VALUES ('Мировая экономика', '2000');
INSERT INTO "Книга" (name, year_out) VALUES ('Информатика', '2007');
INSERT INTO "Книга" (name, year_out) VALUES ('Операционные системы', '2007');
INSERT INTO "Книга" (name, year_out) VALUES ('Физика 11 класс', '1999');
INSERT INTO "Книга" (name, year_out) VALUES ('Химия 10 класс', '2005');
// Сводная
INSERT INTO "Сводная" (id_books, id_authors, id_publishers) VALUES (1,3,1);
INSERT INTO "Сводная" (id_books, id_authors, id_publishers) VALUES (2,1,4);
INSERT INTO "Сводная" (id_books, id_authors, id_publishers) VALUES (6,4,2);
INSERT INTO "Сводная" (id_books, id_authors, id_publishers) VALUES (5,2,2);
INSERT INTO "Сводная" (id_books, id_authors, id_publishers) VALUES (3,3,1);
INSERT INTO "Сводная" (id_books, id_authors, id_publishers) VALUES (3,1,1);
INSERT INTO "Сводная" (id_books, id_authors, id_publishers) VALUES (4,1,1);
INSERT INTO "Сводная" (id_books, id_authors, id_publishers) VALUES (4,1,3);
@@ Задание 2:
INSERT INTO "Книга" (name, year_out) SELECT 'SQL Server', '2008';
@@ Задание 3:
INSERT INTO "Автор" (name) VALUES ('Андреев А. А.'), ('Киреев К. К.'), ('Максимов М. М.');
@@ Задание 4:
INSERT INTO "Книга" (name, year_out) VALUES ('MySQL', DEFAULT)
@@ Задание 5:
INSERT INTO "Сводная" (id_books, id_authors, id_publishers) SELECT MAX(id_books), MIN(id_authors), TRUNC(AVG(id_publishers)) FROM "Книга", "Автор", "Издательство";
@@ Задание 6:
DELETE FROM "Сводная" as sv Where sv.id_books = (SELECT b.id_books from "Книга" as b WHERE year_out = (SELECT MIN(year_out) from "Книга"));
@@ Задание 7:
UPDATE "Сводная" SET "id_publishers" = (select iz."id_publishers" from "Издательство" iz where iz.name = 'Дрофа') WHERE id_authors = 3;
@@ Задание 8:
UPDATE "Книга" SET year_out = year_out + 1 WHERE id_books IN (SELECT id_books FROM "Книга" where name LIKE '_и%');
@@ Задание 9:
SELECT year_out, COUNT(*) AS "Количество книг" FROM "Книга" 
GROUP BY year_out HAVING year_out = (SELECT MAX(year_out) FROM "Книга");
@@ Задание 10:
SELECT name, COUNT(id_authors) FROM "Книга" AS K 
INNER JOIN "Сводная" as S ON S.id_books = K.id_books 
GROUP BY name
@@ Задание 11:
SELECT name FROM "Книга" AS K 
INNER JOIN "Сводная" as S ON S.id_books = K.id_books 
GROUP BY name HAVING COUNT(id_authors) = 2
@@ Задание 12:
SELECT name FROM "Издательство" WHERE (name LIKE ('%ф%')) OR (name LIKE ('%Ф%'))
@@ Задание 13:
SELECT name, year_out FROM "Книга"
WHERE year_out < 2000 OR year_out > 2006
@@ Задание 14:
SELECT name, 
(Select LENGTH(name) - LENGTH(replace(A.name,'о','')) 
 FROM "Автор" where A.id_authors = id_authors) as "Количество букв 'о'"
 FROM "Автор" as A
@@ Задание 15:
SELECT name, COUNT(id_books) FROM "Автор" AS A 
INNER JOIN "Сводная" as S ON S.id_authors = A.id_authors
GROUP BY name HAVING COUNT(id_books) >= 2
@@ Задание 16:
CREATE OR REPLACE FUNCTION count_alphabet() 
RETURNS TABLE ("Символ" text, "Количество повторений" integer)
LANGUAGE plpgsql 
AS $BODY$ 
DECLARE
str text;
i integer;
BEGIN	
	str := (select string_agg(lower(name),'') from "Книга");
	i := ASCII('а');
	WHILE i <= ASCII('я') LOOP
		RETURN QUERY ( SELECT chr(i) , (length(str)- length(replace(str, chr(i), ''))) );
		i := i+1;
	END LOOP;
END;
$BODY$