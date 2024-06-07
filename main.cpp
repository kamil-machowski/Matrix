#include <iostream>
#include "Matrix.h"

using namespace std;

int main() {
    try {
        // Tworzenie dwóch macierzy 2x2 typu int
        Matrix<int> mat1(2, 2);
        Matrix<int> mat2(2, 2);

        // Ustawianie wartości dla mat1
        mat1.set(1, 1, 1); // Ustawienie wartości 1 w komórce (1,1)
        mat1.set(1, 2, 4);
        mat1.set(2, 1, 3);
        mat1.set(2, 2, 4);

        // Ustawianie wartości dla mat2
        mat2.set(1, 1, 5);
        mat2.set(1, 2, 2);
        mat2.set(2, 1, 7);
        mat2.set(2, 2, 8);

        // Wyświetlanie mat1
        cout << "Matrix 1:" << endl;
        mat1.stan(); // Wypisanie zawartości mat1

        // Wyświetlanie mat2
        cout << "Matrix 2:" << endl;
        mat2.stan();

        // Dodawanie mat1 i mat2
        Matrix<int> sum = mat1 + mat2; // Sumowanie macierzy mat1 i mat2
        cout << "Suma Matrix1 i Matrix2:" << endl;
        sum.stan(); // Wypisanie wyniku dodawania

        // Odejmowanie mat2 od mat1
        Matrix<int> diff = mat1 - mat2;
        cout << "Roznica Matrix1 i Matrix2:" << endl;
        diff.stan();

        // Mnożenie mat1 przez wartość skalarną 
        Matrix<int> scalarMul = 2 * mat1;
        cout << "Matrix 1 przemnozony przez skalar 2:" << endl;
        scalarMul.stan();

        // Mnożenie mat1 i mat2 (mnożenie macierzy)
        Matrix<int> product = mat1 * mat2;
        cout << "Wynik mnozenia Matrix1 przez Matrix2:" << endl;
        product.stan();

        //Tworzenie macierzy odwrotnej do mat1
        Matrix<int> invMat = !mat1;
        cout << "Macierz odwrotna do Matrix1:" << endl;
        invMat.stan();

    }
    catch (const bad_alloc& e) {
        cerr << "Błąd alokacji w pamięci: " << e.what() << endl;
    }
    catch (const invalid_argument& e) {
        cerr << "Nieprawidłowy argument: " << e.what() << endl;
    }
    catch (const out_of_range& e) {
        cerr << "Indeks poza zakresem: " << e.what() << endl;
    }
    catch (const exception& e) {
        cerr << "Błąd: " << e.what() << endl;
    }

    return 0; // Zakończenie programu, zwrócenie wartości liczbowej
}
