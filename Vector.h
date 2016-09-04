#pragma once

#include "Alloc.h"
#include "Uninitialized.h"
#include <assert.h>

//vector���õ����ݽṹ�������������ռ�
template<class T,class Alloc = alloc>
class Vector
{
public:
	//Ƕ���ͱ�
	typedef T* Iterator;
	typedef T ValueType;
	typedef T* Pointer;
	typedef T& Reference;
	typedef ptrdiff_t DifferenceType;

	typedef const T* ConstPointer;
	typedef const T* ConstIterator;
	typedef const T& ConstReference;

	typedef SimpleAlloc<ValueType, Alloc> DataAllocator;
	typedef Vector<T> Self;
public:
	Vector()
		:_start(NULL)
		, _end(NULL)
		, _endOfStorage(NULL)
	{}
	Vector(size_t n, const T& value)//���캯����ָ��Vector��С������ֵ
	{
		FillInitializen(n, value);
	}
	void FillInitializen(size_t n,const T& value)
	{
		_start = AllocateAndFill(n,value);
		_end = _start + n;
		_endOfStorage = _end;
	}
	Iterator AllocateAndFill(size_t n,const T& value)
	{
		Iterator result = DataAllocator::Allocate(n);//����ռ�
		//UninitializedFill(result,n,value);
		return result;
	}
	~Vector()
	{}
	Iterator Begin()
	{
		return _start;
	}
	ConstIterator Begin()const
	{
		return _start;
	}
	Iterator End()
	{
		return _end;
	}
	ConstIterator End()const
	{
		return _end;
	}
	Reference Front()
	{
		return *Begin();
	}
	ConstReference Front()const
	{
		return *Begin();
	}
	Reference Back()
	{
		return *(End() - 1);
	}
	ConstReference Back()const
	{
		return *(End() - 1);
	}
	void PushBack(const T& x)
	{
		Insert(End(),x);
	}
	void PopBack()
	{
		if (_start != _end){
			--_end;//��ʾ����β��Ԫ��
			Destroy(_end);
		}
	}
	//iterator insert(iterator position);
	//iterator insert(iterator position, const T& x);
	//void insert(iterator position, InputIterator first, InputIterator last);
	//void insert(iterator position,const_iterator first, const_iterator last);
	//void insert(iterator pos, size_type n, const T& x);
	//void insert(iterator pos, int n, const T& x);
	//void insert(iterator pos, long n, const T& x);
	Iterator Insert(Iterator pos,const T& x)
	{
		//��������ԭ�����ԭ��СΪ0��������1��Ԫ�صĴ�С
		//             ��������ԭ��С������
		size_t n = pos - _start;
		size_t oldsize = Size();
		if (_start == _endOfStorage){//1����ʱû��һ�����ݣ����ÿռ�Ϊ0����Ҫ����
			//_start = new T(x);
			_start = DataAllocator::Allocate();//
			Construct(_start,x);
			//new(_start)ValueType(x);
			_end = _start + 1;
			_endOfStorage = _end;
		}
		else if (pos == End() && _end < _endOfStorage){//2��β���Ҳ���Ҫ����
			_start[oldsize] = x;
			++_end;
		}
		else{
			//3����β��
			_CheckEndOfStorage();
			Iterator tmp = _end;
			pos = _start + n;//**************** very importent  �п������ݣ�pos�Ͳ���Ҫ������Ǹ�λ����
			while (pos <= tmp){
				*(tmp + 1) = *tmp;
				--tmp;
			}
			*pos = x;
			++_end;
		}
		return _start+n;
	}
	//Iterator Erase(Iterator first,Iterator last);
	Iterator Erase(Iterator pos)
	{
		assert(pos != End());

		size_t size = Size();
		if (size == 0){
			return pos;
		}

		size_t n = pos - _start;
		Iterator tmp = pos;
		while (tmp < End()-1){
			*tmp = *(tmp + 1);
			tmp++;
		}
		_end--;
		*_end = ValueType();
		return _start + n;
	}
	bool Empty()
	{
		return Begin()==End();
	}
	size_t Size()
	{
		return _end - _start;
	}
	Reference operator[](size_t index)
	{
		assert(index >= 0 && index < Size());
		return *(Begin() + index);
	}
	ConstReference operator[](size_t index)const
	{
		assert(index >= 0 && index < Size());
		return *(Begin() + index);
	}
	//void resize(size_type sz, T c = T());//����������ͬʱ�ı��С
	void Reserve(size_t n)//Ԥ���ռ䣬ֻ�����������ı��С
	{
		if (n <= (_endOfStorage - _start))
			return;
		size_t oldsize = _end - _start;
		Iterator tmp;
		tmp = DataAllocator::Allocate(n);
		UninitializedCopy(_start, _end, tmp);
		Destroy(_start, _end);
		if (_start)
			DataAllocator::Deallocate(_start, _endOfStorage - _start);
		_start = tmp;
		_end = tmp + oldsize;
		_endOfStorage = _start + n;
	}
	//Reference At(const size_t n);
	//void Swap(Self& x);
	//void Clear();

private:
	void _CheckEndOfStorage()
	{
		int oldcapacity = Size();
		if (_start != NULL && _end < _endOfStorage){
			return;
		}
		int newcapacity = oldcapacity * 2 ;
		
		/*//1.
		T* tmp = new T[newcapacity];
		int index = 0;
		while (index < oldcapacity){
			tmp[index] = _start[index];
			index++;
		}
		if (oldcapacity == 1)
			delete _start;
		else
			delete[] _start;//���ֻ��һ���ռ���delete _start;
		_start = tmp;
		_end = _start + oldcapacity;
		_endOfStorage = _start + newcapacity;*/
	    
		/*//2.
		T* tmp = DataAllocator::Allocate(newcapacity);

		int index = 0;
		while (index < oldcapacity){
			tmp[index] = _start[index];
			index++;
		}
		index = 0;
		while (index < oldcapacity){
			(&_start[index])->~T();
			index++;
		}
		DataAllocator::Deallocate(&_start[index], oldcapacity);
		_start = tmp;
		_end = _start + oldcapacity;
		_endOfStorage = _start + newcapacity;*/

		//3.
		T* tmp = DataAllocator::Allocate(newcapacity);
		UninitializedCopy(_start, _end, tmp);

		Destroy(_start, _end);
		DataAllocator::Deallocate(_start, oldcapacity);

		_start = tmp;
		_end = _start + oldcapacity;
		_endOfStorage = _start + newcapacity; 
	}
protected:
	T* _start;       //��ʾĿǰʹ�ÿռ��ͷ
	T* _end;         //��ʾĿǰʹ�ÿռ��β
	T* _endOfStorage;//��ʾĿǰ���ÿռ��β
};