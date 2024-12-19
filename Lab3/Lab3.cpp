#include <iostream>
#include <iomanip>
#include <windows.h>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cmath>
// #ifdef _OPENMP
// #include <omp.h>
// #endif

using namespace std;


template<typename T>
struct StructResult {
    T _result;
    double _time;

    StructResult(T result, double time) : 
        _result(result), _time(time) {}

    void print() const {
        cout << "[result: " << _result
                << "; time: " << _time << "]" << endl;
    }
    
    static bool compare(const StructResult<T>& left, const StructResult<T>& right) const { 
        return left._time < right._time; 
    }
};


template<typename T = double>
class Vector {
    size_t _size;
    vector<T> _data;
    bool _isInit = false;

public:
    Vector(size_t size) : _size(size) {}

	void fillRandom(long double min, long double max) {
        _data.clear();
        for (size_t i = 0; i < _size; ++i)
        {
            _data.push_back(min + T(rand()) / (T(RAND_MAX / (max - min))));
        }
        _isInit = true;
    }

    void fillByVal(T val) {
        _data.clear();
        for (size_t i = 0; i < _size; ++i)
        {
            _data.push_back(val);
        }
        _isInit = true;
    }

    T minEl(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T min_el = _data[startIdx];
                for (size_t i = startIdx + 1; i <= endIdx; ++i)
                {
                    if (min_el > _data[i]) {
                        min_el = _data[i];
                    }
                }

                return min_el;
            }
            else { throw logic_error("Выход за пределы массива!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T minEl(size_t endIdx) const { return minEl(0, endIdx); }
    T minEl() const { return minEl(0, _size - 1); }

    T maxEl(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T max_el = _data[startIdx];
                for (size_t i = startIdx + 1; i <= endIdx; ++i)
                {
                    if (max_el < _data[i]) {
                        max_el = _data[i];
                    }
                }

                return max_el;
            }
            else { throw logic_error("Выход за пределы массива!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T maxEl(size_t endIdx) const { return maxEl(0, endIdx); }
    T maxEl() const { return maxEl(0, _size - 1); }

    size_t indexMinEl(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                size_t min_idx = startIdx;
                for (size_t i = startIdx + 1; i <= endIdx; ++i) {
                    if (_data[min_idx] > _data[i]) {
                        min_idx = i;
                    }
                }

                return min_idx;
            }
            else { throw logic_error("Выход за пределы массива!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T indexMinEl(size_t endIdx) const { return indexMinEl(0, endIdx); }
    T indexMinEl() const { return indexMinEl(0, _size - 1); }

    size_t indexMaxEl(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                size_t max_idx = startIdx;
                for (size_t i = startIdx + 1; i <= endIdx; ++i) {
                    if (_data[max_idx] < _data[i]) {
                        max_idx = i;
                    }
                }

                return max_idx;
            }
            else { throw logic_error("Выход за пределы массива!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T indexMaxEl(size_t endIdx) const { return indexMaxEl(0, endIdx); }
    T indexMaxEl() const { return indexMaxEl(0, _size - 1); }

    T sum(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T result = 0;
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    result += _data[i];
                }

                return result;
            } else {throw logic_error("Выход за пределы массива!");}
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    T sum(size_t endIdx) const { return sum(0, endIdx); }
    T sum() const { return sum(0, _size - 1); }

    T mean(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T sum = 0;
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    sum += _data[i];
                }

                return sum / endIdx - startIdx;
            } else {throw logic_error("Выход за пределы массива!");}
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    T mean(size_t endIdx) const { return mean(0, endIdx); }
    T mean() const { return mean(0, _size - 1); }

    T euclideanNorm(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T sum = 0;
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    sum += _data[i] * _data[i];
                }

                return sqrt(sum);
            } else {throw logic_error("Выход за пределы массива!");}
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    T euclideanNorm(size_t endIdx) const { return euclideanNorm(0, endIdx); }
    T euclideanNorm() const { return euclideanNorm(0, _size - 1); }

    T manhattanNorm(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T result = 0;
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    result += abs(_data[i]);
                }

                return result;
            } else {throw logic_error("Выход за пределы массива!");}
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    T manhattanNorm(size_t endIdx) const { return manhattanNorm(0, endIdx); }
    T manhattanNorm() const { return manhattanNorm(0, _size - 1); }

    Vector operator*(const Vector& v) const {
        if (_isInit && v._isInit) {
            if (_size == v._size) {
                Vector result(_size);
                for (size_t i = 0; i < _size; ++i) {
                    result._data.push_back(_data[i] * v._data[i]);
                }

                return result;
            }
            else { throw logic_error("Векторы разного размера!");}
        } else { throw logic_error("Векторы не инициализированы!"); }
    }

    void print(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                for (size_t i = startIdx; i <= endIdx; ++i)
                {
                    cout << fixed << setprecision(5) << _data[i] << " ";
                }
                cout << endl << endl;
            } else {throw logic_error("Выход за пределы массива!");}
        } else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    void print(size_t endIdx) const { return print(0, endIdx); }
    void print() const { return print(0, _size - 1); }

};

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));
    size_t size = 10'000'000;
    Vector vec1(size);
    vec1.fillRandom(-10.5, 10.5);
    vec1.print(10);
    // cout << vec1.mean(10) << endl;
}