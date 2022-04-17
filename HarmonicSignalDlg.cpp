#pragma once

#include "pch.h"
#include "framework.h"
#include "HarmonicSignal.h"
#include "HarmonicSignalDlg.h"
#include "afxdialogex.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DOTSPICTURE(x,y) (xpPicture*((x)-xminPicture)),(ypPicture*((y)-ymaxPicture)) 

// Диалоговое окно CHarmonicSignalDlg

CHarmonicSignalDlg::CHarmonicSignalDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HARMONICSIGNAL_DIALOG, pParent)
	, value_ampl(1)
	, value_phase(2. * M_PI)
	, value_discr_freq(44100)
	, value_length_sign(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHarmonicSignalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GET, button_get);
	DDX_Control(pDX, IDC_CLEAR, button_clear);
	DDX_Text(pDX, IDC_AMPL, value_ampl);
	DDX_Text(pDX, IDC_PHASE, value_phase);
	DDX_Text(pDX, IDC_FREQ_DISCRET, value_discr_freq);
	DDX_Text(pDX, IDC_DLIT_SIGN, value_length_sign);
	DDX_Control(pDX, IDC_AMPL, ctrl_ampl);
	DDX_Control(pDX, IDC_PHASE, ctrl_phase);
	DDX_Control(pDX, IDC_FREQ_DISCRET, ctrl_freq);
	DDX_Control(pDX, IDC_QPSK, button_qpsk);
}

BEGIN_MESSAGE_MAP(CHarmonicSignalDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_GET, &CHarmonicSignalDlg::OnBnClickedGet)
	ON_BN_CLICKED(IDC_CLEAR, &CHarmonicSignalDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_QPSK, &CHarmonicSignalDlg::OnBnClickedQpsk)
END_MESSAGE_MAP()

// Обработчики сообщений CHarmonicSignalDlg

BOOL CHarmonicSignalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	//для  картинки
	PicWndPicture = GetDlgItem(IDC_SIGNAL);
	PicDcPicture = PicWndPicture->GetDC();
	PicWndPicture->GetClientRect(&PicPicture);

	this->SetBackgroundColor(RGB(200, 190, 250));

	button_get.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;//required for flatering and use bg color
	button_get.m_bTransparent = false;//reg for use bg color
	button_get.SetFaceColor(RGB(128, 0, 255), true);
	button_get.SetTextColor(RGB(255, 255, 255));

	button_clear.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;//required for flatering and use bg color
	button_clear.m_bTransparent = false;//reg for use bg color
	button_clear.SetFaceColor(RGB(128, 0, 255), true);
	button_clear.SetTextColor(RGB(255, 255, 255));

	button_qpsk.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;//required for flatering and use bg color
	button_qpsk.m_bTransparent = false;//reg for use bg color
	button_qpsk.SetFaceColor(RGB(128, 0, 255), true);
	button_qpsk.SetTextColor(RGB(255, 255, 255));

	DrawSignal(sgn, PicDcPicture, PicPicture);

	UpdateData(FALSE);
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CHarmonicSignalDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CHarmonicSignalDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CHarmonicSignalDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Измените любые атрибуты DC
	if (CTLCOLOR_EDIT == nCtlColor)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkColor(RGB(128, 0, 255));
	}

	// TODO:  Вернуть другое значение дескриптора кисти, если оно не определено по умолчанию
	return hbr;
}

void CHarmonicSignalDlg::Mashtab(Signal sgn, double* mmin, double* mmax)
{
	*mmin = *mmax = sgn.signal[0];

	for (int i = 0; i < sgn.signal.size(); i++)
	{
		if (*mmin > sgn.signal[i]) *mmin = sgn.signal[i];
		if (*mmax < sgn.signal[i]) *mmax = sgn.signal[i];
	}
}

void CHarmonicSignalDlg::DrawSignal(Signal sgn, CDC* WinDc, CRect WinxmaxGraphc)
{
	double GU_X = 10;
	double GU_Y = 10;

	//ГРАФИК СИГНАЛА
	if (!sgn.signal.empty())
	{
		double min = 0;
		double max = 0;
		Mashtab(sgn, &min, &max);

		GU_X = sgn.signal.size();
		GU_Y = (max - min);
	}

	xminPicture = -GU_X * 0.01;
	xmaxPicture = GU_X * 1.01;
	yminPicture = -GU_Y * 0.51;			//минимальное значение y
	ymaxPicture = GU_Y * 0.51;

	// создание контекста устройства
	CBitmap bmp;
	CDC* MemDc;
	MemDc = new CDC;
	MemDc->CreateCompatibleDC(WinDc);

	double widthX = WinxmaxGraphc.Width();
	double heightY = WinxmaxGraphc.Height();
	xpPicture = (widthX / (xmaxPicture - xminPicture));			//Коэффициенты пересчёта координат по Х
	ypPicture = -(heightY / (ymaxPicture - yminPicture));			//Коэффициенты пересчёта координат по У

	bmp.CreateCompatibleBitmap(WinDc, widthX, heightY);
	CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);

	// заливка фона графика белым цветом
	MemDc->FillSolidRect(WinxmaxGraphc, RGB(200, 190, 250));

	if (!sgn.signal.empty())
	{
		CPen circle;
		circle.CreatePen(PS_SOLID, 5, RGB(128, 0, 255));
		MemDc->SelectObject(&circle);
		MemDc->MoveTo(DOTSPICTURE(0, sgn.signal[0]));
		for (int i = 1; i < sgn.signal.size(); i++)
		{
			double xxi = i;
			double yyi = sgn.signal[i];

			MemDc->LineTo(DOTSPICTURE(xxi, yyi));
			
			/*CRect rect(DOTSPICTURE(xxi, yyi + 1), DOTSPICTURE(xxi + 1, yyi));
			LPCRECT lpRect = rect;
			CBrush circle(RGB(0, 255, 255));
			MemDc->FillRect(lpRect, &circle);*/
		}
	}

	// вывод на экран
	WinDc->BitBlt(0, 0, widthX, heightY, MemDc, 0, 0, SRCCOPY);
	delete MemDc;
}

void CHarmonicSignalDlg::GenerateSignal(double ampl, double start_phase, double sampling_freq, int duration) {
	int length_of_signal = round(sampling_freq * (double)duration) + sgn.t;
	// Частота сигнала, Гц.
	double freq = 1.5;
	// Коэффициент увеличения частоты.
	double factor = 3.e-1;

	sgn.signal.clear();
	for (int t = sgn.t; t < length_of_signal; ++t) {
		// Вычисление частоты.
		double w = 2. * M_PI * freq;
		// Добавка, зависящая от времени.
		w += factor * t;

		sgn.signal.push_back(ampl * sin(w * t / sampling_freq + start_phase));
	}
	sgn.t = length_of_signal - 1;
}

void CHarmonicSignalDlg::OnBnClickedGet()
{
	UpdateData(TRUE);
	sgn.signal.clear();

	// Генерация сигнала.
	GenerateSignal(value_ampl, value_phase, value_discr_freq, value_length_sign);

	// Отрисовка сигнала.
	DrawSignal(sgn, PicDcPicture, PicPicture);

	// Блокировка полей изменения параметров.
	// Т.к. новый сигнал - продолжение.
	ctrl_ampl.EnableWindow(false);
	ctrl_phase.EnableWindow(false);
	ctrl_freq.EnableWindow(false);
	
	// Создание объекта для сохранения в wav
	// Пример https://github.com/adamstark/AudioFile
	AudioFile<double>::AudioBuffer buffer;
	// 2 канала
	buffer.resize(2);

	buffer[0].resize(sgn.signal.size());
	buffer[1].resize(sgn.signal.size());

	int num_channels = 2;
	int num_samples_per_channel = sgn.signal.size();
	for (int i = 0; i < sgn.signal.size(); i++) {
		for (int ch = 0; ch < num_channels; ch++) {
			buffer[ch][i] = sgn.signal[i] * 0.5;
		}
	}

	bool is_ok = audio_file.setAudioBuffer(buffer);
	if (is_ok) {
		audio_file.save("audio-file.wav");
	}
}

void CHarmonicSignalDlg::OnBnClickedClear()
{
	sgn.signal.clear();
	sgn.t = 0;

	DrawSignal(sgn, PicDcPicture, PicPicture);

	// Разблокировка полей изменения параметров.
	// Т.к. после очистки рассматривается новый сигнал.
	ctrl_ampl.EnableWindow(true);
	ctrl_phase.EnableWindow(true);
	ctrl_freq.EnableWindow(true);
}

void CHarmonicSignalDlg::OnBnClickedQpsk()
{
	UpdateData(TRUE);
}
