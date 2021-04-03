﻿// Приведенный ниже блок ifdef — это стандартный метод создания макросов, упрощающий процедуру
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа OS11HTAPI_EXPORTS
// Символ, определенный в командной строке. Этот символ не должен быть определен в каком-либо проекте,
// использующем данную DLL. Благодаря этому любой другой проект, исходные файлы которого включают данный файл, видит
// функции OS11HTAPI_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
#ifdef OS11HTAPI_EXPORTS
#define OS11HTAPI_API __declspec(dllexport)
#else
#define OS11HTAPI_API __declspec(dllimport)
#endif

// Этот класс экспортирован из библиотеки DLL
class OS11HTAPI_API COS11HTAPI {
public:
	COS11HTAPI(void);
	// TODO: добавьте сюда свои методы.
};

extern OS11HTAPI_API int nOS11HTAPI;

OS11HTAPI_API int fnOS11HTAPI(void);
