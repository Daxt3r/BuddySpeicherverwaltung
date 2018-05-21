# BuddySpeicherverwaltung
Projekt für das Fach BS&amp;RN

# Projektbeschreibung
Das Projekt simuliert die Buddy-Speicherverwaltung. Das Projekt wird in der Sprache C mit der IDE Visual Studio 2017 geschrieben.

# Aktueller Features
- Prozesse können erstellt werden
- Prozesse können beendet werden
- Alle Prozesse können auf einmal beendet werden
- Statistik über
  + Laufende Prozesse
  + Verfügbare Speicherblöcke (noch in Bearbeitung)
  + Belegter Speicher / freier Speicher wird angezeigt

# Noch nicht implementiert
 - Möglichkeit nach Menüpunktauswahl, wieder zurück ins Menü zu kehren
 - Bei Starten eines neuen Prozesses, prüfen ob überhaupt noch genügen Speicher zur Verfügung steht
 - Die Nummer für Default Prozessnamen wird noch nicht hoch gezählt

# Bekannte Fehler
 - Es werden noch nicht alle freien Buddys bei Ausgabe der Statistik erfasst
 - Wenn zu viele Prozesse mit der größe von 1 erstellt werden, wird das Programm abstürzen
