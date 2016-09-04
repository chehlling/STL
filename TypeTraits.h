#pragma once

//˫����ǰ׺����˼��SGI�ڲ����õĶ���������STL��׼֮��
//IteratorTraits������ȡ������������
//__TypeTraits������ȡ�ͱ������
struct __FalseType
{};

struct __TrueType
{};
//һ���྿��ʲôʱ������Լ���ƽ�����͹���
template<class T>
struct __TypeTraits
{
	typedef __FalseType HasTrivialDefaultConstructor;
	typedef __FalseType HasTrivialCopyConstructor;
	typedef __FalseType HasTrivialAssignmentOperator;
	typedef __FalseType HasTrivialDestructor;//��ƽ�����͵�����
	typedef __FalseType IsPODType;
};
//���ԭ��ָ����Ƶ�ƫ�ػ��汾
template<class T>
struct __TypeTraits<T*>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;//��ƽ�����͵�����
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<char>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;//��ƽ�����͵�����
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<unsigned char>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<short>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<unsigned short>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<int>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
}; 

template<>
struct __TypeTraits<unsigned int>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<long>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<unsigned long>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<float>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<double>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};


template<>
struct __TypeTraits<long long>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<bool>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

//���ǿ�������Զ����࣬�������һ���ػ��汾����ȷ���߱�������������
//template<>
//struct __TypeTraits<Date>
//{
//	typedef __TrueType HasTrivialDefaultConstructor;
//	typedef __TrueType HasTrivialCopyConstructor;
//	typedef __TrueType HasTrivialAssignmentOperator;
//	typedef __TrueType HasTrivialDestructor;
//	typedef __TrueType IsPODType;
//};