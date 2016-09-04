#pragma once

//具体使用哪种迭代器是容器自己所决定的，因为只有容器本身材知道自己应该要用哪种

// 用于标记迭代器类型 
struct InputIteratorTag{};
struct ForwardIteratorTag:public InputIteratorTag{};
struct BidirectionalIteratorTag:public ForwardIteratorTag{};
struct RandomAccessIteratorTag :public BidirectionalIteratorTag{};

//Iterator的保证
//任何迭代器都需要提供内嵌相应型别，方便迭代器萃取
//自行开发的迭代器最好继承下面这个迭代器，五种相应型别的继承
//新的迭代器只需按以下方式写（举例）
//template<class T>
//struct ListIter :public Iterator<BidirectionalIteratorTag,T>
//{};
template<class Category, class T, class Distance = ptrdiff_t,
		class Pointer = T*, class Reference = T&>
struct Iterator
{
	typedef Category IteratorCategory;
	typedef T ValueType;
	typedef Distance DifferenceType;//ptrdiff_t是C/C++标准库中定义的一个与机器相关的数据类型,通常用来保存两个指针减法操作的结果。
	typedef Pointer Pointer;
	typedef Reference Reference;
};

template<class Iterator>
struct IteratorTraits 
{
	typedef typename Iterator::IteratorCategory IteratorCategory;
	typedef typename Iterator::ValueType ValueType;
	typedef typename Iterator::DifferenceType DifferenceType;//用来表示两个迭代器之间的距离
	typedef typename Iterator::Pointer Pointer;
	typedef typename Iterator::Reference Reference;
};

//针对原生指针而设计的偏特化版本
template<class T>
struct IteratorTraits<T*>
{
	typedef RandomAccessIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef ptrdiff_t DifferenceType;
	typedef T* PointerType;
	typedef T& ReferenceType;
};
//针对point-to-const而设计的偏特化版本
template<class T>
struct IteratorTraits<const T*>
{
	typedef const RandomAccessIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef ptrdiff_t DifferenceType;
	typedef const T* PointerType;
	typedef const T& ReferenceType;
};

// 任何迭代器都应提供5种内嵌的相应型别
// 确保了能够更方便的跟STL融合。
// 且方便Iterator Traits的类型萃取
template <class T, class Distance>
struct InputIterator {//只读，迭代器所指对象不允许改变
	typedef InputIteratorTag   IteratorCategory;
	typedef T                  ValueType;
	typedef Distance           DifferenceType;
	typedef T*                 Pointer;
	typedef T&                 Reference;
};

template <class T, class Distance>
struct ForwardIterator {//读写
	typedef ForwardIteratorTag   IteratorCategory;
	typedef T                    ValueType;
	typedef Distance             DifferenceType;
	typedef T*                   Pointer;
	typedef T&                   Reference;
};

template <class T, class Distance>
struct BidirectionalIterator {//双向移动
	typedef BidirectionalIteratorTag   IteratorCategory;
	typedef T                          ValueType;
	typedef Distance                   DifferenceType;
	typedef T*                         Pointer;
	typedef T&                         Reference;
};

template <class T, class Distance>
struct RandomAccessIterator {//随机[]
	typedef RandomAccessIteratorTag    IteratorCategory;
	typedef T                          ValueType;
	typedef Distance                   DifferenceType;
	typedef T*                         Pointer;
	typedef T&                         Reference;
};

//决定迭代器的类型
template<class Iterator>
typename IteratorTraits<Iterator>::IteratorCategory IteratorCategory(const Iterator&)
{
	return IteratorTraits<Iterator>::IteratorCategory();//返回相应类型迭代器的对象
}
//template <class T, class Distance>
//inline input_iterator_tag
//iterator_category(const input_iterator<T, Distance>&) {
//	return input_iterator_tag();
//}
//
//inline output_iterator_tag iterator_category(const output_iterator&) {
//	return output_iterator_tag();
//}
//
//template <class T, class Distance>
//inline forward_iterator_tag
//iterator_category(const forward_iterator<T, Distance>&) {
//	return forward_iterator_tag();
//}
//
//template <class T, class Distance>
//inline bidirectional_iterator_tag
//iterator_category(const bidirectional_iterator<T, Distance>&) {
//	return bidirectional_iterator_tag();
//}
//
//template <class T, class Distance>
//inline random_access_iterator_tag
//iterator_category(const random_access_iterator<T, Distance>&) {
//	return random_access_iterator_tag();
//}
//
//template <class T>
//inline random_access_iterator_tag iterator_category(const T*) {
//	return random_access_iterator_tag();
//}
//决定迭代器的ValueType
template<class Iterator>
typename IteratorTraits<Iterator>::ValueType* ValueType(const Iterator&)
{
	return static_cast<typename IteratorTraits<Iterator>::ValueType*>(0);
}
template<class T, class Distance>
T* ValueType(const InputIterator<T, Distance>&)
{
	return (T*)(0);
}

template<class T, class Distance>
T* ValueType(const ForwardIterator<T, Distance>&)
{
	return (T*)(0);
}

template<class T, class Distance>
T* ValueType(const BidirectionalIterator<T, Distance>&)
{
	return (T*)(0);
}

template<class T, class Distance>
T* ValueType(const RandomAccessIterator<T, Distance>&)
{
	return (T*)(0);
}

template <class T>
T* ValueType(const T*) 
{ 
	return (T*)(0); 
}

//Advance函数////////////////////////////////////////////////////////////////
template<class InputIterator, class Distance>
void __Advance(InputIterator& it, Distance n, InputIteratorTag)
{
	while (n--)
		++it;
}

template<class BidirectionalIterator, class Distance>
void __Advance(BidirectionalIterator& it, Distance n, BidirectionalIteratorTag)
{
	//双向，逐一前进
	if (n >= 0){
		while (n-->0)
			++it;
	}
	else{
		while (n++ < 0)
			--it;
	}
}

template<class RandomAccessIterator, class Distance>
void __Advance(RandomAccessIterator& it, Distance n, RandomAccessIteratorTag)
{
	//双向，跳跃前进
	it += n;
}

//STL算法的命名规则：以算法所能接受之最低阶迭代器类型，来为其迭代器型别参数命名
template<class InputIterator, class Distance>
void Advance(InputIterator& it, Distance n)
{
	__Advance(it, n, IteratorTraits<InputIterator>::IteratorCategory());
}

//Distance函数////////////////////////////////////////////////////////////////
//__Distance()第三个参数为基类对象，可以接受派生类的对象
template<class InputIterator>
typename InputIterator::DifferenceType _Distance(InputIterator first, InputIterator last, InputIteratorTag)
{
	int len = 0;
	while (first != last){
		++len;
		++first;
	}
	return len;
}

template<class RandomAccessIterator>
typename RandomAccessIterator::DifferenceType _Distance(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIteratorTag)
{
	return last - first;
}
template<class InputIterator>
typename InputIterator::DifferenceType Distance(InputIterator first, InputIterator last)
{
	return __Distance(first, last, IteratorTraits<InputIterator>::IteratorCategory());
}

/////////////////////////////////////////////////////////////////////////////
//反向迭代器
//template <class Iterator>
//class reverse_iterator
//{
//protected:
//	Iterator current;
//}