#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <stdexcept>

using namespace std;

template<typename T = double>
class  MatrixDense {
    size_t _row, _col;
    T* _data;

 public:
    MatrixDense(size_t row, size_t col) {
        _row = row;
        _col = col;
        _data = new T[_row*_col];
	}

	MatrixDense(size_t row_col) {
        _row = row_col;
        _col = row_col;
        _data = new T[_row*_col];
	}

    ~MatrixDense() { delete[] _data; }

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

    void exportToFile(const string& fileName) const {
        ofstream outFile("export/" + fileName);
        if (!outFile.is_open()) {
            throw runtime_error("Ошибка! Не удалось открыть файл.");
        }

        outFile << getClassName() << endl;
        outFile << _row << " " << _col << endl;
        for (size_t i = 0; i < _row; ++i) {
            for (size_t j = 0; j < _col; ++j) {
                outFile << (*this)(i, j);
                if (j < _col - 1) outFile << " ";
            }
            outFile << endl;
        }
        outFile.close();
        cout << "Экспорт в " << fileName << " выполнен." << endl;
    }

    static MatrixDense importFromFile(const string& fileName) {
        ifstream inFile("export/" + fileName);
        if (!inFile.is_open()) {
            throw runtime_error("Ошибка! Не удалось открыть файл.");
        }
        string className;
        getline(inFile, className);
		if (className!= "MatrixDense") {
            throw runtime_error("Ошибка! Матрица не является MatrixDense.");
        }
        size_t row, col;
        inFile >> row >> col;
        MatrixDense impMatrixDense(row, col);

        for (size_t i = 0; i < row; ++i) {
            for (size_t j = 0; j < col; ++j) {
                inFile >> impMatrixDense(i, j);
            }
        }
        inFile.close();
        cout << "Импорт из " << fileName << " выполнен." << endl;

        return impMatrixDense;
    }

	pair<size_t, size_t> get_row_col() const {
		return pair(_row, _col);
	}
};