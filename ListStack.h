#pragma once

#include "StackImplementation.h"
#include "LinkedListTemplate.h"

template<typename T>
class ListStack : public StackImplementation<T>, public LinkedList<T>
{
public:
	ListStack();
    ListStack(const ListStack<T>& other);
	ListStack<T>& operator=(const ListStack& other);

	~ListStack() = default;

	void push(const T& value) override;
	void pop() override;
	const T& top() const override;
	bool isEmpty() const override;

	size_t size() const override;

private:
	LinkedList<T> _list;
};

template<typename T>
ListStack<T>::ListStack() {
    _list = LinkedList<T>();
}

template<typename T>
ListStack<T>::ListStack(const ListStack<T>& other) {
    _list = other._list;
}

template<typename T>
ListStack<T>& ListStack<T>::operator=(const ListStack<T>& other) {
    _list = other._list;
    return *this;
}

template<typename T>
void ListStack<T>::push(const T& value) {
	_list.pushBack(value);
}

template<typename T>
void ListStack<T>::pop() {
	_list.removeBack();
}

template<typename T>
const T& ListStack<T>::top() const {
	if (isEmpty()) {
		throw std::out_of_range("Stack is empty");
	}
	return _list[size() - 1];
}

template<typename T>
bool ListStack<T>::isEmpty() const {
	return _list.size() == 0;
}

template<typename T>
size_t ListStack<T>::size() const {
	return _list.size();
}
