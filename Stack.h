#pragma once

using ValueType = double;

enum class StackContainer {
	Vector = 0,
	List,
};

template<typename T>
class StackImplementation;

template<typename T>
class Stack
{
public:
	Stack(StackContainer container = StackContainer::Vector);
	Stack(const ValueType* valueArray, const size_t arraySize,
		  StackContainer container = StackContainer::Vector);

	explicit Stack(const Stack& copyStack);
	Stack& operator=(const Stack& copyStack);

	~Stack();

	void push(const ValueType& value);
	void pop();
	ValueType& top();
	const ValueType& top() const;
	bool isEmpty() const;
	size_t size() const;
private:
	StackImplementation* _pimpl = nullptr;
	StackContainer _containerType;
};
