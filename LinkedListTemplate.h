#pragma once
#include <iostream>
#include <cassert>
#include <cstddef>


template<typename T>
class LinkedList
{
	struct Node {
		Node(const T& value, Node* next = nullptr);
		~Node() = default;

		void insertNext(const T& value);
		void removeNext();

		T _value;
		Node* _next;
	};

	struct Iterator {
    public:
        Iterator(Node* ptr, size_t idx = 0);
        Iterator(const Iterator& other);
        Iterator& operator=(const Iterator& other);
        T& operator*() const;
        bool operator!=(const Iterator& other) const;
        bool operator==(const Iterator& other) const;
        Iterator& operator++();
        Iterator operator++(int);

        size_t getIndex() const;
        Node* getPtr() const;
    private:
        Node* _ptr;
        size_t _index;
    };

public:

    Iterator begin();
    Iterator end() noexcept;

	LinkedList();
	LinkedList(const LinkedList& copyList);
	LinkedList& operator=(const LinkedList& copyList);

	LinkedList(LinkedList&& moveList) noexcept;
	LinkedList& operator=(LinkedList&& moveList) noexcept;

	~LinkedList();

	const T& operator[](const size_t pos) const;
	T& operator[](const size_t pos);
	LinkedList::Node* getNode(const size_t pos) const;

	void insert(const size_t pos, const T& value);
	void insertAfterNode(Node* node, const T& value);
	void pushBack(const T& value);
	void pushFront(const T& value);

	void remove(const size_t pos);
	void removeNextNode(Node* node);
	void removeFront();
	void removeBack();

	long long int findIndex(const T& value) const;
	Node* findNode(const T& value) const;

	void reverse();
	LinkedList getReverseList() const;

	size_t size() const;
	void clear();

	void forEach(void (*fn)(T&));
	void map(T (*fn)(T));
	void filter(bool (*fn)(T));
private:
	Node*	_head;
	size_t	_size;

	void forceNodeDelete(Node* node);
};

template<typename T>
LinkedList<T>::Node::Node(const T& value, Node* next) {
	_value = value;
	_next = next;
}

template<typename T>
void LinkedList<T>::Node::insertNext(const T& value) {
	Node* newNode = new Node(value, _next);
	_next = newNode;
}

template<typename T>
void LinkedList<T>::Node::removeNext() {
	Node* removeNode = _next;
	Node* newNext = removeNode->_next;
	delete removeNode;
	_next = newNext;
}

template<typename T>
LinkedList<T>::LinkedList() {
    _head = nullptr;
    _size = 0;
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) {
	_size = other._size;
	if(_size == 0) {
            _head = nullptr;
	}
	else {
        _head = new Node(other._head->_value);
        Node* cur = _head;
        Node* curCopy = other._head;
        while (curCopy->_next) {
			cur->_next = new Node(curCopy->_next->_value);
			curCopy = curCopy->_next;
			cur = cur->_next;
		}
	}
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
	this->clear();
    _size = other._size;
    _head = nullptr;
    if (_size > 0) {
        Node* temp = other._head;
        Node* tempNew = new Node(temp->_value);
        Node* nextNew = nullptr;
        _head = tempNew;
        temp = temp->_next;
        while (temp != nullptr) {
            nextNew = new Node(temp->_value);
            tempNew->_next = nextNew;
            tempNew = nextNew;
            temp = temp->_next;
        }
    }
    return *this;
}

template<typename T>
LinkedList<T>::LinkedList(LinkedList<T>&& other) noexcept {
	this->_size = other._size;
	this->_head = other._head;
	other._size = 0;
	other._head = nullptr;
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& moveList) noexcept {
	if (this == &moveList) {
		return *this;
	}
	forceNodeDelete(_head);
	this->_size = moveList._size;
	this->_head = moveList._head;

	moveList._size = 0;
	moveList._head = nullptr;

	return *this;
}

template<typename T>
LinkedList<T>::~LinkedList() {
	forceNodeDelete(_head);
}

template<typename T>
const T& LinkedList<T>::operator[](const size_t pos) const {
	return getNode(pos)->_value;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::getNode(const size_t pos) const {
	if (pos < 0) {
		throw std::out_of_range("pos < 0");
	}
	else if (pos >= _size) {
		throw std::out_of_range("pos >= size");
	}
	Node* bufNode = this->_head;
	for (int i = 0; i < pos; ++i) {
		bufNode = bufNode->_next;
	}
	return bufNode;
}

template<typename T>
void LinkedList<T>::insert(const size_t pos, const T& value) {
	if (pos < 0) {
		throw std::out_of_range("pos < 0");
	}
	else if (pos > this->_size) {
		throw std::out_of_range("pos > 0");
	}
	if (pos == 0) {
		pushFront(value);
	}
	else {
        Node* temp = _head;
        for(size_t i = 1; i < pos; i++) {
            temp = temp->_next;
        }
        Node* newNode = new Node(value, temp->_next);
        temp->_next = newNode;
        _size++;
	}
}

template<typename T>
void LinkedList<T>::insertAfterNode(Node* node, const T& value) {
	node->insertNext(value);
	_size++;
}

template<typename T>
void LinkedList<T>::pushBack(const T& value) {
	if(_size == 0) {
        _head = new Node(value, _head);
        _size++;
	}
	else {
        insert(_size, value);
	}
}

template<typename T>
void LinkedList<T>::pushFront(const T& value) {
	_head = new Node(value, _head);
	++_size;
}

template<typename T>
void LinkedList<T>::remove(const size_t pos) {
    if(pos > _size) {
        throw std::out_of_range("pos > _size");
    }
    if(pos < 0) {
        throw std::out_of_range("pos < 0");
    }
	if (pos == 0) {
            removeFront();
	}
	else {
		Node* temp = _head;
		for (size_t i = 0; i < pos - 1; i++) {
			temp = temp->_next;
		}
		temp->removeNext();
		_size--;
	}

}

template<typename T>
void LinkedList<T>::removeFront() {
	if(_size == 1) {
		delete _head;
		_head = nullptr;
		_size = 0;
	}
	else {
        Node* temp = _head->_next;
        delete _head;
        _head = temp;
        _size--;
	}
}

template<typename T>
void LinkedList<T>::removeBack() {
	if (_size > 1) {
        getNode(_size - 2)->removeNext();
        _size--;
    }
	else {
        removeFront();
	}
}

template<typename T>
void LinkedList<T>::removeNextNode(Node* node) {
    node->removeNext();
	_size--;
}

template<typename T>
long long int LinkedList<T>::findIndex(const T& value) const {
    Node* temp = this-> _head;
	for (size_t i = 0; i < _size; i++) {
		if (temp->_value == value){
          return i;
        }
		temp = temp->_next;
	}
	return -1;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::findNode(const T& value) const {
    Node* temp = this->_head;
	for (size_t i = 0; i < _size; i++)
	{
		if (temp->_value == value){
            return temp;
		}
		temp = temp->_next;
	}
	return nullptr;
}

template<typename T>
void LinkedList<T>::reverse() {
    Node* prev = nullptr;
	Node* cur = _head;
	Node* next = _head->_next;
	while (next != nullptr) {
		cur->_next = prev;
		prev = cur;
		cur = next;
		next = cur->_next;
	}
	cur->_next = prev;
	_head = cur;
}

template<typename T>
LinkedList<T> LinkedList<T>::getReverseList() const {
    LinkedList<T>* newList = new LinkedList<T>;
	*newList = *this;
	newList->reverse();

	return *newList;
}

template<typename T>
size_t LinkedList<T>::size() const {
	return _size;
}

template<typename T>
void LinkedList<T>::forceNodeDelete(Node* node) {
	if (node == nullptr) {
		return;
	}
	Node* nextDeleteNode = node->_next;
	delete node;
	forceNodeDelete(nextDeleteNode);
}

template<typename T>
void LinkedList<T>::clear() {
    while (_head != nullptr) {
        this->removeFront();
    }
    _size = 0;
}

template<typename T>
LinkedList<T>::Iterator::Iterator(Node* ptr, size_t idx) {
    _ptr = ptr;
    _index = idx;
}

template<typename T>
LinkedList<T>::Iterator::Iterator(const Iterator& other) {
    _ptr = other._ptr;
    _index = other._index;
}

template<typename T>
typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator=(const Iterator& other) {
    _ptr = other._ptr;
    _index = other._index;
    return *this;
}

template<typename T>
T& LinkedList<T>::Iterator::operator*() const {
    return _ptr->_value;
}

template<typename T>
bool LinkedList<T>::Iterator::operator!=(const LinkedList<T>::Iterator& other) const {
    return (this->_ptr != other._ptr);
}

template<typename T>
bool LinkedList<T>::Iterator::operator==(const LinkedList<T>::Iterator& other) const {
    return (this->_ptr == other._ptr);
}

template<typename T>
typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++() {
    this->_ptr = this->_ptr->_next;
    this->_index++;
    return *this;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename T>
size_t LinkedList<T>::Iterator::getIndex() const {
    return this->_index;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::begin() {
    return Iterator(_head, 0);
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::end() noexcept {
    return Iterator(nullptr, -1);
}

template<typename T>
void LinkedList<T>::forEach(void (*fn)(T&)){
    Node* temp = _head;
    while(temp) {
        fn(temp->_value);
        temp = temp->_next;
    }
}

template<typename T>
void LinkedList<T>::map(T (*fn)(T)) {
    Node* temp = _head;
    while(temp) {
        temp->_value = fn(temp->_value);
        temp = temp->_next;
    }
}

template<typename T>
void LinkedList<T>::filter(bool (*fn)(T)) {
	Node* temp = _head;
	Node* prev = nullptr;
	while (temp) {
		if (!fn(temp->_value)) {
			if (!prev) {
				this->removeFront();
				temp = _head;
			}
			else {
				prev->_next = temp->_next;
				delete temp;
				temp = temp->_next;
			}
		}
		else {
			prev = temp;
			temp = temp->_next;
		}
	}
}
