#pragma once
//哈希表
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
	//error   hashtable迭代器是单向的
	//Self& operator--();
	//Self& operator--(int);
	bool operator==(const Self& h);
	bool operator!=(const Self& h);
};

//如果要在类内定义静态变量，类外初始化
//虽然开链法并不要求表格是质数，但SGI仍以质数设计表格大小，28个质数逐渐呈大约两倍的关系，减少哈希冲突
static const unsigned long __StlNumPrime = 28;
static unsigned long __StlPrimeList[__StlNumPrime] =
{	53ul, 97ul, 193ul, 389ul, 769ul,
	1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
	49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
	1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
	50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
	1610612741ul, 3221225473ul, 4294967291ul
};

//Value          节点的实值型别
//Key            节点的键值型别
//HashFcn        hashfunction的函数型别
//ExtractKey     从节点中取出键值的方法（仿函数）
//EqualKey       判断键值相同与否的方法
//Alloc
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
class HashTable
{
public:
	typedef HashTableIterator< Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> Iterator;
	typedef HashTableNode<Value> Node;
	typedef SimpleAlloc<Node, Alloc> NodeAllocator;
protected:
	//最接近并大于等于n的质数
	inline static unsigned long __StlNextPrime(size_t n)
	{
		//源码中使用lower_bound()，是一种二分查找的版本，试图在已排序的区间寻找某一元素,返回值为迭代器
		unsigned long ret = __StlPrimeList[0];
		for (int i = 0; i < __StlNumPrime; ++i){
			if (n <= __StlPrimeList[i]){
				ret = __StlPrimeList[i];
				break;
			}
		}
		return ret;
	}
	//_bucket的大小，即bucket的个数
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
	//无默认构造
	HashTable(size_t size, HashFcn hashfcn, EqualKey eql)
		:_Hash(hashfcn)
		, _Equals(eql)
		, _numElements(0)
	{
		//初始化buckets
		size_t n = __StlNextPrime(size);
		_bucket.Reserve(n);//reserve增加了容量但不改变大小，resize增加了容量也改变了大小
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
	//判知元素的落脚处
	//只接受实值
	size_t BktNum(const Value& obj)
	{
		//return BktNumKey(GetKey(obj));
	}
	//接受实值和bucket的个数
	size_t BktNum(const Value& obj, size_t n)
	{
		//return BktNumKey(_GetKey(obj),_bucket.Size());
	}
	//只接受键值
	size_t BktNumKey(const Key& key)
	{
		return BktNumKey(key,_bucket.Size());
	}
	//接受键值和bucket的个数
	size_t BktNumKey(const Key& key,size_t n)
	{
		return Hash(key) % n;
	}
public:
	void Resize(size_t size)
	{
		//拿元素个数和bucket vector的size比较
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
		Resize(_numElements+1);//判断是否需要重建表格
		return InsertUniqueNoresize(obj);
	}
	Pair<Iterator, bool> InsertUniqueNoResize(const Value& obj)
	{
		size_t index = BktNum(obj);
		Node* cur = _bucket[index];
		Node* head = cur;
		//Stl中使用equals(),即EqualTo()????????????????????????????????????
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
	
	//和内存释放有关的两个函数
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