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

template<typename T = double>
class Vector {
    vector<T> _data;
    size_t _size;
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

    T sum() const {
        if (_isInit) {
            T result = 0;
            for (const auto& val : _data)
            {
                result += val;
            }
            
            return result;
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }

    T average() const {
        if (!_isInit) throw logic_error("Вектор не инициализирован!");
        T sum = 0;
        for (const auto& val : _data) {
            sum += val;
        }

        return sum / _size;
    }

    T euclideanNorm() const {
        if (!_isInit) throw logic_error("Вектор не инициализирован!");
        T sum = 0;
        for (const auto& val : _data) {
            sum += val * val;
        }

        return sqrt(sum);
    }

    T manhattanNorm() const {
        if (!_isInit) throw logic_error("Вектор не инициализирован!");
        T sum = 0;
        for (const auto& val : _data) {
            sum += abs(val);
        }

        return sum;
    }

    void print() const {
        if (_isInit) {
            for (size_t i = 0; i < _size; ++i)
            {
                cout << _data[i] << " ";
            }
            cout << endl;
        } else {
            throw logic_error("Вектор не инициализирован!");
        }
    }

    void print(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            for (size_t i = startIdx; i <= endIdx; ++i)
            {
                cout << _data[i] << " ";
            }
            cout << endl;
        } else {
            throw logic_error("Вектор не инициализирован!");
        }
    }

};

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));
    size_t size = 10'000'000;
    Vector vec1(size);
    vec1.fillRandom(-10.8, 10.8);
    vec1.print(0, 10);
}