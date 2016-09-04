#pragma once

//双底线前缀的意思是SGI内部所用的东西，不在STL标准之内
//IteratorTraits负责萃取迭代器的特性
//__TypeTraits负责萃取型别的特性
struct __FalseType
{};

struct __TrueType
{};
//一个类究竟什么时候该有自己的平凡类型构造
template<class T>
struct __TypeTraits
{
	typedef __FalseType HasTrivialDefaultConstructor;
	typedef __FalseType HasTrivialCopyConstructor;
	typedef __FalseType HasTrivialAssignmentOperator;
	typedef __FalseType HasTrivialDestructor;//无平凡类型的析构
	typedef __FalseType IsPODType;
};
//针对原生指针设计的偏特化版本
template<class T>
struct __TypeTraits<T*>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;//有平凡类型的析构
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<char>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;//有平凡类型的析构
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<unsigned char>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<short>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<unsigned short>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<int>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
}; 

template<>
struct __TypeTraits<unsigned int>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<long>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<unsigned long>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<float>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<double>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};


template<>
struct __TypeTraits<long long>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<bool>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

//我们可以针对自定义类，自行设计一个特化版本，明确告诉编译器以下事例
//template<>
//struct __TypeTraits<Date>
//{
//	typedef __TrueType HasTrivialDefaultConstructor;
//	typedef __TrueType HasTrivialCopyConstructor;
//	typedef __TrueType HasTrivialAssignmentOperator;
//	typedef __TrueType HasTrivialDestructor;
//	typedef __TrueType IsPODType;
//};