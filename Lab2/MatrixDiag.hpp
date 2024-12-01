#include <vector>
#include <unordered_map>
#include "MatrixDense.hpp"

using namespace std;


template<typename T = double>
class MatrixDiag {
    vector<T> diagonalElements;
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