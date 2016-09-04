#pragma once

template<class T1,class T2>
struct Pair
{
	T1 first;
	T2 second;
	Pair()
		:first(T1())
		, second(T2())
	{}
	Pair(const T1& a, const T2& b)
		: first(a)
		, second(b)
	{}
};