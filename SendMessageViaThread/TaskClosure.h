// Author: WangZhan -> wangzhan.1985@gmail.com
#pragma once
#include <functional>
#include <memory>
#include <windows.h>
#include "iostream"

#define WM_POST_TASK_CLOSURE	WM_USER + 2003
#define WM_SEND_TASK_CLOSURE	WM_USER + 2007

typedef std::function<void(void)> PostedTask;
typedef std::function<void(void)> SentTask;

// ex: void class::func(...)
#define PostTask(wnd, func, ...) \
	PostTaskInner(wnd, func, this, __VA_ARGS__)

#define PostTaskInner(wnd, func, ...) { \
	if (GetCurrentThreadId() != GetWindowThreadProcessId(wnd, NULL)) { \
	std::function<void(void)> *pFunc = new std::function<void(void)>(std::bind(func, __VA_ARGS__)); \
	if (!::PostMessage(wnd, WM_POST_TASK_CLOSURE, 0, (LPARAM)pFunc)) { \
	delete pFunc; \
	} \
	return;\
	} \
}

// ex: void class::func(...)
#define SendNotifyTask(wnd, func, ...) \
	SendNotifyTaskInner(wnd, func, this, __VA_ARGS__)

#define SendNotifyTaskInner(wnd, func, ...) { \
	if (GetCurrentThreadId() != GetWindowThreadProcessId(wnd, NULL)) { \
		std::function<void(void)> *pFunc = new std::function<void(void)>(std::bind(&func, __VA_ARGS__)); \
		if (!::SendNotifyMessage(wnd, WM_POST_TASK_CLOSURE, 0, (LPARAM)pFunc)) { \
			delete pFunc; \
		} \
		return;\
	} \
}

// 函数返回类型萃取
struct FunctionReturnTypeTraits
{
	template <class T> operator const T () { return T(); }
};

template<typename retType>
std::function<retType(void)> FunctorWithReturnType(retType r)
{
	return std::function<retType(void)>();
}

template <typename retType>
struct FuncWrapper
{
	FuncWrapper(const std::function<retType(void)> &func)
	{
		function = func;
	}

	void Run() { retValue = function(); }

	retType GetValue() { return retValue; }
	
	std::function<retType(void)> function;
	retType retValue;
};

template <typename retType>
struct FunctorWithRetTypeContainer
{
	FunctorWithRetTypeContainer(HWND wnd, const std::function<retType(void)> &func)
	{
		pFuncWrapper = new FuncWrapper<retType>(func);
		SendMessage(wnd, WM_SEND_TASK_CLOSURE, 0, (LPARAM)pFuncWrapper);
	}

	retType GetValue() { pFuncWrapper->GetValue(); delete pFuncWrapper; };

	FuncWrapper<retType> *pFuncWrapper;
};

template <typename retType>
retType ExecuteSendMessage(HWND wnd, std::function<retType(void)> func)
{
	return FunctorWithRetTypeContainer<retType>(wnd, func).GetValue();
}

// ex: int class::func(...)
#define SendTaskAndReturn(wnd, func, ...) \
	SendTaskAndReturnInner(wnd, this, func, __VA_ARGS__)

#define SendTaskAndReturnInner(wnd, pThis, func, ...) \
	if (GetCurrentThreadId() != GetWindowThreadProcessId(wnd, NULL)) { \
	auto functor = FunctorWithReturnType(0 ? func(__VA_ARGS__) : FunctionReturnTypeTraits()); \
	functor = std::bind(&func, pThis, __VA_ARGS__); \
	return ExecuteSendMessage(wnd, functor); \
	}

#define SendTaskAndReturn2(wnd, func, ...) \
	SendTaskAndReturnInner2(wnd, this, func, __VA_ARGS__)

#define SendTaskAndReturnInner2(wnd, pThis, func, ...) \
	if (GetCurrentThreadId() != GetWindowThreadProcessId(wnd, NULL)) { \
	std::function<decltype((pThis->func)(__VA_ARGS__))(void)> functor = std::bind(&func, pThis, __VA_ARGS__); \
	return ExecuteSendMessage(wnd, functor); \
	}

#define SendTaskAndReturn3(wnd, func, ...) \
	SendTaskAndReturnInner3(wnd, this, func, __VA_ARGS__)

#define SendTaskAndReturnInner3(wnd, pThis, func, ...) \
	std::function<decltype(pThis->a)(void)> functor = std::bind(&func, pThis, __VA_ARGS__); \
	ExecuteSendMessage(wnd, functor);

#define SendTaskAndReturn4(wnd, func, ...) \
	SendTaskAndReturnInner4(wnd, this, func, __VA_ARGS__)

#define SendTaskAndReturnInner4(wnd, pThis, func, ...) \
	if (GetCurrentThreadId() != GetWindowThreadProcessId(wnd, NULL)) { \
	std::function<decltype((pThis->*func)(__VA_ARGS__))(void)> functor = std::bind(func, pThis, __VA_ARGS__); \
	decltype((pThis->*func)(__VA_ARGS__)) ret; \
	PostedTask functor2([&functor, &ret](){ ret = functor();}); \
	::SendMessage(wnd, WM_SEND_TASK_CLOSURE, 0, (LPARAM)&functor2); \
	return ret; \
}

//template<typename... Args>
//void TestEx(Args... args)
//{
//
//}

//template<typename RetType, typename FuncType, typename ThisType, typename... Args>
//RetType SendTaskAndReturnEx(HANDLE wnd, FuncType func, ThisType pThis, Args... args)
//{
//	RetType ret;
//	if (GetCurrentThreadId() == GetWindowThreadProcessId(wnd, NULL))
//	{
//		ret = (pThis->func)(args...);
//	}
//	else
//	{
//		PostedTask task([&](){ ret = (pThis->func)(args...); });
//		SendMessage(wnd, WM_SEND_TASK_CLOSURE, 0, (LPARAM)&task);
//	}
//	return ret;
//}

//template<typename FuncType, typename ThisType, typename... Args>
//auto SendTaskAndReturnEx2(HANDLE wnd, FuncType func, ThisType pThis, Args... args) -> decltype((pThis->func)(args...))
//{
//	decltype((pThis->func)(args...)) ret;
//	if (GetCurrentThreadId() == GetWindowThreadProcessId(wnd, NULL))
//	{
//		ret = (pThis->func)(args...);
//	}
//	else
//	{
//		PostedTask task([&](){ ret = (pThis->func)(args...); });
//		SendMessage(wnd, WM_SEND_TASK_CLOSURE, 0, (LPARAM)&task);
//	}
//	return ret;
//}

template<typename FuncType, typename ThisType, typename Arg1, typename Arg2>
auto SendTaskAndReturnEx3(HWND wnd, FuncType func, ThisType pThis, Arg1 arg1, Arg2 arg2) -> decltype((pThis->*func)(arg1, arg2))
{
	decltype((pThis->*func)(arg1, arg2)) ret;
	if (GetCurrentThreadId() == GetWindowThreadProcessId(wnd, NULL))
	{
		ret = (pThis->*func)(arg1, arg2);
	}
	else
	{
		PostedTask task([&](){ ret = (pThis->*func)(arg1, arg2); });
		SendMessage(wnd, WM_SEND_TASK_CLOSURE, 0, (LPARAM)&task);
	}
	return ret;
}

template<typename Arg1, typename Arg2>
auto Mx(Arg1 arg1, Arg2 arg2) -> decltype(arg1*arg2)
{
	return arg1 * arg2;
}

class TaskClosure
{
public:
	TaskClosure(void) 
	{
		int i = 10;
	}

	~TaskClosure(void)
	{

	}

	void f(int a, double b) 
	{
		std::cout << "f1" << std::endl;
	}

	int f2(int a, double b) 
	{ 
		std::cout << "f2" <<std::endl;
		return 0; 
	}

	void Test()
	{
		SendNotifyTask(0, TaskClosure::f, 1, 2.0);
		//std::function<void(void)> *pFunc = new std::function<void(void)>(std::bind(&TaskClosure::f, this, 1, 1.0));
		//std::function<void(void)> Func = std::bind(&TaskClosure::f, this, 1, 1.0);
		//std::result_of<TaskClosure::f2(int, double)>::type i = 0;
		//0 ? f2(1, 2) : RTT();
		//extractReturnType(&TaskClosure::f2, (0 ? f2(1, 2.0) : RTT()), this, 1, 2.0);
		//extractReturnType2(&TaskClosure::f2, 1, 2.0);

		//decltype(TaskClosure::f2(1, 2.0)) i = 0;
		//decltype(f2(int, double)) j = 0;

		//auto kaka = extractReturnType(0 ? f2(1, 2.0) : RTT());
		//kaka = std::bind(&TaskClosure::f2, this, 1, 2.0);

		//int a = kaka();

		//0 ? f2(1, 2) : FunctionReturnTypeTraits();

		//FunctorWithReturnType(0 ?  f2(1, 2) : FunctionReturnTypeTraits());
		//SendTaskAndReturn(0, TaskClosure::f2, 1, 2.0);
		//SendTaskAndReturnEx(0, TaskClosure::f2, 1, 2.0);

		//auto ii = std::bind(iff, 1);
		//std::function<int(int)> ii = std::bind(iff, 1);
		//ffff(ii);
	}

	int Test2()
	{
		SendTaskAndReturn(0, TaskClosure::f2, 1, 2.0);
		SendTaskAndReturn2(0, TaskClosure::f2, 1, 2.0);
		SendTaskAndReturn4(0, &TaskClosure::f2, 1, 2.0);
		SendTaskAndReturnEx3(0, &TaskClosure::f2, this, 1, 2.0);

		Mx(1, 2);

		return 1;
	}

	int a;
};

typedef int(TaskClosure::*xx)(int, double);

template <typename T, typename U>
auto xs1 (T t, U u) -> decltype(t*u)
{
	return t*u;
}

template <typename T, typename U>
decltype(*(T*)0**(U*)0) xs2 (T t, U u)
{
	return t*u;
}

template <typename T, typename U>
decltype(std::declval<T>()*std::declval<U>()) xs3 (T t, U u)
{
	return t*u;
}


//void Test2_1()
//{
//	TaskClosure tc;
//	//std::result_of<xx>::type i = 0;
//	decltype(tc.a) i = 10;
//
//	int j = 10;
//	int k = 5;
//	decltype(&j) kaka = &k;
//	decltype((j)) k2 = k;
//
//	//xs1(1, 2);
//	//xs2(1, 2);
//	//xs3(1, 2);
//}
