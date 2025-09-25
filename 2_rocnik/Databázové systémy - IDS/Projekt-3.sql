-- Vytvoření tabulek
DROP TABLE Pojistovna CASCADE CONSTRAINTS;
DROP TABLE Prispevek CASCADE CONSTRAINTS;
DROP TABLE Zakaznik CASCADE CONSTRAINTS;
DROP TABLE Predpis CASCADE CONSTRAINTS;
DROP TABLE Lek CASCADE CONSTRAINTS;
DROP TABLE Lek_na_predpis CASCADE CONSTRAINTS;
DROP TABLE Lek_bez_predpisu CASCADE CONSTRAINTS;

CREATE TABLE Pojistovna (
    Kod NUMBER(3) PRIMARY KEY,
    Nazev VARCHAR2(50) NOT NULL,
    Adresa VARCHAR2(50) NOT NULL
);

CREATE TABLE Prispevek (
    Kod_pojistovny NUMBER(3) NOT NULL,
    ID_leku NUMBER(10) NOT NULL,
    Prispevek NUMBER(10,2) NOT NULL,
    CONSTRAINT Prispevek_PK PRIMARY KEY (Kod_pojistovny, ID_leku)
);

CREATE TABLE Zakaznik (
  Cislo_pojistence VARCHAR2(10) PRIMARY KEY,
  CHECK (
    LENGTH(REGEXP_SUBSTR(Cislo_pojistence, '^[0-9]+$')) IN (9,10)
    AND (
        SUBSTR(Cislo_pojistence, 3, 2) BETWEEN 1 AND 12
        OR SUBSTR(Cislo_pojistence, 3, 2) BETWEEN 51 AND 62
    )
    AND (
        (SUBSTR(Cislo_pojistence, 3, 2) != 2 AND TO_NUMBER(SUBSTR(Cislo_pojistence, 3, 2)) IN (1,3,5,7,8,10,12) AND SUBSTR(Cislo_pojistence, 5, 2) BETWEEN 1 AND 31)
        OR (SUBSTR(Cislo_pojistence, 3, 2) != 2 AND TO_NUMBER(SUBSTR(Cislo_pojistence, 3, 2)) IN (4, 6, 9, 11) AND SUBSTR(Cislo_pojistence, 5, 2) BETWEEN 1 AND 30)
        OR (SUBSTR(Cislo_pojistence, 3, 2) = 2 AND MOD(TO_NUMBER(SUBSTR(Cislo_pojistence, 1, 2)), 4) = 0 AND SUBSTR(Cislo_pojistence, 5, 2) BETWEEN 1 AND 29)
        OR (SUBSTR(Cislo_pojistence, 3, 2) = 2 AND MOD(TO_NUMBER(SUBSTR(Cislo_pojistence, 1, 2)), 4) != 0 AND SUBSTR(Cislo_pojistence, 5, 2) BETWEEN 1 AND 28)
    )
    AND(LENGTH(Cislo_pojistence) = 9 OR MOD(TO_NUMBER(Cislo_pojistence), 11) = 0)
   ),

  Jmeno VARCHAR2(30) NOT NULL,
  Prijmeni VARCHAR2(30) NOT NULL,
  Adresa VARCHAR2(50) NOT NULL,
  Datum_narozeni DATE NOT NULL,
  Kod_pojistovny NUMBER(3) NOT NULL
);

CREATE TABLE Predpis (
    ID NUMBER(10) GENERATED ALWAYS AS IDENTITY (START WITH 1 INCREMENT BY 1) PRIMARY KEY,
    Cislo_pojistence VARCHAR2(20) NOT NULL,
    ID_leku NUMBER(10) NOT NULL,
    Datum_vystaveni DATE NOT NULL,
    Je_vydyno NUMBER(1) NOT NULL --0 = Nevydáno, 1 = Vydáno
);

-- generalizace/specializace řešena dědičností, kde tabulky "Lek_na_predpis" a "Lek_bez_predpisu" dědí z tabulyk "Lek"
-- konfliktu primarních klíču jsem zamezil triggery

CREATE TABLE Lek(
  ID_leku INT PRIMARY KEY,
  Nazev VARCHAR2(255) NOT NULL,
  Cena NUMBER(10,2) NOT NULL,
  Pocet_na_sklade INTEGER NOT NULL
);

CREATE TABLE Lek_na_predpis (
  ID_leku INT PRIMARY KEY
);

CREATE TABLE Lek_bez_predpisu (
  ID_leku INT PRIMARY KEY,
  Kategorie_pouziti VARCHAR2(255) NOT NULL,
  Uzivani VARCHAR2(255) NOT NULL
);

ALTER TABLE Prispevek ADD CONSTRAINT Prispevek_Pojistovna_FK FOREIGN KEY (Kod_pojistovny) REFERENCES Pojistovna (Kod);
ALTER TABLE Prispevek ADD CONSTRAINT Prispevek_Lek_FK FOREIGN KEY (ID_leku) REFERENCES Lek_na_predpis (ID_leku);
ALTER TABLE Zakaznik ADD CONSTRAINT Zakaznik_Pojistovna_FK FOREIGN KEY (Kod_pojistovny) REFERENCES Pojistovna (Kod);
ALTER TABLE Predpis ADD CONSTRAINT Predpis_Zakaznik_FK FOREIGN KEY (Cislo_pojistence) REFERENCES Zakaznik (Cislo_pojistence);
ALTER TABLE Predpis ADD CONSTRAINT Predpis_Lek_FK FOREIGN KEY (ID_leku) REFERENCES Lek_na_predpis (ID_leku);
ALTER TABLE Lek_na_predpis ADD CONSTRAINT Lek_na_predpis_Lek_FK FOREIGN KEY (ID_leku) REFERENCES Lek (ID_leku);
ALTER TABLE Lek_bez_predpisu ADD CONSTRAINT Lek_bez_predpisu_Lek_FK FOREIGN KEY (ID_leku) REFERENCES Lek (ID_leku);


CREATE OR REPLACE TRIGGER check_LekID1
BEFORE INSERT OR UPDATE ON Lek_na_predpis
FOR EACH ROW
DECLARE
    l_count INTEGER;
BEGIN
    SELECT COUNT(*)
    INTO l_count
    FROM (
        SELECT ID_leku FROM Lek_bez_predpisu WHERE ID_leku = :new.ID_leku        
    );
    IF l_count > 0 THEN
        RAISE_APPLICATION_ERROR(-20001, 'ID already exists in Lek_bez_predpisu table');
    END IF;
END;
/

CREATE OR REPLACE TRIGGER check_LekID2
BEFORE INSERT OR UPDATE ON Lek_bez_predpisu
FOR EACH ROW
DECLARE
    l_count INTEGER;
BEGIN
    SELECT COUNT(*)
    INTO l_count
    FROM (
        SELECT ID_leku FROM Lek_na_predpis WHERE ID_leku = :new.ID_leku        
    );
    IF l_count > 0 THEN
        RAISE_APPLICATION_ERROR(-20001, 'ID already exists in Lek_na_predpis table');
    END IF;
END;
/

--vkládání dat

INSERT INTO Pojistovna (Kod, Nazev, Adresa) VALUES (212, 'Zdraví Plus Pojišťovna', 'Pražská 123, Praha');
INSERT INTO Pojistovna (Kod, Nazev, Adresa) VALUES (225, 'Pojištění Centrum', 'Brněnská 456, Brno');
INSERT INTO Pojistovna (Kod, Nazev, Adresa) VALUES (310, 'Univerzální Zdravotní Pojišťovna', 'Ostravská 789, Ostrava');


INSERT INTO Lek (ID_leku, Nazev, Cena, Pocet_na_sklade) VALUES (1, 'Paralen', 50.00, 200);
INSERT INTO Lek (ID_leku, Nazev, Cena, Pocet_na_sklade) VALUES (2, 'Ibuprofen', 70.00, 150);
INSERT INTO Lek (ID_leku, Nazev, Cena, Pocet_na_sklade) VALUES (3, 'Amoxicilin', 150.00, 100);
INSERT INTO Lek (ID_leku, Nazev, Cena, Pocet_na_sklade) VALUES (4, 'Aspirin', 60.00, 300);
INSERT INTO Lek (ID_leku, Nazev, Cena, Pocet_na_sklade) VALUES (5, 'Xanax', 500.00, 250);
INSERT INTO Lek (ID_leku, Nazev, Cena, Pocet_na_sklade) VALUES (6, 'Lexaurin', 450.00, 350);
INSERT INTO Lek (ID_leku, Nazev, Cena, Pocet_na_sklade) VALUES (7, 'Laughitol', 500.00, 100);


INSERT INTO Lek_na_predpis (ID_leku) VALUES (5);
INSERT INTO Lek_na_predpis (ID_leku) VALUES (6);
INSERT INTO Lek_na_predpis (ID_leku) VALUES (7);


INSERT INTO Lek_bez_predpisu (ID_leku, Kategorie_pouziti, Uzivani) VALUES (1, 'Analgetika', 'Užívat každých 6 hodin pro úlevu od bolesti');

INSERT INTO Lek_bez_predpisu (ID_leku, Kategorie_pouziti, Uzivani) VALUES (2, 'Protizánětlivé', 'Užívat 1 tabletu dvakrát denně po jídle');

INSERT INTO Lek_bez_predpisu (ID_leku, Kategorie_pouziti, Uzivani) VALUES (3, 'Antibiotika', 'Užívat 1 tabletu třikrát denně po jídle');

INSERT INTO Lek_bez_predpisu (ID_leku, Kategorie_pouziti, Uzivani) VALUES (4, 'Analgetika', 'Užívat každých 4 až 6 hodin pro úlevu od bolesti, ne více než 4 tablety denně');


INSERT INTO Zakaznik (Cislo_pojistence, Jmeno, Prijmeni, Adresa, Datum_narozeni, Kod_pojistovny) VALUES 
('8004150000', 'Jan', 'Novák', 'Pražská 10, Praha', TO_DATE('1980-04-15', 'YYYY-MM-DD'), 225);

INSERT INTO Zakaznik (Cislo_pojistence, Jmeno, Prijmeni, Adresa, Datum_narozeni, Kod_pojistovny) VALUES 
('7507220006', 'Eva', 'Svobodová', 'Brněnská 20, Brno', TO_DATE('1975-07-22', 'YYYY-MM-DD'), 310);

INSERT INTO Zakaznik (Cislo_pojistence, Jmeno, Prijmeni, Adresa, Datum_narozeni, Kod_pojistovny) VALUES 
('9001300000', 'Petr', 'Kovařík', 'Ostravská 30, Ostrava', TO_DATE('1990-01-30', 'YYYY-MM-DD'), 212);

INSERT INTO Zakaznik (Cislo_pojistence, Jmeno, Prijmeni, Adresa, Datum_narozeni, Kod_pojistovny) VALUES 
('8505140006', 'Martina', 'Pechová', 'Olomoucká 40, Olomouc', TO_DATE('1985-05-14', 'YYYY-MM-DD'), 225);

INSERT INTO Zakaznik (Cislo_pojistence, Jmeno, Prijmeni, Adresa, Datum_narozeni, Kod_pojistovny) VALUES 
('8211020004', 'David', 'Vrba', 'Plzeňská 50, Plzeň', TO_DATE('1982-11-02', 'YYYY-MM-DD'), 310);

INSERT INTO Zakaznik (Cislo_pojistence, Jmeno, Prijmeni, Adresa, Datum_narozeni, Kod_pojistovny) VALUES 
('7909190003', 'Lucie', 'Horáková', 'Liberecká 60, Liberec', TO_DATE('1979-09-19', 'YYYY-MM-DD'), 212);

INSERT INTO Predpis (Cislo_pojistence, ID_leku, Datum_vystaveni, Je_vydyno) VALUES 
('8211020004', 5, TO_DATE('2024-03-15', 'YYYY-MM-DD'), 0);

INSERT INTO Predpis (Cislo_pojistence, ID_leku, Datum_vystaveni, Je_vydyno) VALUES 
('7909190003', 6, TO_DATE('2024-03-16', 'YYYY-MM-DD'), 0);

INSERT INTO Predpis (Cislo_pojistence, ID_leku, Datum_vystaveni, Je_vydyno) VALUES 
('9001300000', 5, TO_DATE('2024-03-17', 'YYYY-MM-DD'), 1);

INSERT INTO Prispevek (Kod_pojistovny, ID_leku, Prispevek) VALUES (225, 6, 30.00);
INSERT INTO Prispevek (Kod_pojistovny, ID_leku, Prispevek) VALUES (225, 5, 200.00);
INSERT INTO Prispevek (Kod_pojistovny, ID_leku, Prispevek) VALUES (310, 5, 20.00);
INSERT INTO Prispevek (Kod_pojistovny, ID_leku, Prispevek) VALUES (310, 6, 250.00);
INSERT INTO Prispevek (Kod_pojistovny, ID_leku, Prispevek) VALUES (310, 7, 150.00); 
INSERT INTO Prispevek (Kod_pojistovny, ID_leku, Prispevek) VALUES (212, 6, 25.00);
INSERT INTO Prispevek (Kod_pojistovny, ID_leku, Prispevek) VALUES (212, 5, 220.00); 
INSERT INTO Prispevek (Kod_pojistovny, ID_leku, Prispevek) VALUES (212, 7, 250.00); 


COMMIT;

--SELECT SPOJENI DVOU TABULEK
--Select pro zobrazení pojistenců a jejich pojištoven
SELECT Cislo_pojistence, Jmeno, Prijmeni, Nazev FROM Zakaznik Z, Pojistovna P WHERE Z.Kod_pojistovny = P.Kod;

--Select pro vypsání léků na které jsou příspěvky
SELECT DISTINCT Nazev, Cena FROM Lek NATURAL JOIN Prispevek;

--SELECT SPOJENI TRI TABULEK
--Select pro vybrání zákazníka jejich předepsané léky a kdy byl vystaven předpis na tyto leky
SELECT Jmeno, Prijmeni, Nazev, Datum_vystaveni From Zakaznik NATURAL JOIN  PREDPIS NATURAL JOIN LEK;

--SELEV GROUP BY A AGREGACNI FUNCKCE
--Select pro vypsání počtu předpisů, které byly vypsány pro určité léky
SELECT Nazev, COUNT(*) Pocet_predpisu FROM Lek NATURAL JOIN Predpis GROUP BY Nazev;

--Select pro vypsání celkové částky, kterou  přispívají pojištovny na jednotlivé léky
SELECT Nazev, Sum(Prispevek) Celkovy_prispevek FROM LEK NATURAL JOIN Prispevek GROUP By Nazev;

--SELECT exist
--Select pro vypsání zákazníků, kteří nikdy neměli předepsané žádné léky
SELECT Prijmeni, Jmeno, Datum_narozeni, Cislo_pojistence FROM Zakaznik Z WHERE NOT EXISTS (SELECT * FROM Predpis P WHERE P.Cislo_pojistence = Z.Cislo_pojistence);

--SELECT IN
--Select pro vypsání léků, na které jsou příspevky, ale nidky nebyli předepsány
SELECT ID_leku, Nazev, Pocet_na_sklade FROM Lek Where ID_leku IN ( SELECT ID_leku FROM Prispevek WHERE ID_leku NOT IN (SELECT ID_leku FROM Predpis ));

