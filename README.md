# Algorytmy i struktury danych II - grupa 2 2024/25

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


## <div align="center">Projekt</div>
