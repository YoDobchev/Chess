#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>

template <typename T> class Vector {
  private:
	void resize(size_t new_capacity) {
		T* new_data = new T[new_capacity];
		for (size_t i = 0; i < size_; ++i) {
			new_data[i] = data_[i];
		}
		delete[] data_;
		data_ = new_data;
		capacity_ = new_capacity;
	}

	T* data_;
	size_t size_;
	size_t capacity_;

  public:
	Vector() : data_(nullptr), size_(0), capacity_(0) {}

	Vector(const Vector& other) : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {
		if (capacity_) {
			data_ = new T[capacity_];
			for (size_t i = 0; i < size_; ++i)
				data_[i] = other.data_[i];
		}
	}

	Vector& operator=(const Vector& other) {
		if (this != &other) {
			T* newData = nullptr;
			if (other.capacity_) {
				newData = new T[other.capacity_];
				for (size_t i = 0; i < other.size_; ++i)
					newData[i] = other.data_[i];
			}
			delete[] data_;
			data_ = newData;
			size_ = other.size_;
			capacity_ = other.capacity_;
		}
		return *this;
	}

	Vector(Vector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
		other.data_ = nullptr;
		other.size_ = other.capacity_ = 0;
	}

	Vector& operator=(Vector&& other) noexcept {
		if (this != &other) {
			delete[] data_;
			data_ = other.data_;
			size_ = other.size_;
			capacity_ = other.capacity_;
			other.data_ = nullptr;
			other.size_ = other.capacity_ = 0;
		}
		return *this;
	}

	~Vector() { delete[] data_; }

	void push_back(const T& value) {
		if (size_ == capacity_) {
			resize(capacity_ == 0 ? 1 : capacity_ * 2);
		}
		data_[size_++] = value;
	}

	void pop_back() {
		if (size_ > 0) {
			--size_;
		}
	}

	T& operator[](size_t index) { return data_[index]; }

	const T& operator[](size_t index) const { return data_[index]; }

	size_t size() const { return size_; }

	size_t capacity() const { return capacity_; }

	bool empty() const { return size_ == 0; }

	void clear() { size_ = 0; }
};

#endif
