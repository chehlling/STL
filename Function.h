#pragma once

//以下均在STL中的<stl_function.h>中定义
template <class Arg, class Result>
struct UnaryFunction //一元函数
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