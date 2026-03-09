# Clicker RPG - Zakończony projekt C++/Qt

## Podsumowanie

Stworzyłem pełnoprawną grę Clicker RPG w C++ z biblioteką Qt, spełniającą wszystkie Twoje wymagania:

✅ **System walki**: Duży klikalny obszar zamiast małego przycisku
✅ **System map**: 3 mapy z różnymi przeciwnikami i wymaganiami odblokowania
✅ **Automatyczne leczenie**: Po pokonaniu wroga
✅ **Paski postępu HP i EXP**: Widoczne na dole okna
✅ **Statystyki gracza**: Atak, Obrona, Max HP z możliwością ulepszeń
✅ **Nagrody za pokonanie wrogów**: EXP i złoto
✅ **Odblokowywanie map**: Po 50 zabitych wrogów na każdej mapie

## Główne funkcje

1. **Trzy mapy z różnymi przeciwnikami**:
   - Las: Szczury, Szlamy, Gobliny, Wilki, Jelenie
   - Jaskinie: Szkielety, Mroczne Nietoperze, Trolle, Mroczne Elfowie, Golemy Kamienne
   - Wulkan: Fire Golemy, Demony, Smoki, Płonące Elementale, Diaboliczni Rycerze

2. **System rozwoju**:
   - Ulepszanie ataku, obrony i maksymalnego HP
   - zdobywanie poziomów i ulepszanie statystyk

3. **Interfejs użytkownika**:
   - Przejrzysty pasek boczny z zakładkami
   - Paski postępu dla HP i EXP
   - Widoczne złoto i poziom
   - Sklep z ulepszeniami

## Jak uruchomić w Qt Creator?

1. Otwórz Qt Creator
2. Wybierz: **File → Open File or Project**
3. Wybierz plik: **ClickerRPG.pro**
4. Wybierz odpowiedni kit (np. Desktop Qt 5.15.2 MinGW 64-bit)
5. Kliknij: **Build** (młotek) lub **Ctrl+B**
6. Kliknij: **Run** (zielona strzałka) lub **Ctrl+R**

## Struktura projektu

- `ClickerRPG.pro` - Plik projektu Qt
- `src/main.cpp` - Główny plik aplikacji
- `src/mainwindow.h/cpp` - Główny okno i logika gry
- `src/player.h/cpp` - Klasa gracza z statystykami
- `src/enemy.h/cpp` - Klasa przeciwnika z losowaniem

Gra jest gotowa do gry w Qt Creator!
