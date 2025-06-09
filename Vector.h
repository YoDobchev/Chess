#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>

template <typename T> class Vector {
  private:
	void resize(size_t new_capacity);

	T* data_;
	size_t size_;
	size_t capacity_;

  public:
	Vector();
	~Vector();

	void push_back(const T& value);
	void pop_back();

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	size_t size() const;
	size_t capacity() const;
	bool empty() const;

	void clear();
};

#include "Vector.cpp"

#endif
