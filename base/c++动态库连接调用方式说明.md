# c++动态库连接调用方式说明

函数调用方式：`cdecl，stdcall，fastcall，vectorcall，thiscall`几种，比较常用的是`cdecl`和`stdcall`。

通常在动态库导出时，我们需要指定调用方式，特别是在跨语言协作时，否则会出现堆栈对不齐等一些列问题。

通常在c或者c++中，我们可以通过头文件的方式找到函数名，直接连接即可。但其他语言，一般通过动态库名字查找（`e.g. Windows: LoadLibrary,GetProcAddress, Linux: dllopen`）的方式找到函数地址；

动态库函数声明导出：

```c
// windows
#define DEMO_API __declspec(dllexport)
#define CALL_TYPE __stdcall
//#define CALL_TYPE __cdecl

// 通常提供给其他语言使用的都是c语言接口，所以使用extern "C"修饰；
extern "C" {
	DEMO_API void CALL_TYPE MyFunction();
}


```



以下是关于几种调用方式的简要说明：

- `cdecl`

  - 导出的函数名不会经过额外的修饰，可通过`GetProcAddress`直接查询到函数地址；
  - 

- `stdcall`

  - 导出的函数会经过修饰：比如参数的占用空间大小（e.g. 修饰后的`_MyFunction@0`），这时`GetProcAddress`不能够直接查找到函数地址；

  - 这种不能查询到函数地址的情况，我们可以通过2种方式解决；

    - 定义def导出文件，并在`msvc`中：Linker->Input->Module Definition File填入导出文件；

      ```def
      EXPORTS
          MyFunction
      ```

      

    - 在函数实现文件中增加：`#pragma comment(linker, "/EXPORT:MyFunction=_MyFunction@0")`

