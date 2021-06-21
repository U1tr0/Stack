#pragma once

#include "StackImplementation.h"
#include "MyVectorTemplate.h"

template<typename T>
class VectorStack : public StackImplementation<T>, public MyVector<T>
{
public:
	VectorStack();
    VectorStack(const VectorStack<T>& other);
	VectorStack<T>& operator=(const VectorStack& other);

	~VectorStack() = default;

	void push(const T& value) override;
	void pop() override;
	const T& top() const override;
	bool isEmpty() const override;

	size_t size() const override;

private:
	MyVector<T> _vector;
};

template<typename T>
VectorStack<T>::VectorStack() {
    _vector = MyVector<T>();
}

template<typename T>
VectorStack<T>::VectorStack(const VectorStack<T>& other) {
    _vector = other._vector;
}

template<typename T>
VectorStack<T>& VectorStack<T>::operator=(const VectorStack<T>& other) {
	_vector = other._vector;
	return *this;
}

template<typename T>
void VectorStack<T>::push(const T& value) {
	_vector.pushBack(value);
}

template<typename T>
void VectorStack<T>::pop() {
	_vector.popBack();
}

template<typename T>
const T& VectorStack<T>::top() const {
	if (isEmpty()) {
		throw std::out_of_range("Stack is empty");
	}
	return _vector[size() - 1];
}

template<typename T>
bool VectorStack<T>::isEmpty() const {
	return _vector.size() == 0;
}

template<typename T>
size_t VectorStack<T>::size() const {
	return _vector.size();
}
