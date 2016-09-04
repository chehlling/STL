//Alloc.h�����ڴ�ռ���������ͷ�
//Construct.h����������ݵĹ���������
//������ͷ�ļ���memory�ļ��а���
#pragma once

typedef void(*HANDLER_FUNC)();

//һ���ռ�������
template <int inst>
class __MallocAllocTemplate 
{
public:
	static void* Allocate(size_t size)//��̬��Ա����
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

	//�����ڴ�ʧ�ܴ������ľ������ָ��,��ֵΪNULL���ͻ��˿����趨
	static void(*__MallocAllocOomHandler)();
private:
	static void* __OomMalloc(size_t size);
	static void* __OomRealloc(void *p,size_t size);

//	ģ��C++��set_new_handler();
//	static void(*SetMallocHandler(void(*f)()))(){//����
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
		if (0 == myMallocHandler) {//û���Զ����ڴ��쳣��������STL�����쳣
			std::cout << "out of memory" << std::endl;
			exit(-1);
		}
		(*myMallocHandler)();//�����Զ����ڴ��쳣������
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
//�����ռ�������
enum{__ALIGN=8};
enum{__MAXBYTES=128};
enum{ __NFREELISTS = __MAXBYTES/__ALIGN};//��������ĸ���


//��һ���������ڶ��̣߳��ڶ�������û�������ó�
template <bool threads, int inst>
class __DefaultAllocTemplate 
{
private:
	union Obj{
		union Obj* freeLinkList;
		char clientData[1];//���û�ʹ��
	};
	static size_t FREELIST_INDEX(size_t size)//����size����ʹ�ü�����������
	{
		return ((size + (__ALIGN - 1)) / __ALIGN - 1);
	}
	static size_t ROUND_UP(size_t size) {//��size���ϵ���Ϊ8�ı���
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
	static char* _start;//�ڴ��
	static char* _end;
	static size_t _heapSize;

	static Obj* _freeList[__NFREELISTS];//ָ����������
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
	Obj* myFreeList = _freeList[index];//STL���õ��Ƕ���ָ��
	if (myFreeList != NULL){//���������Ӧλ�ù����ڴ��
		_freeList[index] = myFreeList->freeLinkList;
		return myFreeList;
	}
	//���������Ӧλ��û���ڴ�飬���ڴ��ȡ
	void *result = Refill(ROUND_UP(size));
	return result;
}
template <bool threads, int inst>
void __DefaultAllocTemplate<threads, inst>::Deallocate(void* ptr,size_t size)
{
	if (size > __MAXBYTES){//����һ���ռ�������
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
//size��ʱ�ѵ���8�ı������ڶ�����������������
template <bool threads, int inst>
char*  __DefaultAllocTemplate<threads, inst>::ChunkAlloc(size_t size, size_t& Objs)
{
	size_t TotalBytes =Objs*size;
	size_t LeftBytes = _end - _start;
	char* result=NULL;
	if (TotalBytes <= LeftBytes){//�ڴ��������������ڴ�
		result = _start;
		_start += TotalBytes;
	}
	else{
		if (LeftBytes < size){//�ڴ��û���㹻�󣬵���һ���ռ�������
			size_t requSize = 2 * TotalBytes + ROUND_UP(_heapSize >> 4);
			int index = 0;
			Obj* myfreeList=NULL;
			if (LeftBytes>0){//�ڴ�ػ���һЩ��ͷ���ҵ���Ӧ��λ�ã���һ����8�ı�����
				myfreeList = (Obj*)_start;
				index = FREELIST_INDEX(LeftBytes);
				myfreeList->freeLinkList = _freeList[index];
				_freeList[index] = myfreeList;
			}
			//����һ���ռ������������ڴ��
			_start = (char*)__MallocAllocTemplate<0>::Allocate(requSize);
			if (_start == 0){//һ���ռ�����������ʧ�ܣ�����������������ȡ����Ҫ��С�ڴ������ڴ�
				index = FREELIST_INDEX(size);
		
				for (index; index < __NFREELISTS; ++index){
					if (_freeList[index] != NULL){
						_start = (char*)_freeList[index];
						_freeList[index]=((Obj*)_start)->freeLinkList;
						_end = _start + (index+1)*__ALIGN;
						return(	ChunkAlloc(size, Objs));
					}
				}
				_end = NULL;//û���κοռ����

				_start = (char*)__MallocAllocTemplate<0>::Allocate(requSize);
			}
			_end = _start + requSize;
			_heapSize += requSize;
			return (ChunkAlloc(size,Objs));
		}
		else{//�ڴ��������һ���ռ�Ĵ�С
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
	size_t Objs = 20;//������20������Ĵ�С�ҵ�����������
	char* chunk = ChunkAlloc(size,Objs);//�õ�һ�����ڴ�׼�����ҵ���������
	
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