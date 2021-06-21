#pragma once

#include <cstddef>

const size_t ResizeCoef = 2;
const size_t ResizeDelta = 8;

enum class ResizeStrategy {
    Additive,
    Multiplicative
};

template<typename T>
class MyVector {
public:
    struct Iterator {
    public:

        Iterator(T* ptr);
        Iterator(const Iterator& other);
        Iterator& operator=(const Iterator& other);
        T& operator*() const;
        Iterator& operator++();
        Iterator operator++(int);
        bool operator!=(const Iterator& other) const;
        bool operator==(const Iterator& other) const;
        bool operator<(const Iterator& other) const;
        bool operator>(const Iterator& other) const;
        bool operator<=(const Iterator& other) const;
        bool operator>=(const Iterator& other) const;
    private:
        T* _ptr;
    };

    Iterator begin() const;
    Iterator end() const noexcept;

    MyVector(const size_t size = 0,
            ResizeStrategy strategy = ResizeStrategy::Multiplicative,
            size_t coef = 2);
    MyVector(MyVector& other);
    MyVector(MyVector&& other) noexcept;

    ~MyVector();

    T& at(size_t idx);
    const T& at(size_t idx) const;
    T& operator[](const size_t i);
    const T& operator[](const size_t i) const;


    void pushBack(const T& value);
    void pushFront(const T& value);
    void insert(const T& value, size_t idx);

    void clear();
    void erase(size_t i);
    void erase(size_t i, size_t len);
    void popBack();
    size_t size();

private:
    T* _data;
    size_t _size;
    size_t _capacity;
    size_t _resizeCoef;
    ResizeStrategy _strategy;
};

template<typename T>
MyVector<T>::MyVector(const size_t size, ResizeStrategy strategy, size_t coef) {
    this->_size = size;
    this->_capacity = 1;
    this->_resizeCoef = coef;
    this->_strategy = strategy;
    if (_size > 0) {
        if(_strategy == ResizeStrategy::Multiplicative) {
            _capacity = _size * ResizeCoef;
        }
        else{
            _capacity = _size + ResizeCoef;
        }
    }
     _data = new T[_capacity];
}

template<typename T>
MyVector<T>::MyVector(MyVector<T>& other) {
    _size = other.size();
    _capacity = other._capacity;
    _data = new T[_capacity];
    _strategy = other._strategy;
    _resizeCoef = other._resizeCoef;
    for(size_t i = 0; i < _size; ++i) {
            _data[i] = other[i];
    }
}

template<typename T>
MyVector<T>::MyVector(MyVector<T>&& other) noexcept {
    _size = other.size();
    _data = other._data;
    _capacity = other._capacity;
    _strategy = other._strategy;
    _resizeCoef = other._resizeCoef;
    other._size = 0;
    other._data = nullptr;
    other._capacity = 0;
    other._resizeCoef = 0;
    other._strategy = ResizeStrategy::Multiplicative;
}

template<typename T>
MyVector<T>::~MyVector() {
    delete[] _data;
    _size = 0;
    _capacity = 0;
    _resizeCoef = 0;
    _strategy = ResizeStrategy::Multiplicative;
}

template<typename T>
T& MyVector<T>::at(size_t idx) {
    if (idx >= _size) {
		throw std::out_of_range("idx >= size");
	}
    return _data[idx];
}

template<typename T>
const T& MyVector<T>::at(const size_t idx) const {
       if (idx >= _size) {
		throw std::out_of_range("idx >= size");
	}
    return _data[idx];
}

template<typename T>
T& MyVector<T>::operator[](const size_t idx) {
    return at(idx);
}

template<typename T>
const T& MyVector<T>::operator[](const size_t idx) const {
    return at(idx);
}

template<typename T>
void MyVector<T>::insert(const T& value, size_t idx) {
    if(idx > _size) {
        throw std::out_of_range("idx > size");
    }
    T* newData = _data;
    if(_size == _capacity) {
        if(_strategy == ResizeStrategy::Multiplicative) {
            _capacity *= ResizeCoef;
        }
        else {
            _capacity += ResizeCoef;
        }
        newData = new T[_capacity];
        for(size_t i = 0; i < idx; i++) {
            newData[i] = _data[i];
        }
    }
    for(size_t i = _size; i > idx; i--) {
        newData[i] = _data[i];
    }
    if(_size == _capacity) {
        delete[] _data;
        _data = newData;
    }
    _data[idx] = value;
    _size++;
}

template<typename T>
void MyVector<T>::pushBack(const T& value) {
    this->insert(value, _size);
}

template<typename T>
void MyVector<T>::pushFront(const T& value) {
    this->insert(value, 0);
}

template<typename T>
void MyVector<T>::clear() {
    delete[] _data;
    _size = 0;
    _capacity = 1;
    _data = new T[_capacity];
    _resizeCoef = 0;
}

template<typename T>
void MyVector<T>::erase(size_t idx) {
    this->erase(idx, 1);
}

template<typename T>
void MyVector<T>::erase(size_t idx, size_t len) {
    if (idx >= _size) {
        throw std::out_of_range("idx >= _size");
    }
    if (idx + len - 1 >= _size) {
        len = _size - idx;
    }
    T* newData = _data;
    size_t newSize = _size - len;
    bool flag = false;
    if(_strategy == ResizeStrategy::Multiplicative) {
        while(newSize < (_capacity / ResizeCoef)) {
            _capacity /= ResizeCoef;
            flag = true;
        }
    }
    else {
        while(newSize < (_capacity - _resizeCoef)) {
            _capacity -= _resizeCoef;
        }
    }
    if(flag) {
        newData = new T[_capacity];
        for(size_t i = 0; i < idx; i++) {
            newData[i] = _data[i];
        }
    }
    for(size_t i = idx; i < newSize; i++) {
        newData[i] = _data[i + len];
    }
    if(flag) {
        delete[] _data;
        _data = newData;
    }
    _size = newSize;
}

template<typename T>
void MyVector<T>::popBack() {
    if (_size == 0) {
        throw std::out_of_range("size = 0");
    }
    this->erase(_size - 1);
}

template<typename T>
size_t MyVector<T>::size() {
    return _size;
}

template<typename T>
MyVector<T>::Iterator::Iterator(T* ptr) {
    _ptr = ptr;
}

template<typename T>
MyVector<T>::Iterator::Iterator(const MyVector<T>::Iterator& other) {
    _ptr = other._ptr;
}

template<typename T>
typename MyVector<T>::Iterator& MyVector<T>::Iterator::operator=(const MyVector<T>::Iterator& other) {
    _ptr = other._ptr;
    return *this;
}

template<typename T>
T& MyVector<T>::Iterator::operator*() const {
    return *_ptr;
}

template<typename T>
typename MyVector<T>::Iterator& MyVector<T>::Iterator::operator++() {
    _ptr++;
    return *this;
}

template<typename T>
typename MyVector<T>::Iterator MyVector<T>::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename T>
bool MyVector<T>::Iterator::operator!=(const MyVector<T>::Iterator& other) const {
    return _ptr != other._ptr;
}

template<typename T>
bool MyVector<T>::Iterator::operator==(const MyVector<T>::Iterator& other) const {
    return !(*this != other);
}

template<typename T>
bool MyVector<T>::Iterator::operator<(const MyVector<T>::Iterator& other) const {
    return _ptr < other._ptr;
}

template<typename T>
bool MyVector<T>::Iterator::operator>(const MyVector<T>::Iterator& other) const {
    return _ptr > other._ptr;
}

template<typename T>
bool MyVector<T>::Iterator::operator<=(const MyVector<T>::Iterator& other) const {
    return !(*this > other);
}

template<typename T>
bool MyVector<T>::Iterator::operator>=(const MyVector<T>::Iterator& other) const {
    return !(*this < other);
}

template<typename T>
typename MyVector<T>::Iterator MyVector<T>::begin() const {
    return MyVector<T>::Iterator(&_data[0]);
}

template<typename T>
typename MyVector<T>::Iterator MyVector<T>::end() const noexcept {
    return MyVector<T>::Iterator(&_data[_size]);
}
