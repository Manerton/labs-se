CREATE OR REPLACE PROCEDURE public.calculatecommission(IN "_Сумма" numeric, IN "_Дата" date, IN "_ID_Заказчик" integer, IN "_ID_Агент" integer)
    LANGUAGE 'plpgsql'
    
AS $BODY$
declare "orderCount" integer;
"summaOfOrders" real; 
"startSumma" real; 
"result" real; 

BEGIN
"startSumma" := "_Сумма" * 0.1;
"result" := "startSumma";
"orderCount" := (SELECT COUNT(*) FROM "Заказ" WHERE "ID_Агент" = "_ID_Агент");
"summaOfOrders" := (SELECT SUM("Сумма") FROM "Заказ" WHERE "ID_Агент" = "_ID_Агент");

IF ("orderCount" > 3 AND "orderCount" <= 10)
THEN "result" := "startSumma" + "startSumma" * 0.1;
ELSEIF ("orderCount" > 10)
THEN "result" := "startSumma" + "startSumma" * 0.3;
END IF;

IF ("summaOfOrders" > 1000000)
THEN "result" := "startSumma" + "startSumma" * 0.5;
END IF;

IF (EXTRACT(DOW FROM CAST("_Дата" AS TIMESTAMP)) = 0 OR 
	(CONCAT(EXTRACT(DAY FROM CAST("_Дата" AS TIMESTAMP)), EXTRACT(MONTH FROM CAST("_Дата" AS TIMESTAMP))) IN ('0101','0701','2302','0803','3004','0105','0905','1206','0411')))
THEN "result" := "startSumma" * 2;
END IF;

UPDATE "Агент" SET "Комиссионные" = "result" WHERE "ID_Агент" = "_ID_Агент";
END
$BODY$;