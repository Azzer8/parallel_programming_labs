#include <vector>
#include "MatrixDense.hpp"

using namespace std;


template<typename T = double>
class MatrixDiag {
    vector<int> _diagonals_idxs;
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
        bool nonZeroDiag = false;
        for (size_t i = 0; i < _row; ++i) {
            T value = matrix(i, i);
            _data.push_back(value);
            if (value != 0) { nonZeroDiag = true; }
        }

        if (nonZeroDiag) { _diagonals_idxs.push_back(0); }
        else { _data.clear(); }

        // Обработка диагоналей ниже главной
        for (size_t d = 1; d < _row; ++d) {
            nonZeroDiag = false;
            vector<T> lowerDiagonal;
            for (size_t i = 0; i < _row - d; ++i) {
                T value = matrix(i + d, i);
                lowerDiagonal.push_back(value);
                if (value != 0) { nonZeroDiag = true; }
            }
            
            if (nonZeroDiag) {
                _data.insert(_data.end(), lowerDiagonal.begin(), lowerDiagonal.end());
                _diagonals_idxs.push_back(-int(d));
            }
        }

        // Обработка диагоналей выше главной
        for (size_t d = 1; d < _row; ++d) {
            nonZeroDiag = false;
            vector<T> upperDiagonal;
            for (size_t i = 0; i < _row - d; ++i) {
                T value = matrix(i, i + d);
                upperDiagonal.push_back(value);
                if (value != 0) { nonZeroDiag = true; }
            }

            if (nonZeroDiag) {
                _data.insert(_data.end(), upperDiagonal.begin(), upperDiagonal.end());
                _diagonals_idxs.push_back(d);
            }
        }
    }

    T operator()(size_t row, size_t col) const {
        if (row >= _row || col >= _col) {
            throw out_of_range("Индекс вне диапазона матрицы");
        }

        int reqstd_diag = int(col - row);
        size_t idx_of_rd;
        bool found = false;

        for (size_t i = 0; i < _diagonals_idxs.size(); ++i) {
            if (_diagonals_idxs[i] == reqstd_diag) {
                found = true;
                idx_of_rd = i;
                break;
            }
        }

        if (found) {
            size_t data_index = 0;
            if (reqstd_diag != 0) {
                for (size_t i = 0; i < idx_of_rd; ++i) {
                        // Добавляем количество элементов на каждой предыдущей диагонали
                        data_index += _row - abs(_diagonals_idxs[i]);
                    }
            }

            if (reqstd_diag <= 0) { data_index += col; }
            else { data_index += row; }

            return _data[data_index];
        }
        
        return T(0);
    }

    void print() const {
        cout << "Массив индексов ненулевых диагоналей: ";
        for (const auto& value : _diagonals_idxs) {
            cout << value << " ";
        }
        cout << endl;
        cout << "MatrixDense: ";
        for (const auto& value : _data) {
            cout << value << " ";
        }
        cout << endl;
        cout << endl;
    }
};