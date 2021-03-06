#ifndef VL_DATA_ITERATOR
#define VL_DATA_ITERATOR

#include "VL_Data_Basic.h"

namespace vl
{

/*********************************************************************************************************
Support
*********************************************************************************************************/

	template<typename _Mark>
	class VL_Markable:public IVL_Interface
	{
		friend class VL_MarkManager;
		friend class VL_MarkManagerNode;
	private:
		_Mark	FMark;
	protected:
		_Mark GetMark()
		{
			return FMark;
		}
		void SetMark(_Mark Mark)
		{
			FMark=Mark;
		}
	};

	class VL_MarkManagerNode;

	typedef VL_Markable<VL_MarkManagerNode*> VL_MarkManagerItem;

	class VL_MarkManagerNode:public VL_Base
	{
	protected:
		VL_MarkManagerNode*	FPrev;
		VL_MarkManagerNode*	FNext;
		VL_MarkManagerItem*	FItem;
	public:
		VL_MarkManagerNode(VL_MarkManagerNode* Prev , VL_MarkManagerNode* Next);
		~VL_MarkManagerNode();

		VL_MarkManagerNode*	GetPrev();
		VL_MarkManagerNode*	GetNext();
		void				SetPrev(VL_MarkManagerNode* Prev);
		void				SetNext(VL_MarkManagerNode* Next);
		void				SetItem(VL_MarkManagerItem* Item);
		void				DestroyItem();
	};

	class VL_MarkManager:public VL_Base
	{
	protected:
		VL_MarkManagerNode*	FHead;
		VL_MarkManagerNode*	FTail;
	public:
		VL_MarkManager();
		~VL_MarkManager();

		void				Mark(VL_MarkManagerItem* Markee);
		VBool				IsEmpty();
	};

	template<typename _Type>
	class IVL_Iterator:public IVL_Interface
	{
	public:
		virtual _Type&	Item()=0;		//迭代器对应的当前数据
		virtual void	First()=0;
		virtual void	Next()=0;
		virtual VBool	IsFinished()=0;	//迭代器是否无法继续Next()，true的时候Item()已经失效
		virtual VBool	IsAvailable()=0;	//迭代器的Item()是否有效
	};

	template<typename _Type>
	class IVL_ModifiableIterator:public IVL_Iterator<_Type>
	{
	public:
		virtual void	Delete()=0;
		virtual void	InsertPrev(_Type Item)=0;
		virtual void	InsertNext(_Type Item)=0;
	};

	template<typename _Type>
	class IVL_BiIterator:public IVL_Interface
	{
	public:
		virtual _Type& Item()=0;		//迭代器对应的当前数据
		virtual void	First()=0;
		virtual void	Last()=0;
		virtual void	Next()=0;
		virtual void	Prev()=0;
		virtual VBool	IsFirst()=0;		//迭代器是否对应第一个数据
		virtual VBool	IsLast()=0;		//迭代器是否对应最后一个数据
		virtual VBool	IsAvailable()=0;	//迭代器的Item()是否有效
	};

/*********************************************************************************************************
Iterator
*********************************************************************************************************/

	template<typename _Type>
	class VL_Iterator:public VL_Base
	{
	protected:
		VL_AutoPtr<IVL_Iterator<_Type>>	FIterator;

	public:
		VL_Iterator(IVL_Iterator<_Type>* Iterator)
		{
			FIterator=Iterator;
		}

		IVL_Iterator<_Type>* operator -> ()
		{
			return FIterator.Object();
		}

		operator VBool()
		{
			return FIterator->IsFinished();
		}
		void operator ++(VInt)
		{
			FIterator->Next();
		}
	};

	template<typename _Type>
	class VL_ModifiableIterator:public VL_Base
	{
	protected:
		VL_AutoPtr<IVL_ModifiableIterator<_Type>>	FIterator;
		
	public:
		VL_ModifiableIterator(IVL_ModifiableIterator<_Type>* Iterator)
		{
			FIterator=Iterator;
		}

		IVL_ModifiableIterator<_Type>* operator -> ()
		{
			return FIterator.Object();
		}

		operator VBool()
		{
			return FIterator->IsFinished();
		}
		void operator ++(VInt)
		{
			FIterator->Next();
		}
	};

	template<typename _Type>
	class VL_BiIterator:public VL_Base
	{
	protected:
		VL_AutoPtr<IVL_BiIterator<_Type>>	FIterator;

	public:
		VL_BiIterator(IVL_BiIterator<_Type>* Iterator)
		{
			FIterator=Iterator;
		}

		IVL_BiIterator<_Type>* operator -> ()
		{
			return FIterator.Object();
		}

		operator VBool()
		{
			return !FIterator->IsAvailable();
		}
		void operator ++(VInt)
		{
			FIterator->Next();
		}
		void operator --(VInt)
		{
			FIterator->Prev();
		}
	};
}

#endif