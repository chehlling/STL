//所有元素都会根据键值进行排序,set不允许有两个相同的键值
//不能通过迭代器对set的元素进行修改，set的键值即实值，实值即键值，底层为const
//set使用红黑树的insert_unique(),mutilset才能使用insert_equal()
//关联式容器进行查找时最好使用其自己提供的find,而不是STL算法find

#include "Pair.h"

template<class K,class Compare=Less<K>,class Alloc=alloc>//缺省采用递增排序
class Set
{
private:
	template<class T>
	struct Identity :public UnaryFunction<T, T>//unary_function<T, T>定义于<stl_function.h>
	{
		const T& operator(const T& x)const
		return x;
	};
	typedef RBTree<K, V, Identity<V>, Compare, Alloc> RepType;
	RepType t;//用红黑树来表示set
public:
	typedef typename RepType::ConstIterator Iterator;//表示无法通过set的迭代器进行修改
	typedef K ValueType;
public:
	Iterator Begin();
	Iterator End();
	void Clear();
	bool Empty()const;
	size_t Count(const K& key);
	size_t Size()const;
	void Swap(Set<K,Compare,Alloc>& s);

	Pair<Iterator,bool> Insert(const K& key);
	Iterator Insert(Iterator pos,const K& key);
	template<class InputIterator>
	void Insert(InputIterator first,Iterator last);

	size_t Erase(const K& key);
	void Erase(Iterator pos);
	void Erase(Iterator first,Iterator last);

	Iterator Find(const K& key)const;
};