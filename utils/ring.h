#pragma once

#define NULL 0

template <typename T>
class ring
{
private:
	T* array;
	int _size;
	int _current;

public:
	ring(void);
	ring(int arraySize);
	ring(T* arr, int current = -1);
	~ring(void);

	void add(T val);
	void setIndex(int i);

	T relative(int i);
	T peek(void);
	T item(int i);
	T* items(void);
	int size(void) { return _size; };
	int current(void) { return _current; };
};

template <typename T>
ring<T>::ring(void)
{
	_size = 0;
	_current = -1;
}
template <typename T>
ring<T>::ring(int arraySize)
{
	array = new T[arraySize];
	_size = arraySize;
	_current = -1;
}
template <typename T>
ring<T>::ring(T* arr, int current)
{
	array = arr;
	_size = (int)(sizeof(arr) / sizeof(arr[0]));
	if (current >= _size || current < -1)
	{
		_current = -1;
	}
	else
	{
		_current = current;
	}
}
template <typename T>
ring<T>::~ring(void)
{
	delete array;
}
template <typename T>
void ring<T>::add(T val)
{
	++_current;
	if (_current >= _size)
	{
		_current = 0;
	}
	array[_current] = val;
}
template <typename T>
void ring<T>::setIndex(int i)
{
	_current = i - 1;

}

template <typename T>
T ring<T>::relative(int i)
{
	int idx = _current + i;
	if (idx >= _size)
	{
		idx -= _size;
	}
	else if (idx < 0)
	{
		idx += _size;
	}
	if (idx >= _size || idx < 0)
	{
		return NULL;
	}
	else
	{
		return array[idx];
	}
}
template <typename T>
T ring<T>::peek(void)
{
	return array[_current];
}
template <typename T>
T ring<T>::item(int i)
{
	if (i >= _size || i < 0)
	{
		return NULL;
	}
	else
	{
		return array[i];
	}
}
template <typename T>
T* ring<T>::items(void)
{
	return array;
}