#ifndef DYNAMYC_ARRAY_HEADER_INCLUDED
#define DYNAMYC_ARRAY_HEADER_INCLUDED

#include <iostream>
#include <cassert>

template <typename T>
class DynamicArray
{
public:
	DynamicArray();
	DynamicArray(int newSize);
	DynamicArray(const DynamicArray& other);
	DynamicArray& operator=(const DynamicArray& rhs);
	~DynamicArray();

	int GetSize() const;
	int GetCapacity() const;
	void PushBack(const T& newData);
	void RemoveAt(int pos); // remove from pos
	T& operator[](int pos);
	const T& operator[](int pos) const;
	void PrintDynamicArray() const;
	
	void CopyDynamicArray(const DynamicArray& other);
	void FreeDynamicArray();
	void ResizeDynamicArray(int newCap);

private:
	T* arrData;
	int size;
	int capacity;
};


template <typename T>
DynamicArray<T>::DynamicArray() : capacity(5), size(0), arrData(nullptr)
{
	arrData = new(std::nothrow) T[capacity];
	if (!arrData)
	{
		std::cerr << "Not enoug memory!\n";
		exit(1);
	}
}

template <typename T>
DynamicArray<T>::DynamicArray(int newSize): arrData(nullptr), size(0), capacity(newSize)
{
	arrData = new(std::nothrow) T[capacity];
	if (!arrData)
	{
		std::cerr << "Could not allocate memory!\n";
		exit(1);
	}
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other)
{
	CopyDynamicArray(other);
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& rhs)
{
	if (this != &rhs)
	{
		FreeDynamicArray();
		CopyDynamicArray(rhs);
	}
	return *this;
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
	FreeDynamicArray();
}

template <typename T>
int DynamicArray<T>::GetSize() const
{
	return size;
}

template <typename T>
int DynamicArray<T>::GetCapacity() const
{
	return capacity;
}

template<typename T>
void DynamicArray<T>::PushBack(const T& newData)
{
	if (size == capacity)
		ResizeDynamicArray(capacity * 2);

	arrData[size++] = newData;
}

template <typename T>
void DynamicArray<T>::RemoveAt(int pos)
{
	if (pos < 0 || pos > size)
	{
		std::cerr << "Index must be between " << 0 << " and " << size - 1 << "!\n";
		return;
	}

	for (int i = pos; i < size - 1; ++i)
	{
		arrData[i] = arrData[i + 1];
	}
	size--;
}

template <typename T>
T& DynamicArray<T>::operator[](int pos)
{
	return arrData[pos];
}

template <typename T>
const T& DynamicArray<T>::operator[](int pos) const
{
	return arrData[pos];
}

template <typename T>
void DynamicArray<T>::PrintDynamicArray() const
{
	for (int i = 0; i < size; ++i)
	{
		std::cout << arrData[i] << std::endl;
	}
}

template <typename T>
void DynamicArray<T>::CopyDynamicArray(const DynamicArray& other)
{
	arrData = new(std::nothrow) T[other.capacity]; // allocate
	if (!arrData)
	{
		std::cerr << "Not enough memory!\n";
		exit(1);
	}

	for (int i = 0; i < other.size; ++i)
	{
		arrData[i] = other.arrData[i]; // copy elements
	}

	size = other.size;
	capacity = other.capacity;
}

template <typename T>
void DynamicArray<T>::FreeDynamicArray()
{
	delete[] arrData;
	arrData = nullptr;
	size = 0;
	capacity = 0;
}

template <typename T>
void DynamicArray<T>::ResizeDynamicArray(int newCap)
{
	if (newCap < capacity)
	{
		int tmp = 0;
		do
		{
			std::cout << "The new capacity is less than the old. Please enter new value: ";
			std::cin >> tmp;
		} while (tmp > capacity);
		
		capacity = tmp;
	}

	capacity = newCap;

	T* newData = new(std::nothrow) T[capacity]; // allocate
	if (!newData)
	{
		std::cerr << "Not enough memory!\n";
		exit(1);
	}

	for (int i = 0; i < size; ++i)
	{
		newData[i] = arrData[i]; // copy the old elemenets
	}
	delete[] arrData; // delete old elements
	arrData = newData; // assign pointer
}

#endif
