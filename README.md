# Algorytmy i struktury danych II - grupa 2 2024/25

## <div align="center">Projekt</div>

### Spis treści

1. Zespół 
2. Opis projektu 
3. Wymagania 
4. Struktura katalogów 
5. Główne klasy i pliki 
6. Implementacje algorytmów 
7. Opis działania 
8. Zależności 

<br>

### Zespół

Zespół jest złożony z 4 osób.
  - Dorian Konwiński - programista, współtwórca dokumentacji
  - Maciej Krawczyk - kierownik zespołu, współtwórca dokumentacji
  - Jakub Kwiatkowski - główny programista, UI designer
  - Kacper Nadolny - programista, twórca dokumentacji

<br>

### Opis projektu

***Not Enough Ale*** to gra symulacyjna z graficznym interfejsem użytkownika, wykorzystująca bibliotekę SFML. Użytkownik może budować sieć połączeń pomiędzy różnymi typami budynków (farmy, tawerny, karczmy, skrzyżowania), zarządzać zasobami oraz przeprowadzać symulacje przepływu surowców. Gra umożliwia zapisywanie i wczytywanie stanu rozgrywki.

<br>

### Wymagania

 - Kompilator C++ (standard C++17)
 - Visual Studio 2022
 - Biblioteka SFML (zawarta już w projekcie)

<br>

### Struktura katalogów

```
ProjektAISD/
│
├── Button.cpp / hpp             # Klasa przycisku GUI
├── Convex.cpp / hpp             # Logika otoczki
├── Functions.cpp / hpp          # Funkcje pomocnicze, narzędziowe
├── FileCompression.cpp / hpp    # Funkcje związane z kompresją plików
├── FlowAlgorithm.cpp / hpp      # Obliczenia przepływów podczas tury
├── Game.cpp / hpp               # Główna logika gry
├── Line.cpp / hpp               # Klasa reprezentująca połączenie (drogę)
├── Menu.cpp / hpp               # Menu gry
├── Node.cpp / hpp               # Klasa reprezentująca budynek/węzeł
├── Źródło.cpp                   # Plik główny (main)
├── Textury/                     # Folder z teksturami do gry
└── ...
```

<br>

### Główne klasy i pliki
#### Game (Game.cpp / Game.hpp)
- Centralna klasa zarządzająca logiką gry, renderowaniem, obsługą zdarzeń, zapisem i wczytywaniem stanu gry.
- Najważniejsze metody:
	- run() – główna pętla gry.
	- processEvents() – obsługa zdarzeń okna.
	- update() – aktualizacja stanu gry.
	- render() – rysowanie elementów na ekranie.
	- handleMouseInput() – obsługa myszy (dodawanie węzłów, linii, interakcje z UI).
	- handleKeyboardInput() – obsługa klawiatury (np. zoom, zamknięcie gry).
	- SaveGame() / LoadGame() – zapis i odczyt stanu gry do/z pliku.
	- TurnEnd() – zakończenie tury, uruchomienie algorytmu przepływu.

#### Node (Node.cpp / Node.hpp)
- Reprezentuje budynek/węzeł na mapie (farma, tawerna, karczma, skrzyżowanie).
- Właściwości: Pozycja, typ, pojemność, tekstura.
#### Line (Line.cpp / Line.hpp)
- Reprezentuje połączenie (drogę) pomiędzy dwoma węzłami.
- Właściwości: Wskaźniki na węzły początkowy i końcowy, pojemność, koszt, tekstura.
#### Button (Button.cpp / Button.hpp)
- Prosty przycisk GUI, wykorzystywany do obsługi trybów gry i akcji użytkownika.
#### Functions (Functions.cpp / Functions.hpp)
- Zbiór funkcji narzędziowych, m.in. do obsługi kolizji, rysowania, kompresji danych.
#### Menu (Menu.cpp / Menu.hpp)
- Obsługa menu gry (np. ekran startowy, wybór opcji).
#### Źródło.cpp
- Plik główny, uruchamiający aplikację i inicjalizujący główne komponenty.

<br>

### Implementacje algorytmów

1. Przepływ w sieci - obliczanie maksymalnego przepływu z minimalnym kosztem - algorytm Busackera-Gowena; w implementacji do symulacji przejścia od farm do karczm przez piwiarnie problem został podzielony na dwie warstwy. Wyznaczane są przepływy z farm do piwiarni, a następnie z piwiarni do karczm.
   $O(F\cdot T)$;     $T=O((V + E) log V)$, $F=\text{iteracje pętli}$
2. Wyznaczanie drogi przepływu - znalezienie najszybszej drogi do karczmy - algorytm Dijkstry z rozszerzony o potencjały.
   $O((V + E) log V)$
3. Otoczki wypukłe - do tworzenia pola i wzmacnianie plonów farm - algorytm Grahama.
   $O(n\log n)$,     $n=15$
4. Sprawdzanie położenia obiektów - szukanie farm w polu otoczki - wyszukiwanie binarne
   $O(\log n)$,     $n=\text{rozmiar kontenera}$
5. Zapis danych do pliku - efektywne oszczędzanie pamięci i kompresja danych gry - kodowanie Huffmana.
   $O(n\log n)$,     $n=\text{ilość unikalnych znaków}$
6. Szukanie wzorca w tekście - szybkie powracanie do informacji z poprzednich tur - algorytm Boyera-Moore'a.
   $O(n\cdot m)$,     $n=\text{długość tekstu}$, $m=\text{długość wzorca}$

<br>

### Opis działania

1.	Uruchomienie gry: Tworzone jest okno SFML, ładowane są tekstury i czcionki.
2.	Interakcja użytkownika: Użytkownik może dodawać budynki i drogi, zmieniać tryby za pomocą przycisków, przesuwać widok, zapisywać/wczytywać grę.
3.	Symulacja: Po zakończeniu tury uruchamiany jest algorytm przepływu, który oblicza dostawy i koszty.
4.	Zapis/Wczytanie: Stan gry (budynki, drogi, statystyki) jest kompresowany i zapisywany do pliku, a następnie może być odczytany.

<br>

### Zależności

- SFML: Biblioteka do obsługi grafiki, dźwięku i wejścia/wyjścia.
- Standardowa biblioteka C++: Wykorzystywana do obsługi kontenerów, strumieni, itp.

<br>

---

## <div align="center">Zadania z zajęć</div>

### [Zadanie 1](https://github.com/EustachyGra/AISD2_projekt/tree/main/Zadanie%201) - Reprezentacja digrafów ważonych

Dla przykładowego digrafu z wagami (numeracja od 0):
![](https://raw.githubusercontent.com/EustachyGra/AISD2_projekt/refs/heads/main/Zadanie%201/graf_test.jpg)

zestaw [danych wejściowych](https://github.com/EustachyGra/AISD2_projekt/blob/main/Zadanie%201/test.txt) będzie wyglądał następująco:
```
7 10    // Ilość wierzchołków, ilość krawędzi
1 0 3   // Wierzchołek 1, wierzchołek 2, waga krawędzi
0 1 3
2 1 9
0 4 5
4 3 1
2 5 10
5 2 10
4 2 7
6 5 2
4 5 12
```
<br>

Program zawarty w [main.cpp](https://github.com/EustachyGra/AISD2_projekt/blob/main/Zadanie%201/main.cpp) podaje następujące wyniki:

```
Macierz sasiedztwa:
  0   3   0   0   5   0   0
  3   0   0   0   0   0   0
  0   9   0   0   0  10   0
  0   0   0   0   0   0   0
  0   0   7   1   0  12   0
  0   0  10   0   0   0   0
  0   0   0   0   0   2   0

Lista sasiadow:
0: (1, 3) (4, 5)
1: (0, 3)
2: (1, 9) (5, 10)
3:
4: (3, 1) (2, 7) (5, 12)
5: (2, 10)
6: (5, 2)

Dwie tablice:
Indeksy: 0 2 3 5 5 8 9
Sasiedzi: (1, 3) (4, 5) (0, 3) (1, 9) (5, 10) (3, 1) (2, 7) (5, 12) (2, 10) (5, 2)
```
---
<br>

### [Zadanie 2](https://github.com/EustachyGra/AISD2_projekt/tree/main/Zadanie%202) - Algorytm Floyda-Warshalla
Algorytm Floyda-Warshalla do obliczenia najkrótszych dróg między wierzchołkami w digrafie ważonym (numeracja od 1). Zestaw [danych wejściowych](https://github.com/EustachyGra/AISD2_projekt/blob/main/Zadanie%202/input.txt) będzie wyglądał następująco:
```
4 6    // Ilość wierzchołków, ilość krawędzi
1 2 8  // Wierzchołek 1, wierzchołek 2, waga krawędzi
1 4 1
2 3 1
3 1 4
4 2 2
4 3 9
2 4    // Szukanie najkrótszej drogi między tymi wierzchołkami
2      // Szukanie najkrótszego cyklu zawierającego ten wierzchołek
```
<br>

Program zawarty w [main.cpp](https://github.com/EustachyGra/AISD2_projekt/blob/main/Zadanie%202/main.cpp) podaje następujące wyniki:
```
Najkrotsza droga miedzy wierzcholkami 2, 4:
6 2 3 1 4 
Najkrotszy cykl zawierajacy wierzcholek 2:
8 2 3 1 4 2
```
---
<br>

### [Zadanie 3](https://github.com/EustachyGra/AISD2_projekt/tree/main/Zadanie%203) - Algorytmy Forda-Fulkersona i Edmondsa-Karpa
Algorytm Forda-Fulkersona wykorzystuje w tej implementacji DFS (Depth-First Search) do przeglądania grafu, natomiast algorytm Edmondsa-Karpa jest często nazywany poprawką poprzedniego algorytmu i używa BFS (Breadth-First Search). Symulowany przepływ ma miejsce od wierzchołka o najmniejszym indeksie do wierzchołka o największym indeksie.

Dla przykładowego digrafu z wagami (numeracja od 1):
![](https://raw.githubusercontent.com/EustachyGra/AISD2_projekt/refs/heads/main/Zadanie%203/graf.jpg)

zestaw [danych wejściowych](https://github.com/EustachyGra/AISD2_projekt/blob/main/Zadanie%203/input.txt) będzie wyglądał następująco:

```
7 11    // Ilość wierzchołków, ilość krawędzi
1 2 20  // Wierzchołek 1, wierzchołek 2, waga krawędzi
1 3 50
3 2 10
2 4 50
4 6 80
6 7 40
4 7 40
2 5 60
5 4 10
5 7 20
3 5 30
```
<br>

Program zawarty w [main.cpp](https://github.com/EustachyGra/AISD2_projekt/blob/main/Zadanie%203/main.cpp) podaje następujące wyniki:
```
Algorytm Forda-Fulkersona:
60
Algorytm Edmondsa-Karpa:
60
```
---
<br>

### [Zadanie 5](https://github.com/EustachyGra/AISD2_projekt/tree/main/Zadanie%203) - Zrównoważone drzewa BST
Dla n liczb stwórz drzewo czerwono-czarne i wypisz jego zawartość w porządku preorder. 

Zestaw [danych wejściowych](https://github.com/EustachyGra/AISD2_projekt/blob/main/Zadanie%205/input.txt) będzie wyglądał następująco:

```
7 // Ilość liczb do drzewa
2 1 26 5 3 29 13  // Kolejne liczby
```
<br>

Program zawarty w [main.cpp](https://github.com/EustachyGra/AISD2_projekt/blob/main/Zadanie%205/main.cpp) podaje następujący wynik:
```
Porzadek preorder BST:
2 1 5 3 26 13 29
```
