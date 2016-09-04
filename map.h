//map������Ԫ�ض���pair��pair�ĵ�һԪ�س�Ϊ��ֵ���ڶ�Ԫ�س�Ϊʵֵ
//map��������ֵ�������򣬲�����������ͬ�ļ�ֵ
//map������ͨ���������޸����ֵ���������޸���ʵֵ

#include "Pair.h"

template<class K,class T, class Compare = Less<K>, class Alloc = alloc>//ȱʡ���õ�������
class Map
{
private:
	typedef RBTree<K, V, Selectlst<V>, Compare, Alloc> RepType;
	RepType t;//�Ժ��������Map

	typedef Map<K, T, Compare, Alloc> Self;
public:
	typedef Pair<const K, T> ValueType;
	typedef typename RepType::Iterator Iterator;//������Setһ��������������ΪConstIterator
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
