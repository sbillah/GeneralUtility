﻿========================================================================
    Introduction
========================================================================
 本程序示例如何根据用户输入的流程图，实现自动规划运行路径，以及输出结果。
 着重于模块化，用户可以以plugin的方式加入新的算法或者数据类型，不需要修改核心规划模块

========================================================================
    Todo
========================================================================

1.(-->Done) Implement short buildRunSeq(std::vector< RunItem> &algSeqs) function

实现方式：
每个alg pin有个状态iavalid来表示是否已经有数据了
用一颗graph表示不同算法之间的依赖关系
用一个数组保存现在可以执行的任务,当一个任务完成后，检查他的后续任务是否可以开始。如果可以，就放入该数组

2. (-->Done) Support to warp a Flow to an algorithm
参考7
-->Done

3. (-->Done) Support serialization

5. (-->Done) 用一个统一的内存分配器，以便容易处理内存泄露
-->Done

6. 添加logging以便出了问题容易调试

7. (-->Done) 检测用户的链接有无错误
有个简单的方式就是通过强制设置一个start，和end节点，要求一定要开始于start，而结束于end
这样貌似都会很容易实现2）warp a Flow to an algorithm
如果能允许用户设置start，end输入输出的参数个数和类型，那么用户可以任意实现了
-->Done

8. (-->Done) 允许用户动态设置某些算法的节点类型。在配置阶段，而不是运行阶段
-->Done

俺可以设置一个规则： 允许同时有start和out两种节点，有start就说明该算法需要输入额外的用户参数，而有end说明需要有输出参数

9.(-->Done) 加速！输出和下一阶段的输入采用同一变量，避免拷贝代价
Done

10. (-->Done) 允许用户指定输入端和输出端，这样可以取消start，和end节点

11. (-->Done) Fix "Debug Assertion Failed! Expression: _pFirstBlock == pHead"
by c++-->代码生成 -->多线程调试dll运行库
参考 http://stackoverflow.com/questions/18882760/debug-assertion-failed-expression-pfirstblock-phead
/////////////////////////////////////////////////////////////////////////////
