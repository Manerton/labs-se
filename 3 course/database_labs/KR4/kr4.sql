@@ Задание 1:
CREATE OR REPLACE FUNCTION public.count_books(fio text)
    RETURNS integer
    LANGUAGE 'plpgsql'
AS $BODY$
DECLARE
count_books integer;
author_id integer;
BEGIN	
	IF (SELECT COUNT(id_authors) FROM "Автор" AS A 
		WHERE A.name = fio) = 0 THEN
	RETURN -1;
	END IF;
	author_id := (SELECT id_authors FROM "Автор" AS A WHERE A.name = fio);
	count_books := (SELECT COUNT(id_books) FROM "Сводная" AS S WHERE S.id_authors = author_id);
	RETURN count_books;
END;
$BODY$;
@@ Задание 2:
CREATE OR REPLACE FUNCTION display_user_func()
    RETURNS TABLE (
		"Определение функции" text,
		"Возвращаемый результат" text
	)
    LANGUAGE 'plpgsql'
AS $BODY$
DECLARE
f integer;
BEGIN	
	for f in select oid from pg_proc where prolang > 16 loop
	RETURN QUERY
	select p1.*, p2.* from pg_get_functiondef(f) as p1, pg_get_function_result(f) as p2;
	end loop;
END;
$BODY$;
// Вывод
select * from display_user_func()
@@ Задание 3:
?
@@ Задание 4:
drop function count_books;
@@ Задание 5:
CREATE OR REPLACE FUNCTION books_published(izdat text)
    RETURNS TABLE("Название книги" text, "Фамилии авторов" text, "Дата издания" smallint) 
    LANGUAGE 'plpgsql'
AS $BODY$
	DECLARE
	izd_id integer;
BEGIN	
	izd_id := (SELECT id_publishers from "Издательство" WHERE name = "izdat");
	return query
	SELECT cast(K.name as text), string_agg(DISTINCT A.name, ', '), year_out FROM "Сводная" as S
	INNER JOIN "Книга" AS K ON S.id_books = K.id_books
	INNER JOIN "Автор" AS A ON S.id_authors = A.id_authors
	WHERE S.id_books IN (select id_books from "Сводная" as S WHERE S.id_publishers = izd_id)
	GROUP BY K.name, year_out;
END;
$BODY$;
@@ Задание 6:
CREATE OR REPLACE FUNCTION fibonacci(n integer)
    RETURNS TABLE("Число Фибоначчи" numeric) 
    LANGUAGE 'plpgsql'
AS $BODY$
DECLARE 
	i integer;
	one numeric;
	two numeric;
BEGIN	
	two := 1;
	return query (SELECT two);
	if n > 2 then
		i := 3;
		one := 1;
		return query (SELECT one);
		while n >= i loop
			two := one + two;
			one := two - one;
			i := i + 1;
			return query (SELECT two);	
		end loop;
	end if;
END;
$BODY$;