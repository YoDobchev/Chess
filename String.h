#ifndef STRING_H
#define STRING_H

#include <iostream>

class Utility {
  public:
	static size_t strlen(const char* src);

	static char* strcpy(char* dest, const char* src);

	static int strcmp(const char* s1, const char* s2);
};

class String {
  private:
	void resize(size_t new_capacity);

	char* data_;
	size_t size_;
	size_t capacity_;

  public:
	String();
	String(const char* s);
	String(const String& other);
    String& operator=(const String& other);
	~String();

	char& operator[](size_t index);
	const char& operator[](size_t index) const;
	bool operator==(const String& other) const;
	friend std::ostream& operator<<(std::ostream& os, const String str);
	friend std::istream& getline(std::istream& is, String& str);
	const char* c_str() const;

	size_t size() const;
	size_t capacity() const;
	bool empty() const;
	char* data() const;

	void push_back(char c);
	void pop_back();
	void clear();
};

#include "String.cpp"
#endif
