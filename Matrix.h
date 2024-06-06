#pragma once //Zapobieganie włączania wielokrotnie tego samego pliku nagłówkowego podczas kompilaccji
#include <iostream> //Definicja strumieni wejścia i wyjścia
using namespace std; //Pozwala na dostęp do symboli nazw w przestrzeni std;

template <typename T> //Deklaracja szablonu
class Matrix //Uworzenie klasy "Matrix"
{
//Deklaracja elementów publicznych, czyli dostępnym poza klasą
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
    try
    {
        m = new double* [r];
        for (int i = 0; i < r; i++)
            m[i] = new double[c];
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++)
                m[i][j] = org.m[i][j];
    }
    catch (const bad_alloc& e)
    {
        cerr << "Błąd alokacji w pamięci: " << e.what() << endl;
        // Jeśli alokacja pamięci się nie powiodła, należy oczyścić już zaalokowaną pamięć
        for (int i = 0; i < r; i++)
        {
            delete[] m[i];
        }
        delete[] m;
        throw; // Rzucenie wyjątku ponownie, aby poinformować o błędzie
    }
}

// Destruktor
template <typename T>
Matrix<T>::~Matrix()
{
    try
    {
        for (int i = 0; i < r; i++)
            delete[] m[i];
        delete[] m;
        m = NULL;
    }
    catch (const exception& e)
    {
        cerr << "Błąd podczas usuwania pamięci: " << e.what() << endl;
        throw; // Ponowne rzucenie wyjątku, aby poinformować o błędzie
    }
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

    try
    {
        for (T i = 0; i < r; i++)
            for (T j = 0; j < c; j++)
                m[i][j] = org.m[i][j];
    }
    catch (const exception& e)
    {
        cerr << "Błąd podczas przypisywania macierzy: " << e.what() << endl;
        throw;
    }

    return *this;
}

// Ustawienie wartości x w komórce (i, j)
template <typename T>
void Matrix<T>::set(int i, int j, T x)
{
    if (i < 1 || i > r || j < 1 || j > c)
        throw std::out_of_range("Indeks poza zakresem macierzy!!!");

    try
    {
        m[i - 1][j - 1] = x;
    }
    catch (const exception& e)
    {
        cerr << "Błąd podczas ustawiania wartości: " << e.what() << endl;
        throw;
    }
}

// Odejmowanie macierzy
template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& y) const
{
    if (r != y.r || c != y.c)
        throw invalid_argument("Macierze muszą mieć takie same wymiary!!!");

    Matrix wyn(r, c);
    try
    {
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++)
                wyn.m[i][j] = m[i][j] - y.m[i][j];
    }
    catch (const out_of_range& e)
    {
        cerr << "Błąd indeksowania podczas odejmowania macierzy: " << e.what() << endl;
        throw;
    }
    return wyn;
}

// Dodawanie macierzy
template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& y) const
{
    if (r != y.r || c != y.c)
        throw invalid_argument("Macierze muszą mieć takie same wymiary!!!");

    Matrix wyn(r, c);
    try
    {
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++)
                wyn.m[i][j] = m[i][j] + y.m[i][j];
    }
    catch (const out_of_range& e)
    {
        cerr << "Błąd indeksowania podczas dodawania macierzy: " << e.what() << endl;
        throw;
    }
    return wyn;
}

// Mnożenie macierzy przez skalar
template <typename T>
Matrix<T> operator*(const T x, const Matrix<T>& y)
{
    try
    {
        Matrix<T> wyn(y.r, y.c);
        for (int i = 0; i < y.r; i++)
            for (int j = 0; j < y.c; j++)
                wyn.m[i][j] = x * y.m[i][j];
        return wyn;
    }
    catch (const out_of_range& e)
    {
        cerr << "Błąd indeksowania podczas mnożenia macierzy przez skalar: " << e.what() << endl;
        throw;
    }
}

// Mnożenie macierzy przez macierz
template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix& mat) const
{
    if (c != mat.r) // Liczba kolumn pierwszej macierzy musi być równa liczbie wierszy drugiej macierzy
        throw invalid_argument("Liczba kolumn pierwszej macierzy musi być równa liczbie wierszy drugiej macierzy!!!");

    try
    {
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
    catch (const out_of_range& e)
    {
        cerr << "Błąd indeksowania podczas mnożenia macierzy przez macierz: " << e.what() << endl;
        throw;
    }
}

// Stan macierzy (wypisywanie zawartości)
template <typename T>
void Matrix<T>::stan()
{
    try
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
                cout << m[i][j] << '\t';
            cout << endl;
        }
    }
    catch (const out_of_range& e)
    {
        cerr << "Błąd indeksowania podczas wyświetlania stanu macierzy: " << e.what() << endl;
        throw;
    }
}