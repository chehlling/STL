#include <iostream>
#include "List.h"
#include <list>

struct aa
{
	int _a;
	char _c;
	aa(int a = 0, char c = 'a')
		:_a(a)
		, _c(c)
	{}
};

void printList1(List<int> l)
{
	List<int>::Iterator ite = l.Begin();
	*ite = 2;
	for (ite = l.Begin(); ite != l.End(); ++ite){
		std::cout << *ite << std::endl;
	}
}

void printList2(const List<int> l)
{
	List<int>::ConstIterator ite = l.Begin();
	//*ite = 2;
	for (ite = l.Begin(); ite != l.End(); ++ite){
		std::cout << *ite << std::endl;
	}
}

void TestList()
{
	/*std::list<aa> l1;
	std::list<aa>::iterator items;
	l1.push_back(aa());
	l1.push_back(aa(1, 'b'));
	l1.push_back(aa(2, 'c'));
	l1.push_back(aa(3, 'd'));
	items = l1.begin();

	for (; items != l1.end(); ){
		std::cout << items->_a << "  " <<items->_c <<std::endl;
		items++;
	}*/

	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	List<int>::Iterator ite;
	for (ite = l.Begin(); ite != l.End(); ++ite){
		std::cout << *ite << std::endl;
	}
	std::cout << l.Size() << std::endl;

	ite = l.Begin();
	l.Insert(ite,100);
	std::cout << *ite << std::endl;

	l.PopFront();
	l.PopBack();
	l.PushFront(78);
	l.PushFront(100);

	List<int>::Iterator it = l.Begin();
	List<int>::Iterator its = l.End();
	List<int>::Iterator item;
	std::cout<<*it<<std::endl;
	++it;
	its--;
	std::cout << (it != its) << std::endl;
	
	int data1 = l.Front();
	int data2= l.Back();
	std::cout<<l.Size()<<std::endl;
	std::cout << !l.Empty() << std::endl;


	//list<int> l;
	//l.PushBack(1);
	//l.PushBack(2);
	////printlist1(l);
	//printlist2(l);
}

#include "Vector.h"
#include <vector>

void printVector2(Vector<int> v)
{
	size_t size = v.Size();
	Vector<int>::Iterator it = v.Begin();
	while (it!=v.End()){
		std::cout << *it << std::endl;
		++it;
	}
}
void printvector1(std::vector<int> v)
{
	size_t size = v.size();
	std::vector<int>::iterator it = v.begin();
	while (it != v.end()){
		std::cout << *it << std::endl;
		++it;
	}
}

void TestVector()
{
	//std::vector<int> v1;
	//v1.push_back(2);
	//std::vector<int>::iterator item;
	//item=v1.Begin();
	////v1.erase(item);
	////v1.pop_back();
	//printvector1(v1);


	Vector<int> v2;
	v2.PushBack(2);
	Vector<int>::Iterator ite;
	ite = v2.Begin();
	v2.Erase(ite);
	printVector2(v2);

	v2.PopBack();
	v2.PushBack(1);
	v2.PushBack(2);
	v2.PushBack(3);
	v2.PushBack(4);
	v2.PopBack();
	std::cout<<v2[0]<<std::endl;
	printVector2(v2);
	//ite = v2.end();
	//v2.erase(ite);
	//printvector2(v2);
	/*ite = v2.Begin();
	v2.insert(ite,5);
	printvector2(v2);
	v2.erase(ite);
	ite = v2.Begin();
	printvector2(v2);*/
	/*ite = v2.end();
	v2.erase(ite);*/
	//printvector2(v2);
}

#include <iterator>
#include "Iterator.h"

void TestIterator()
{
	Vector<int> v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);
	v.PushBack(5);

	Vector<int>::Iterator it1 = v.Begin();
	Advance(it1,10);

	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	l.PushBack(5);

	List<int>::Iterator it2 = l.Begin();
	Advance(it2, 4);
}

#include "Alloc.h"

void Testalloc1()
{
	/*char* ptr1 = new char;
	char* ptr2 = new char[4];*/
	std::cout << " 测试调用一级配置器分配内存 " << std::endl;
	char*p1 = SimpleAlloc< char, alloc>::Allocate(129);
	p1 = SimpleAlloc< char, alloc>::Allocate(8);
	SimpleAlloc<char, alloc>::Deallocate(p1, 129);

	// 测试调用二级配置器分配内存    
	std::cout << " 测试调用二级配置器分配内存 " << std::endl;
	char*p2 = SimpleAlloc< char, alloc>::Allocate(128);
	char*p3 = SimpleAlloc< char, alloc>::Allocate(128);
	char*p4 = SimpleAlloc< char, alloc>::Allocate(128);
	char*p5 = SimpleAlloc< char, alloc>::Allocate(128);
	SimpleAlloc<char, alloc>::Deallocate(p2, 128);
	SimpleAlloc<char, alloc>::Deallocate(p3, 128);
	SimpleAlloc<char, alloc>::Deallocate(p4, 128);
	SimpleAlloc<char, alloc>::Deallocate(p5, 128);
	for (int i = 0; i < 21; ++i){
		printf(" 测试第%d次分配 \n", i + 1);
		char*p = SimpleAlloc< char, alloc>::Allocate(128);
	}
}

void TestAlloc2()
{
	// 测试特殊场景 
	std::cout<<" 测试内存池空间不足分配个 "<<std::endl ;     // 8*20->8*2->320     
	char*p1 = SimpleAlloc< char, alloc>::Allocate (8);
	char*p2 = SimpleAlloc< char, alloc>::Allocate(8);
	std::cout << " 测试内存池空间不足，系统堆进行分配 " << std::endl;   
	char*p3 = SimpleAlloc< char, alloc>::Allocate(12);
}

void TestAlloc3()
{
	std::cout << " 测试系统堆内存耗尽 " << std::endl;
	SimpleAlloc<char, alloc>::Allocate(1024 * 1024 * 1024); 
	SimpleAlloc<char, alloc>::Allocate(1024 * 1024);
	// 不好测试，说明系统管理小块内存的能力还是很强的。     
	for (int i = 0; i < 100000; ++i ){
		char*p1 = SimpleAlloc< char, alloc>::Allocate (128);  
	} 

}
void TestAlloc()
{
	//TestAlloc1();
	//TestAlloc2();
	TestAlloc3();
}

#include "HashTable.h"
#include "HashFun.h"
#include "Function.h"

void TestHashTable()
{
	HashTable<int, int, Hash<int>, Identity<int>, EqualTo<int>> ht(50,Hash<int>(),EqualTo<int>());
}

int main()
{
	//TestAlloc();
	//TestList();
	//TestVector();
	//TestIterator();
	TestHashTable();

	system("pause");
	return 0;
}
