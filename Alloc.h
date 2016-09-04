//Alloc.h负责内存空间的配置与释放
//Construct.h负责对象内容的构造与析构
//这两个头文件在memory文件中包含
#pragma once

typedef void(*HANDLER_FUNC)();

//一级空间配置器
template <int inst>
class __MallocAllocTemplate 
{
public:
	static void* Allocate(size_t size)//静态成员函数
	{
		void* result = malloc(size);
		if (result == NULL){
			result = __OomMalloc(size);
		}
		return result;
	}
	static void Deallocate(void* ptr)
	{
		free(ptr);
	}
	static void* Reallocate(void* ptr,size_t size)
	{
		void* result = realloc(ptr,size);
		if (result == NULL){
			result = __OomRealloc(ptr,size);
		}
		return result;
	}

	//分配内存失败处理函数的句柄函数指针,初值为NULL，客户端可以设定
	static void(*__MallocAllocOomHandler)();
private:
	static void* __OomMalloc(size_t size);
	static void* __OomRealloc(void *p,size_t size);

//	模仿C++的set_new_handler();
//	static void(*SetMallocHandler(void(*f)()))(){//函数
	static HANDLER_FUNC SetMallocHandler(HANDLER_FUNC f){
		HANDLER_FUNC old = __MallocAllocOomHandler;
		__MallocAllocOomHandler = f;
		return old;
	}
};

template<int inst>         
void (* __MallocAllocTemplate<inst>::__MallocAllocOomHandler)() = NULL;

template<int inst>
void* __MallocAllocTemplate<inst>::__OomMalloc(size_t size)
{
	void(*myMallocHandler)();
	void *result;

	for (;;) {
		myMallocHandler = __MallocAllocOomHandler;
		if (0 == myMallocHandler) {//没有自定义内存异常处理函数，STL中抛异常
			std::cout << "out of memory" << std::endl;
			exit(-1);
		}
		(*myMallocHandler)();//调用自定义内存异常处理函数
		result = malloc(size);
		if (result) return(result);
	}
}
template<int inst>
void* __MallocAllocTemplate<inst>::__OomRealloc(void *ptr,size_t size)
{
	void(*myMallocHandler)();
	void *result;

	for (;;) {
		myMallocHandler = __MallocAllocOomHandler;
		if (0 == myMallocHandler) { 
			std::cout << "out of memory" << std::endl;
			exit(-1);
		}
		(*myMallocHandler)();
		result = realloc(ptr, size);
		if (result) 
			return (result);
	}
}

template<class T, class Alloc>
class SimpleAlloc 
{
public:
	static T *Allocate(size_t n)
	{
		return 0 == n ? 0 : (T*)Alloc::Allocate(n * sizeof (T));
	}
	static T *Allocate(void)
	{
		return (T*)Alloc::Allocate(sizeof (T));
	}
	static void Deallocate(T *ptr, size_t n)
	{
		if (0 != n) Alloc::Deallocate(ptr, n * sizeof (T));
	}
	static void Deallocate(T *ptr)
	{
		Alloc::Deallocate(ptr, sizeof (T));
	}
};


//template <class Alloc>
//class debug_alloc {}

# ifdef __USE_MALLOC

typedef __MallocAllocTemplate<0> alloc;

# else
//二级空间配置器
enum{__ALIGN=8};
enum{__MAXBYTES=128};
enum{ __NFREELISTS = __MAXBYTES/__ALIGN};//自由链表的个数


//第一个参数用于多线程，第二个参数没有派上用场
template <bool threads, int inst>
class __DefaultAllocTemplate 
{
private:
	union Obj{
		union Obj* freeLinkList;
		char clientData[1];//给用户使用
	};
	static size_t FREELIST_INDEX(size_t size)//根据size决定使用几号自由链表
	{
		return ((size + (__ALIGN - 1)) / __ALIGN - 1);
	}
	static size_t ROUND_UP(size_t size) {//将size往上调整为8的倍数
		return ((size + __ALIGN - 1) & ~(__ALIGN - 1));
	}
public:
	static void* Allocate(size_t size);
	static void Deallocate(void* ptr,size_t size);
	static void* Reallocate(void* p,size_t size);
private:
	static char* ChunkAlloc(size_t size, size_t& Objs);
	static void* Refill(size_t size);
public:
	static char* _start;//内存池
	static char* _end;
	static size_t _heapSize;

	static Obj* _freeList[__NFREELISTS];//指向自由链表
};
template <bool threads, int inst>
char* __DefaultAllocTemplate<threads, inst>::_start = 0;
template <bool threads, int inst>
char* __DefaultAllocTemplate<threads, inst>::_end = 0;
template <bool threads, int inst>
size_t __DefaultAllocTemplate<threads, inst>::_heapSize = 0;
template <bool threads, int inst>
typename __DefaultAllocTemplate<threads, inst>::Obj* \
__DefaultAllocTemplate<threads, inst>::_freeList[__NFREELISTS] =\
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

typedef __DefaultAllocTemplate<true, 0> alloc;

template <bool threads, int inst>
void* __DefaultAllocTemplate<threads, inst>::Allocate(size_t size)
{
	if (size > __MAXBYTES){
		return __MallocAllocTemplate<0>::Allocate(size);
	}
	size_t index = FREELIST_INDEX(size);
	Obj* myFreeList = _freeList[index];//STL中用的是二级指针
	if (myFreeList != NULL){//自由链表对应位置挂有内存块
		_freeList[index] = myFreeList->freeLinkList;
		return myFreeList;
	}
	//自由链表对应位置没有内存块，到内存池取
	void *result = Refill(ROUND_UP(size));
	return result;
}
template <bool threads, int inst>
void __DefaultAllocTemplate<threads, inst>::Deallocate(void* ptr,size_t size)
{
	if (size > __MAXBYTES){//调用一级空间配置器
		__MallocAllocTemplate<0>::Deallocate(ptr);
	}
	else{
		size_t index = FREELIST_INDEX(size);
		((Obj*)ptr)->freeLinkList = _freeList[index];
		_freeList[index] = (Obj*)ptr;
	}
}
template <bool threads, int inst>
void* __DefaultAllocTemplate<threads, inst>::Reallocate(void* p, size_t size)
{

}
//size此时已调至8的倍数，第二个参数必须是引用
template <bool threads, int inst>
char*  __DefaultAllocTemplate<threads, inst>::ChunkAlloc(size_t size, size_t& Objs)
{
	size_t TotalBytes =Objs*size;
	size_t LeftBytes = _end - _start;
	char* result=NULL;
	if (TotalBytes <= LeftBytes){//内存池有满足需求的内存
		result = _start;
		_start += TotalBytes;
	}
	else{
		if (LeftBytes < size){//内存池没有足够大，调用一级空间配置器
			size_t requSize = 2 * TotalBytes + ROUND_UP(_heapSize >> 4);
			int index = 0;
			Obj* myfreeList=NULL;
			if (LeftBytes>0){//内存池还有一些零头，挂到相应的位置（它一定是8的倍数）
				myfreeList = (Obj*)_start;
				index = FREELIST_INDEX(LeftBytes);
				myfreeList->freeLinkList = _freeList[index];
				_freeList[index] = myfreeList;
			}
			//调用一级空间配置器配置内存池
			_start = (char*)__MallocAllocTemplate<0>::Allocate(requSize);
			if (_start == 0){//一级空间配置器分配失败，到自由链表中试着取比需要大小内存更大的内存
				index = FREELIST_INDEX(size);
		
				for (index; index < __NFREELISTS; ++index){
					if (_freeList[index] != NULL){
						_start = (char*)_freeList[index];
						_freeList[index]=((Obj*)_start)->freeLinkList;
						_end = _start + (index+1)*__ALIGN;
						return(	ChunkAlloc(size, Objs));
					}
				}
				_end = NULL;//没有任何空间可用

				_start = (char*)__MallocAllocTemplate<0>::Allocate(requSize);
			}
			_end = _start + requSize;
			_heapSize += requSize;
			return (ChunkAlloc(size,Objs));
		}
		else{//内存池能满足一个空间的大小
			Objs = LeftBytes / size;
			TotalBytes = Objs * size;
			result = _start;
			_start += TotalBytes;
		}
	}
	return result;
}

template <bool threads, int inst>
void* __DefaultAllocTemplate<threads, inst>::Refill(size_t size)
{
	size_t Objs = 20;//想申请20个对象的大小挂到自由链表中
	char* chunk = ChunkAlloc(size,Objs);//拿到一大块的内存准备往挂到自由链表
	
	if (Objs == 1)
		return chunk;

	Obj* result = (Obj*)chunk;
	Obj* curObj = NULL;
	Obj* nextObj = NULL;
	size_t index = FREELIST_INDEX(size);
	_freeList[index] = nextObj = (Obj*)(chunk + size);
	for (size_t i = 0; i < Objs-2; ++i){
		curObj = nextObj;
		nextObj = curObj + 1;
		curObj->freeLinkList = nextObj;
	}
	curObj->freeLinkList = NULL;

	return result;
}
#endif