/*******************************************************************************
Vczh Library++ 2.0
数据结构::Unicode字符串工具包
开发者：陈梓瀚

接口：
  IVL_UniTokenParser<_HelpType>	：词法分析器
基本操作：
  VL_UniBuilder					：字符串构造器
  VL_UniReplacer				：具有多重替换对象的字符串替换器
  VL_UniSpliter					：具有多重分割符的字符串分割器
  VL_UniStrings					：自动通过换行符分割的字符串列表
词法分析框架：
  VL_UniToken<_HelpType>		：记号
  VL_UniTokens<_HelpType>		：记号栈
Sepstr分析器：
  *Sepstr是一种分割字符串的格式。字符串被空格、tab、换行符和结束符分隔。其中换行符和结束符也作为一种记号出现。
  *Sepstr支持双引号扩起来的字符串。在这种字符串里，"\"、"""、换行符被转义为"\\"、"\""、"\r"和"\n"。
  VL_UniSepstrBuilder			：词法分析器
  VL_UniSepstrToken				：记号
  VL_UniSepstrTokens			：记号栈
函数：
*******************************************************************************/

#ifndef VL_UNIOP
#define VL_UNIOP

#define _CRT_SECURE_NO_DEPRECATE
#include "Data\VL_Data_String.h"
#include "Data\VL_Data_List.h"
#include "Data\VL_Data_Chain.h"
#include "Data\VL_Data_Map.h"

namespace vl
{
	namespace uniop
	{

		using namespace collection;
/*********************************************************************************************************
Basic Operation
*********************************************************************************************************/

		class VL_UniBuilder : public VL_Base
		{
		protected:
			PWChar											FBuffer;
			VInt											FBlockSize;
			VInt											FLength;
			VInt											FSize;

			void											RequireSpace(VInt Count);
		public:
			VL_UniBuilder(VInt BlockSize);
			~VL_UniBuilder();

			VInt											Length();
			VUnicodeString									GetString();
			VUnicodeString									GetString(VInt Index , VInt Count);
			void											Append(VUnicodeString String);
			void											Append(PWChar String , VInt Count=-1);
			void											Append(VWChar Char);
			void											Insert(VInt Index , VUnicodeString String);
			void											Insert(VInt Index , PWChar String , VInt Count=-1);
			void											Delete(VInt Index , VInt Count);
			void											Clear();
			void											operator +=(VUnicodeString String);
			void											operator +=(PWChar String);
			void											operator +=(VWChar Char);
			VWChar&											operator [](VInt Index);
		};

		class VL_UniReplacer : public VL_Base
		{
		protected:
			VL_ListedMap<VUnicodeString , VUnicodeString>	FRepList;
			VBool											FCacheBuilt;
			VUnicodeString*									FStrFroms;
			VUnicodeString*									FStrTos;
			VInt											FStrCount;
			VInt											FMaxRatio;

			void											DestroyCache();
			void											BuildCache();
		public:
			VL_UniReplacer();
			~VL_UniReplacer();

			void											SetItem(VUnicodeString StrFrom , VUnicodeString StrTo);
			VUnicodeString									Replace(VUnicodeString Str , VInt& ReplaceCount);
		};

		class VL_UniSpliter : public VL_Base
		{
		protected:
			VL_ListedMap<VUnicodeString , VBool>			FSpliterList;
			VBool											FCacheBuilt;
			VUnicodeString*									FSpliters;
			VBool*											FReserveds;
			VInt											FSpliterCount;

			void DestroyCache();
			void BuildCache();
		public:
			VL_UniSpliter();
			~VL_UniSpliter();

			void SetItem(VUnicodeString Spliter , VBool Reserved);
			void Split(VUnicodeString Str , VL_List<VUnicodeString , false>& Result , VBool DiscardEmptyPart);
		};

		class VL_UniStrings : public VL_Base
		{
			typedef IVL_Iterator<VUnicodeString> IVL_StringsIterator;
			typedef IVL_BiIterator<VUnicodeString> IVL_StringsBiIterator;
		protected:
			VL_List<VUnicodeString , false>					FStrings;

			void											Split(VUnicodeString Text , VL_List<VUnicodeString , false>& Result);
		public:
			VL_UniStrings();
			~VL_UniStrings();

			VInt											GetCount();
			VUnicodeString									Fetch(VInt Index);
			void											Delete(VInt Index);
			void											Clear();
			VUnicodeString&									operator[](VInt Index);
			void											Add(VUnicodeString String);
			void											Insert(VInt Index , VUnicodeString String);
			VInt											IndexOf(VUnicodeString String);
			VBool											Exists(VUnicodeString String);
			VUnicodeString									GetText();
			void											SetText(VUnicodeString Text);
		};

/*********************************************************************************************************
Lexical Analyser Shell
*********************************************************************************************************/

		template<typename _HelpType>
		class VL_UniToken : public VL_Base
		{
			typedef VL_UniToken<_HelpType>	_Token;
		public:
			VUnicodeString	Token;
			_HelpType		Info;

			_Token& operator =(_Token aToken)
			{
				Token=aToken.Token;
				Info=aToken.Info;
				return *this;
			}
		};

		template<typename _HelpType>
		class IVL_UniTokenParser : public IVL_Interface
		{
		protected:
			typedef VL_UniToken<_HelpType>	_Token;
		public:
			virtual void SetString(VUnicodeString String)=0;
			virtual _Token GetToken(VBool& Exists)=0;
		};

		template<typename _HelpType>
		class VL_UniTokens : public VL_Base
		{
			typedef VL_UniToken<_HelpType>			_Token;
			typedef IVL_UniTokenParser<_HelpType>	_Parser;
			typedef VL_Chain<_Token>				_Container;
		protected:
			_Container	FTokens;
			VL_Base*	FCurrentNode;
			_Parser*	FParser;
			
			VBool		FStoreToken;
		public:
			VL_UniTokens(VBool StoreToken)
			{
				FCurrentNode=0;
				FStoreToken=StoreToken;
			}

			void SetParser(_Parser* Parser)
			{
				FParser=Parser;
			}

			void Clear()
			{
				FTokens.Clear();
			}

			VBool Backward(VInt Count)
			{
				if(FStoreToken)
				{
					if(FCurrentNode)
					{
						for(VInt i=0;i<Count;i++)
						{
							VL_Chain<_Token>::VL_ocNode* Temp=dynamic_cast<VL_Chain<_Token>::VL_ocNode*>(FCurrentNode)->GetPrev();
							if(Temp)
							{
								FCurrenNode=Temp;
							}
							else
							{
								return false;
							}
						}
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}

			_Token* GetToken()
			{
				if(FCurrentNode)
				{
					FCurrentNode=dynamic_cast<VL_Chain<_Token>::VL_ocNode*>(FCurrentNode)->GetNext();
				}
				if(!FCurrentNode)
				{
					VBool Exists=false;
					_Token Token=FParser->GetToken(Exists);
					if(Exists)
					{
						if(FStoreToken)
						{
							FTokens.InsertAtLast(Token);
							FCurrentNode=FTokens.GetLastNode();
							return &(dynamic_cast<VL_Chain<_Token>::VL_ocNode*>(FCurrentNode)->Data());
						}
						else
						{
							if(FTokens.GetCount())
							{
								FCurrentNode=FTokens.GetLastNode();
								dynamic_cast<VL_Chain<_Token>::VL_ocNode*>(FCurrentNode)->Data()=Token;
							}
							else
							{
								FTokens.InsertAtLast(Token);
								FCurrentNode=FTokens.GetLastNode();
							}
							return &(dynamic_cast<VL_Chain<_Token>::VL_ocNode*>(FCurrentNode)->Data());
						}
					}
					else
					{
						FCurrentNode=FTokens.GetLastNode();
						return 0;
					}
				}
				else
				{
					return &(dynamic_cast<VL_Chain<_Token>::VL_ocNode*>(FCurrentNode)->Data());
				}
			}
		};

/*********************************************************************************************************
Separated String Analyser
*********************************************************************************************************/

		enum VLE_SepstrInfo
		{
			vsiNormal,
			vsiEOL,
			vsiEOS
		};

		class VL_UniSepstrBuilder : public VL_Base
		{
		protected:
			VL_UniBuilder	FBuilder;
			VL_UniReplacer	FReplacer;
		public:
			VL_UniSepstrBuilder(VInt BlockSize);

			VInt			Length();
			VUnicodeString	GetString();
			void			Append(VUnicodeString String);
			void			Append(PWChar String , VInt Count=-1);
			void			AppendUnencoded(VUnicodeString String);
			void			AppendUnencoded(PWChar String , VInt Count=-1);
		};

		class VL_UniSepstrParser : public IVL_UniTokenParser<VLE_SepstrInfo>
		{
		protected:
			VL_UniBuilder	FBuilder;
			VUnicodeString	FStringBuffer;
			PWChar			FReadPoint;
		public:
			VL_UniSepstrParser();
			~VL_UniSepstrParser();

			void			SetString(VUnicodeString String);
			_Token			GetToken(VBool& Exists);
		};

		typedef VL_UniToken<VLE_SepstrInfo>		VL_UniSepstrToken;
		typedef VL_UniTokens<VLE_SepstrInfo>	VL_UniSepstrTokens;

	}
}

#endif