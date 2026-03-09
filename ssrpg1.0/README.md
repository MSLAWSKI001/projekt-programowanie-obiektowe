# Clicker RPG - Qt C++

Prosta gra Clicker RPG stworzona w C++ z wykorzystaniem biblioteki Qt.

## Funkcje gry

- **System walki**: Atakuj przeciwników klikając w duży klikalny obszar
- **Statystyki gracza**: Ulepszaj swoje atrybuty (Atak, Obrona, Max HP)
- **System map**: Odblokowuj nowe mapy po pokonaniu 50 wrogów na poprzedniej
- **Przeciwnicy zależni od mapy**: Każda mapa ma silniejsze i bardziej wartościowe potwory
- **System poziomów**: Zdobądź EXP za pokonanie wrogów, awansuj na wyższe poziomy
- **Sklep**: Wydawaj zdobyte złoto na ulepszenia
- **Paski postępu HP i EXP**: Widoczne na dole okna

## Instalacja i uruchomienie w Qt Creator

### Wymagania
- Qt Creator (zaleca się wersję 4.11 lub nowszą)
- Qt 5.15 lub nowszy (zainstalowany z Qt Installer)
- Kompilator C++ (MinGW dla Windows, GCC dla Linux, Clang dla macOS)

### Krok po kroku

1. **Pobierz i zainstaluj Qt Creator**
   - Wejdź na https://www.qt.io/download
   - Pobierz Qt Online Installer
   - Zainstaluj Qt Creator i wybraną wersję Qt (np. Qt 5.15.2)

2. **Skonfiguruj projekt w Qt Creator**
   - Otwórz Qt Creator
   - W menu głównym wybierz: **File → Open File or Project**
   - Przejdź do folderu z projektem i wybierz plik **ClickerRPG.pro**
   - W oknie konfiguracji wybierz odpowiedni kit (np. Desktop Qt 5.15.2 MinGW 64-bit)
   - Kliknij przycisk "Configure Project"

3. **Budowanie i uruchomienie gry**
   - Po załadowaniu projektu kliknij przycisk **Build** (młotek na lewym panelu)
   - Lub naciśnij klawisz **Ctrl+B**
   - Po zakończeniu budowania kliknij przycisk **Run** (zielona strzałka na lewym panelu)
   - Lub naciśnij klawisz **Ctrl+R**

## Struktura projektu

```
ClickerRPG/
├── ClickerRPG.pro          # Plik projektu Qt
├── README.md                # Plik README z instrukcjami
├── src/                     # Katalog z plikami źródłowymi
│   ├── main.cpp             # Punkt wejścia aplikacji
│   ├── mainwindow.h/cpp     # Główne okno i logika gry
│   ├── player.h/cpp         # Klasa gracza z statystykami
│   └── enemy.h/cpp          # Klasa przeciwnika z losowaniem
└── resources/               # Katalog z zasobami
    └── resources.qrc        # Plik zasobów Qt
```

## Jak grać

1. **Walka**: W zakładce "Walka" klikaj w duży klikalny obszar z potworem, aby go zaatakować
2. **Statystyki**: W zakładce "Statystyki" wydawaj zdobyte złoto na ulepszenia
3. **Mapy**: W zakładce "Mapa" wybieraj i odblokowuj nowe lokacje
4. **Odblokowywanie map**: Pokonaj 50 wrogów na jednej mapie, aby odblokować kolejną

## Rozwiązywanie problemów

**Problem: Qt Creator nie znajduje plików projektu**
- Upewnij się, że wybrałeś plik `ClickerRPG.pro` a nie inny plik

**Problem: Błąd podczas budowania - "No valid kits found"**
- Zainstaluj odpowiednią wersję Qt i dodaj kit w ustawieniach Qt Creator

**Problem: Błąd linkowania - nie znalezione biblioteki Qt**
- Upewnij się, że masz zainstalowaną pełną wersję Qt (nie tylko biblioteki)

## Licencja

Ten projekt jest dostępny na licencji MIT.
