# Jak uruchomić projekt w Qt Creator

## Przed rozpoczęciem

Upewnij się, że masz zainstalowane:
- **Qt Creator** (wraz z Qt 5.15 lub nowszym)
- **Kompilator C++**:
  - Windows: MinGW lub MSVC
  - macOS: Clang (Xcode Command Line Tools)
  - Linux: GCC lub Clang

## Krok 1: Otwórz projekt w Qt Creator

1. Uruchom **Qt Creator**
2. Kliknij **File → Open File or Project** (lub użyj skrótu `Ctrl+O`)
3. Przejdź do folderu z projektem
4. Wybierz plik **CMakeLists.txt** (lub **ClickerRPG.pro**)
5. Kliknij **Open**

## Krok 2: Skonfiguruj projekt

Qt Creator wyświetli okno **Configure Project** z dostępnymi zestawami narzędzi (kits):

1. **Wybierz zestaw narzędzi**:
   - Wybierz co najmniej jeden zestaw z listy (np. Desktop Qt 5.15.2 MinGW 64-bit)
   - Upewnij się, że zestaw ma status **Ready**

2. **Kliknij Configure Project**

## Krok 3: Sprawdź konfigurację (opcjonalnie)

Po załadowaniu projektu:

1. Przejdź do **Projects** (panel po lewej stronie)
2. W sekcji **Build & Run** sprawdź:
   - **Kit**: Wybrany zestaw narzędzi
   - **Build directory**: Folder, w którym będą generowane pliki kompilacji
   - **CMake Configuration**: Ustawienia CMake (ścieżki do Qt itp.)

## Krok 4: Skompiluj projekt

Aby skompilować projekt:

1. Kliknij ikonę **Build** (młotek) w lewym górnym rogu
   - Lub użyj skrótu `Ctrl+B`
2. Poczekaj na zakończenie kompilacji
3. Sprawdź panel **Compile Output** na dole ekranu:
   - Powinien pojawić się komunikat **"Elapsed time: ..."** bez błędów

## Krok 5: Uruchom grę

Aby uruchomić skompilowaną grę:

1. Kliknij ikonę **Run** (zielona strzałka) w lewym górnym rogu
   - Lub użyj skrótu `Ctrl+R`
2. Gra powinna uruchomić się w nowym oknie

## Alternatywa: Użycie pliku .pro

Jeśli wolisz użyć pliku `ClickerRPG.pro` zamiast CMake:

1. W Qt Creator kliknij **File → Open File or Project**
2. Wybierz plik **ClickerRPG.pro**
3. Skonfiguruj projekt jak w Kroku 2
4. Skompiluj i uruchom jak w Krokach 4 i 5

## Rozwiązywanie problemów

### Błąd: "Qt not found" lub "CMake could not find Qt"

**Rozwiązanie**:
1. Przejdź do **Tools → Options → Kits → Qt Versions**
2. Kliknij **Add** i wybierz ścieżkę do pliku `qmake` (np. `C:\Qt\5.15.2\mingw81_64\bin\qmake.exe`)
3. Przejdź do **Tools → Options → Kits → Kits**
4. Wybierz używany zestaw i w polu **Qt version** wybierz dodaną wersję Qt

### Błąd: "No suitable kits found"

**Rozwiązanie**:
1. Zainstaluj dodatkowe zestawy narzędzi w **Qt Maintenance Tool**:
   - Uruchom **Qt Maintenance Tool**
   - Wybierz **Add or remove components**
   - Rozwiń **Qt 5.15.2** i wybierz odpowiedni zestaw (np. MinGW 64-bit)
   - Kliknij **Next** i zainstaluj

### Błąd kompilacji: "undefined reference to vtable..."

**Rozwiązanie**:
1. Usuń folder build (np. `build-ClickerRPG-Desktop_Qt_5_15_2_MinGW_64_bit-Debug`)
2. Kliknij **Build → Rebuild All**
3. Ponownie uruchom kompilację

### Gra uruchamia się, ale okno jest puste

**Rozwiązanie**:
1. Sprawdź, czy pliki zasobów są poprawnie włączone w projekcie
2. Upewnij się, że wszystkie pliki `.cpp` są dodane do `CMakeLists.txt` lub `ClickerRPG.pro`
3. Przeczytaj panel **Application Output** na dole ekranu w poszukiwaniu błędów

## Porady dla lepszej pracy w Qt Creator

### Skróty klawiszowe

- `Ctrl+B` - Skompiluj projekt
- `Ctrl+R` - Uruchom projekt
- `F5` - Uruchom w trybie debugowania
- `Ctrl+I` - Wcięcie zaznaczonego tekstu
- `Ctrl+/` - Komentarz/dekomentarz linii
- `Ctrl+Shift+R` - Zastąp wszystkie wystąpienia

### Przydatne panele

- **Projects** - Zarządzanie konfiguracją projektu
- **Edit** - Edycja kodu źródłowego
- **Design** - Edytor interfejsów użytkownika (dla plików `.ui`)
- **Debug** - Tryb debugowania
- **Help** - Dokumentacja Qt (wyszukaj klasy i funkcje)

### Autouzupełnianie kodu

Qt Creator oferuje inteligentne autouzupełnianie:
- Wpisz pierwsze litery nazwy klasy lub funkcji i naciśnij `Ctrl+Space`
- Aby zobaczyć dokumentację funkcji, naciśnij `F1`

### Debugowanie

1. Ustaw punkty przerwania (breakpoints) klikając obok numerów linii
2. Naciśnij `F5`, aby uruchomić w trybie debugowania
3. Użyj przycisków w panelu **Debug** do kontrolowania wykonywania kodu:
   - **Step Over** (`F10`) - Przejdź do następnej linii
   - **Step Into** (`F11`) - Wejdź do funkcji
   - **Step Out** (`Shift+F11`) - Wyjdź z funkcji

## Dodatkowe zasoby

- [Dokumentacja Qt Creator](https://doc.qt.io/qtcreator/)
- [Dokumentacja Qt 5](https://doc.qt.io/qt-5/)
- [Przykłady Qt](https://doc.qt.io/qt-5/qtexamples.html)
- [Forum Qt](https://forum.qt.io/)

## Podsumowanie

Teraz powinieneś być w stanie:
1. Otworzyć projekt w Qt Creator
2. Skonfigurować zestaw narzędzi
3. Skompilować i uruchomić grę
4. Rozwiązywać podstawowe problemy

Jeśli napotkasz dalsze trudności, sprawdź:
1. Panel **Compile Output** i **Application Output** w Qt Creator
2. Dokumentację Qt
3. Forum społeczności Qt

Miłej gry! 🎮