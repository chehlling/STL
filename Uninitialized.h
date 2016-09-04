#pragma once

//STL���������ȫ�ֺ�����������δ��ʼ���ռ���
#include <iostream>
#include <string.h>
//#include <algorithm>
#include "Construct.h"

template<class InputIterator, class ForwardIterator>
ForwardIterator __UninitializedCopyAux(InputIterator first, InputIterator last, \
	ForwardIterator result, __TrueType)
{
	return (ForwardIterator)memcpy(result,first,((int)last-(int)first));
	//return copy(first, last, result);	
}
// ����ʵ���ǵ��õ�C++�����copy
// �ڲ�Ҫͨ��ģ�����ݳ����ݵ����ͣ��ٵ���memcpy

template<class InputIterator, class ForwardIterator>
ForwardIterator __UninitializedCopyAux(InputIterator first, InputIterator last, \
	ForwardIterator result, __FalseType)
{
	ForwardIterator cur = result;
	for (; first != last; ++first, ++cur)
		Construct(&*cur, *first);
	/*try{
		for (; first != last; ++first, ++cur)
			Construct(&*cur, *first);
	}
	catch (...){
		Destroy(result, cur);
		throw;
	}*/
	
	return cur;
}

template<class InputIterator, class ForwardIterator,class T>
ForwardIterator __UninitializedCopy(InputIterator first, InputIterator last, ForwardIterator result, T*)
{
	typedef __TypeTraits<T>::IsPODType IsPODType;
	return __UninitializedCopyAux(first, last, result, IsPODType());
}

template<class InputIterator, class ForwardIterator>
ForwardIterator UninitializedCopy(InputIterator first, InputIterator last, ForwardIterator result)
{
	return __UninitializedCopy(first,last,result,ValueType(result));
}

// �ػ�
char* UninitializedCopy(const char* first, const char* last, char* result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}

//template<class InputIterator,class T>
//void UninitializedFill(InputIterator ,const T value)
//{
//
//}
//template<class InputIterator,class T>
//void UninitializedFillN(InputIterator ,size_t size, const T value)
//{}