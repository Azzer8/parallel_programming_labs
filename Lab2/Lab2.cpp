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
        MatrixDense<int> matrixDense3 = MatrixDense<int>::importFromFile("MatrixDense3.txt");
        cout << "MatrixDense 3: " << endl;
        matrixDense3.print();
        MatrixDiag diagMatrix(matrixDense3);
        diagMatrix.print();

        cout << "Главная" << endl;
        cout << matrixDense3(0, 0) << " = " << diagMatrix(0, 0) << endl;
        cout << matrixDense3(1, 1) << " = " << diagMatrix(1, 1) << endl;
        cout << matrixDense3(2, 2) << " = " << diagMatrix(2, 2) << endl;
        // cout << matrixDense3(3, 3) << " = " << diagMatrix(3, 3) << endl;
        
        cout << "\nНижняя 1" << endl;
        cout << matrixDense3(1, 0) << " = " << diagMatrix(1, 0) << endl;
        cout << matrixDense3(2, 1) << " = " << diagMatrix(2, 1) << endl;
        // cout << matrixDense3(3, 2) << " = " << diagMatrix(3, 2) << endl;
        cout << "Нижняя 2" << endl;
        cout << matrixDense3(2, 0) << " = " << diagMatrix(2, 0) << endl;
        // cout << matrixDense3(3, 1) << " = " << diagMatrix(3, 1) << endl;
        // cout << "Нижняя 3" << endl;
        // cout << matrixDense3(3, 2) << " = " << diagMatrix(3, 2) << endl;
        
        cout << "\nВерхняя 1" << endl;
        cout << matrixDense3(0, 1) << " = " << diagMatrix(0, 1) << endl;
        cout << matrixDense3(1, 2) << " = " << diagMatrix(1, 2) << endl;
        // cout << matrixDense3(2, 3) << " = " << diagMatrix(2, 3) << endl;
        cout << "Верхняя 2" << endl;
        cout << matrixDense3(0, 2) << " = " << diagMatrix(0, 2) << endl;
        // cout << matrixDense3(1, 3) << " = " << diagMatrix(1, 3) << endl;
        // cout << "Верхняя 3" << endl;
        // cout << matrixDense3(0, 3) << " = " << diagMatrix(0, 3) << endl;

    return 0;
}