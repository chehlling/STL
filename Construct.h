//Alloc.h�����ڴ�ռ���������ͷ�
//Construct.h����������ݵĹ���������
//������ͷ�ļ���memory�ļ��а���

//STL���������ȫ�ֺ�����������δ��ʼ���ռ���
#pragma once

#include <new.h>//ʹ����new�Ķ�λ���ʽ
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