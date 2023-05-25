-- SQL skript pro vytvo?en?­ z?kladn?­ch objekt? sch?matu datab?ze.
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
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
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

--INSERT

INSERT INTO "Agency/Artist" ("name", "email", "client_t")
VALUES ('Dmytro', 'xsadov06@gmail.com', 'artist');

INSERT INTO "Agency/Artist" ("name", "email", "client_t")
VALUES ('Eduard', 'xgalli01@gmail.com', 'agency');

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
VALUES ('A New Look at Old Masters', TO_DATE('12-12-2020', 'DD-MM-YYYY'), TO_DATE('12-12-2021', 'DD-MM-YYYY'), '1', '1','2');

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

--SELECT

-- Za jake expozicni mista zam?stnanec Alexander Serov zodpovedny?
-- spojen? 2 tabulek
-- (id, room_id)
SELECT "Expousure_place"."id", "Expousure_place"."room_id" FROM "Expousure_place" JOIN "Employee" ON "Employee"."id" = "r_employee_id"  WHERE "name" = 'Alexander Serov' ORDER BY "Expousure_place"."id";

-- Jaka dila k expozici A New Look at Old Masters patri?
-- spojen? 2 tabulek
-- (id, name)
SELECT "Work"."id", "Work"."name" FROM "Work" JOIN "Exposition" ON "Work"."exp_id" = "Exposition"."id"  WHERE "Exposition"."name" = 'A New Look at Old Masters';

--Uka? zamn?stnance a jejich telefonn? ??slo kter? pracovali na v?stav?ch v roce 2020
-- spojeni 3 tabulek
-- (name, number)
SELECT "Employee"."name", "number" FROM "Employee" JOIN "Tel_number" ON "Employee"."id" = "Tel_number"."tel_id" JOIN "Exposition" ON "Employee"."id" = "Exposition"."employee_id" WHERE "begining_time" BETWEEN TO_DATE('01-01-2020', 'DD-MM-YYYY') AND TO_DATE('01-01-2021', 'DD-MM-YYYY');

-- Ktera expozice ma nejvetsi pocet del a kolik jich je
-- klauzule GROUP BY s pouzitim agregacni funkce
-- (name, count)
SELECT "Exposition"."name" AS "name", COUNT("Exposition"."id") AS "count"
    FROM "Exposition" JOIN "Work" ON "Work"."exp_id" = "Exposition"."id"
    GROUP BY "Exposition"."name"
    HAVING COUNT("Exposition"."id") >= ALL (
        SELECT COUNT("Exposition"."id")
        FROM "Exposition" JOIN "Work" ON "Work"."exp_id" = "Exposition"."id"
        GROUP BY "Exposition"."id");

--vr?? seznam autor? a po?et jejich d?l
-- klauzule GROUP BY s pouzitim agregacni funkce
-- (autor, name)
SELECT "autor", COUNT("id") AS "count" FROM "Work" GROUP BY "autor";

--vr?? seznam pracovn?k?, pokud je mezi nimi alespo? jeden pr?vodce a sta?er
-- predikat EXISTS
-- (id, name, position, email)
SELECT * FROM "Employee" WHERE EXISTS(SELECT * FROM "Employee" WHERE "position" = 'guide' OR "position" = 'trainee');

--Ktery ze zamestnancu neni odpovedny za zadne expozicni misto
-- predikat IN s vnorenym SELECT
-- (id, name)
SELECT "Employee"."id", "Employee"."name" FROM "Employee" WHERE "Employee"."id" NOT IN (
	SELECT DISTINCT "Employee"."id" FROM "Employee" JOIN "Expousure_place" ON "Employee"."id" = "Expousure_place"."r_employee_id"
);
