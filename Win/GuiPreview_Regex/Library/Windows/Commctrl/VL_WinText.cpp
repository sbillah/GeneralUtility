#include "VL_WinText.h"

namespace vl
{
	namespace windows
	{

/*********************************************************************************************************
VL_WinEdit
*********************************************************************************************************/

	VL_WinEdit::VL_WinEdit(VL_WinContainer* Parent , VBool Multiline , VBool AlwaysShowSelection)
	{
		_CreateWindow(
			0,
			WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT | (AlwaysShowSelection?ES_NOHIDESEL:0) | (Multiline?ES_MULTILINE:0) | ES_WANTRETURN ,
			WC_EDIT,
			Parent);
	}

	VL_WinEdit::~VL_WinEdit()
	{
	}

	LRESULT VL_WinEdit::ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure)
	{
		LRESULT Result=0;
		switch(Message)
		{
		case WM_COMMAND_DISPATCHED:
			switch(HIWORD(wParam))
			{
			case EN_CHANGE:
				OnChanged(this);
				break;
			case EN_HSCROLL:
				OnHScroll(this);
				break;
			case EN_VSCROLL:
				OnVScroll(this);
				break;
			case EN_MAXTEXT:
				OnMaxText(this);
				break;
			case EN_UPDATE:
				OnUpdate(this);
				break;
			}
			Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
			break;
		case WM_SETTEXT:
			Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
			if(GetMultiline())
			{
				OnChanged(this);
			}
			break;
		default:
			Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
		}
		return Result;
	}

	VLE_WinHAlign VL_WinEdit::GetTextAlign()
	{
		if(GetStyle(ES_RIGHT))
		{
			return vwhaRight;
		}
		else if(GetStyle(ES_CENTER))
		{
			return vwhaCenter;
		}
		else
		{
			return vwhaLeft;
		}
	}

	void VL_WinEdit::SetTextAlign(VLE_WinHAlign Align)
	{
		SetStyle(ES_LEFT,Align==vwhaLeft);
		SetStyle(ES_CENTER,Align==vwhaCenter);
		SetStyle(ES_RIGHT,Align==vwhaRight);
	}

	VBool VL_WinEdit::GetMultiline()
	{
		return GetStyle(ES_MULTILINE);
	}

	void VL_WinEdit::SetMultiline(VBool Value)
	{
		SetStyle(ES_MULTILINE,Value);
	}

	VLE_EditCharDisplay VL_WinEdit::GetCharDisplay()
	{
		if(GetStyle(ES_LOWERCASE))
		{
			return vecdLowerCase;
		}
		else if(GetStyle(ES_UPPERCASE))
		{
			return vecdUpperCase;
		}
		else if(GetStyle(ES_NUMBER))
		{
			return vecdDigitOnly;
		}
		else if(GetStyle(ES_PASSWORD))
		{
			return vecdPassword;
		}
		else
		{
			return vecdNone;
		}
	}

	void VL_WinEdit::SetCharDisplay(VLE_EditCharDisplay Display)
	{
		SetStyle(ES_LOWERCASE,Display==vecdLowerCase);
		SetStyle(ES_UPPERCASE,Display==vecdUpperCase);
		SetStyle(ES_NUMBER,Display==vecdDigitOnly);
		SetStyle(ES_PASSWORD,Display==vecdPassword);
	}

	VWChar VL_WinEdit::GetPasswordChar()
	{
		return (VWChar)SendMessage(FHandle,EM_GETPASSWORDCHAR,0,0);
	}

	void VL_WinEdit::SetPasswordChar(VWChar Char)
	{
		SendMessage(FHandle,EM_SETPASSWORDCHAR,Char,0);
	}

	VBool VL_WinEdit::GetHScroll()
	{
		return GetStyle(WS_HSCROLL);
	}

	void VL_WinEdit::SetHScroll(VBool Value)
	{
		SetStyle(WS_HSCROLL,Value);
	}

	VBool VL_WinEdit::GetVScroll()
	{
		return GetStyle(WS_VSCROLL);
	}

	void VL_WinEdit::SetVScroll(VBool Value)
	{
		SetStyle(WS_VSCROLL,Value);
	}

	VBool VL_WinEdit::GetReadonly()
	{
		return GetStyle(ES_READONLY);
	}

	void VL_WinEdit::SetReadonly(VBool Value)
	{
		SendMessage(FHandle,EM_SETREADONLY,(Value?TRUE:FALSE),0);
	}

	VBool VL_WinEdit::GetWantReturn()
	{
		return GetStyle(ES_WANTRETURN);
	}

	void VL_WinEdit::SetWantReturn(VBool Value)
	{
		SetStyle(ES_WANTRETURN,Value);
	}

	VBool VL_WinEdit::GetBorder()
	{
		return GetStyle(WS_BORDER);
	}

	void VL_WinEdit::SetBorder(VBool Value)
	{
		SetStyle(WS_BORDER,Value);
	}

	VInt VL_WinEdit::GetTextLimit()
	{
		return SendMessage(FHandle,EM_GETLIMITTEXT,0,0);
	}

	void VL_WinEdit::SetTextLimit(VInt Value)
	{
		SendMessage(FHandle,EM_SETLIMITTEXT,Value,0);
	}

	VBool VL_WinEdit::GetModified()
	{
		return SendMessage(FHandle,EM_GETMODIFY,0,0)!=0;
	}

	void VL_WinEdit::SetModified(VBool Value)
	{
		SendMessage(FHandle,EM_SETMODIFY,Value?TRUE:FALSE,0);
	}

	VUnicodeString VL_WinEdit::GetSel()
	{
		return GetText().SubString(GetSelStart(),GetSelLength());
	}

	void VL_WinEdit::SetSel(VUnicodeString Sel)
	{
		SendMessage(FHandle,EM_REPLACESEL,TRUE,(LPARAM)Sel.Buffer());
	}

	VInt VL_WinEdit::GetSelStart()
	{
		LRESULT Result=SendMessage(FHandle,EM_GETSEL,0,0);
		return LOWORD(Result);
	}

	void VL_WinEdit::SetSelStart(VInt Value)
	{
		Select(Value,0);
	}

	VInt VL_WinEdit::GetSelLength()
	{
		LRESULT Result=SendMessage(FHandle,EM_GETSEL,0,0);
		return HIWORD(Result)-LOWORD(Result);
	}

	void VL_WinEdit::SetSelLength(VInt Value)
	{
		Select(GetSelStart(),Value);
	}

	void VL_WinEdit::Select(VInt Index , VInt Count)
	{
		SendMessage(FHandle,EM_SETSEL,Index,Index+Count);
	}

	void VL_WinEdit::SelectAll()
	{
		SendMessage(FHandle,EM_SETSEL,0,-1);
	}

	void VL_WinEdit::ClearSel()
	{
		SendMessage(FHandle,WM_CLEAR,0,0);
	}

	void VL_WinEdit::Copy()
	{
		SendMessage(FHandle,WM_COPY,0,0);
	}

	void VL_WinEdit::Cut()
	{
		SendMessage(FHandle,WM_CUT,0,0);
	}

	void VL_WinEdit::Paste()
	{
		SendMessage(FHandle,WM_PASTE,0,0);
	}

	VBool VL_WinEdit::GetCanUndo()
	{
		return SendMessage(FHandle,EM_CANUNDO,0,0)!=0;
	}

	VInt VL_WinEdit::GetCharFromPos(VInt X , VInt Y)
	{
		return LOWORD(SendMessage(FHandle,EM_CHARFROMPOS,0,MAKEWORD(X,Y)));
	}

	void VL_WinEdit::GetPosFromChar(VInt Index , VInt& X , VInt& Y)
	{
		LRESULT Result=SendMessage(FHandle,EM_POSFROMCHAR,Index,0);
		X=LOWORD(Result);
		Y=HIWORD(Result);
	}

	VInt VL_WinEdit::GetFirstVisible()
	{
		return SendMessage(FHandle,EM_GETFIRSTVISIBLELINE,0,0);
	}

	VUnicodeString VL_WinEdit::GetLine(VInt LineIndex)
	{
		VInt LineLength=SendMessage(FHandle,EM_LINELENGTH,LineIndex,0);
		PWChar Buffer=new VWChar[LineLength+1];
		Buffer[SendMessage(FHandle,EM_GETLINE,LineIndex,(LPARAM)Buffer)]=L'\0';
		VUnicodeString Result=Buffer;
		delete[] Buffer;
		return Result;
	}

	VInt VL_WinEdit::GetLineCount()
	{
		return SendMessage(FHandle,EM_GETLINECOUNT,0,0);
	}

	VInt VL_WinEdit::GetCurrentLine()
	{
		return GetLineIndex(-1);
	}

	VInt VL_WinEdit::GetLineIndex(VInt CharIndex)
	{
		return SendMessage(FHandle,EM_LINEFROMCHAR,CharIndex,0);
	}

	VInt VL_WinEdit::GetCharIndex(VInt LineIndex)
	{
		return SendMessage(FHandle,EM_LINEINDEX,LineIndex,0);
	}

	void VL_WinEdit::ScrollToCaret()
	{
		SendMessage(FHandle,EM_SCROLLCARET,0,0);
	}

	void VL_WinEdit::ClearUndoBuffer()
	{
		SendMessage(FHandle,EM_EMPTYUNDOBUFFER,0,0);
	}

	void VL_WinEdit::Undo()
	{
		SendMessage(FHandle,WM_UNDO,0,0);
	}

/*********************************************************************************************************
VL_WinStatic
*********************************************************************************************************/

	VL_WinStatic::VL_WinStatic(VL_WinContainer* Parent)
	{
		_CreateWindow(
			0,
			WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | SS_LEFT | SS_NOTIFY | SS_NOPREFIX,
			WC_STATIC,
			Parent);
	}

	VL_WinStatic::~VL_WinStatic()
	{
	}

	LRESULT VL_WinStatic::ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure)
	{
		LRESULT Result=0;
		switch(Message)
		{
		case WM_COMMAND_DISPATCHED:
			switch(HIWORD(wParam))
			{
			case STN_CLICKED:
				OnClick(this);
				break;
			case STN_DBLCLK:
				OnDbClick(this);
				break;
			}
			Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
			break;
		default:
			Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
		}
		return Result;
	}

	VLE_WinHAlign VL_WinStatic::GetTextAlign()
	{
		if(GetStyle(SS_RIGHT))
		{
			return vwhaRight;
		}
		else if(GetStyle(SS_CENTER))
		{
			return vwhaCenter;
		}
		else
		{
			return vwhaLeft;
		}
	}

	void VL_WinStatic::SetTextAlign(VLE_WinHAlign Align)
	{
		SetStyle(SS_LEFT,Align==vwhaLeft);
		SetStyle(SS_CENTER,Align==vwhaCenter);
		SetStyle(SS_RIGHT,Align==vwhaRight);
	}

	VLE_StatusEdge VL_WinStatic::GetBorderStyle()
	{
		if(GetStyle(SS_ETCHEDFRAME))
		{
			return vseEtched;
		}
		else if(GetStyle(SS_SUNKEN))
		{
			return vseSunken;
		}
		else
		{
			return vseNone;
		}
	}

	void VL_WinStatic::SetBorderStyle(VLE_StatusEdge Value)
	{
		SetStyle(SS_ETCHEDFRAME,Value==vseEtched);
		SetStyle(SS_SUNKEN,Value==vseSunken);
	}

/*********************************************************************************************************
VL_WinListBox
*********************************************************************************************************/

	VBool VL_WinListBox::IsMultiSelection()
	{
		return GetStyle(LBS_EXTENDEDSEL) || GetStyle(LBS_MULTIPLESEL);
	}

	VL_WinListBox::VL_WinListBox(VL_WinContainer* Parent , VLE_ListBoxStyle Style)
	{
		DWORD ListStyle=0;
		switch(Style)
		{
		case vlbsMultiSelection:
			ListStyle=LBS_EXTENDEDSEL;
			break;
		case vlbsCheckSelection:
			ListStyle=LBS_MULTIPLESEL;
			break;
		case vlbsMultiAndCheck:
			ListStyle=LBS_EXTENDEDSEL | LBS_MULTIPLESEL;
			break;
		}
		_CreateWindow(
			0,
			WS_VSCROLL | WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LBS_HASSTRINGS | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | ListStyle,
			WC_LISTBOX,
			Parent);
	}

	VL_WinListBox::~VL_WinListBox()
	{
	}

	LRESULT  VL_WinListBox::ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure)
	{
		LRESULT Result=0;
		switch(Message)
		{
		case WM_COMMAND_DISPATCHED:
			switch(HIWORD(wParam))
			{
			case LBN_DBLCLK:
				OnDbClick(this);
				break;
			case LBN_SELCANCEL:
			case LBN_SELCHANGE:
				OnSelChanged(this);
				break;
			}
			Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
			break;
		default:
			Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
		}
		return Result;
	}

	VInt VL_WinListBox::GetFocusedIndex()
	{
		return SendMessage(FHandle,LB_GETCARETINDEX,0,0);
	}

	void VL_WinListBox::SetFocusedIndex(VInt Value)
	{
		SendMessage(FHandle,LB_SETCARETINDEX,Value,FALSE);
	}

	VInt VL_WinListBox::GetSelectedIndex()
	{
		if(IsMultiSelection() || GetCount()==0)
		{
			return -1;
		}
		else
		{
			VInt Result=SendMessage(FHandle,LB_GETCURSEL,0,0);
			return Result==LB_ERR?-1:Result;
		}
	}

	void VL_WinListBox::SetSelectedIndex(VInt Value)
	{
		if(!IsMultiSelection())
		{
			SendMessage(FHandle,LB_SETCURSEL,Value,0);
		}
	}

	VInt VL_WinListBox::GetItemHeight(VInt Index)
	{
		if(!GetStyle(LBS_OWNERDRAWVARIABLE))Index=0;
		return SendMessage(FHandle,LB_GETITEMHEIGHT,Index,0);
	}

	void VL_WinListBox::SetItemHeight(VInt Index , VInt Value)
	{
		if(!GetStyle(LBS_OWNERDRAWVARIABLE))Index=0;
		SendMessage(FHandle,LB_SETITEMHEIGHT,Index,Value);
	}

	VInt VL_WinListBox::GetFirstVisibleIndex()
	{
		return SendMessage(FHandle,LB_GETTOPINDEX,0,0);
	}

	void VL_WinListBox::SetFirstVisibleIndex(VInt Value)
	{
		SendMessage(FHandle,LB_SETTOPINDEX,Value,0);
	}

	VBool VL_WinListBox::GetSelected(VInt Index)
	{
		return SendMessage(FHandle,LB_GETSEL,0,0)>0;
	}

	void VL_WinListBox::SetSelected(VInt Index , VBool Value)
	{
		SendMessage(FHandle,LB_SETSEL,(Value?TRUE:FALSE),Index);
	}

	void VL_WinListBox::GetSelectedIndices(VL_List<VInt , true>& Indices)
	{
		Indices.Clear();
		if(IsMultiSelection())
		{
			VInt Count=GetCount();
			VInt* Buffer=new VInt[Count];
			VInt Result=SendMessage(FHandle,LB_GETSELITEMS,Count,(LPARAM)Buffer);
			for(VInt i=0;i<Result;i++)
			{
				Indices.Add(Buffer[i]);
			}
			delete[] Buffer;
		}
	}

	void VL_WinListBox::SelectAll(VBool Selected)
	{
		SetSelected(-1,Selected);
	}

	void VL_WinListBox::AddString(VUnicodeString String)
	{
		SendMessage(FHandle,LB_ADDSTRING,0,(LPARAM)String.Buffer());
	}

	void VL_WinListBox::InsertString(VInt Index , VUnicodeString String)
	{
		if(Index==GetCount())Index=-1;
		SendMessage(FHandle,LB_INSERTSTRING,Index,(LPARAM)String.Buffer());
	}

	void VL_WinListBox::DeleteString(VInt Index)
	{
		SendMessage(FHandle,LB_DELETESTRING,Index,0);
	}

	VInt VL_WinListBox::GetCount()
	{
		return SendMessage(FHandle,LB_GETCOUNT,0,0);
	}

	VUnicodeString VL_WinListBox::GetString(VInt Index)
	{
		VInt Count=SendMessage(FHandle,LB_GETTEXTLEN,Index,0);
		if(Count==LB_ERR)
		{
			return L"";
		}
		else
		{
			PWChar Buffer=new VWChar[Count+1];
			SendMessage(FHandle,LB_GETTEXT,Index,(LPARAM)Buffer);
			VUnicodeString Result=Buffer;
			delete[] Buffer;
			return Result;
		}
	}

	void VL_WinListBox::SetString(VInt Index , VUnicodeString String)
	{
		DeleteString(Index);
		InsertString(Index,String);
	}

	void VL_WinListBox::Clear()
	{
		SendMessage(FHandle,LB_RESETCONTENT,0,0);
	}

	VInt VL_WinListBox::FindStringPrefix(VUnicodeString String)
	{
		VInt Result=SendMessage(FHandle,LB_FINDSTRING,-1,(LPARAM)String.Buffer());
		return Result==LB_ERR?-1:Result;
	}

	VInt VL_WinListBox::FindString(VUnicodeString String)
	{
		VInt Result=SendMessage(FHandle,LB_FINDSTRINGEXACT,-1,(LPARAM)String.Buffer());
		return Result==LB_ERR?-1:Result;
	}

	void VL_WinListBox::SelectPrefix(VUnicodeString String)
	{
		SendMessage(FHandle,LB_SELECTSTRING,-1,(LPARAM)String.Buffer());
	}

/*********************************************************************************************************
VL_WinComboBox
*********************************************************************************************************/

	VL_WinComboBox::VL_WinComboBox(VL_WinContainer* Parent , VBool ReadOnly)
	{
		_CreateWindow(
			0,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_AUTOHSCROLL | CBS_HASSTRINGS | (ReadOnly?CBS_DROPDOWNLIST:CBS_DROPDOWN),
			WC_COMBOBOX,
			Parent);
	}

	VL_WinComboBox::~VL_WinComboBox()
	{
	}

	LRESULT VL_WinComboBox::ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure)
	{
		LRESULT Result=0;
		switch(Message)
		{
		case WM_COMMAND_DISPATCHED:
			switch(HIWORD(wParam))
			{
			case CBN_CLOSEUP:
				OnCloseUp(this);
				break;
			case CBN_DBLCLK:
				OnDbClick(this);
				break;
			case CBN_DROPDOWN:
				OnDropDown(this);
				break;
			case CBN_EDITCHANGE:
				OnChanged(this);
				break;
			case CBN_EDITUPDATE:
				OnUpdate(this);
				break;
			case CBN_SELCHANGE:
				OnSelChanged(this);
				break;
			case CBN_SELENDCANCEL:
				OnRejectSel(this);
				break;
			case CBN_SELENDOK:
				OnAcceptSel(this);
				break;
			}
			Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
			break;
		default:
			Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
		}
		return Result;
	}

	VBool VL_WinComboBox::GetDisabledScroll()
	{
		return GetStyle(CBS_DISABLENOSCROLL);
	}

	void VL_WinComboBox::SetDiabledScroll(VBool Value)
	{
		SetStyle(CBS_DISABLENOSCROLL,Value);
	}

	VInt VL_WinComboBox::GetSelStart()
	{
		DWORD Start,End;
		SendMessage(FHandle,CB_GETEDITSEL,(WPARAM)&Start,(LPARAM)&End);
		return Start;
	}

	void VL_WinComboBox::SetSelStart(VInt Value)
	{
		Select(Value,0);
	}

	VInt VL_WinComboBox::GetSelLength()
	{
		DWORD Start,End;
		SendMessage(FHandle,CB_GETEDITSEL,(WPARAM)&Start,(LPARAM)&End);
		return End-Start;
	}

	void VL_WinComboBox::SetSelLength(VInt Value)
	{
		Select(GetSelStart(),Value);
	}

	void VL_WinComboBox::Select(VInt Index , VInt Length)
	{
		SendMessage(FHandle,CB_SETEDITSEL,0,MAKEWORD(Index,Index+Length));
	}

	VInt VL_WinComboBox::GetMinVisible()
	{
		return SendMessage(FHandle,CB_GETMINVISIBLE,0,0);
	}

	void VL_WinComboBox::SetMinVisible(VInt Value)
	{
		SendMessage(FHandle,CB_SETMINVISIBLE,Value,0);
	}

	void VL_WinComboBox::SelectAll()
	{
		Select(0,-1);
	}

	void VL_WinComboBox::ClearSel()
	{
		Select(-1,0);
	}

	VInt VL_WinComboBox::GetSelectedIndex()
	{
		return SendMessage(FHandle,CB_GETCURSEL,0,0);
	}

	void VL_WinComboBox::SetSelectedIndex(VInt Index)
	{
		SendMessage(FHandle,CB_SETCURSEL,Index,0);
		OnSelChanged(this);
	}

	VInt VL_WinComboBox::GetItemHeight()
	{
		return SendMessage(FHandle,CB_GETITEMHEIGHT,0,0);
	}

	void VL_WinComboBox::SetItemHeight(VInt Value)
	{
		SendMessage(FHandle,CB_SETITEMHEIGHT,0,Value);
	}

	VInt VL_WinComboBox::GetFirstVisibleIndex()
	{
		return SendMessage(FHandle,CB_GETTOPINDEX,0,0);
	}

	void VL_WinComboBox::SetFirstVisibleIndex(VInt Index)
	{
		SendMessage(FHandle,CB_SETTOPINDEX,Index,0);
	}

	VBool VL_WinComboBox::IsOpened()
	{
		return SendMessage(FHandle,CB_GETDROPPEDSTATE,0,0)==TRUE;
	}

	void VL_WinComboBox::AddString(VUnicodeString String)
	{
		SendMessage(FHandle,CB_ADDSTRING,0,(LPARAM)String.Buffer());
	}

	void VL_WinComboBox::InsertString(VInt Index , VUnicodeString String)
	{
		SendMessage(FHandle,CB_INSERTSTRING,Index,(LPARAM)String.Buffer());
	}

	void VL_WinComboBox::DeleteString(VInt Index)
	{
		SendMessage(FHandle,CB_DELETESTRING,Index,0);
	}

	VInt VL_WinComboBox::GetCount()
	{
		return SendMessage(FHandle,CB_GETCOUNT,0,0);
	}

	VUnicodeString VL_WinComboBox::GetString(VInt Index)
	{
		VInt Length=SendMessage(FHandle,CB_GETLBTEXTLEN,Index,0);
		if(Length==CB_ERR)
		{
			return L"";
		}
		else
		{
			PWChar Buffer=new VWChar[Length+1];
			SendMessage(FHandle,CB_GETLBTEXT,Index,(LPARAM)Buffer);
			VUnicodeString Result=Buffer;
			delete[] Buffer;
			return Result;
		}
	}

	void VL_WinComboBox::SetString(VInt Index , VUnicodeString String)
	{
		VInt Sel=GetSelectedIndex();
		DeleteString(Index);
		InsertString(Index,String);
		SetSelectedIndex(Sel);
	}

	void VL_WinComboBox::Clear()
	{
		for(VInt i=GetCount()-1;i>=0;i--)
		{
			DeleteString(i);
		}
	}

	VInt VL_WinComboBox::FindStringPrefix(VUnicodeString String)
	{
		LRESULT Result=SendMessage(FHandle,CB_FINDSTRING,-1,(LPARAM)String.Buffer());
		return Result==CB_ERR?-1:Result;
	}

	VInt VL_WinComboBox::FindString(VUnicodeString String)
	{
		LRESULT Result=SendMessage(FHandle,CB_FINDSTRINGEXACT,-1,(LPARAM)String.Buffer());
		return Result==CB_ERR?-1:Result;
	}

	void VL_WinComboBox::SelectPrefix(VUnicodeString String)
	{
		SendMessage(FHandle,CB_SELECTSTRING,-1,(LPARAM)String.Buffer());
	}

	void VL_WinComboBox::OpenList()
	{
		SendMessage(FHandle,CB_SHOWDROPDOWN,TRUE,0);
	}

	void VL_WinComboBox::CloseList()
	{
		SendMessage(FHandle,CB_SHOWDROPDOWN,FALSE,0);
	}

/*********************************************************************************************************
VL_WinStatusItem
*********************************************************************************************************/

	VL_WinStatusItem::VL_WinStatusItem(VL_WinStatus* Status , VInt Index)
	{
		FStatus=Status;
		FIndex=Index;
	}

	VL_WinStatusItem::VL_WinStatusItem()
	{
		FStatus=0;
		FIndex=0;
	}

	VL_WinStatusItem::VL_WinStatusItem(const VL_WinStatusItem& Item)
	{
		FStatus=Item.FStatus;
		FIndex=Item.FIndex;
	}

	VL_WinStatusItem::~VL_WinStatusItem()
	{
	}

	VL_WinStatusItem& VL_WinStatusItem::operator=(const VL_WinStatusItem& Item)
	{
		FStatus=Item.FStatus;
		FIndex=Item.FIndex;
		return *this;
	}

	VL_WinStatusItem::operator VBool()
	{
		return FStatus!=0 && FIndex>=0 && FIndex<FStatus->FItems.GetCount();
	}

	VUnicodeString VL_WinStatusItem::GetTextLeft()
	{
		if(*this)
		{
			return FStatus->FItems[FIndex].TextLeft;
		}
		else
		{
			return L"";
		}
	}

	void VL_WinStatusItem::SetTextLeft(VUnicodeString Value)
	{
		if(*this)
		{
			FStatus->FItems[FIndex].TextLeft=Value;
			FStatus->RefreshItem(FIndex);
		}
	}

	VUnicodeString VL_WinStatusItem::GetTextCenter()
	{
		if(*this)
		{
			return FStatus->FItems[FIndex].TextCenter;
		}
		else
		{
			return L"";
		}
	}

	void VL_WinStatusItem::SetTextCenter(VUnicodeString Value)
	{
		if(*this)
		{
			FStatus->FItems[FIndex].TextCenter=Value;
			FStatus->RefreshItem(FIndex);
		}
	}

	VUnicodeString VL_WinStatusItem::GetTextRight()
	{
		if(*this)
		{
			return FStatus->FItems[FIndex].TextRight;
		}
		else
		{
			return L"";
		}
	}

	void VL_WinStatusItem::SetTextRight(VUnicodeString Value)
	{
		if(*this)
		{
			FStatus->FItems[FIndex].TextRight=Value;
			FStatus->RefreshItem(FIndex);
		}
	}

	VInt VL_WinStatusItem::GetWidth()
	{
		if(*this)
		{
			return FStatus->FItems[FIndex].Width;
		}
		else
		{
			return 0;
		}
	}

	void VL_WinStatusItem::SetWidth(VInt Value)
	{
		if(*this && Value>=0)
		{
			FStatus->FItems[FIndex].Width=Value;
			FStatus->RefreshItems();
		}
	}

	VBool VL_WinStatusItem::GetBorder()
	{
		if(*this)
		{
			return FStatus->FItems[FIndex].Border;
		}
		else
		{
			return false;
		}
	}

	void VL_WinStatusItem::SetBorder(VBool Value)
	{
		if(*this)
		{
			FStatus->FItems[FIndex].Border=Value;
			FStatus->RefreshItem(FIndex);
		}
	}

	VBool VL_WinStatusItem::GetRaise()
	{
		if(*this)
		{
			return FStatus->FItems[FIndex].Raise;
		}
		else
		{
			return false;
		}
	}

	void VL_WinStatusItem::SetRaise(VBool Value)
	{
		if(*this)
		{
			FStatus->FItems[FIndex].Raise=Value;
			FStatus->RefreshItem(FIndex);
		}
	}

/*********************************************************************************************************
VL_WinStatus
*********************************************************************************************************/

	VL_IMPLEMENT_VOID_EVENT	(VE_StatusItemEvent	,(Sender , Item)					,(VL_Base* Sender , VInt Item));

	const VInt STATUS_BAR_MAX_ITEM_COUNT=256;

	VL_WinStatus::ItemRecord::ItemRecord()
	{
		Width=0;
		Border=true;
		Raise=false;
	}

	void VL_WinStatus::RefreshItem(VInt Index)
	{
		INT wParam=Index;
		if(FItems[Index].Border)
		{
			if(FItems[Index].Raise)
			{
				wParam|=SBT_POPOUT;
			}
		}
		else
		{
			wParam|=SBT_NOBORDERS;
		}
		VUnicodeString Text=FItems[Index].TextLeft+L'\t'+FItems[Index].TextCenter+L'\t'+FItems[Index].TextRight;
		SendMessage(FHandle,SB_SETTEXT,wParam,(LPARAM)Text.Buffer());
	}

	void VL_WinStatus::RefreshItems()
	{
		INT Borders[3];
		SendMessage(FHandle,SB_GETBORDERS,0,(LPARAM)Borders);
		VInt w=Borders[0];
		VInt h=Borders[1];
		VInt s=Borders[2];
		INT Rights[STATUS_BAR_MAX_ITEM_COUNT]={0};
		VInt CurrentRight=w;
		for(VInt i=0;i<FItems.GetCount();i++)
		{
			if(i==FItems.GetCount()-1 && FItems[i].Width==0)
			{
				Rights[i]=-1;
			}
			else
			{
				CurrentRight+=FItems[i].Width;
				Rights[i]=CurrentRight;
				CurrentRight+=s;
			}
		}
		SendMessage(FHandle,SB_SETPARTS,FItems.GetCount(),(LPARAM)Rights);
		for(VInt i=0;i<FItems.GetCount();i++)
		{
			RefreshItem(i);
		}
	}
	
	VL_WinStatus::VL_WinStatus(VL_WinContainer* Parent)
	{
		_CreateWindow(
			0,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SBARS_SIZEGRIP,
			STATUSCLASSNAME,
			Parent);
		SendMessage(FHandle,SB_SETUNICODEFORMAT,TRUE,0);
	}

	VL_WinStatus::~VL_WinStatus()
	{
	}

	LRESULT VL_WinStatus::ProcessMessage(UINT Message , WPARAM& wParam , LPARAM& lParam , VBool& CallDefaultProcedure)
	{
		LRESULT Result=0;
		switch(Message)
		{
		case WM_SIZE:
			CallDefaultProcedure=false;
			Result= VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
			break;
		case WM_NOTIFY_DISPATCHED:
			switch(((NMHDR*)lParam)->code)
			{
			case NM_CLICK:
				{
					VInt Index=((NMMOUSE*)lParam)->dwItemSpec;
					OnClick(this,Index);
				}
				break;
			case NM_DBLCLK:
				{
					VInt Index=((NMMOUSE*)lParam)->dwItemSpec;
					OnDbClick(this,Index);
				}
				break;
			case NM_RCLICK:
				{
					VInt Index=((NMMOUSE*)lParam)->dwItemSpec;
					OnRightClick(this,Index);
				}
				break;
			case NM_RDBLCLK:
				{
					VInt Index=((NMMOUSE*)lParam)->dwItemSpec;
					OnRightDbClick(this,Index);
				}
				break;
			}
			Result=VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
			break;
		default:
			Result= VL_WinControl::ProcessMessage(Message,wParam,lParam,CallDefaultProcedure);
		}
		return Result;
	}

	VL_WinStatusItem VL_WinStatus::AddItem()
	{
		return InsertItem(GetItemCount());
	}

	VL_WinStatusItem VL_WinStatus::InsertItem(VInt Index)
	{
		if(Index>=0 && Index<=GetItemCount() && GetItemCount()<STATUS_BAR_MAX_ITEM_COUNT)
		{
			ItemRecord Item;
			FItems.Insert(Index,Item);
			RefreshItems();
			return VL_WinStatusItem(this,Index);
		}
		else
		{
			return VL_WinStatusItem();
		}
	}

	VL_WinStatusItem VL_WinStatus::GetItem(VInt Index)
	{
		if(Index>=0 && Index<GetItemCount())
		{
			return VL_WinStatusItem(this,Index);
		}
		else
		{
			return VL_WinStatusItem();
		}
	}

	VInt VL_WinStatus::GetItemCount()
	{
		return FItems.GetCount();
	}

	void VL_WinStatus::DeleteItem(VInt Index)
	{
		if(Index>=0 && Index<GetItemCount())
		{
			FItems.Delete(Index);
			RefreshItems();
		}
	}

	}
}