#pragma once

//���¾���STL�е�<stl_function.h>�ж���
template <class Arg, class Result>
struct UnaryFunction //һԪ����
{
	typedef Arg ArgumentType;
	typedef Result ResultType;
};
template<class T>
struct Identity :public UnaryFunction<T, T>
{
	const T& operator()(const T& x) const
	{
		return x;
	}
};

template<class Arg1, class Arg2, class Result>
struct BinaryFunction
{
	typedef Arg1 FirstArgumentType;
	typedef Arg2 SecondArgumentType;
	typedef Result ResulType;
};
template<class T>
struct EqualTo :public BinaryFunction<T, T, bool>
{
	bool operator()(const T& x, const T& y) const
	{
		return x == y;
	}
};