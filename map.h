//map的所有元素都是pair，pair的第一元素称为键值，第二元素称为实值
//map会根据其键值进行排序，不允许两个相同的键值
//map不可以通过迭代器修改其键值，但可以修改其实值

#include "Pair.h"

template<class K,class T, class Compare = Less<K>, class Alloc = alloc>//缺省采用递增排序
class Map
{
private:
	typedef RBTree<K, V, Selectlst<V>, Compare, Alloc> RepType;
	RepType t;//以红黑树表现Map

	typedef Map<K, T, Compare, Alloc> Self;
public:
	typedef Pair<const K, T> ValueType;
	typedef typename RepType::Iterator Iterator;//它不像Set一样将迭代器定义为ConstIterator
public:
	Iterator Begin();
	Iterator End();
	size_t Count(const T& x)const;
	bool Empty()const;
	size_t Size()const;
	void Swap(Self& m);
	void Clear();

	Pair<Iterator, bool> Insert(const ValueType& value);
	Iterator Insert(Iterator pos,const ValueType& value);//Position of the first element to be compared for the insertion operation.
	template<class InputIterator>
	void Insert(InputIterator first, InputIterator last);
	
	size_t Erase(const K& key);
	void Erase(Iterator pos);
	void Erase(Iterator first, Iterator last);
	
	Iterator Find(const K& key);
	
	T& operator[](const K& key)
	{
		return ((*(Insert(ValueType(key, T()))).first).second);
	}
};
