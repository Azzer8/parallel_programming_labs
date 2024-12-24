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
#include <omp.h>

using namespace std;


template<typename T>
struct StructResult {
    const T _result;
    const double _time;

    StructResult(T result, double time) : 
        _result(result), _time(time) {}

    void print() const {
        cout << "[result: " << _result
                << "; time: " << _time << "]" << endl;
    }
    
    static bool compare(const StructResult<T>& left, const StructResult<T>& right) { 
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

    size_t size() const {
        if (!_isInit) { throw logic_error("Вектор не инициализирован!"); }
        return _data.size();
    }

    T operator[](size_t index) const {
        if (!_isInit) { throw logic_error("Вектор не инициализирован!"); }
        if (index >= _size) {
            throw out_of_range("Выход за пределы вектора!");
        }
        return _data[index];
    }

    T minEl(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T min_el = _data[startIdx];
                for (size_t i = startIdx + 1; i < endIdx; ++i)
                {
                    if (min_el > _data[i]) {
                        min_el = _data[i];
                    }
                }

                return min_el;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T minEl(size_t endIdx) const { return minEl(0, endIdx); }
    T minEl() const { return minEl(0, _size); }

    void minEl_thread(size_t startIdx, size_t endIdx, T& min_el, mutex& m) {
        T local_min = minEl(startIdx, endIdx);

        lock_guard<mutex> lock(m);
        if (min_el > local_min) {
            min_el = local_min;
        }
    }
    T minElParallel(size_t startIdx, size_t endIdx, unsigned threadsNum) {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                mutex m;
                T min_el = _data[startIdx];
                vector<thread> threads;
                size_t thChunkSize = (endIdx - startIdx + 1) / threadsNum;
                for (size_t i = 0; i < threadsNum; ++i) {
                    size_t thStartIdx = startIdx + i * thChunkSize;
                    size_t thEndIdx = thStartIdx + thChunkSize;
                    if (i == threadsNum - 1) { thEndIdx = endIdx; }

                    threads.push_back(thread(minEl_thread, this, thStartIdx, thEndIdx, ref(min_el), ref(m)));
                }

                for(auto& th : threads) { th.join(); }

                return min_el;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T minElParallel(size_t endIdx, unsigned threadsNum) {
        return minElParallel(0, endIdx, threadsNum); 
    }
    T minElParallel(unsigned threadsNum) {
        return minElParallel(0, _size, threadsNum);
    }

    T minElOMP(size_t startIdx, size_t endIdx, unsigned threadsNum) {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T min_el = _data[startIdx];
                #pragma omp parallel for num_threads(threadsNum) reduction(min:min_el)
                for (size_t i = startIdx + 1; i < endIdx; ++i)
                {
                    if (min_el > _data[i]) {
                        min_el = _data[i];
                    }
                }

                return min_el;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T minElOMP(size_t endIdx, unsigned threadsNum) {
        return minElOMP(0, endIdx, threadsNum);
    }
    T minElOMP(unsigned threadsNum) {
        return minElOMP(0, _size, threadsNum);
    }

    T maxEl(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T max_el = _data[startIdx];
                for (size_t i = startIdx + 1; i < endIdx; ++i)
                {
                    if (max_el < _data[i]) {
                        max_el = _data[i];
                    }
                }

                return max_el;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T maxEl(size_t endIdx) const { return maxEl(0, endIdx); }
    T maxEl() const { return maxEl(0, _size); }

    void maxEl_thread(size_t startIdx, size_t endIdx, T& max_el, mutex& m) {
        T local_max = maxEl(startIdx, endIdx);

        lock_guard<mutex> lock(m);
        if (max_el < local_max) {
            max_el = local_max;
        }
    }
    T maxElParallel(size_t startIdx, size_t endIdx, unsigned threadsNum) {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                mutex m;
                T max_el = _data[startIdx];
                vector<thread> threads;
                size_t thChunkSize = (endIdx - startIdx + 1) / threadsNum;
                for (size_t i = 0; i < threadsNum; ++i) {
                    size_t thStartIdx = startIdx + i * thChunkSize;
                    size_t thEndIdx = thStartIdx + thChunkSize;
                    if (i == threadsNum - 1) { thEndIdx = endIdx; }

                    threads.push_back(thread(maxEl_thread, this, thStartIdx, thEndIdx, ref(max_el), ref(m)));
                }

                for(auto& th : threads) { th.join(); }

                return max_el;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T maxElParallel(size_t endIdx, unsigned threadsNum) {
        return maxElParallel(0, endIdx, threadsNum); 
    }
    T maxElParallel(unsigned threadsNum) {
        return maxElParallel(0, _size, threadsNum);
    }

    T maxElOMP(size_t startIdx, size_t endIdx, unsigned threadsNum) {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T max_el = _data[startIdx];
                #pragma omp parallel for num_threads(threadsNum) reduction(max:max_el)
                for (size_t i = startIdx + 1; i < endIdx; ++i)
                {
                    if (max_el < _data[i]) {
                        max_el = _data[i];
                    }
                }

                return max_el;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T maxElOMP(size_t endIdx, unsigned threadsNum) {
        return maxElOMP(0, endIdx, threadsNum);
    }
    T maxElOMP(unsigned threadsNum) {
        return maxElOMP(0, _size, threadsNum);
    }

    size_t indexMinEl(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                size_t min_idx = startIdx;
                for (size_t i = startIdx + 1; i < endIdx; ++i) {
                    if (_data[min_idx] > _data[i]) {
                        min_idx = i;
                    }
                }

                return min_idx;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    size_t indexMinEl(size_t endIdx) const { return indexMinEl(0, endIdx); }
    size_t indexMinEl() const { return indexMinEl(0, _size); }

    void indexMinEl_thread(size_t startIdx, size_t endIdx, size_t& min_idx, mutex& m) {
        size_t local_idx = indexMinEl(startIdx, endIdx);

        lock_guard<mutex> lock(m);
        if ((*this)[min_idx] > (*this)[local_idx]) {
            min_idx = local_idx;
        }
    }
    size_t indexMinElParallel(size_t startIdx, size_t endIdx, unsigned threadsNum) {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                mutex m;
                size_t min_idx = startIdx;
                vector<thread> threads;
                size_t thChunkSize = (endIdx - startIdx + 1) / threadsNum;
                for (size_t i = 0; i < threadsNum; ++i) {
                    size_t thStartIdx = startIdx + i * thChunkSize;
                    size_t thEndIdx = thStartIdx + thChunkSize;
                    if (i == threadsNum - 1) { thEndIdx = endIdx; }

                    threads.push_back(thread(indexMinEl_thread, this, thStartIdx, thEndIdx, ref(min_idx), ref(m)));
                }

                for(auto& th : threads) { th.join(); }

                return min_idx;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    size_t indexMinElParallel(size_t endIdx, unsigned threadsNum) {
        return indexMinElParallel(0, endIdx, threadsNum); 
    }
    size_t indexMinElParallel(unsigned threadsNum) {
        return indexMinElParallel(0, _size, threadsNum);
    }

    size_t indexMinElOMP(size_t startIdx, size_t endIdx, unsigned threadsNum) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                size_t min_idx = startIdx;
                #pragma omp parallel num_threads(threadsNum)
                {
                    size_t local_idx = startIdx;
                    #pragma omp for
                    for (size_t i = startIdx + 1; i <= endIdx; ++i) {
                        if (_data[local_idx] > _data[i]) {
                            local_idx = i;
                        }
                    }
                    #pragma omp critical
                    {
                        if (_data[min_idx] > _data[local_idx]) {
                            min_idx = local_idx;
                        }
                    }
                }

                return min_idx;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    size_t indexMinElOMP(size_t endIdx, unsigned threadsNum) const {
        return indexMinElOMP(0, endIdx, threadsNum);
    }
    size_t indexMinElOMP(unsigned threadsNum) const {
        return indexMinElOMP(0, _size, threadsNum);
    }

    size_t indexMaxEl(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                size_t max_idx = startIdx;
                for (size_t i = startIdx + 1; i < endIdx; ++i) {
                    if (_data[max_idx] < _data[i]) {
                        max_idx = i;
                    }
                }

                return max_idx;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    size_t indexMaxEl(size_t endIdx) const { return indexMaxEl(0, endIdx); }
    size_t indexMaxEl() const { return indexMaxEl(0, _size); }

    void indexMaxEl_thread(size_t startIdx, size_t endIdx, size_t& max_idx, mutex& m) {
        size_t local_idx = indexMaxEl(startIdx, endIdx);

        lock_guard<mutex> lock(m);
        if ((*this)[max_idx] < (*this)[local_idx]) {
            max_idx = local_idx;
        }
    }
    size_t indexMaxElParallel(size_t startIdx, size_t endIdx, unsigned threadsNum) {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                mutex m;
                size_t max_idx = startIdx;
                vector<thread> threads;
                size_t thChunkSize = (endIdx - startIdx + 1) / threadsNum;
                for (size_t i = 0; i < threadsNum; ++i) {
                    size_t thStartIdx = startIdx + i * thChunkSize;
                    size_t thEndIdx = thStartIdx + thChunkSize;
                    if (i == threadsNum - 1) { thEndIdx = endIdx; }

                    threads.push_back(thread(indexMaxEl_thread, this, thStartIdx, thEndIdx, ref(max_idx), ref(m)));
                }

                for(auto& th : threads) { th.join(); }

                return max_idx;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    size_t indexMaxElParallel(size_t endIdx, unsigned threadsNum) {
        return indexMaxElParallel(0, endIdx, threadsNum); 
    }
    size_t indexMaxElParallel(unsigned threadsNum) {
        return indexMaxElParallel(0, _size, threadsNum);
    }

    size_t indexMaxElOMP(size_t startIdx, size_t endIdx, unsigned threadsNum) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                size_t max_idx = startIdx;
                #pragma omp parallel num_threads(threadsNum)
                {
                    size_t local_idx = startIdx;
                    #pragma omp for
                    for (size_t i = startIdx + 1; i < endIdx; ++i) {
                        if (_data[local_idx] < _data[i]) {
                            local_idx = i;
                        }
                    }
                    #pragma omp critical
                    {
                        if (_data[max_idx] < _data[local_idx]) {
                            max_idx = local_idx;
                        }
                    }
                }

                return max_idx;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    size_t indexMaxElOMP(size_t endIdx, unsigned threadsNum) const {
        return indexMaxElOMP(0, endIdx, threadsNum);
    }
    size_t indexMaxElOMP(unsigned threadsNum) const {
        return indexMaxElOMP(0, _size, threadsNum);
    }

    T sumEl(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T result = 0;
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    result += _data[i];
                }

                return result;
            } else { throw logic_error("Выход за пределы вектора!"); }
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    T sumEl(size_t endIdx) const { return sumEl(0, endIdx); }
    T sumEl() const { return sumEl(0, _size); }

    void sum_thread(size_t startIdx, size_t endIdx, T& sum_result, mutex& m) {
        T sum_local = sumEl(startIdx, endIdx);

        lock_guard<mutex> lock(m);
        sum_result += sum_local;
    }
    T sumElParallel(size_t startIdx, size_t endIdx, unsigned threadsNum) {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                mutex m;
                T sum_result = 0;
                vector<thread> threads;
                size_t thChunkSize = (endIdx - startIdx + 1) / threadsNum;
                for (size_t i = 0; i < threadsNum; ++i) {
                    size_t thStartIdx = startIdx + i * thChunkSize;
                    size_t thEndIdx = thStartIdx + thChunkSize;
                    if (i == threadsNum - 1) { thEndIdx = endIdx; }

                    threads.push_back(thread(sum_thread, this, thStartIdx, thEndIdx, ref(sum_result), ref(m)));
                }

                for(auto& th : threads) { th.join(); }

                return sum_result;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T sumElParallel(size_t endIdx, unsigned threadsNum) {
        return sumElParallel(0, endIdx, threadsNum); 
    }
    T sumElParallel(unsigned threadsNum) {
        return sumElParallel(0, _size, threadsNum);
    }

    T sumElOMP(size_t startIdx, size_t endIdx, unsigned threadsNum) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T result = 0;
                #pragma omp parallel for num_threads(threadsNum) reduction(+:result)
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    result += _data[i];
                }

                return result;
            } else { throw logic_error("Выход за пределы вектора!"); }
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    T sumElOMP(size_t endIdx, unsigned threadsNum) const {
        return sumElOMP(0, endIdx, threadsNum);
    }
    T sumElOMP(unsigned threadsNum) const {
        return sumElOMP(0, _size, threadsNum);
    }

    T meanEl(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T sum = 0;
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    sum += _data[i];
                }

                return sum / (endIdx - startIdx) ;
            } else { throw logic_error("Выход за пределы вектора!"); }
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    T meanEl(size_t endIdx) const { return meanEl(0, endIdx); }
    T meanEl() const { return meanEl(0, _size); }

    T meanElParallel(size_t startIdx, size_t endIdx, unsigned threadsNum) {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                mutex m;
                T sum_result = 0;
                vector<thread> threads;
                size_t thChunkSize = (endIdx - startIdx + 1) / threadsNum;
                for (size_t i = 0; i < threadsNum; ++i) {
                    size_t thStartIdx = startIdx + i * thChunkSize;
                    size_t thEndIdx = thStartIdx + thChunkSize;
                    if (i == threadsNum - 1) { thEndIdx = endIdx; }

                    threads.push_back(thread(sum_thread, this, thStartIdx, thEndIdx, ref(sum_result), ref(m)));
                }

                for(auto& th : threads) { th.join(); }

                return sum_result / (endIdx - startIdx);
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T meanElParallel(size_t endIdx, unsigned threadsNum) {
        return meanElParallel(0, endIdx, threadsNum); 
    }
    T meanElParallel(unsigned threadsNum) {
        return meanElParallel(0, _size, threadsNum);
    }

    T meanElOMP(size_t startIdx, size_t endIdx, unsigned threadsNum) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T sum = 0;
                #pragma omp parallel for num_threads(threadsNum) reduction(+:sum)
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    sum += _data[i];
                }

                return sum / (endIdx - startIdx) ;
            } else { throw logic_error("Выход за пределы вектора!"); }
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    T meanElOMP(size_t endIdx, unsigned threadsNum) const {
        return meanElOMP(0, endIdx, threadsNum);
    }
    T meanElOMP(unsigned threadsNum) const {
        return meanElOMP(0, _size, threadsNum);
    }

    T euclidNorm(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T sum_sq = 0;
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    sum_sq += _data[i] * _data[i];
                }

                return sqrt(sum_sq);
            } else { throw logic_error("Выход за пределы вектора!"); }
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    T euclidNorm(size_t endIdx) const { return euclidNorm(0, endIdx); }
    T euclidNorm() const { return euclidNorm(0, _size); }

    void euclidNorm_thread(size_t startIdx, size_t endIdx, T& euclidNorm_result, mutex& m) {
        T euclidNorm_local = pow(euclidNorm(startIdx, endIdx), 2);

        lock_guard<mutex> lock(m);
        euclidNorm_result += euclidNorm_local;
    }
    T euclidNormParallel(size_t startIdx, size_t endIdx, unsigned threadsNum) {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                mutex m;
                T euclidNorm_result = 0;
                vector<thread> threads;
                size_t thChunkSize = (endIdx - startIdx + 1) / threadsNum;
                for (size_t i = 0; i < threadsNum; ++i) {
                    size_t thStartIdx = startIdx + i * thChunkSize;
                    size_t thEndIdx = thStartIdx + thChunkSize;
                    if (i == threadsNum - 1) { thEndIdx = endIdx; }

                    threads.push_back(thread(euclidNorm_thread, this, thStartIdx, thEndIdx, ref(euclidNorm_result), ref(m)));
                }

                for(auto& th : threads) { th.join(); }

                return sqrt(euclidNorm_result);
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T euclidNormParallel(size_t endIdx, unsigned threadsNum) {
        return euclidNormParallel(0, endIdx, threadsNum); 
    }
    T euclidNormParallel(unsigned threadsNum) {
        return euclidNormParallel(0, _size, threadsNum);
    }

    T euclidNormOMP(size_t startIdx, size_t endIdx, unsigned threadsNum) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T sum_sq = 0;
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    sum_sq += _data[i] * _data[i];
                }

                return sqrt(sum_sq);
            } else { throw logic_error("Выход за пределы вектора!"); }
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    T euclidNormOMP(size_t endIdx, unsigned threadsNum) const {
        return euclidNormOMP(0, endIdx, threadsNum);
    }
    T euclidNormOMP(unsigned threadsNum) const {
        return euclidNormOMP(0, _size, threadsNum);
    }

    T mnhtnNorm(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T result = 0;
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    result += abs(_data[i]);
                }

                return result;
            } else {throw logic_error("Выход за пределы вектора!");}
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    T mnhtnNorm(size_t endIdx) const { return mnhtnNorm(0, endIdx); }
    T mnhtnNorm() const { return mnhtnNorm(0, _size); }

    void mnhtnNorm_thread(size_t startIdx, size_t endIdx, T& mnhtnNorm_result, mutex& m) {
        T mnhtnNorm_local = mnhtnNorm(startIdx, endIdx);

        lock_guard<mutex> lock(m);
        mnhtnNorm_result += mnhtnNorm_local;
    }
    T mnhtnNormParallel(size_t startIdx, size_t endIdx, unsigned threadsNum) {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                mutex m;
                T mnhtnNorm_result = 0;
                vector<thread> threads;
                size_t thChunkSize = (endIdx - startIdx + 1) / threadsNum;
                for (size_t i = 0; i < threadsNum; ++i) {
                    size_t thStartIdx = startIdx + i * thChunkSize;
                    size_t thEndIdx = thStartIdx + thChunkSize;
                    if (i == threadsNum - 1) { thEndIdx = endIdx; }

                    threads.push_back(thread(mnhtnNorm_thread, this, thStartIdx, thEndIdx, ref(mnhtnNorm_result), ref(m)));
                }

                for(auto& th : threads) { th.join(); }

                return mnhtnNorm_result;
            }
            else { throw logic_error("Выход за пределы вектора!"); }
        } else { throw logic_error("Вектор не инициализирован!"); }
    }
    T mnhtnNormParallel(size_t endIdx, unsigned threadsNum) {
        return mnhtnNormParallel(0, endIdx, threadsNum); 
    }
    T mnhtnNormParallel(unsigned threadsNum) {
        return mnhtnNormParallel(0, _size, threadsNum);
    }

    T mnhtnNormOMP(size_t startIdx, size_t endIdx, unsigned threadsNum) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                T result = 0;
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    result += abs(_data[i]);
                }

                return result;
            } else {throw logic_error("Выход за пределы вектора!");}
        }
        else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    T mnhtnNormOMP(size_t endIdx, unsigned threadsNum) const {
        return mnhtnNormOMP(0, endIdx, threadsNum);
    }
    T mnhtnNormOMP(unsigned threadsNum) const {
        return mnhtnNormOMP(0, _size, threadsNum);
    }

    Vector operator*(const Vector& other_v) const {
        if (_isInit && other_v._isInit) {
            if (_size == other_v._size) {
                Vector result(_size);
                for (size_t i = 0; i < _size; ++i) {
                    result._data.push_back(_data[i] * other_v._data[i]);
                }

                return result;
            }
            else { throw logic_error("Векторы разного размера!");}
        } else { throw logic_error("Векторы не инициализированы!"); }
    }

    void print(size_t startIdx, size_t endIdx) const {
        if (_isInit) {
            if (endIdx < _size && endIdx > startIdx) {
                for (size_t i = startIdx; i < endIdx; ++i)
                {
                    cout << fixed << setprecision(2) << _data[i] << " ";
                }
                cout << endl << endl;
            } else {throw logic_error("Выход за пределы вектора!");}
        } else {
            throw logic_error("Вектор не инициализирован!");
        }
    }
    void print(size_t endIdx) const { return print(0, endIdx); }
    void print() const { return print(0, _size); }

};

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));
    size_t size = 100'000'000;
    Vector vec1(size);
    vec1.fillRandom(-10.5, 10.5);
    vec1.print(10);

    cout << setw(8) << vec1.maxEl(10) << " | " << setw(8) << vec1.maxElParallel(10, 4) << " | " << vec1.maxElOMP(10, 4) << endl;
    cout << setw(8) << vec1.minEl(10) << " | " << setw(8) << vec1.minElParallel(10, 4) << " | " << vec1.minElOMP(10, 4) << endl;
    cout << setw(8) << vec1.indexMaxEl(10) << " | " << setw(8) << vec1.indexMaxElParallel(10, 4) << " | " << vec1.indexMaxElOMP(10, 4) << endl;
    cout << setw(8) << vec1.indexMinEl(10) << " | " << setw(8) << vec1.indexMinElParallel(10, 4) << " | " << vec1.indexMinElOMP(10, 4) << endl;
    cout << setw(8) << vec1.sumEl(10) << " | " << setw(8) << vec1.sumElParallel(10, 4) << " | " << vec1.sumElOMP(10, 4) << endl;
    cout << setw(8) << vec1.meanEl(10) << " | " << setw(8) << vec1.meanElParallel(10, 4) << " | " << vec1.meanElOMP(10, 4) << endl;
    cout << setw(8) << vec1.euclidNorm(10) << " | " << setw(8) << vec1.euclidNormParallel(10, 4) << " | " << vec1.euclidNormOMP(10, 4) << endl;
    cout << setw(8) << vec1.mnhtnNorm(10) << " | " << setw(8) << vec1.mnhtnNormParallel(10, 4) << " | " << vec1.mnhtnNormOMP(10, 4) << endl;
}