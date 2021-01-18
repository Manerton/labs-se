--
-- PostgreSQL database dump
--

-- Dumped from database version 13.1
-- Dumped by pg_dump version 13.1

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: lombard; Type: DATABASE; Schema: -; Owner: postgres
--

CREATE DATABASE lombard WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE = 'Russian_Russia.1251';


ALTER DATABASE lombard OWNER TO postgres;

\connect lombard

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: Отчет; Type: TYPE; Schema: public; Owner: postgres
--

CREATE TYPE public."Отчет" AS (
	"ID_Сделка" integer,
	"Состояние" text,
	"Товар" text,
	"Дата изготовления" date,
	"Клиент" text,
	"Сотрудник" text,
	"Дата оформления" date,
	"Срок хранения" date,
	"Место" text,
	"Залог" double precision,
	"Дата реализации" date,
	"Сумма реализации" double precision,
	"Доход" double precision
);


ALTER TYPE public."Отчет" OWNER TO postgres;

--
-- Name: calc_profit(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.calc_profit() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
	IF ((SELECT S."Срок хранения" FROM "Сделка" as S WHERE NEW."ID_Сделка" = S."ID_Сделка") > NEW."Дата реализации") THEN
		RAISE EXCEPTION 'Указана неверная дата реализации: она не может быть раньше конца срока хранения!';
		RETURN NULL;
	ELSIF (SELECT S."ID_Состояние" FROM "Сделка" as S WHERE NEW."ID_Сделка" = S."ID_Сделка") != 2 THEN
		RAISE EXCEPTION 'Реализуемый товар не является просроченным по хранению!';
		RETURN NULL;
	END IF;	
	NEW."Доход" := NEW."Сумма реализации" - (SELECT S."Сумма, выданная под залог" FROM "Сделка" as S WHERE NEW."ID_Сделка" = S."ID_Сделка");
	UPDATE "Сделка" as S SET "ID_Состояние" = 4 WHERE NEW."ID_Сделка" = S."ID_Сделка";
	RETURN NEW;
END
$$;


ALTER FUNCTION public.calc_profit() OWNER TO postgres;

--
-- Name: get_clients_fio(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.get_clients_fio() RETURNS TABLE("ID_Клиент" integer, "ФИО" text)
    LANGUAGE plpgsql
    AS $$
begin
return query
SELECT cl."ID_Клиент", concat(cl."Фамилия клиента",' ',LEFT(cl."Имя клиента",1),'. ',LEFT(cl."Отчество клиента",1),'.') from "Клиент" as cl;
end
$$;


ALTER FUNCTION public.get_clients_fio() OWNER TO postgres;

--
-- Name: get_workers_fio(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.get_workers_fio() RETURNS TABLE("ID_Сотрудник" integer, "ФИО" text)
    LANGUAGE plpgsql
    AS $$
begin
return query
SELECT w."ID_Сотрудник", concat(w."Фамилия сотрудника",' ',LEFT(w."Имя сотрудника",1),'. ',LEFT(w."Отчество сотрудника",1),'.') from "Сотрудник" as w;
end
$$;


ALTER FUNCTION public.get_workers_fio() OWNER TO postgres;

--
-- Name: prevent_delete(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.prevent_delete() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
	RAISE EXCEPTION 'Запрещено удалять записи из таблицы!';
	RETURN NULL;
END
$$;


ALTER FUNCTION public.prevent_delete() OWNER TO postgres;

--
-- Name: show_by_client(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.show_by_client(id integer) RETURNS SETOF public."Отчет"
    LANGUAGE plpgsql
    AS $$
begin
return query
SELECT * from show_deal() where "ID_Сделка" IN 
(SELECT "ID_Сделка" FROM "Сделка" WHERE "ID_Клиент" = id);
end
$$;


ALTER FUNCTION public.show_by_client(id integer) OWNER TO postgres;

--
-- Name: show_by_date(date); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.show_by_date(_date date) RETURNS SETOF public."Отчет"
    LANGUAGE plpgsql
    AS $$
begin
return query
SELECT * from show_deal() where "Срок хранения" = _date;
end
$$;


ALTER FUNCTION public.show_by_date(_date date) OWNER TO postgres;

--
-- Name: show_by_product(text); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.show_by_product(title text) RETURNS SETOF public."Отчет"
    LANGUAGE plpgsql
    AS $$
begin
return query
SELECT * from show_deal() where "ID_Сделка" IN 
(SELECT "ID_Сделка" FROM "Сделка" WHERE "ID_Товар" 
 IN (SELECT "ID_Товар" FROM "Товар" WHERE "Наименование товара" LIKE CONCAT('%',title,'%')));
end
$$;


ALTER FUNCTION public.show_by_product(title text) OWNER TO postgres;

--
-- Name: show_by_worker(integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.show_by_worker(id integer) RETURNS SETOF public."Отчет"
    LANGUAGE plpgsql
    AS $$
begin
return query
SELECT * from show_deal() where "ID_Сделка" IN 
(SELECT "ID_Сделка" FROM "Сделка" WHERE "ID_Сотрудник" = id);
end
$$;


ALTER FUNCTION public.show_by_worker(id integer) OWNER TO postgres;

--
-- Name: show_deal(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.show_deal() RETURNS SETOF public."Отчет"
    LANGUAGE plpgsql
    AS $$
begin
return query 
SELECT sd."ID_Сделка", sost."Название состояния", t."Наименование товара", sd."Дата изготовления",
concat(cl."Фамилия клиента",' ',LEFT(cl."Имя клиента",1),'. ',LEFT(cl."Отчество клиента",1),'.'), 
concat(sot."Фамилия сотрудника",' ',LEFT(sot."Имя сотрудника",1),'. ',LEFT(sot."Отчество сотрудника",1),'.'), 
sd."Дата оформления", sd."Срок хранения", m."Название",
sd."Сумма, выданная под залог", sold."Дата реализации", sold."Сумма реализации", 
sold."Доход" 
FROM "Сделка" as sd
JOIN "Состояние товара" as sost ON sd."ID_Состояние" = sost."ID_Состояние"
JOIN "Клиент" as cl ON sd."ID_Клиент" = cl."ID_Клиент"
JOIN "Товар" as t ON sd."ID_Товар" = t."ID_Товар"
JOIN "Сотрудник" as sot ON sd."ID_Сотрудник" = sot."ID_Сотрудник"
JOIN "Место хранения" as m ON sd."ID_Место" = m."ID_Место"
LEFT JOIN "Реализованная сделка" as sold ON sd."ID_Сделка" = sold."ID_Сделка"
ORDER BY sd."ID_Сделка";
end
$$;


ALTER FUNCTION public.show_deal() OWNER TO postgres;

--
-- Name: show_expired(date, date); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.show_expired(_start date, _end date) RETURNS SETOF public."Отчет"
    LANGUAGE plpgsql
    AS $$
begin
return query
SELECT * from show_deal() where "Состояние" = 'Просрочено хранение' AND ("Срок хранения" >= "_start" AND "Срок хранения" <= "_end");
end
$$;


ALTER FUNCTION public.show_expired(_start date, _end date) OWNER TO postgres;

--
-- Name: show_profit(date, date); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.show_profit(_start date, _end date) RETURNS TABLE("Количество реализованных товаров" bigint, "Общая сумма доходов" double precision)
    LANGUAGE plpgsql
    AS $$
begin
return query
SELECT COUNT(*), SUM("Доход") from "Сделка" as s INNER JOIN "Реализованная сделка" as sold ON s."ID_Сделка" = sold."ID_Сделка" WHERE ("Дата реализации" >= "_start" AND "Дата реализации" <= "_end");
end
$$;


ALTER FUNCTION public.show_profit(_start date, _end date) OWNER TO postgres;

--
-- Name: update_count(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.update_count() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
	IF ((OLD."ID_Состояние" IN (1,2)) AND (NEW."ID_Состояние" IN (3,4))) THEN
		UPDATE "Место хранения" as pl SET "Количество товаров" = ("Количество товаров" - 1) WHERE NEW."ID_Место" = pl."ID_Место";
	END IF;
	RETURN NEW;
END
$$;


ALTER FUNCTION public.update_count() OWNER TO postgres;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: Категория товара; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Категория товара" (
    "ID_Категория" integer NOT NULL,
    "Название категории" text NOT NULL
);


ALTER TABLE public."Категория товара" OWNER TO postgres;

--
-- Name: Категория товара_ID_Категория_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public."Категория товара" ALTER COLUMN "ID_Категория" ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public."Категория товара_ID_Категория_seq"
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: Клиент; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Клиент" (
    "ID_Клиент" integer NOT NULL,
    "Фамилия клиента" text NOT NULL,
    "Имя клиента" text NOT NULL,
    "Отчество клиента" text NOT NULL,
    "Телефон клиента" text NOT NULL
);


ALTER TABLE public."Клиент" OWNER TO postgres;

--
-- Name: Клиент_ID_Клиент_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public."Клиент" ALTER COLUMN "ID_Клиент" ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public."Клиент_ID_Клиент_seq"
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: Место хранения; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Место хранения" (
    "ID_Место" integer NOT NULL,
    "Название" text NOT NULL,
    "Площадь" double precision NOT NULL,
    "Количество товаров" integer
);


ALTER TABLE public."Место хранения" OWNER TO postgres;

--
-- Name: Место хранения_ID_Место_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public."Место хранения" ALTER COLUMN "ID_Место" ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public."Место хранения_ID_Место_seq"
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: Производитель; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Производитель" (
    "ID_Производитель" integer NOT NULL,
    "Название производителя" text NOT NULL
);


ALTER TABLE public."Производитель" OWNER TO postgres;

--
-- Name: Производитель_ID_Производитель_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public."Производитель" ALTER COLUMN "ID_Производитель" ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public."Производитель_ID_Производитель_seq"
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: Реализованная сделка; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Реализованная сделка" (
    "ID_Запись" integer NOT NULL,
    "ID_Сделка" integer NOT NULL,
    "Дата реализации" date NOT NULL,
    "Сумма реализации" double precision NOT NULL,
    "Доход" double precision
);


ALTER TABLE public."Реализованная сделка" OWNER TO postgres;

--
-- Name: Реализованная сделка_ID_Запись_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public."Реализованная сделка" ALTER COLUMN "ID_Запись" ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public."Реализованная сделка_ID_Запись_seq"
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: Сделка; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Сделка" (
    "ID_Сделка" integer NOT NULL,
    "ID_Клиент" integer NOT NULL,
    "ID_Товар" integer NOT NULL,
    "ID_Сотрудник" integer NOT NULL,
    "Дата изготовления" date NOT NULL,
    "Дата оформления" date NOT NULL,
    "Срок хранения" date NOT NULL,
    "Сумма, выданная под залог" double precision NOT NULL,
    "ID_Состояние" integer NOT NULL,
    "ID_Место" integer NOT NULL
);


ALTER TABLE public."Сделка" OWNER TO postgres;

--
-- Name: Сделка_ID_Сделка_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public."Сделка" ALTER COLUMN "ID_Сделка" ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public."Сделка_ID_Сделка_seq"
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: Состояние товара; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Состояние товара" (
    "ID_Состояние" integer NOT NULL,
    "Название состояния" text NOT NULL
);


ALTER TABLE public."Состояние товара" OWNER TO postgres;

--
-- Name: Состояние товара_ID_Состояние_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public."Состояние товара" ALTER COLUMN "ID_Состояние" ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public."Состояние товара_ID_Состояние_seq"
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: Сотрудник; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Сотрудник" (
    "ID_Сотрудник" integer NOT NULL,
    "Имя сотрудника" text NOT NULL,
    "Фамилия сотрудника" text NOT NULL,
    "Отчество сотрудника" text,
    "Телефон сотрудника" character varying(15) NOT NULL,
    "Зарплата сотрудника" double precision NOT NULL
);


ALTER TABLE public."Сотрудник" OWNER TO postgres;

--
-- Name: Сотрудник_ID_Сотрудник_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public."Сотрудник" ALTER COLUMN "ID_Сотрудник" ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public."Сотрудник_ID_Сотрудник_seq"
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Name: Товар; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Товар" (
    "ID_Товар" integer NOT NULL,
    "Наименование товара" text NOT NULL,
    "Оценочная стоимость" double precision NOT NULL,
    "ID_Производитель" integer NOT NULL,
    "ID_Категория" integer NOT NULL
);


ALTER TABLE public."Товар" OWNER TO postgres;

--
-- Name: Товар_Код товара_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

ALTER TABLE public."Товар" ALTER COLUMN "ID_Товар" ADD GENERATED BY DEFAULT AS IDENTITY (
    SEQUENCE NAME public."Товар_Код товара_seq"
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);


--
-- Data for Name: Категория товара; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public."Категория товара" VALUES (1, 'Пылесосы');
INSERT INTO public."Категория товара" VALUES (2, 'Ювелирные изделия');
INSERT INTO public."Категория товара" VALUES (3, 'Видеокарты');
INSERT INTO public."Категория товара" VALUES (4, 'Смартфоны');


--
-- Data for Name: Клиент; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public."Клиент" VALUES (1, 'Пупкин', 'Василий', 'Павлович', '+79246123999');
INSERT INTO public."Клиент" VALUES (2, 'Иванов', 'Иван', 'Алексеевич', '+79246123345');
INSERT INTO public."Клиент" VALUES (3, 'Алексеев', 'Алексей', 'Иванович', '+79246424924');
INSERT INTO public."Клиент" VALUES (4, 'Ололоев', 'Максим', 'Алексеевич', '+79276427927');


--
-- Data for Name: Место хранения; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public."Место хранения" VALUES (2, 'Основной склад', 120, 0);
INSERT INTO public."Место хранения" VALUES (3, 'Подсобка', 40, 1);
INSERT INTO public."Место хранения" VALUES (1, 'Витрина', 15, 2);


--
-- Data for Name: Производитель; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public."Производитель" VALUES (1, 'ASUS');
INSERT INTO public."Производитель" VALUES (2, 'Sunlight');
INSERT INTO public."Производитель" VALUES (3, 'Samsung');
INSERT INTO public."Производитель" VALUES (4, 'Xiaomi');


--
-- Data for Name: Реализованная сделка; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public."Реализованная сделка" VALUES (1, 1, '2021-01-13', 30000, 8000);
INSERT INTO public."Реализованная сделка" VALUES (2, 2, '2021-01-17', 10000, 3000);


--
-- Data for Name: Сделка; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public."Сделка" VALUES (1, 3, 2, 1, '2020-07-15', '2021-01-06', '2021-01-12', 22000, 4, 3);
INSERT INTO public."Сделка" VALUES (2, 2, 4, 3, '2019-05-26', '2021-01-08', '2021-01-16', 7000, 4, 1);
INSERT INTO public."Сделка" VALUES (3, 1, 1, 2, '2019-03-26', '2021-01-11', '2021-01-29', 7000, 1, 1);
INSERT INTO public."Сделка" VALUES (5, 4, 1, 2, '2019-07-05', '2021-01-14', '2021-01-16', 7500, 2, 3);
INSERT INTO public."Сделка" VALUES (6, 1, 2, 2, '2018-03-05', '2021-01-19', '2021-01-25', 21000, 3, 2);
INSERT INTO public."Сделка" VALUES (4, 2, 3, 1, '2020-05-08', '2021-01-11', '2021-01-17', 6500, 2, 1);


--
-- Data for Name: Состояние товара; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public."Состояние товара" VALUES (1, 'В залоге');
INSERT INTO public."Состояние товара" VALUES (2, 'Просрочено хранение');
INSERT INTO public."Состояние товара" VALUES (3, 'Выкуплен');
INSERT INTO public."Состояние товара" VALUES (4, 'Реализован по истечению времени');


--
-- Data for Name: Сотрудник; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public."Сотрудник" VALUES (1, 'Михаил', 'Зубенко', 'Петрович', '+79096999999', 25650);
INSERT INTO public."Сотрудник" VALUES (2, 'Михаил', 'Терентьев', 'Павлович', '+79096999888', 32550);
INSERT INTO public."Сотрудник" VALUES (3, 'Никита', 'Михалков', 'Сергеевич', '+79096595858', 43500);


--
-- Data for Name: Товар; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public."Товар" VALUES (2, 'Jet 75 pet', 35000, 3, 1);
INSERT INTO public."Товар" VALUES (3, 'Золотая цепь', 12000, 2, 2);
INSERT INTO public."Товар" VALUES (1, 'GeForce GTX 1060 DUAL', 15000, 1, 3);
INSERT INTO public."Товар" VALUES (4, 'Redmi Note 8T', 12000, 4, 4);


--
-- Name: Категория товара_ID_Категория_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Категория товара_ID_Категория_seq"', 3, true);


--
-- Name: Клиент_ID_Клиент_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Клиент_ID_Клиент_seq"', 4, true);


--
-- Name: Место хранения_ID_Место_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Место хранения_ID_Место_seq"', 3, true);


--
-- Name: Производитель_ID_Производитель_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Производитель_ID_Производитель_seq"', 3, true);


--
-- Name: Реализованная сделка_ID_Запись_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Реализованная сделка_ID_Запись_seq"', 8, true);


--
-- Name: Сделка_ID_Сделка_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Сделка_ID_Сделка_seq"', 6, true);


--
-- Name: Состояние товара_ID_Состояние_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Состояние товара_ID_Состояние_seq"', 7, true);


--
-- Name: Сотрудник_ID_Сотрудник_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Сотрудник_ID_Сотрудник_seq"', 3, true);


--
-- Name: Товар_Код товара_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Товар_Код товара_seq"', 4, true);


--
-- Name: Категория товара Категория товара_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Категория товара"
    ADD CONSTRAINT "Категория товара_pkey" PRIMARY KEY ("ID_Категория");


--
-- Name: Клиент Клиент_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Клиент"
    ADD CONSTRAINT "Клиент_pkey" PRIMARY KEY ("ID_Клиент");


--
-- Name: Место хранения Место хранения_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Место хранения"
    ADD CONSTRAINT "Место хранения_pkey" PRIMARY KEY ("ID_Место");


--
-- Name: Производитель Производитель_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Производитель"
    ADD CONSTRAINT "Производитель_pkey" PRIMARY KEY ("ID_Производитель");


--
-- Name: Реализованная сделка Реализованная сделка_ID_Сделка_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Реализованная сделка"
    ADD CONSTRAINT "Реализованная сделка_ID_Сделка_key" UNIQUE ("ID_Сделка");


--
-- Name: Реализованная сделка Реализованная сделка_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Реализованная сделка"
    ADD CONSTRAINT "Реализованная сделка_pkey" PRIMARY KEY ("ID_Запись");


--
-- Name: Сделка Сделка_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сделка"
    ADD CONSTRAINT "Сделка_pkey" PRIMARY KEY ("ID_Сделка");


--
-- Name: Состояние товара Состояние товара_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Состояние товара"
    ADD CONSTRAINT "Состояние товара_pkey" PRIMARY KEY ("ID_Состояние");


--
-- Name: Сотрудник Сотрудник_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сотрудник"
    ADD CONSTRAINT "Сотрудник_pkey" PRIMARY KEY ("ID_Сотрудник");


--
-- Name: Товар Товар_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Товар"
    ADD CONSTRAINT "Товар_pkey" PRIMARY KEY ("ID_Товар");


--
-- Name: Производитель t_prevent_delete; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER t_prevent_delete BEFORE DELETE ON public."Производитель" FOR EACH STATEMENT EXECUTE FUNCTION public.prevent_delete();


--
-- Name: Сделка t_prevent_delete; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER t_prevent_delete BEFORE DELETE ON public."Сделка" FOR EACH ROW EXECUTE FUNCTION public.prevent_delete();


--
-- Name: Реализованная сделка t_sale_product; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER t_sale_product BEFORE INSERT ON public."Реализованная сделка" FOR EACH ROW EXECUTE FUNCTION public.calc_profit();


--
-- Name: Сделка t_update_count; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER t_update_count BEFORE UPDATE ON public."Сделка" FOR EACH ROW EXECUTE FUNCTION public.update_count();


--
-- Name: Реализованная сделка Реализованная сделка_ID_Сделка_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Реализованная сделка"
    ADD CONSTRAINT "Реализованная сделка_ID_Сделка_fkey" FOREIGN KEY ("ID_Сделка") REFERENCES public."Сделка"("ID_Сделка");


--
-- Name: Сделка Сделка_ID_Клиент_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сделка"
    ADD CONSTRAINT "Сделка_ID_Клиент_fkey" FOREIGN KEY ("ID_Клиент") REFERENCES public."Клиент"("ID_Клиент");


--
-- Name: Сделка Сделка_ID_Место_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сделка"
    ADD CONSTRAINT "Сделка_ID_Место_fkey" FOREIGN KEY ("ID_Место") REFERENCES public."Место хранения"("ID_Место");


--
-- Name: Сделка Сделка_ID_Состояние_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сделка"
    ADD CONSTRAINT "Сделка_ID_Состояние_fkey" FOREIGN KEY ("ID_Состояние") REFERENCES public."Состояние товара"("ID_Состояние");


--
-- Name: Сделка Сделка_ID_Сотрудник_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сделка"
    ADD CONSTRAINT "Сделка_ID_Сотрудник_fkey" FOREIGN KEY ("ID_Сотрудник") REFERENCES public."Сотрудник"("ID_Сотрудник");


--
-- Name: Сделка Сделка_ID_Товар_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Сделка"
    ADD CONSTRAINT "Сделка_ID_Товар_fkey" FOREIGN KEY ("ID_Товар") REFERENCES public."Товар"("ID_Товар");


--
-- Name: Товар Товар_ID_Категория_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Товар"
    ADD CONSTRAINT "Товар_ID_Категория_fkey" FOREIGN KEY ("ID_Категория") REFERENCES public."Категория товара"("ID_Категория");


--
-- Name: Товар Товар_ID_Производитель_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Товар"
    ADD CONSTRAINT "Товар_ID_Производитель_fkey" FOREIGN KEY ("ID_Производитель") REFERENCES public."Производитель"("ID_Производитель");


--
-- PostgreSQL database dump complete
--

