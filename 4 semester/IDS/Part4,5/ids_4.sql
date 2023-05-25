-- SQL skript pro vytvo?en?� z?kladn?�ch objekt? sch?matu datab?ze.
-----------------------------------------------------------------
-- Autor: Dmytro Sadovskyi <xsadov06@stud.fit.vutbr.cz>.
-- Autor: Galliamov Eduard <xgalli01@stud.fit.vutbr.cz>.

--DROP

DROP TABLE "Agency/Artist" CASCADE CONSTRAINTS;
DROP TABLE "Work" CASCADE CONSTRAINTS;
DROP TABLE "Exposition" CASCADE CONSTRAINTS;
DROP TABLE "Expousure_place" CASCADE CONSTRAINTS;
DROP TABLE "Room" CASCADE CONSTRAINTS;
DROP TABLE "Tel_number" CASCADE CONSTRAINTS;
DROP TABLE "Employee" CASCADE CONSTRAINTS;
DROP TABLE "Equipment" CASCADE CONSTRAINTS;
DROP TABLE "Lightning" CASCADE CONSTRAINTS;
DROP TABLE "Rack" CASCADE CONSTRAINTS;
DROP MATERIALIZED VIEW "expositions";
DROP SEQUENCE "exposition_seq";
DROP INDEX "clients_ind";

--CREATE

CREATE TABLE "Agency/Artist" (
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"name" VARCHAR(80) NOT NULL,
	"email" VARCHAR(255) NOT NULL
		CHECK(REGEXP_LIKE(
			"email", '^[a-z]+[a-z0-9\.]*@[a-z0-9\.-]+\.[a-z]{2,}$', 'i'
		)),
    "client_t" VARCHAR(80) NOT NULL
);

CREATE TABLE "Employee" (
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"name" VARCHAR(80) NOT NULL,
	"position" VARCHAR(80) NOT NULL,
	"email" VARCHAR(255) NOT NULL
		CHECK(REGEXP_LIKE(
			"email", '^[a-z]+[a-z0-9\._]*@[a-z0-9\.-]+\.[a-z]{2,}$', 'i'
		))
);

CREATE TABLE "Exposition" (
	"id" INT DEFAULT NULL PRIMARY KEY,
    "name" VARCHAR(80) NOT NULL,
	"begining_time" DATE NOT NULL,
	"end_time" DATE NOT NULL,
    "pay_stat" CHAR(1) NOT NULL,
    "client_id" INT NOT NULL,
    "employee_id" INT DEFAULT NULL,
    CONSTRAINT "fk_client_id"
		FOREIGN KEY ("client_id") REFERENCES "Agency/Artist" ("id")
        ON DELETE SET NULL,
    CONSTRAINT "fk_employee_id"
		FOREIGN KEY ("employee_id") REFERENCES "Employee" ("id")
        ON DELETE SET NULL
);

CREATE TABLE "Work" (
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"name" VARCHAR(80) NOT NULL,
	"creation date" VARCHAR(80) DEFAULT NULL,
	"autor" VARCHAR(80) DEFAULT NULL,
	"style" VARCHAR(80) DEFAULT NULL,
	"price" INT DEFAULT NULL,
    "exp_id" INT NOT NULL,
    CONSTRAINT "fk_exp_id"
		FOREIGN KEY ("exp_id") REFERENCES "Exposition" ("id")
        ON DELETE SET NULL
);

CREATE TABLE "Room" (
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"capacity" INT NOT NULL,
    "name" VARCHAR(15) NOT NULL
);

CREATE TABLE "Expousure_place" (
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"width" INT NOT NULL,
	"length" INT NOT NULL,
    "exp_place_t" VARCHAR(80) NOT NULL,
	"price" INT NOT NULL,
    "room_id" INT NOT NULL,
    "r_employee_id" INT NOT NULL,
    "exposition_id" INT DEFAULT NULL,
    CONSTRAINT "fk_room_id"
		FOREIGN KEY ("room_id") REFERENCES "Room" ("id")
        ON DELETE SET NULL,
    CONSTRAINT "fk_r_employee_id"
		FOREIGN KEY ("r_employee_id") REFERENCES "Employee" ("id")
        ON DELETE SET NULL,
    CONSTRAINT "fk_exposition_id"
		FOREIGN KEY ("exposition_id") REFERENCES "Exposition" ("id")
        ON DELETE SET NULL
);

CREATE TABLE "Tel_number" (
	"tel_id" INT NOT NULL,
	"number" VARCHAR(20) NOT NULL
		CHECK(REGEXP_LIKE(
			"number", '^\+?[1-9][0-9]{7,14}$', 'i'
		)),
	CONSTRAINT "fk_emp_id"
		FOREIGN KEY ("tel_id") REFERENCES "Employee" ("id")
        ON DELETE SET NULL
);

CREATE TABLE "Lightning" (
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"type" VARCHAR(80) NOT NULL,
	"brightness" SMALLINT NOT NULL
);

CREATE TABLE "Rack" (
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"width" INT NOT NULL,
	"length" INT NOT NULL
);

CREATE TABLE "Equipment" (
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"price" INT NOT NULL,
	"Date of acquisition" DATE NOT NULL,
	"lightning_id" INT DEFAULT NULL,
	"rack_id" INT DEFAULT NULL,
    "place_id" INT DEFAULT NULL,
    CONSTRAINT "fk_lightning_id"
		FOREIGN KEY ("lightning_id") REFERENCES "Lightning" ("id")
        ON DELETE SET NULL,
	CONSTRAINT "equ_rack_id"
		FOREIGN KEY ("rack_id") REFERENCES "Rack" ("id")
		ON DELETE SET NULL,
    CONSTRAINT "fk_place_id"
		FOREIGN KEY ("place_id") REFERENCES "Expousure_place" ("id")
        ON DELETE SET NULL
);

-- TRIGGERS

-- Kontrola, ?e datum ukon?en? expozici je v?t?? ne? datum zah?jen? 
CREATE OR REPLACE TRIGGER "trigger_date"
	AFTER INSERT OR UPDATE ON "Exposition"
	FOR EACH ROW
BEGIN
	IF	:new."begining_time" is null OR
		:new."end_time" is null OR 
		:new."begining_time" > :new."end_time"
	THEN
		Raise_Application_Error(-20111, 'Datum zah?jen? mus? b?t men?? ne? datum ukon?en?');
	END IF;
END;
/

-- Generov?n? prim?rn?ho kl??e tabulky Employee.
CREATE SEQUENCE "exposition_seq";
CREATE OR REPLACE TRIGGER "exposition_id" BEFORE INSERT ON "Exposition" FOR EACH ROW WHEN (NEW."id" IS NULL)
BEGIN
	:NEW."id" := "exposition_seq".NEXTVAL;
END;
/

--INSERT

INSERT INTO "Agency/Artist" ("name", "email", "client_t")
VALUES ('Dmytro', 'xsadov06@gmail.com', 'artist');

INSERT INTO "Agency/Artist" ("name", "email", "client_t")
VALUES ('Eduard', 'xgalli01@gmail.com', 'agency');

INSERT INTO "Agency/Artist" ("name", "email", "client_t")
VALUES ('Bodrow', 'lli01@gmail.com', 'agency');

INSERT INTO "Agency/Artist" ("name", "email", "client_t")
VALUES ('Smia', 'smia@gmail.com', 'agency');

INSERT INTO "Agency/Artist" ("name", "email", "client_t")
VALUES ('RarA', 'rara_help@gmail.com', 'agency');

INSERT INTO "Employee" ("name", "position", "email")
VALUES ('Alexander Serov', 'guide', 'rando_mail@mail.com');

INSERT INTO "Employee" ("name", "position", "email")
VALUES ('Jeff Bezos', 'guide', 'rando_mail2@amazon.com');

INSERT INTO "Employee" ("name", "position", "email")
VALUES ('Serdar Berdimuhamedow', 'trainee', 'serdar@mail.com');

INSERT INTO "Employee" ("name", "position", "email")
VALUES ('Stephane Breitweiser ', 'trainee', 'breitweiser@mail.com');

INSERT INTO "Exposition" ("name", "begining_time", "end_time", "pay_stat", "client_id", "employee_id")
VALUES ('The African Origin of Civilization', TO_DATE('12-06-2021', 'DD-MM-YYYY'), TO_DATE('14-06-2022', 'DD-MM-YYYY'), '0', '1', '1');

INSERT INTO "Exposition" ("name", "begining_time", "end_time", "pay_stat", "client_id", "employee_id")
VALUES ('A New Look at Old Masters', TO_DATE('12-12-2020', 'DD-MM-YYYY'), TO_DATE('12-12-2021', 'DD-MM-YYYY'), '1', '2','2');

INSERT INTO "Exposition" ("name", "begining_time", "end_time", "pay_stat", "client_id", "employee_id")
VALUES ('Eduards works', TO_DATE('12-11-2020', 'DD-MM-YYYY'), TO_DATE('03-08-2021', 'DD-MM-YYYY'), '1', '1','1');

INSERT INTO "Exposition" ("name", "begining_time", "end_time", "pay_stat", "client_id", "employee_id")
VALUES ('3 works', TO_DATE('12-11-2020', 'DD-MM-YYYY'), TO_DATE('03-08-2021', 'DD-MM-YYYY'), '1', '2','2');

INSERT INTO "Exposition" ("name", "begining_time", "end_time", "pay_stat", "client_id", "employee_id")
VALUES ('1 work', TO_DATE('12-11-2020', 'DD-MM-YYYY'), TO_DATE('03-08-2021', 'DD-MM-YYYY'), '1', '2','2');

INSERT INTO "Work" ("name", "exp_id")
VALUES ('First', '5');

INSERT INTO "Work" ("name", "exp_id")
VALUES ('One', '4');
INSERT INTO "Work" ("name", "exp_id")
VALUES ('Two', '4');
INSERT INTO "Work" ("name", "exp_id")
VALUES ('Three', '4');

INSERT INTO "Work" ("name", "creation date", "autor", "style", "exp_id")
VALUES ('Seated Male Nude in Three-Quarter View, with Right Arm Extended to the Left', TO_DATE('1774', 'YYYY'), 'Jacques Louis David', 'Radical Draftsman', '2');

INSERT INTO "Work" ("name", "creation date", "autor", "style", "exp_id")
VALUES ('The Combat of Diomedes', TO_DATE('1776', 'YYYY'), 'Jacques Louis David', 'Radical Draftsman','2');

INSERT INTO "Work" ("name", "creation date", "autor", "exp_id")
VALUES ('Plank piece I and II', TO_DATE('1973', 'YYYY'), 'Charles Ray', '1');

INSERT INTO "Work" ("name", "creation date", "autor", "exp_id")
VALUES ('Tractor', TO_DATE('2005', 'YYYY'), 'Charles Ray', '2');

INSERT INTO "Room" ("name","capacity")
VALUES ('a03','30');

INSERT INTO "Room" ("name","capacity")
VALUES ('a05','42');

INSERT INTO "Expousure_place" ("width", "length", "exp_place_t", "price", "room_id", "r_employee_id","exposition_id")
VALUES ('600', '300', 'wall', '500', '1', '2', '1');

INSERT INTO "Expousure_place" ("width", "length", "exp_place_t", "price", "room_id", "r_employee_id","exposition_id")
VALUES ('560', '290', 'wall', '2700', '2', '2', '1');

INSERT INTO "Expousure_place" ("width", "length", "exp_place_t", "price", "room_id", "r_employee_id","exposition_id")
VALUES ('650', '200', 'flor', '2500', '1', '2', '1');

INSERT INTO "Expousure_place" ("width", "length", "exp_place_t", "price", "room_id", "r_employee_id","exposition_id")
VALUES ('200', '80', 'flor', '200', '2', '1', '2');

INSERT INTO "Expousure_place" ("width", "length", "exp_place_t", "price", "room_id", "r_employee_id","exposition_id")
VALUES ('210', '100', 'wall', '270', '2', '1', '2');

INSERT INTO "Tel_number" ("tel_id", "number")
VALUES ('1', '+420694204242');

INSERT INTO "Tel_number" ("tel_id", "number")
VALUES ('2', '+420880055535');

INSERT INTO "Lightning" ("type", "brightness")
VALUES ('wall', '3200');

INSERT INTO "Lightning" ("type", "brightness")
VALUES ('flor', '300');

INSERT INTO "Rack" ("width", "length")
VALUES ('600', '300');

INSERT INTO "Rack" ("width", "length")
VALUES ('200', '80');

INSERT INTO "Equipment" ("price", "Date of acquisition", "lightning_id")
VALUES ('500', TO_DATE('15-07-2004', 'DD-MM-YYYY'), '1');

INSERT INTO "Equipment" ("price", "Date of acquisition", "rack_id")
VALUES ('300', TO_DATE('08-11-2009', 'DD-MM-YYYY'), '1');


-- P?edveden? triggeru

-- Triger 1
-- The African Origin of Civilization ma id 1, New Look at Old Masters 2, Eduards works 3
SELECT "id", "name", "begining_time", "end_time"
FROM "Exposition"
ORDER BY "id";

-- Triger 2
-- Chybn? datum - vyhodi chybu
INSERT INTO "Exposition" ("name", "begining_time", "end_time", "pay_stat", "client_id", "employee_id")
VALUES ('TEST BAD EXP', TO_DATE('14-06-2022', 'DD-MM-YYYY'), TO_DATE('12-06-2021', 'DD-MM-YYYY'), '0', '1', '1');

-- PROCEDURE

-- Procedura vyp??e p?ehled expozice, po?et dil, a zisk
CREATE OR REPLACE PROCEDURE "profit"
AS
	"exp_count" NUMBER;
	"works_count" NUMBER;
	"zisk" NUMBER;
    "avg_prof_per_work" NUMBER;
    
BEGIN
	SELECT COUNT("id") INTO "exp_count" FROM "Exposition";
	SELECT COUNT("id") INTO "works_count" FROM "Work";
	SELECT SUM("price") INTO "zisk" FROM "Expousure_place" WHERE "exposition_id" IS NOT NULL;
    
    "avg_prof_per_work" := "zisk"/"works_count";
    
	DBMS_OUTPUT.PUT_LINE(
		'Pocet expozic - '
		|| "exp_count" || ', pocet dil - '
		|| "works_count" || ', zisk - '
		|| "zisk" || '.'
	);
    
    DBMS_OUTPUT.put_line(
		'Zisk na jedne dilo '
		|| "avg_prof_per_work"
	);
    
	EXCEPTION WHEN ZERO_DIVIDE THEN
	BEGIN
		IF "works_count" = 0 THEN
			DBMS_OUTPUT.put_line('Zisk se rovna 0');
		END IF;
	END;
END;
/
-- spou?t?n?
set serveroutput on
BEGIN "profit"; END;
/

CREATE OR REPLACE PROCEDURE "emp_count"
AS
    CURSOR "name_list" IS SELECT "name" FROM "Employee";
    "count" NUMBER;
    "iter" "Employee"."name"%TYPE;
BEGIN
    IF "name_list" %ISOPEN THEN
        CLOSE "name_list";
    END IF;
    OPEN "name_list";

    "count" := 0;

    LOOP
        FETCH "name_list" INTO "iter";
        EXIT WHEN "name_list"%NOTFOUND;
        "count" := "count" + 1;
    END LOOP;

    CLOSE "name_list";

    DBMS_OUTPUT.PUT_LINE('Pracovniků celkem: ' || "count" || '.');
END;
/
set serveroutput on
BEGIN "emp_count"; END;
/

-- EXPLAIN PLAN

-- Kte?a expozici zakaznika Eduard maji alespon 3 dila, a kolik.
EXPLAIN PLAN FOR
SELECT
	"e"."name",
	COUNT("w"."id") AS "count"
FROM "Exposition" "e"
JOIN "Work" "w" ON "w"."exp_id" = "e"."id"
JOIN "Agency/Artist" "a" ON "a"."id" = "e"."client_id"
WHERE "a"."name" = 'Eduard'
GROUP BY "e"."name"
HAVING (COUNT("w"."exp_id") > 2)
ORDER BY "e"."name";

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);

CREATE INDEX "clients_ind" ON "Agency/Artist" ("name");

-- druh? pokus
EXPLAIN PLAN FOR
SELECT
	"e"."name",
	COUNT("w"."id") AS "count"
FROM "Exposition" "e"
JOIN "Work" "w" ON "w"."exp_id" = "e"."id"
JOIN "Agency/Artist" "a" ON "a"."id" = "e"."client_id"
WHERE "a"."name" = 'Eduard'
GROUP BY "e"."name"
HAVING (COUNT("w"."exp_id") > 2)
ORDER BY "e"."name";

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);


-- Materializovan? pohled na v?echny expozice a po?et jejich dil.

CREATE MATERIALIZED VIEW "expositions" AS
SELECT
	"Exposition"."id",
	"Exposition"."name",
	COUNT("Work"."id") AS "works_count" FROM "Exposition" JOIN "Work" ON "Work"."exp_id" = "Exposition"."id" GROUP BY "Exposition"."name", "Exposition"."id" ORDER BY "Exposition"."id";

-- V?pis
SELECT * FROM "expositions";

-- Aktualizace
UPDATE "Work" SET "exp_id" = 2 WHERE "id" = 1;

-- Data se v materializovan?m pohledu neaktualizuj?.
SELECT * FROM "expositions";


-- PRIVILEGES
GRANT ALL ON "Agency/Artist" TO xgalli01;
GRANT ALL ON "Work" TO xgalli01;
GRANT ALL ON "Exposition" TO xgalli01;
GRANT ALL ON "Expousure_place" TO xgalli01;
GRANT ALL ON "Room" TO xgalli01;
GRANT ALL ON "Tel_number" TO xgalli01;
GRANT ALL ON "Employee" TO xgalli01;
GRANT ALL ON "Equipment" TO xgalli01;
GRANT ALL ON "Lightning" TO xgalli01;
GRANT ALL ON "Rack" TO xgalli01;
--GRANT EXECUTE ON "" TO xgalli01; --CHANGE
--GRANT EXECUTE ON "" TO xgalli01;			  --CHANGE
GRANT ALL ON "expositions" TO xgalli01;
