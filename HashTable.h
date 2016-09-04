#pragma once
//��ϣ��
//
#include "Vector.h"
#include "Iterator.h"
#include "Alloc.h"
#include "HashFun.h"
#include "Pair.h"

template<class Value>
struct HashTableNode
{
	HashTableNode* _next;
	Value _value;
};

template<class Value,class Key,class HashFcn,class ExtractKey,class EqualKey,class Alloc=alloc>
class HashTable;

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
class HashTableIterator
{
public:
	typedef ForwardIteratorTag IteratorCategory;
	typedef Value* Pointer;
	typedef Value& Reference;
	typedef ptrdiff_t Difference;
	typedef Value ValueType;

	typedef HashTable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> HashTable;
	typedef HashTableNode<Value> Node;
	typedef HashTableIterator< Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> Self;

	HashTable* _hashTable;
	HashTableNode* _cur;



	Value& operator*();
	Value& operator->();
	Self& operator++();
	Self operator++(int);
	//error   hashtable�������ǵ����
	//Self& operator--();
	//Self& operator--(int);
	bool operator==(const Self& h);
	bool operator!=(const Self& h);
};

//���Ҫ�����ڶ��徲̬�����������ʼ��
//��Ȼ����������Ҫ��������������SGI����������Ʊ���С��28�������𽥳ʴ�Լ�����Ĺ�ϵ�����ٹ�ϣ��ͻ
static const unsigned long __StlNumPrime = 28;
static unsigned long __StlPrimeList[__StlNumPrime] =
{	53ul, 97ul, 193ul, 389ul, 769ul,
	1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
	49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
	1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
	50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
	1610612741ul, 3221225473ul, 4294967291ul
};

//Value          �ڵ��ʵֵ�ͱ�
//Key            �ڵ�ļ�ֵ�ͱ�
//HashFcn        hashfunction�ĺ����ͱ�
//ExtractKey     �ӽڵ���ȡ����ֵ�ķ������º�����
//EqualKey       �жϼ�ֵ��ͬ���ķ���
//Alloc
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
class HashTable
{
public:
	typedef HashTableIterator< Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> Iterator;
	typedef HashTableNode<Value> Node;
	typedef SimpleAlloc<Node, Alloc> NodeAllocator;
protected:
	//��ӽ������ڵ���n������
	inline static unsigned long __StlNextPrime(size_t n)
	{
		//Դ����ʹ��lower_bound()����һ�ֶ��ֲ��ҵİ汾����ͼ�������������Ѱ��ĳһԪ��,����ֵΪ������
		unsigned long ret = __StlPrimeList[0];
		for (int i = 0; i < __StlNumPrime; ++i){
			if (n <= __StlPrimeList[i]){
				ret = __StlPrimeList[i];
				break;
			}
		}
		return ret;
	}
	//_bucket�Ĵ�С����bucket�ĸ���
	size_t BucketCount()const
	{
		return _bucket.Size();
	}
protected:
	Node* NewNode(const Value& obj)
	{
		Node* node = NodeAllocator::Allocate();
		node->_next = NULL;
		Construct(&node->_value, obj);
		return node;
	}
	void DeleteNode(Node* node)
	{
		Destroy(&node->_value);
		NodeAllocator::Deallocate(node);
	}
public:
	//��Ĭ�Ϲ���
	HashTable(size_t size, HashFcn hashfcn, EqualKey eql)
		:_Hash(hashfcn)
		, _Equals(eql)
		, _numElements(0)
	{
		//��ʼ��buckets
		size_t n = __StlNextPrime(size);
		_bucket.Reserve(n);//reserve���������������ı��С��resize����������Ҳ�ı��˴�С
		for (int i = 0; i < n; ++i){
			_bucket.Insert(_bucket.End(),NULL);
		}
	}
	~HashTable()
	{
		/*size_t count = BucketCount();
		int i = 0;
		while (count-- && i < _numElements){
			;
		}*/
	}
public:
	//��֪Ԫ�ص���Ŵ�
	//ֻ����ʵֵ
	size_t BktNum(const Value& obj)
	{
		//return BktNumKey(GetKey(obj));
	}
	//����ʵֵ��bucket�ĸ���
	size_t BktNum(const Value& obj, size_t n)
	{
		//return BktNumKey(_GetKey(obj),_bucket.Size());
	}
	//ֻ���ܼ�ֵ
	size_t BktNumKey(const Key& key)
	{
		return BktNumKey(key,_bucket.Size());
	}
	//���ܼ�ֵ��bucket�ĸ���
	size_t BktNumKey(const Key& key,size_t n)
	{
		return Hash(key) % n;
	}
public:
	void Resize(size_t size)
	{
		//��Ԫ�ظ�����bucket vector��size�Ƚ�
		if (size > _bucket.Size()){
			size_t oldsize = _bucket.Size();
			size_t newsize = __StlNextPrime(oldsize);
			Node* tmp = NewNode();
			_bucket.Reserve(newsize);
			for (int i = 0; i < newsize; ++i){
				_bucket.Insert(_bucket.End(), NULL);
			}
			for (int i = 0; i < oldsize; ++i){
				
			}
		}
	}
	Pair<Iterator, bool> InsertUnique(const Value& obj)
	{
		Resize(_numElements+1);//�ж��Ƿ���Ҫ�ؽ����
		return InsertUniqueNoresize(obj);
	}
	Pair<Iterator, bool> InsertUniqueNoResize(const Value& obj)
	{
		size_t index = BktNum(obj);
		Node* cur = _bucket[index];
		Node* head = cur;
		//Stl��ʹ��equals(),��EqualTo()????????????????????????????????????
		while (cur){
			if (cur->_value == obj)
				return Pair<Iterator, bool>(Iterator(cur,this),false);
			cur = cur->_next;
		}
		Node* tmp = NewNode(obj);
		tmp->_next = head;
		_bucket[index] = tmp;
		++_numElements;
		return Pair<Iterator, bool>(Iterator(tmp, this),true);
	}
	//Pair<Iterator,bool> InsertEqual(const Value& obj);
	
	//���ڴ��ͷ��йص���������
	/*void Clear();
	void CopyFrom();

	Iterator Find(const Key& key)const;
	size_t Count(const Key& key)const;*/
private:
	Vector<Node*, Alloc> _bucket;
	size_t _numElements;

	HashFcn _Hash;
	ExtractKey _GetKey;
	EqualKey _Equals;//##############
};