#include <vector>
#include <unordered_map>
#include "MatrixDense.hpp"

using namespace std;


template<typename T = double>
class MatrixDiag {
    vector<T> _diagonal_idx;
    vector<T> _data;
    size_t _row, _col;
 public:
    MatrixDiag(const MatrixDense<T>& matrix) {
        _row = matrix.get_row_col().first;
        _col = matrix.get_row_col().second;
        if (_row != _col) {
            throw runtime_error("Ошибка! Матрица должна быть квадратной.");
        }

        // Обработка главной диагонали
        bool hasNonZeroMain = false;
        for (size_t i = 0; i < _row; ++i) {
            T value = matrix(i, i);
            _data.push_back(value);
            if (value != 0) {
                hasNonZeroMain = true;
            }
        }
        if (hasNonZeroMain) {
            _diagonal_idx.push_back(0); // Главная диагональ
        }
        else {
            _data.clear();
        }

        // Обработка диагоналей ниже главной
        for (size_t d = 1; d < _row; ++d) {
            bool hasNonZeroLower = false;
            vector<T> lowerDiagonal;
            for (size_t i = 0; i < _row - d; ++i) {
                T value = matrix(i + d, i);
                lowerDiagonal.push_back(value);
                if (value != 0) {
                    hasNonZeroLower = true;
                }
            }
            if (hasNonZeroLower) {
                _data.insert(_data.end(), lowerDiagonal.begin(), lowerDiagonal.end());
                _diagonal_idx.push_back(-static_cast<int>(d)); // Индекс для нижней диагонали
            }
        }

        // Обработка диагоналей выше главной
        for (size_t d = 1; d < _row; ++d) {
            bool hasNonZeroUpper = false;
            vector<T> upperDiagonal;
            for (size_t i = 0; i < _row - d; ++i) {
                T value = matrix(i, i + d);
                upperDiagonal.push_back(value);
                if (value != 0) {
                    hasNonZeroUpper = true;
                }
            }
            if (hasNonZeroUpper) {
                _data.insert(_data.end(), upperDiagonal.begin(), upperDiagonal.end());
                _diagonal_idx.push_back(d); // Индекс для верхней диагонали
            }
        }
    }

    T operator()(size_t row, size_t col) const {
        if (row >= _row || col >= _col) {
            throw std::out_of_range("Индекс вне диапазона матрицы");
        }

        return T(0);
    }

    void print() const {
        cout << "Массив индексов ненулевых диагоналей: ";
        for (const auto& value : _diagonal_idx) {
            cout << value << " ";
        }
        cout << endl;
        cout << "Массив ненулевых диагоналей: ";
        for (const auto& value : _data) {
            cout << value << " ";
        }
        cout << endl;
        cout << endl;
    }
};