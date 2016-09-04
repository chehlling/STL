#pragma once

//����ʹ�����ֵ������������Լ��������ģ���Ϊֻ�����������֪���Լ�Ӧ��Ҫ������

// ���ڱ�ǵ��������� 
struct InputIteratorTag{};
struct ForwardIteratorTag:public InputIteratorTag{};
struct BidirectionalIteratorTag:public ForwardIteratorTag{};
struct RandomAccessIteratorTag :public BidirectionalIteratorTag{};

//Iterator�ı�֤
//�κε���������Ҫ�ṩ��Ƕ��Ӧ�ͱ𣬷����������ȡ
//���п����ĵ�������ü̳����������������������Ӧ�ͱ�ļ̳�
//�µĵ�����ֻ�谴���·�ʽд��������
//template<class T>
//struct ListIter :public Iterator<BidirectionalIteratorTag,T>
//{};
template<class Category, class T, class Distance = ptrdiff_t,
		class Pointer = T*, class Reference = T&>
struct Iterator
{
	typedef Category IteratorCategory;
	typedef T ValueType;
	typedef Distance DifferenceType;//ptrdiff_t��C/C++��׼���ж����һ���������ص���������,ͨ��������������ָ����������Ľ����
	typedef Pointer Pointer;
	typedef Reference Reference;
};

template<class Iterator>
struct IteratorTraits 
{
	typedef typename Iterator::IteratorCategory IteratorCategory;
	typedef typename Iterator::ValueType ValueType;
	typedef typename Iterator::DifferenceType DifferenceType;//������ʾ����������֮��ľ���
	typedef typename Iterator::Pointer Pointer;
	typedef typename Iterator::Reference Reference;
};

//���ԭ��ָ�����Ƶ�ƫ�ػ��汾
template<class T>
struct IteratorTraits<T*>
{
	typedef RandomAccessIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef ptrdiff_t DifferenceType;
	typedef T* PointerType;
	typedef T& ReferenceType;
};
//���point-to-const����Ƶ�ƫ�ػ��汾
template<class T>
struct IteratorTraits<const T*>
{
	typedef const RandomAccessIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef ptrdiff_t DifferenceType;
	typedef const T* PointerType;
	typedef const T& ReferenceType;
};

// �κε�������Ӧ�ṩ5����Ƕ����Ӧ�ͱ�
// ȷ�����ܹ�������ĸ�STL�ںϡ�
// �ҷ���Iterator Traits��������ȡ
template <class T, class Distance>
struct InputIterator {//ֻ������������ָ��������ı�
	typedef InputIteratorTag   IteratorCategory;
	typedef T                  ValueType;
	typedef Distance           DifferenceType;
	typedef T*                 Pointer;
	typedef T&                 Reference;
};

template <class T, class Distance>
struct ForwardIterator {//��д
	typedef ForwardIteratorTag   IteratorCategory;
	typedef T                    ValueType;
	typedef Distance             DifferenceType;
	typedef T*                   Pointer;
	typedef T&                   Reference;
};

template <class T, class Distance>
struct BidirectionalIterator {//˫���ƶ�
	typedef BidirectionalIteratorTag   IteratorCategory;
	typedef T                          ValueType;
	typedef Distance                   DifferenceType;
	typedef T*                         Pointer;
	typedef T&                         Reference;
};

template <class T, class Distance>
struct RandomAccessIterator {//���[]
	typedef RandomAccessIteratorTag    IteratorCategory;
	typedef T                          ValueType;
	typedef Distance                   DifferenceType;
	typedef T*                         Pointer;
	typedef T&                         Reference;
};

//����������������
template<class Iterator>
typename IteratorTraits<Iterator>::IteratorCategory IteratorCategory(const Iterator&)
{
	return IteratorTraits<Iterator>::IteratorCategory();//������Ӧ���͵������Ķ���
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
//������������ValueType
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

//Advance����////////////////////////////////////////////////////////////////
template<class InputIterator, class Distance>
void __Advance(InputIterator& it, Distance n, InputIteratorTag)
{
	while (n--)
		++it;
}

template<class BidirectionalIterator, class Distance>
void __Advance(BidirectionalIterator& it, Distance n, BidirectionalIteratorTag)
{
	//˫����һǰ��
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
	//˫����Ծǰ��
	it += n;
}

//STL�㷨�������������㷨���ܽ���֮��ͽ׵��������ͣ���Ϊ��������ͱ��������
template<class InputIterator, class Distance>
void Advance(InputIterator& it, Distance n)
{
	__Advance(it, n, IteratorTraits<InputIterator>::IteratorCategory());
}

//Distance����////////////////////////////////////////////////////////////////
//__Distance()����������Ϊ������󣬿��Խ���������Ķ���
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
//���������
//template <class Iterator>
//class reverse_iterator
//{
//protected:
//	Iterator current;
//}