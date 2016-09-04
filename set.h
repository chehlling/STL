//����Ԫ�ض�����ݼ�ֵ��������,set��������������ͬ�ļ�ֵ
//����ͨ����������set��Ԫ�ؽ����޸ģ�set�ļ�ֵ��ʵֵ��ʵֵ����ֵ���ײ�Ϊconst
//setʹ�ú������insert_unique(),mutilset����ʹ��insert_equal()
//����ʽ�������в���ʱ���ʹ�����Լ��ṩ��find,������STL�㷨find

#include "Pair.h"

template<class K,class Compare=Less<K>,class Alloc=alloc>//ȱʡ���õ�������
class Set
{
private:
	template<class T>
	struct Identity :public UnaryFunction<T, T>//unary_function<T, T>������<stl_function.h>
	{
		const T& operator(const T& x)const
		return x;
	};
	typedef RBTree<K, V, Identity<V>, Compare, Alloc> RepType;
	RepType t;//�ú��������ʾset
public:
	typedef typename RepType::ConstIterator Iterator;//��ʾ�޷�ͨ��set�ĵ����������޸�
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