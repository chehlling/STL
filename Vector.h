#pragma once

#include "Alloc.h"
#include "Uninitialized.h"
#include <assert.h>

//vector采用的数据结构就是线性连续空间
template<class T,class Alloc = alloc>
class Vector
{
public:
	//嵌套型别
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
	Vector(size_t n, const T& value)//构造函数，指定Vector大小并填充初值
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
		Iterator result = DataAllocator::Allocate(n);//分配空间
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
			--_end;//表示放弃尾部元素
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
		//以下配置原则：如果原大小为0，则配置1个元素的大小
		//             否则，配置原大小的两倍
		size_t n = pos - _start;
		size_t oldsize = Size();
		if (_start == _endOfStorage){//1、此时没有一个数据，可用空间为0，需要扩容
			//_start = new T(x);
			_start = DataAllocator::Allocate();//
			Construct(_start,x);
			//new(_start)ValueType(x);
			_end = _start + 1;
			_endOfStorage = _end;
		}
		else if (pos == End() && _end < _endOfStorage){//2、尾插且不需要扩容
			_start[oldsize] = x;
			++_end;
		}
		else{
			//3、非尾插
			_CheckEndOfStorage();
			Iterator tmp = _end;
			pos = _start + n;//**************** very importent  有可能扩容，pos就不是要插入的那个位置了
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
	//void resize(size_type sz, T c = T());//增加容量的同时改变大小
	void Reserve(size_t n)//预留空间，只增加容量不改变大小
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
			delete[] _start;//如果只有一个空间用delete _start;
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
	T* _start;       //表示目前使用空间的头
	T* _end;         //表示目前使用空间的尾
	T* _endOfStorage;//表示目前可用空间的尾
};