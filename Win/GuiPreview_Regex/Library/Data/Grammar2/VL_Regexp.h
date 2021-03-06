/*******************************************************************************
Vczh Library++ 2.0
语法工具::正则表达式分析
开发者：陈梓瀚

接口：
  IVL_RegExpRecursiveAlgorithm				：关于正则表达式语法树的递归算法接口
类：
  VL_RegExpBase								：语法树
  VL_RegExpCharset							：字符集
  VL_RegExpLoop								：循环
  VL_RegExpFunc								：功能
  VL_RegExpSection							：分支或序列
函数：
  CharRangeToString							：把字符区间转换为字符串
  CharSetToString							：把字符集转换为字符串
  RegularExpressionAnalysis					：分析一个正则表达式，可以指定是否支持扩展语法，并给出正则表达式语法树或错误信息
  RegularExpressionToString					：把正则表达式语法书转换为字符串
  RegularExpressionCharsetNormalize			：规格化字符集
正则表达式语法：
	语法说明：
	  1：字符集
		[A1[-A2]A3[-A4]]，					例如[a-zA-Z_]，本例代表字母和下划线
		[^A1[-A2]A3[-A4]]，					例如[^a-zA-Z_]，本例代表除字母和下划线外的字符
		A1,\A1，							例如a代表字符a，\d代表[0-9]
	  2：转义字符：
		\^									^
		\$									$

		\{									{
		\}									}
		\[									[
		\]									]
		\(									(
		\)									)
		\?									?
		\+									+
		\-									-
		\*									*
		\|									|
		\\									\
		\<									<
		\>									>

		\n									#10
		\r									#13
		\t									Tab

		\d									[0-9]			【不可用在[]内】
		\D									[^0-9]			【不可用在[]内】
		\s									[ \n\r\t]		【不可用在[]内】
		\S									[^ \n\r\t]		【不可用在[]内】
		\w									[0-9a-zA-Z_]	【不可用在[]内】
		\W									[^0-9a-zA-Z_]	【不可用在[]内】
		\l									[a-zA-Z]		【不可用在[]内】
		\l									[^a-zA-Z]		【不可用在[]内】

		\.									任意字符		【不可用在[]内】
		\xFFFF								Unicode字符

		^						【扩展语法】字符串开始		【不可用在[]内】
		$						【扩展语法】字符串结尾		【不可用在[]内】
	  3：循环
		{m}									循环m次
		{m,}								循环m至无穷多次
		{m,n}								循环m至n次
		*									{0,}
		+									{1,}
		?									{0,1}
								【扩展语法】在循环标志后加"?"代表非贪婪循环
											譬如"ABCDE"匹配\w+\w+得到"ABCD","E"，但是匹配\w+?\w+则得到"A","BCDE"
	  4：功能
	    a|b									匹配a或者b的其中一个
		(?Pattern)				【扩展语法】匹配后获取
		(?:Pattern)							匹配后不获取
		(?=Pattern)				【扩展语法】正向预查，Pattern不能包含扩展语法
		(?!Pattern)				【扩展语法】负向预查，Pattern不能包含扩展语法
		其中Pattern可以是
		  表达式							普通表达式
		  <name>表达式						为表达式命名
		  <name>							如果name曾经被用来命名，则取命名的表达式，【不可递归】
		  <#name>表达式			【扩展语法】匹配后存放进name号存储区
		  <$name>				【扩展语法】匹配字符串是否等于name号存储区的某个记录，如果打开StorageAutoReplace则存储区只存放一个记录
*******************************************************************************/

#ifndef VL_REGEXP
#define VL_REGEXP

#include "..\Data\VL_Data_Basic.h"
#include "..\Data\VL_Data_String.h"
#include "..\Data\VL_Data_List.h"

namespace vl
{
	namespace grammar
	{
		using namespace collection;

		typedef VL_Fullpair<VWChar , VWChar>			VL_RegCharRange;
		typedef VL_OrderedList<VL_RegCharRange , true>	VL_RegCharSet;

/*********************************************************************************************************
访问器
*********************************************************************************************************/

		class VL_RegExpCharset;
		class VL_RegExpLoop;
		class VL_RegExpFunc;
		class VL_RegExpSection;

		class IVL_RegExpRecursiveAlgorithm : public IVL_Interface
		{
		public:
			virtual void		VisitBegin	(VL_RegExpCharset*	Node , VBool& VisitChildren , VBool& RunEnd)=0;
			virtual void		VisitBegin	(VL_RegExpLoop*		Node , VBool& VisitChildren , VBool& RunEnd)=0;
			virtual void		VisitBegin	(VL_RegExpFunc*		Node , VBool& VisitChildren , VBool& RunEnd)=0;
			virtual void		VisitBegin	(VL_RegExpSection*	Node , VBool& VisitChildren , VBool& RunEnd)=0;
			virtual void		VisitEnd	(VL_RegExpCharset*	Node)=0;
			virtual void		VisitEnd	(VL_RegExpLoop*		Node)=0;
			virtual void		VisitEnd	(VL_RegExpFunc*		Node)=0;
			virtual void		VisitEnd	(VL_RegExpSection*	Node)=0;
		};

/*********************************************************************************************************
语法树
*********************************************************************************************************/

		class VL_RegExpBase : public VL_Base
		{
		public:
			VL_RegExpBase();

			virtual void		Visit(IVL_RegExpRecursiveAlgorithm* Algorithm)=0;
		};
		typedef VL_AutoPtr<VL_RegExpBase>						VL_RegExpPtr;
		typedef VL_List<VL_RegExpPtr , false , VL_RegExpBase*>	VL_RegExpList;

		enum VLE_RegExpCharset
		{
			vrecCharSet,						/*匹配字符集*/
			vrecStringBegin,					/*匹配字符串起始位置*/
			vrecStringEnd						/*匹配字符串终结位置*/
		};
		class VL_RegExpCharset : public VL_RegExpBase
		{
		public:
			VL_RegCharSet		CharSet;		/*字符集*/
			VBool				Reverse;		/*是否取余集*/
			VLE_RegExpCharset	Type;			/*类型*/

			VL_RegExpCharset();

			void				Visit(IVL_RegExpRecursiveAlgorithm* Algorithm);
		};

		class VL_RegExpLoop : public VL_RegExpBase
		{
		public:
			VL_RegExpPtr		Expression;		/*循环体*/
			VInt				Begin;			/*最少循环次数*/
			VInt				End;			/*最多循环次数*/
			VBool				Finite;			/*是否无限循环*/
			VBool				Short;			/*是否短次数优先*/

			VL_RegExpLoop();

			void				Visit(IVL_RegExpRecursiveAlgorithm* Algorithm);
		};

		enum VLE_RegExpFunc
		{
			vrefCatch,							/*获取*/
			vrefUncatch,						/*不获取*/
			vrefPositive,						/*正向预查*/
			vrefNegative						/*反向预查*/
		};
		enum VLE_RegExpContent
		{
			vrecNamed,							/*命名表达式*/
			vrecStorage,						/*指定管道存储*/
			vrecMatch,							/*指定管道匹配*/
			vrecNormal							/*普通*/
		};
		class VL_RegExpFunc : public VL_RegExpBase
		{
		public:
			VL_RegExpPtr		Expression;		/*子表达式*/
			VL_RegExpBase*		ReferenceSource;/*被引用目标*/
			VLE_RegExpFunc		Function;		/*附加功能*/
			VLE_RegExpContent	Content;		/*附加属性*/
			VUnicodeString		Pattern;		/*模式名称*/

			VL_RegExpFunc();

			void				Visit(IVL_RegExpRecursiveAlgorithm* Algorithm);
		};

		enum VLE_RegExpSection
		{
			vresSequence,						/*序列*/
			vresBranch							/*分析*/
		};
		class VL_RegExpSection : public VL_RegExpBase
		{
		public:
			VLE_RegExpSection	Type;			/*子表达式组织方式*/
			VL_RegExpList		Sections;		/*子表达式*/

			VL_RegExpSection();

			void				Visit(IVL_RegExpRecursiveAlgorithm* Algorithm);
		};

/*********************************************************************************************************
工具
*********************************************************************************************************/

		class VL_RegExpResult : public VL_Base
		{
		public:
			VL_RegExpPtr		Expression;		/*表达式*/
			VInt				ErrorPosition;	/*错误位置*/
			VUnicodeString		ErrorMessage;	/*错误信息*/
			VBool				Error;			/*表达式是否有错误*/

			VL_RegExpResult();
		};

		class VL_RegExpCharClass : public VL_Base
		{
		public:
			VInt				CharClass[65536];	/*每一个Char对应的Class，-1为其它类型*/
			VL_RegCharSet		CharRange;			/*每一个Class对应的范围*/
		};

		extern VUnicodeString	CharRangeToString(VL_RegCharRange Range);
		extern VUnicodeString	CharSetToString(VL_RegCharSet& CharSet);
		extern VL_RegExpResult	RegularExpressionAnalysis(VUnicodeString Expression , VBool AllowExtension);
		extern VUnicodeString	RegularExpressionToString(VL_RegExpPtr Expression);
		extern void				RegularExpressionCharsetNormalize(VL_RegExpPtr Expression , VL_RegExpCharClass& CharClass);
	}
}

#endif