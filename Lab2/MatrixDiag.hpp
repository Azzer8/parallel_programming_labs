// #include <iostream>
// #include <fstream>
// #include <string>
// #include <cstdlib>
// #include <ctime>
// #include <cmath>
// #include <iomanip>
// #include <stdexcept>

// template<typename T = double>
// class  MatrixDiag {
//     size_t _row, _col;
//     T* _data;

//  public:
// 	MatrixDiag(size_t row, size_t col) {
// 		if (row != col) throw invalid_argument("Количество строк и столбцов должны быть равны.");
//         _row = row; _col = col;
//         _data = new T[_row * _col];
// 	}



// };

#include <iostream>
#include <vector>
#include <map>
#include "MatrixDense.hpp"

using namespace std;


template<typename T = double>
class MatrixDiag {
    map<int, vector<T>> diagonals;
 public:
    MatrixDiag(const MatrixDense<T>& matrix) {
        size_t _row = matrix.get_row_col().first;
        size_t _col = matrix.get_row_col().second;
        if (_row != _col) {
            throw runtime_error("Ошибка! Матрица должна быть квадратной.");
        }

        for (size_t d = 0; d < _row; ++d) {
            vector<T> upperDiagonal;
            vector<T> lowerDiagonal;
            bool hasNonZeroUpper = false;
            bool hasNonZeroLower = false;

            for (size_t i = 0; i < _row - d; ++i) {
                T upperValue = matrix(i, i + d);
                upperDiagonal.push_back(upperValue);
                if (upperValue != 0) {
                    hasNonZeroUpper = true;
                }

                if (d > 0) {
                    T lowerValue = matrix(i + d, i);
                    lowerDiagonal.push_back(lowerValue);
                    if (lowerValue != 0) {
                        hasNonZeroLower = true;
                    }
                }
            }

            if (hasNonZeroUpper) {
                diagonals[d] = upperDiagonal;
            }
            if (hasNonZeroLower) {
                diagonals[-static_cast<int>(d)] = lowerDiagonal;
            }
        }
    }


    void printDiagonals() const {
        for (const auto& diag : diagonals) {
            cout << "Диагональ " << diag.first << ": ";
            for (const auto& value : diag.second) {
                cout << value << " ";
            }
            cout << endl;
        }
    }
};