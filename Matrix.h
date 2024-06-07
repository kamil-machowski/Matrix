#pragma once //Zapobieganie włączania wielokrotnie tego samego pliku nagłówkowego podczas kompilacji
#include <iostream> //Definicja strumieni wejścia i wyjścia
#include <stdexcept> //Definicja wyjątków
using namespace std; //Pozwala na dostęp do symboli nazw w przestrzeni std;

template <typename T> //Deklaracja szablonu
class Matrix //Uworzenie klasy "Matrix"
{
    //Deklaracja elementów publicznych, czyli dostępnych poza klasą
public:
    Matrix<T>(T row, T col); // Konstruktor tworzący macierz o rozmiarach row x col
    Matrix<T>(const Matrix<T>& org); // Konstruktor kopiujący
    ~Matrix<T>(); // Destruktor
    Matrix<T>& operator=(const Matrix<T>& org); // Operator przypisania
    void set(int i, int j, T x); // Ustawienie wartości x w komórce (i, j)
    T get(int i, int j) const; // Pobranie wartości z komórki (i, j)
    Matrix<T> operator-(const Matrix<T>& y) const; // Odejmowanie macierzy
    Matrix<T> operator+(const Matrix<T>& y) const; // Dodawanie macierzy
    template <typename U>
    friend Matrix<U> operator*(const U x, const Matrix<U>& y); // Mnożenie macierzy przez skalar
    Matrix<T> operator*(const Matrix& mat) const; // Mnożenie macierzy przez macierz
    Matrix<T> operator!() const; // Macierzy odwrotna
    bool operator==(const Matrix<T>& other) const; // Porównanie czy dwie macierze są do siebie odwrotne
    void stan(); // Stan macierzy (wypisywanie zawartości)

    //Analogicznie deklaracja elementów prywatnych, czyli dostępnych wewnątrz klasy
private:
    const T r, c; // Liczba wierszy (r) i kolumn (c)
    double** m; // Wskaźnik do tablicy dwuwymiarowej przechowującej wartości macierzy
};

// Konstruktor tworzący macierz o rozmiarach row x col
template <typename T>
Matrix<T>::Matrix(T row, T col) : r(row), c(col)
{
    m = new double* [row];
    for (int i = 0; i < row; i++)
        m[i] = new double[col];
}

// Konstruktor kopiujący
template <typename T>
Matrix<T>::Matrix(const Matrix& org) : r(org.r), c(org.c)
{
    m = new double* [r];
    for (int i = 0; i < r; i++)
        m[i] = new double[c];
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            m[i][j] = org.m[i][j];
}

// Destruktor
template <typename T>
Matrix<T>::~Matrix()
{
    for (int i = 0; i < r; i++)
        delete[] m[i];
    delete[] m;
    m = NULL;
}

// Operator przypisania
template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& org)
{
    if (this == &org)
        return *this;

    // Sprawdzenie czy rozmiary macierzy są takie same
    if (r != org.r || c != org.c)
        throw invalid_argument("Rozmiary macierzy są niezgodne!!!");

    for (T i = 0; i < r; i++)
        for (T j = 0; j < c; j++)
            m[i][j] = org.m[i][j];

    return *this;
}

// Ustawienie wartości x w komórce (i, j)
template <typename T>
void Matrix<T>::set(int i, int j, T x)
{
    if (i < 1 || i > r || j < 1 || j > c)
        throw std::out_of_range("Indeks poza zakresem macierzy!!!");

    m[i - 1][j - 1] = x;
}

// Odejmowanie macierzy
template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& y) const
{
    if (r != y.r || c != y.c)
        throw invalid_argument("Macierze muszą mieć takie same wymiary!!!");

    Matrix wyn(r, c);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            wyn.m[i][j] = m[i][j] - y.m[i][j];
    return wyn;
}

// Dodawanie macierzy
template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& y) const
{
    if (r != y.r || c != y.c)
        throw invalid_argument("Macierze muszą mieć takie same wymiary!!!");

    Matrix wyn(r, c);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            wyn.m[i][j] = m[i][j] + y.m[i][j];
    return wyn;
}

// Mnożenie macierzy przez skalar
template <typename T>
Matrix<T> operator*(const T x, const Matrix<T>& y)
{
    Matrix<T> wyn(y.r, y.c);
    for (int i = 0; i < y.r; i++)
        for (int j = 0; j < y.c; j++)
            wyn.m[i][j] = x * y.m[i][j];
    return wyn;
}

// Mnożenie macierzy przez macierz
template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix& mat) const
{
    if (c != mat.r) // Liczba kolumn pierwszej macierzy musi być równa liczbie wierszy drugiej macierzy
        throw invalid_argument("Liczba kolumn pierwszej macierzy musi być równa liczbie wierszy drugiej macierzy!!!");

    Matrix res(r, mat.c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < mat.c; j++) {
            res.m[i][j] = 0;
            for (int k = 0; k < c; k++) {
                res.m[i][j] += m[i][k] * mat.m[k][j];
            }
        }
    }
    return res;
}

// Utworzenie macierzy odwrotnej
template <typename T>
Matrix<T> Matrix<T>::operator!() const {
    if (r != c) {
        throw invalid_argument("Macierz musi być kwadratowa, aby można było obliczyć macierz odwrotną.");
    }

    // Obliczenie macierzy odwrotnej tylko dla macierzy 2x2
    if (r == 2 && c == 2) {
        double determinant = m[0][0] * m[1][1] - m[0][1] * m[1][0];
        if (determinant == 0) {
            throw runtime_error("Macierz jest osobliwa i nie ma macierzy odwrotnej.");
        }

        Matrix<T> inv(r, c);
        inv.m[0][0] = m[1][1] / determinant;
        inv.m[0][1] = -m[0][1] / determinant;
        inv.m[1][0] = -m[1][0] / determinant;
        inv.m[1][1] = m[0][0] / determinant;

        return inv;
    }
    else {
        // Obsługa innych rozmiarów macierzy może być dodana tutaj
        throw runtime_error("Obsługiwane są tylko macierze 2x2.");
    }
}

// Sprawdzenie czy dwie macierze są do siebie odwrotne
template <typename T>
bool Matrix<T>::operator==(const Matrix<T>& other) const {
    if (r != other.r || c != other.c) {
        throw invalid_argument("Macierze muszą mieć takie same wymiary, aby można było porównać ich odwrotności.");
    }

    Matrix<T> identity(r, c);
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            identity.m[i][j] = (i == j) ? 1 : 0;
        }
    }

    Matrix<T> product = (*this) * other;
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            if (abs(product.m[i][j] - identity.m[i][j]) > 1e-9) {
                return false;
            }
        }
    }

    return true;
}

// Stan macierzy (wypisywanie zawartości)
template <typename T>
void Matrix<T>::stan()
{
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++)
            cout << m[i][j] << '\t';
        cout << endl;
    }
}
