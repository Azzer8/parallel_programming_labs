#include <windows.h>
#include "MatrixDiag.hpp"


int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));
    
    // // ДЕМОНСТРАЦИЯ РАБОТЫ MatrixDense
    //     MatrixDense matrixDense1(3, 5), matrixDense2(3, 5);
    //     matrixDense1.fillMatrix(1, 20);
    //     matrixDense2.fillMatrix(1, 20);

    //     cout << "MatrixDense 1: " << endl;
    //     matrixDense1.print();
    //     cout << "MatrixDense 2: " << endl;
    //     matrixDense2.print();
        
    //     cout << "Сумма MatrixDense 1 и MatrixDense 2: " << endl;
    //     MatrixDense sumMatrixDense = matrixDense1 + matrixDense2;
    //     sumMatrixDense.print();
        
    //     cout << "Разность MatrixDense 1 и MatrixDense 2: " << endl;
    //     MatrixDense diffMatrixDense = matrixDense1 - matrixDense2;
    //     diffMatrixDense.print();

    //     cout << "Поэлементное умножение MatrixDense 1 и MatrixDense 2: " << endl;
    //     MatrixDense mulMatrixDense = matrixDense1 * matrixDense2;
    //     mulMatrixDense.print();
        
    //     cout << "Произведение MatrixDense 1 и числа 2: " << endl;
    //     MatrixDense scalarMatrixDense = matrixDense1 * 2;
    //     scalarMatrixDense.print();

    //     cout << "Транспонирование MatrixDense 1: " << endl;
    //     matrixDense1.transpose();
    //     matrixDense1.print();

    //     cout << "Произведение MatrixDense 1 и MatrixDense 2: " << endl;
    //     MatrixDense dotMatrixDense = matrixDense1.dot(matrixDense2);
    //     dotMatrixDense.print();

    //     cout << "Экспорт MatrixDense:" << endl;
    //     matrixDense1.exportToFile("MatrixDense1.txt");

    //     cout << "Импорт MatrixDense:" << endl;
    //     MatrixDense impMatrixDense = MatrixDense<>::importFromFile("MatrixDense1.txt");
    //     impMatrixDense.print();


    // ДЕМОНСТРАЦИЯ РАБОТЫ MatrixDiag
        MatrixDense<int> matrixDense3(5);
        matrixDense3.fillMatrix(0, 2);
        cout << "MatrixDense 3: " << endl;
        matrixDense3.print();
        MatrixDiag diagMatrix(matrixDense3);
        diagMatrix.printDiagonals();

    return 0;
}