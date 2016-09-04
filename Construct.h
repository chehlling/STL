//Alloc.h负责内存空间的配置与释放
//Construct.h负责对象内容的构造与析构
//这两个头文件在memory文件中包含

//STL定义有五个全局函数，作用于未初始化空间上
#pragma once

#include <new.h>//使用了new的定位表达式
#include "TypeTraits.h"
#include "Iterator.h"


template<class T1,class T2>
void Construct(T1* ptr, const T2& x)
{
	new(ptr)T1(x);
}

template<class T>
void Destroy(T* ptr)
{
	ptr->~T();
}

//template<class InputIterator>
template<class ForwardIterator>
void __DestroyAux(ForwardIterator first, ForwardIterator last, __FalseType)
{
	for (; first != last;++first){
		Destroy(&*first);
	}
}

template<class ForwardIterator>
void __DestroyAux(ForwardIterator first, ForwardIterator last, __TrueType)
{}

template<class ForwardIterator,class T>
void __Destroy(ForwardIterator first, ForwardIterator last,T*)
{
	typedef __TypeTraits<T>::HasTrivialDestructor HasTrivialDestructor;
	__DestroyAux(first, last, HasTrivialDestructor());
}

template<class ForwardIterator>
void Destroy(ForwardIterator first,ForwardIterator last)
{
	__Destroy( first, last, ValueType(first));
}

void Destroy(char*,char*)
{}