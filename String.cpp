#ifndef STRING_CPP
#define STRING_CPP

#include "String.h"

size_t Utility::strlen(const char* src) {
	size_t len = 0;
	while (*src++)
		len++;
	return len;
}

char* Utility::strcpy(char* dest, const char* src) {
	char* original = dest;
	while ((*dest++ = *src++) != '\0') {
	}
	return original;
}

int Utility::strcmp(const char* s1, const char* s2) {
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return (unsigned char)*s1 - (unsigned char)*s2;
}

String::String() : data_(new char[1]), size_(0), capacity_(0) { data_[0] = '\0'; }

String::String(const String& other)
    : data_(new char[other.capacity_ + 1]), size_(other.size_), capacity_(other.capacity_) {
	Utility::strcpy(data_, other.data_);
}

String& String::operator=(const String& other) {
	if (this != &other) {
		delete[] data_;
		size_ = other.size_;
		capacity_ = other.capacity_;
		data_ = new char[capacity_ + 1];
		Utility::strcpy(data_, other.data_);
	}
	return *this;
}

String::String(const char* s) {
	if (s) {
		size_ = Utility::strlen(s);
		capacity_ = size_;
		data_ = new char[capacity_ + 1];
		Utility::strcpy(data_, s);
	} else {
		data_ = nullptr;
		size_ = 0;
		capacity_ = 0;
	}
}

String::~String() { delete[] data_; }

char& String::operator[](size_t index) { return data_[index]; }

const char& String::operator[](size_t index) const { return data_[index]; }

bool String::operator==(const String& other) const { return Utility::strcmp(data_, other.data_) == 0; }

std::ostream& operator<<(std::ostream& os, const String str) {
	os << str.c_str();
	return os;
}

std::istream& getline(std::istream& is, String& str) {
	str.clear();
	char c;
	while (is.get(c)) {
		if (c == '\n') break;
		str.push_back(c);
	}
	return is;
}

const char* String::c_str() const { return data_ ? data_ : ""; }

size_t String::size() const { return size_; }

size_t String::capacity() const { return capacity_; }

bool String::empty() const { return size_ == 0; }

char* String::data() const { return data_; }

void String::push_back(char c) {
	if (size_ == capacity_) {
		resize(capacity_ == 0 ? 1 : capacity_ * 2);
	}
	data_[size_++] = c;
	data_[size_] = '\0';
}

void String::pop_back() {
	if (size_ > 0) {
		--size_;
		data_[size_] = '\0';
	}
}

void String::clear() {
	size_ = 0;
	data_[0] = '\0';
}

void String::resize(size_t new_capacity) {
	if (new_capacity < size_) {
		return;
	}
	char* new_data = new char[new_capacity + 1];
	if (data_) {
		Utility::strcpy(new_data, data_);
	}
	delete[] data_;
	data_ = new_data;
	capacity_ = new_capacity;
}

#endif
