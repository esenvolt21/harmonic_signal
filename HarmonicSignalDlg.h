#pragma once
#define _USE_MATH_DEFINES
#include <vector>
#include <math.h>
#include "AudioFile/AudioFile.h"

using namespace std;

struct Signal
{
	// Буфер для хранения куска сигнала.
	vector<double> signal;
	// Время последнего временного отсчёта.
	int t = 0;
};

// Диалоговое окно CHarmonicSignalDlg
class CHarmonicSignalDlg : public CDialogEx
{
// Создание
public:
	CHarmonicSignalDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HARMONICSIGNAL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//Oбласти рисования.
	CWnd* PicWndPicture;
	CDC* PicDcPicture;
	CRect PicPicture;

	//Переменные для рисования.
	double xpPicture = 0, ypPicture = 0,			//коэфициенты пересчета
		xminPicture = -1, xmaxPicture = 1,			//максисимальное и минимальное значение х 
		yminPicture = -0.5, ymaxPicture = 5;			//максисимальное и минимальное значение y

public:
	// Переменные интерфейса.
	CMFCButton button_get;
	CMFCButton button_clear;
	CMFCButton button_qpsk;
	double value_ampl;
	double value_phase;
	double value_discr_freq;
	int value_length_sign;
	CEdit ctrl_ampl;
	CEdit ctrl_phase;
	CEdit ctrl_freq;

	// Объект для работы с wav
	AudioFile<double> audio_file;

	// Обработчики событий.
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedGet();
	afx_msg void OnBnClickedClear();

	// Глобальные переменные.
	Signal sgn;

	// Функции.
	void Mashtab(Signal sgn, double* mmin, double* mmax);
	void DrawSignal(Signal sgn, CDC* WinDc, CRect WinxmaxGraphc);
	void GenerateSignal(double ampl, double start_phase, double sampling_freq, int duration);
	afx_msg void OnBnClickedQpsk();
};
