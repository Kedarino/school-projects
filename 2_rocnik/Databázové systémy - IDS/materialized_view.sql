CREATE MATERIALIZED VIEW XLIBRA02.Contributions_summary
REFRESH COMPLETE ON COMMIT
AS
SELECT L.Nazev, SUM(P.Prispevek) AS Celkove_prispevky
FROM XSAMEK10.Lek L
JOIN XSAMEK10.Prispevek P ON L.ID_leku = P.ID_leku
GROUP BY L.Nazev;

GRANT ALL ON XLIBRA02.Contributions_summary TO XSAMEK10;
SELECT * FROM XLIBRA02.Contributions_summary;


