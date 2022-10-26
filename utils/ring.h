#pragma once
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
	int size(void){return _size};
	int current(void){return _current};
};

