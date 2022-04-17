
// HarmonicSignal.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CHarmonicSignalApp:
// Сведения о реализации этого класса: HarmonicSignal.cpp
//

class CHarmonicSignalApp : public CWinApp
{
public:
	CHarmonicSignalApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CHarmonicSignalApp theApp;
