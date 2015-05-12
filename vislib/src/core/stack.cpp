#include <vis/stack.h>
#include <vector>

struct	STK_FRAME{
	std::vector<VIS_VOID*>	vPtr;
};

struct	STK_STK{
	std::vector<STK_FRAME>	vFrame;
};

static STK_STK g_stk;

VIS_INT16	STK_Push()
{
	g_stk.vFrame.push_back(STK_FRAME());
	return VIS_OK;
}

VIS_INT16	STK_Pop()
{
	VIS_INT16 wSts=VIS_OK;
	VIS_CHK_ASSERT(!g_stk.vFrame.empty());
	
	STK_FRAME &frame =g_stk.vFrame.back();
	for( size_t i=0, n=frame.vPtr.size(); i<n; i++)
		free(frame.vPtr[i]);
	
	g_stk.vFrame.pop_back();
Exit:	
	return wSts;
}

VIS_INT16	STK_Malloc(VIS_UINT32 n, VIS_VOID** pp)
{
	VIS_INT16 wSts=VIS_OK;
	
	VIS_CHK_ASSERT(!g_stk.vFrame.empty());
	STK_FRAME &frame =g_stk.vFrame.back();
	
	VIS_VOID* ptr =malloc(n);
	VIS_CHK_ASSERT(ptr!=NULL);
	frame.vPtr.push_back(ptr);

	*pp =ptr;
Exit:	
	return wSts;
}

template <class T>
VIS_INT16	STK_Malloc(VIS_SIZE const &size, VIS_BUF_T<T> *ptr)
{
	VIS_INT16 wSts=VIS_OK;
	
	VIS_CHK_ASSERT(size.x && size.y);
	
	ptr->size =size;
	ptr->linestep =size.x;
	wSts = STK_Malloc(ptr->linestep *ptr->size.y*sizeof(VIS_BUF_T<T>::type), (VIS_VOID**)&ptr->ptr);
	VIS_CHK_STS;
	
Exit:	
	return wSts;
}

template 
VIS_XXPORT VIS_INT16	STK_Malloc(VIS_SIZE const &size, VIS_BUF_T<VIS_UINT8> *ptr);

template 
VIS_XXPORT VIS_INT16	STK_Malloc(VIS_SIZE const &size, VIS_BUF_T<VIS_UINT16> *ptr);

template 
VIS_XXPORT VIS_INT16	STK_Malloc(VIS_SIZE const &size, VIS_BUF_T<VIS_FLOAT32> *ptr);