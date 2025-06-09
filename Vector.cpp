#ifndef VECTOR_CPP
#define VECTOR_CPP

#include "Vector.h"

template <typename T> Vector<T>::Vector() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T> Vector<T>::~Vector() { delete[] data_; }

template <typename T> void Vector<T>::push_back(const T& value) {
	if (size_ == capacity_) {
		resize(capacity_ == 0 ? 1 : capacity_ * 2);
	}
	data_[size_++] = value;
}

template <typename T> void Vector<T>::pop_back() {
	if (size_ > 0) {
		--size_;
	}
}

template <typename T> T& Vector<T>::operator[](size_t index) { return data_[index]; }

template <typename T> const T& Vector<T>::operator[](size_t index) const { return data_[index]; }

template <typename T> size_t Vector<T>::size() const { return size_; }

template <typename T> size_t Vector<T>::capacity() const { return capacity_; }

template <typename T> bool Vector<T>::empty() const { return size_ == 0; }

template <typename T> void Vector<T>::clear() { size_ = 0; }

template <typename T> void Vector<T>::resize(size_t new_capacity) {
	T* new_data = new T[new_capacity];
	for (size_t i = 0; i < size_; ++i) {
		new_data[i] = data_[i];
	}
	delete[] data_;
	data_ = new_data;
	capacity_ = new_capacity;
}

#endif
