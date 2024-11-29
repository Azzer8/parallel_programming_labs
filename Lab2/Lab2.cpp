#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <cmath>
#include <iomanip>
#include <stdexcept>

using namespace std;


template<typename T = double>
class Matrix {
 public:
    void exportToFile(const string& fileName) {
        ofstream outFile("export/" + fileName);
        if (!outFile.is_open()) {
            throw runtime_error("Ошибка! Не удалось открыть файл.");
        }

        outFile << this->GetClassName() << endl;
        outFile << this->_row << " " << this->_col << endl;
        for (size_t i = 0; i < this->_row; ++i) {
            for (size_t j = 0; j < this->_col; ++j) {
                outFile << this->_data[i*this->_col + j];
                if (j < this->_col - 1) outFile << " ";
            }
            outFile << endl;
        }
        outFile.close();
        cout << "Экспорт в " << fileName << " выполнен." << endl;
    }

    // T importFromFile(const string& fileName) {
    //     ifstream inFile("import/" + fileName);
    //     if (!inFile.is_open()) {
    //         throw runtime_error("Ошибка! Не удалось открыть файл.");
    //     }

    //     for (size_t i = 0; i < this->_row; ++i) {
    //         for (size_t j = 0; j < this->_col; ++j) {
    //             inFile >> this->_data[i*this->_col + j];
    //         }
    //     }
    // }

};


template<typename T = double>
class MatrixDense {
    size_t _row, _col;
    T* _data;

 public:
    MatrixDense(size_t row, size_t col) {
        _row = row;
        _col = col;
        _data = new T[_row*_col];
    }

    string getClassName() const {
        return "MatrixDense";
    }

    T& operator()(size_t i, size_t j) const {
        if (i >= _row || j >= _col) {
            throw runtime_error("Ошибка! Индексы выходят за границы матрицы.");
        }

        return _data[i*_col + j];
    }

    MatrixDense operator+(const MatrixDense& other) {
        if (_row != other._row || _col != other._col) {
            throw runtime_error("Ошибка! Матрицы имеют разный размер.");
        }

        MatrixDense result(_row, _col);
        for (size_t i = 0; i < _row; ++i) {
            for (size_t j = 0; j < _col; ++j) {
                result(i, j) = (*this)(i, j) + other(i, j);
            }
        }

        return result;
    }

    MatrixDense operator-(const MatrixDense& other) {
        if (_row != other._row || _col != other._col) {
            throw runtime_error("Ошибка! Матрицы имеют разный размер.");
        }

        MatrixDense result(_row, _col);
        for (size_t i = 0; i < _row; ++i) {
            for (size_t j = 0; j < _col; ++j) {
                result(i, j) = (*this)(i, j) - other(i, j);
            }
        }

        return result;
    }

    MatrixDense operator*(const MatrixDense& other) {
        if (_row != other._row || _col != other._col) {
            throw runtime_error("Ошибка! Матрицы имеют разный размер.");
        }

        MatrixDense result(_row, _col);
        for (size_t i = 0; i < _row; ++i) {
            for (size_t j = 0; j < _col; ++j) {
                result(i, j) = (*this)(i, j) * other(i, j);
            }
        }

        return result;
    }

    MatrixDense operator*(const T scalar) {
        MatrixDense result(_row, _col);
        for (size_t i = 0; i < _row; ++i) {
            for (size_t j = 0; j < _col; ++j) {
                result(i, j) = (*this)(i, j) * scalar;
            }
        }

        return result;
    }

    MatrixDense dot(const MatrixDense& other) {
        if (_col != other._row) {
            throw runtime_error("Ошибка! Число столбцов первой матрицы не совпадает с числом строк второй матрицы.");
        }

        MatrixDense result(_row, other._col);
        for (size_t i = 0; i < _row; ++i) {
            for (size_t j = 0; j < other._col; ++j) {
                for (size_t k = 0; k < _col; ++k) {
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }

        return result;
    }

    void  transpose() {
        T* new_data = new T[_col * _row];
        for (size_t i = 0; i < _row; ++i) {
            for (size_t j = 0; j < _col; ++j) {
                new_data[j*_row + i] = (*this)(i, j);
            }
        }
        
        delete[] _data;
        _data = new_data;
        size_t temp = _row;
        _row = _col;
        _col = temp;
    }

    void fillMatrix(size_t min, size_t max) {
        for (size_t i = 0; i < _row; ++i) {
            for (size_t j = 0; j < _col; ++j) {
                (*this)(i, j) = min + T(rand()) / (T(RAND_MAX / (max - min)));
            }
        }
    }

    void fillMatrix(T value) {
        for (size_t i = 0; i < _row; ++i) {
            for (size_t j = 0; j < _col; ++j) {
                (*this)(i, j) = value;
            }
        }
    }

    void print() const {
        for (size_t i = 0; i < _row; ++i) {
            for (size_t j = 0; j < _col; ++j) {
                cout << setw(8) << fixed << setprecision(2) << (*this)(i, j);
                if (j < _col - 1) cout << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    ~MatrixDense() { delete[] _data; }
};



int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));
    
    MatrixDense matrixDense1(3, 5), matrixDense2(3, 5);
    matrixDense1.fillMatrix(1, 20);
    matrixDense2.fillMatrix(1, 20);

    cout << "MatrixDense 1: " << endl;
    matrixDense1.print();
    cout << "MatrixDense 2: " << endl;
    matrixDense2.print();
    
    cout << "Сумма MatrixDense 1 и MatrixDense 2: " << endl;
    MatrixDense sumMatrixDense = matrixDense1 + matrixDense2;
    sumMatrixDense.print();
    
    cout << "Разность MatrixDense 1 и MatrixDense 2: " << endl;
    MatrixDense diffMatrixDense = matrixDense1 - matrixDense2;
    diffMatrixDense.print();

    cout << "Поэлементное умножение MatrixDense 1 и MatrixDense 2: " << endl;
    MatrixDense mulMatrixDense = matrixDense1 * matrixDense2;
    mulMatrixDense.print();
    
    cout << "Произведение MatrixDense 1 и числа 2: " << endl;
    MatrixDense scalarMatrixDense = matrixDense1 * 2;
    scalarMatrixDense.print();

    cout << "Транспонирование MatrixDense 1: " << endl;
    matrixDense1.transpose();
    matrixDense1.print();

    cout << "Произведение MatrixDense 1 и MatrixDense 2: " << endl;
    MatrixDense dotMatrixDense = matrixDense1.dot(matrixDense2);
    dotMatrixDense.print();

    
    
    return 0;
}