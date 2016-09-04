#pragma once

template<class T>
struct Hash
{};

size_t HashString(const char* s)
{
	unsigned long h= 0;
	while (*s){
		h = h * 5 + *s++;
	}
	return h;
}

template<>
struct Hash<char*>
{
	size_t operator()(const char* s)
	{
		return HashString(s);
	}
};
template<>
struct Hash<const char*>
{
	size_t operator()(const char* s)
	{
		return HashString(s);
	}
};
template<>
struct Hash<char>
{
	size_t operator()(const char x)
	{
		return x;
	}
};
template<>
struct Hash<unsigned char>
{
	size_t operator()(unsigned char x)
	{
		return x;
	}
};
template<>
struct Hash<int>
{
	size_t operator()(int x)
	{
		return (size_t)x;
	}
};
template<>
struct Hash<unsigned int>
{
	size_t operator()(unsigned int x)
	{
		return x;
	}
};

