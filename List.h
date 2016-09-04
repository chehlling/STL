#ifndef __List__
#define __List__

#include "Iterator.h"
#include "Alloc.h"
#include "Construct.h"

//STL对于区间前闭后开

//List的节点
template<class T>
struct ListNode
{
	typedef ListNode<T>* Node_p;
	Node_p _prev;
	Node_p _next;
	T _data;
	ListNode(const T& data=T() )
		:_prev(NULL)
		, _next(NULL)
		, _data(data)
	{}
};

//List中的迭代器(由于list不是线性空间结构，通用迭代器无法正常移动，所以list需要定义专门的迭代器。)
template<class T,class Ref,class Ptr>//???????????????
struct __ListIterator
{
	//List使用的迭代器应该是可双向移动的（五种相应型别必须有，或者继承struct Iterator）
	typedef BidirectionalIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef Ptr Pointer;
	typedef Ref Reference;
	typedef ptrdiff_t DifferenceType;

	typedef __ListIterator<T, Ref, Ptr> Self;

	typedef ListNode<T> Node, *Node_p;
	Node_p _node;

	//__ListIterator(Node_p x) 
	//	: _node(x) 
	//{}
	__ListIterator()
	{}
	__ListIterator(Node_p node)
		:_node(node)
	{}
	//取节点的数值
	Ref operator*()const
	{
		return _node->_data;
	}
	Ptr operator->()const
	{
		return &(operator*());
	}
	bool operator == (const Self& x)
	{
		return (_node==x._node);
	}
	bool operator != (const Self& x)
	{
		return (_node != x._node);
	}
	Self& operator++()
	{
		_node = _node->_next;
		return *this;
	}
	Self operator++(int)
	{
		Self old = *this;
		++(*this);
		return old;
	}
	Self& operator--()
	{
		(_node = _node->_prev);
		return *this;
	}
	Self operator--(int)
	{
		Self old = *this;
		--(*this);
		return old;
	}
};

//template <class T, class Ref, class Ptr>
// BidirectionalIteratorTag IteratorCategory(const __ListIterator<T, Ref, Ptr>&) 
//{
//		return BidirectionalIteratorTag();
//}
//template <class T, class Ref, class Ptr>
//T* ValueType(const __ListIterator<T, Ref, Ptr>&) 
//{
//	return 0;
//}
//template <class T, class Ref, class Ptr>
//ptrdiff_t* DistanceType(const __ListIterator<T, Ref, Ptr>&) 
//{
//	return 0;
//}

//List是一个带头结点的双向循环链表
template<class T, class Alloc = alloc>
class List
{
public:
	typedef SimpleAlloc<ListNode<T>, Alloc> ListNodeAllocator;

	typedef __ListIterator<T, T&, T*> Iterator;//STL强制要求
	typedef __ListIterator<T, const T&, const T*> ConstIterator;

	typedef T ValueType;
	typedef T& Reference;
	typedef const T& ConstReference;
	typedef T* Pointer;
	typedef const T* ConstPointer;
	//typedef List<T, Alloc> Self;
private:
	typedef ListNode<T> Node, *Node_p;

	Node* CreateNode(size_t size,const T& data=T())
	{
		//Node_p node = new Node(data);
		//分配内存
		Node* node = ListNodeAllocator::Allocate(size);
		//配置空间
		Construct(node, data);
		return node;
	}
	void DestroyNode(Node_p node)
	{
		//调用析构
		Destroy(node);
		//释放内存
		ListNodeAllocator::Deallocate(node);
	}
	
public:
	List()
		:_list(CreateNode(sizeof(Node)))
	{
		_list->_next = _list;
		_list->_prev = _list;
	}
	//List(Iterator first,Iterator last);
	//List(const List<T, Alloc>& l);
	//List<T,Alloc>& operator=(const List<T,Alloc>& l);
	~List()
	{
		Clear();
		//DestroyNode(Node_p node)
		//DestoryNode(_list);
		_list = NULL;
	}
	/*~List()
	{
		size_t size = Size();
		Destroy(Begin(),End());
		ListNodeAllocator::Deallocate(_list,size);
	}*/

	Iterator Begin() 
	{
		return _list->_next;
	}
	Iterator End() 
	{
		return _list;
	}
	ConstIterator Begin() const
	{
		return _list->_next;
		//return ConstIterator(_list->_next);
	}
	ConstIterator End() const
	{
		return _list->_next;
		//return ConstIterator(_list);
	}
	void PushBack(const T& data)
	{
		Insert(End(),data);
	}
	void PopBack()
	{
		Iterator tmp = End();
		Erase(--tmp);
	}
	void PushFront(const T& data)
	{
		Insert(Begin(),data);
	}
	void PopFront()
	{
		Erase(Begin());
	}
	//取头结点的内容（元素值）
	Reference Front()
	{
		return *Begin();
	}
	ConstReference Front()const
	{
	}
	Reference Back()
	{
		return *(--End());
	}
	ConstReference Back()const
	{
	}
	//在当前位置的前面插入
	void Insert(Iterator pos, const T& data)
	{
		Node_p cur = pos._node;
		Node_p prev = cur->_prev;
		//Node_p tmp = new Node(data);
		Node_p tmp = CreateNode(sizeof(Node),data);
		tmp->_next = cur;
		cur->_prev = tmp;
		prev->_next = tmp;
		tmp->_prev = prev;
	}
	Iterator Erase(Iterator pos)
	{
		Node_p cur = pos._node;
		Node_p prev = cur->_prev;
		Node_p next = cur->_next;
		prev->_next = next;
		next->_prev = prev;
		delete cur;
		return Iterator(next);
	}
	bool Empty()const
	{
		if (_list->_next == _list){
			return true;
		}
		else{
			return false;
		}
	}
	size_t Size()
	{
		/*size_t size;
		Distance(Begin(),End(),size);
		return size;*/
		int size = 0;
		Node_p start = _list->_next;
		Node_p end = _list;
		while (start != end){
			++size;
			start = start->_next;
		}
		return size;
	}
	void Clear()//All the elements in the list container are dropped
	{
		Iterator tmp = Begin();
		while (tmp != End()){
			//DestroyNode(*tmp);//error
			//DestroyNode(tmp._node);
			++tmp;
		}
		_list->_prev = _list;
		_list->_next = _list;
	}
	//assign函数用于重置list的内容
	//void assign(InputIterator first, InputIterator last);//用一段连续的空间重置
	//void assign(size_type n, const T& u);//用具体的值重置
	//void Remove(const T& x);
	//void Sort();
	//void Unique();
	//void Reverse();
	//void Merge(List<T,Alloc>& l);

	//接合函数，把以下内容接合到pos所指定的位置之前
	//一个链表的所有值
	//一个链表所指定的迭代器所指向的值
	//一个链表所指定的两个迭代器之间所指向的值
	//void Splice(Iterator pos,List<T,Alloc>& x);
	//void Splice(Iterator pos, List<T, Alloc>& x,Iterator it);
	//void Splice(Iterator pos, List<T, Alloc>& x,Iterator first,Iterator last);

	//Void Swap(List<T,Alloc>& l);

	//不支持随机访问，无[]重载
protected:
	Node_p _list;
};
#endif