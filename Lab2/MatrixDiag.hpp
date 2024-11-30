#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <stdexcept>

template<typename T = double>
class  MatrixDiag {
    size_t _row, _col;
    T* _data;

 public:
	MatrixDiag(size_t row, size_t col) {
		if (row != col) throw std::invalid_argument("Количество строк и столбцов должны быть равны.");
        _row = row; _col = col;
        _data = new T[_row * _col];
	}

};