#ifndef _CMANAGER_GUI_H_
#define _CMANAGER_GUI_H_

#include "DXData.h"
#include "font.h"
#include <vector>

using namespace std;



#define  D3DFVF_GUI (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
struct stVertexGUI
{
	D3DXVECTOR3 vectorPos;
	DWORD color;
	float tu, tv;
};

// -------------- как выделять ---------
#define TYPE_SELECTION_COLOR_FONT 1
#define TYPE_SELECTION_SIZE_FONT  2
#define TYPE_SELECTION_FRAME      3

;
// ====================== structures (begin) ==================================================
enum enTypeElemGui {NOT_ELEM = -1, BUTTON, CHECK_BOX, STATIC_TEXT, IMAGE_BOX, MOUSE, GROUP, HSCROLL_BAR, VSCROLL_BAR, WINDOW_PANEL}; // перечисление всех видов элементов
enum enTypeWindowFill {FILL_NO = 0, FILL_REPEAT, FILL_SCALE}; /* Как заливать поверхность окна фоновым 
															  изображением (не заливать, заливать с повтором, отмаштабировать под размеры окна)*/
enum enElemGuiState {ELEM_STATE_NOT = 0, ELEM_STATE_NORMAL, ELEM_STATE_SELECTED, ELEM_STATE_ON, ELEM_STATE_OFF}; // в каком состоянии находится объект пока только для окон

struct stMouseInfoGUI
{
	stMouseInfoGUI()
	{
		//bDownLB = false;
		bLButtonClick = false;
		bLButtonDown = false;
	}
	POINT pointMousePos;  // позиция мыши

	//bool bDownLB;
	bool bLButtonClick; // был ли клик левой клавишей
	bool bLButtonDown; // левая клавиша зажата
	POINT pointLBDownPos;   // где была нажата левая клавиша
	POINT pointLBUpPos;	 // где была отпущена левая клавиша
};

struct stButton
{
	stButton()
	{
		textureButton = NULL;
		textureSelectionButton = NULL;
		pTextureRender = NULL;

		bEnable = true;
		bVisible = true;
		bSelectEnable = true;
		
		vectorScale = D3DXVECTOR2(1.0f, 1.0f);
		enState = ELEM_STATE_NOT;
	};

	LPDIRECT3DTEXTURE9 *pTextureRender; // адрес текстуры которую необходимо вывести
	LPDIRECT3DTEXTURE9 textureButton; // готовая текстура кнопки
	LPDIRECT3DTEXTURE9 textureSelectionButton; // готовая текстура кнопки с наложеным выделением

	char *pcButtonName; // имя кнопки
	D3DXVECTOR2 vectorTrans;
	D3DXVECTOR2 vectorScale;
	RECT rectButton; // прямоугольник в который заключёна кнопка, например для нажатий мышкой
	RECT rectForWindowMouse; // для нажатия мышкой в окне
	int iIDWindow; // индекс окна которому принадлежит кнопка

	bool bSelectEnable; // разрешить выделение
	bool bEnable;   // доступность
	bool bVisible;  // видимость
	enElemGuiState enState;

	void (*pFuncExecute)(); // указатель на функцию которая выполнится при нажатии
};

struct stCheckBox
{
	stCheckBox()
	{
		textureCheckBoxOFF = NULL;
		textureCheckBoxON = NULL;
		pTextureRender = NULL;

		bCheckBoxState = false;
		enState = ELEM_STATE_OFF;
		bEnable = true;
		bVisible = true;
		bRunFunc = true;
	};

	LPDIRECT3DTEXTURE9 *pTextureRender; // адрес текстуры которую необходимо вывести
	LPDIRECT3DTEXTURE9 textureCheckBoxOFF; // готовая текстура checkBox = false
	LPDIRECT3DTEXTURE9 textureCheckBoxON; // готовая текстура checkBox = true

	char *pcCheckBoxName; // имя checkBox
	D3DXVECTOR2 vectorTrans;
	RECT rectCheckBox; // прямоугольник в который заключён checkBox, например для нажатий мышкой
	RECT rectForWindowMouse; // для нажатия мышкой в окне
	int iIDWindow; // индекс окна которому принадлежит checkBox
	enElemGuiState enState;

	bool bCheckBoxState; // хранит состояние в котором находится (ON, OFF)
	bool bEnable;   // доступность
	bool bVisible;  // видимость
	bool bRunFunc; // запустить функции на выполнение

	void (*pFuncExecute)(bool bCheckBoxOn); // указатель на функцию которая выполнится при нажатии, в bCheckBoxOn запишет результат состояния
};

struct stRadioButton
{
	stRadioButton()
	{
		textureRadioButtonOFF = NULL;
		textureRadioButtonON = NULL;
		pTextureRender = NULL;

		bRadioButtonState = false;
		enState = ELEM_STATE_OFF;
		bEnable = true;
		bVisible = true;

		vectorScale = D3DXVECTOR2(1.0f, 1.0f);
	};

	LPDIRECT3DTEXTURE9 *pTextureRender; // адрес текстуры которую необходимо вывести
	LPDIRECT3DTEXTURE9 textureRadioButtonOFF; // готовая текстура checkBox = false
	LPDIRECT3DTEXTURE9 textureRadioButtonON; // готовая текстура checkBox = true

	char *pcRadioButtonName; // имя RadioButton
	D3DXVECTOR2 vectorTrans;
	D3DXVECTOR2 vectorScale;
	RECT rectRadioButton; // прямоугольник в который заключён RadioButton, например для нажатий мышкой
	RECT rectForWindowMouse; // для нажатия мышкой в окне
	int iIDWindow; // индекс окна которому принадлежит RadioButton

	enElemGuiState enState;
	bool bRadioButtonState; // хранит состояние в котором находится (ON, OFF)
	bool bEnable;   // доступность
	bool bVisible;  // видимость

	void (*pFuncExecute)(bool bRadioButtonState); // указатель на функцию которая выполнится при нажатии, в bRadioButtonState запишет результат состояния
};

struct stGroup
{
	stGroup()
	{
		m_iQuantRadioButton = 0;
		iCurIndexRadioButton = 0;
		bRunRadioButtonFunc = true;
	};

	int m_iQuantRadioButton;
	bool bRunRadioButtonFunc; // запустить функцию RadioButton на выполнение
	vector<stRadioButton> m_stRadioButton;
	int iCurIndexRadioButton;   // чтобы предыдущей кнопке присвоить false
};

struct stStaticText
{
	stStaticText()
	{
		textureStaticText = NULL;
		//pTextureRender = NULL;
		bVisible = true;
	};

	//LPDIRECT3DTEXTURE9 *pTextureRender; // адрес текстуры которую необходимо вывести
	LPDIRECT3DTEXTURE9 textureStaticText; // готовая текстура StaticText
	
	char *text;
	D3DXVECTOR2 vectorTrans;
	int iIDWindow; // индекс окна которому принадлежит элемент
	bool bVisible;  // видимость
	int iFontSize; // размер шрифта

	RECT rectStaticText;
};

struct stImageBox
{
	stImageBox()
	{
		textureImageBox = NULL;
		bVisible = true;

		bAnimEnable = false;
		iRateInSec = 1;
		dwLastUpdate = 0;
		iNumCurCadr = 0;
		iQuantCadr = 0;
	};

	//LPDIRECT3DTEXTURE9 *pTextureRender; // адрес текстуры которую необходимо вывести
	LPDIRECT3DTEXTURE9 textureImageBox; // готовая текстура stImageBox

	D3DXVECTOR2 vectorTrans;
	D3DXVECTOR2 vectorScale;
	int iIDWindow; // индекс окна которому принадлежит элемент
	bool bVisible;  // видимость

	// Для анимированых изображений
	bool bAnimEnable;  // разрешение на анимацию
	int iRateInSec; // частота кадров в секунду
	int iDelayFrame; // задержка между кадрами
	DWORD dwLastUpdate; // последенее обновление кадра рисунка
	int iNumCurCadr;  // номер текущего кадра
	int iQuantCadr; // количество кадров
	RECT *pRectRender; // какую часть текстуры отображать, при загрузке заполняем на каждый кадр
	RECT rectFrameForWindow; // Чтобы коректно зачищять
	stRECT m_stRectPos; // ограничивающий прямоугольник
};

struct stSliderInfo  // Структура передаётся в назначеную Slider'y функцию
{
	stSliderInfo()
	{
		m_fMinValue = 0.0f;
		m_fCurValue = 0.0f;
		m_fMaxValue = 0.0f;

		m_fRaznizaMinMax = 0.0f;
		fDelta = 1.0f;
	};
	stSliderInfo(float fMinValue, float fCurValue, float fMaxValue, float delta)
	{
		m_fMinValue = fMinValue;
		m_fCurValue = fCurValue;
		m_fMaxValue = fMaxValue;

		m_fRaznizaMinMax = 0.0f;
		fDelta = delta;
	};

	float m_fMinValue;
	float m_fCurValue;
	float m_fMaxValue;
	
	float m_fRaznizaMinMax;  // разница между min и max ПО МОДУЛЮ => диапазон значений
	float fDelta; // Посколько прибавлять
};
struct stScrollBar
{
	stScrollBar()
	{
		textureSliderString = NULL;
		bEnable = true;
		bVisible = true;
		bSlider = false;
		bHorizontalSlider = true;

		iSliderStringWidth = 0;

		vectorScale.x = 1.0f;
		vectorScale.y = 1.0f;

		iDelayScroll = 70;
		dwLastUpdate = 0;

		iButtonSliderBegPosX = 0;
		iButtonSliderBegPosY = 0;
		iSliderButtonWidth = 0;

		fPixelInValue = 0.0f;
		bFollowMouse = false;
	};

	bool bSlider;
	bool bFollowMouse; // Следовать ли рычажку за курсором мышки
	bool bHorizontalSlider; // горизонтальный слайдер = true, иначе это вертикальный слайдер
	D3DXVECTOR2 vectorTrans;
	D3DXVECTOR2 vectorScale; // масштаб прямоугольника по которому ходит рычажок
	int iSliderStringWidth; // ширина прямоугольника по которому ходит рычажок
	int iSliderButtonWidth; // ширина рычажка
	float fPixelInValue;  // узнаём сколько в единице значения пикселей
	bool bEnable;   // доступность
	bool bVisible;  // видимость
	int iButtonSliderBegPosX;
	int iButtonSliderBegPosY;

	int iDelayScroll; // задержка между скролингами
	DWORD dwLastUpdate; // последнее обновление времени скроллинга

	stSliderInfo m_stSliderInfo; // информация про слайдер (min, cur, max) по значениям внутри него
	stButton buttonSlider; // рычажок в середине строки за который тянуть
	stButton buttonNext;
	stButton buttonPrev;
	
	LPDIRECT3DTEXTURE9 textureSliderString; // текстура прямоугольника по которому ходит рычажок

	void (*pFuncExecute)(stSliderInfo *pSliderInfo);
};

struct stGuiMouse
{
	stImageBox image;
};

struct stWindowPanel // создаётся текстура под размер окна всё что не попадает не рисует (Простой вид окна)
{
	stWindowPanel()
	{
		textureRenderWindow = NULL;
		textureRenderFon = NULL;
		
		bVisible = true;

		m_iQuantImageBox = 0;
		m_iQuantButtons = 0;
		m_iQuantCheckBox = 0;
		m_iQuantStaticText = 0;
		m_iQuantGroup = 0;
	};

	LPDIRECT3DTEXTURE9 textureRenderWindow;  // готовая текстура окна ( при перезадании размеров окна пересоздаётся )
	LPDIRECT3DTEXTURE9 textureRenderFon; // заливается при создании окна и содержит фоновое изображение с установленым параметром

	enTypeWindowFill typeWindowFill; // как заливать фоновым изображением

	RECT RectWindow; // позиция и размер окна (Для нажатий)
	stRECT stRect;   // просто татк удобнее хранить размеры
	D3DXVECTOR2 vectorTransWindow;
	bool bVisible;


	// Элементы которые будут в окне ( кнопки, меню, ... )
	vector<stImageBox> m_stImageBox;
	vector<stButton> m_stButtons;
	vector<stCheckBox> m_stCheckBox;
	vector<stStaticText> m_stStaticText;
	vector<stGroup> m_stGroup;
	int m_iQuantImageBox;
	int m_iQuantButtons;
	int m_iQuantCheckBox;
	int m_iQuantStaticText;
	int m_iQuantGroup;
};

struct stHandleGui  // Для обращения к объектам, например Show, Hide
{
	stHandleGui()
	{
		typeElementGui = NOT_ELEM; // по умолчанию говорим что нет элемента
		typeWindow = NOT_ELEM;
	};

	enTypeElemGui typeWindow;  // тип окна которому принадлежит элемент
	int iIDWindow; // индекс окна которому принадлежит элемент

	enTypeElemGui typeElementGui;  // тип элемента
	int iIndex;  // индекс элемента

	//int iIndexButton;
	//int iIndexCheckBox;
	//int iIndexWindow;
};
// ====================== structures ( end ) ==================================================



// ==================================  class CManagerGUI ( BEGIN ) ==================================
class CManagerGUI
{
	private:
		int i; // для циклов
		int j; // для циклов
		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
		LPDIRECT3DINDEXBUFFER9  m_pIndexBuffer;
		LPDIRECT3DTEXTURE9 m_boxTexture;

		DXData *m_directx;

		D3DXMATRIX m_matWorld;
		D3DXMATRIX m_matView;
		D3DXMATRIX m_matProjection;
		D3DXMATRIX m_matRot;
		D3DXMATRIX m_matTrans;
		D3DXMATRIX m_matScale;

		// Звуковые семплы
		int m_iSampleSoundID_MouseDown; // был нажат элемент
		int m_iSampleSoundID_MouseOver; // мышь над элементом

		D3DCOLOR m_colorTextDefault;
		D3DCOLOR m_colorTextDefault_forRestore;  // цвет текста который прописан в классе для востановления
		D3DCOLOR m_colorFrameCheckBox; // цвет рамки CheckBox
		D3DCOLOR m_colorFrameRadioButton; // цвет рамки RadioButton
		//FONT *m_pFontDefault_s20; // размер шрифта 20
		//FONT *m_pFontDefault_s25; // размер шрифта 25

		int m_iCurNumMouse;  // текущий номер мышки
		bool m_bCreatedMouse; // была ли создана мышь

		vector<stWindowPanel> m_stWindowPanel;
		vector<stHandleGui> m_stHandleWindowRender; // массив отрисовки окон (в какой последовательности рисовать)
		//stHandleGui m_stHandleActiveWindow; // Активное в данный момент окно
		//stWindows m_stWindows;
		vector<stGuiMouse> m_stGuiMouse;
		vector<stGroup> m_stGroup;
		vector<stScrollBar> m_stHorizScrollBar; // горизонтальный ScrollBar
		vector<stScrollBar> m_stVertScrollBar; // вертикальный ScrollBar
		vector<stButton> m_stButtons; // кнопки не принадлежащие ни одному окну
		vector<stCheckBox> m_stCheckBox; // checkBox не принадлежащие ни одному окну
		vector<stRadioButton> m_stRadioButton; // RadioButton не принадлежащие ни одному окну
		vector<stStaticText> m_stStaticText; // StaticText не принадлежащие ни одному окну
		vector<stImageBox> m_stImageBox; // ImageBox не принадлежащие ни одному окну
		int m_iQuantWindowsPanel; // количество Panel окон
		int m_iQuantButtons; // количество кнопок
		int m_iQuantCheckBox; // количество checkBox
		int m_iQuantRadioButton; // количество RadioButton
		int m_iQuantStaticText; // количество StaticText
		int m_iQuantImageBox; // количество ImageBox
		int m_iQuantMouse; // количество мышек
		int m_iQuantGroup; // количество груп
		int m_iQuantHorizScrollBar; // количество горизонтальных ScrollBar
		int m_iQuantVertScrollBar; // количество вертикальных ScrollBar
	public:
		CManagerGUI();
		~CManagerGUI();

		void vBeginInit(DXData *directx);  // начало и конец инициализации ВСЕХ кнопок, меню, и так далее...
		void vEndInit();
		void vLoadSamples(char *pcSampleMouseDown, char *pcSampleMouseOver, int iSoundVolume);

		void vRender(); // отрисовка
		void vSetMatrix(); // установка матрицы - проекции 2D вида

		void vCalculate(stMouseInfoGUI *mouseInfoGui); // Вызывать в главном цикле, передаём данные мыши
		inline bool bCalculateWindowPanels(stMouseInfoGUI *mouseInfoGui); // обработка Panel window's
		bool bShow(stHandleGui *handleGui);  // показать элемент
		bool bHide(stHandleGui *handleGui);  // спрятать элемент
		bool bReplaceStaticText(stHandleGui *handleGui, char *newText); // заменяет надпись в указаном StaticText
		bool bSetObjectPos(D3DXVECTOR2 pos, stHandleGui *handleGui);   // устанавливает объекту новые координаты
		void vSetCurTextColor(D3DCOLOR color);  // устанавливает текущий цвет текста
		void vRestoreTextColor(); // востанавливает цвет текста который прописан в классе по-умолчанию


		// Создание кнопок
		stHandleGui hCreateButton(stHandleGui *handleWindow, char *name, stRECT rectPos, void (*pFunc)(), bool bSelecEnable, int iTypeSelection);
		stHandleGui hCreateButton(stHandleGui *handleWindow, char *name, stRECT rectPos, void (*pFunc)(), bool bSelecEnable, int iTypeSelection, char *textureButtonName);
		stHandleGui hCreateButton(stHandleGui *handleWindow, char *name, stRECT rectPos, void (*pFunc)(), char *textureButtonName, char *textureButtonSelectName);

		// Создание checkBox
		stHandleGui hCreateCheckBox(stHandleGui *handleWindow, char *name, stRECT rectPos, void (*pFunc)(bool bCheckBoxOn));
		stHandleGui hCreateCheckBox(stHandleGui *handleWindow, char *name, stRECT rectPos, void (*pFunc)(bool bCheckBoxOn), char *textureNameCheckBoxOFF, char *textureNameCheckBoxON);

		// Создание группы GroupRadioButton и  самого RadioButton
		stHandleGui hCreateGroup(stHandleGui *handleWindow);
		void vCreateRadioButton(stHandleGui *handleGroup, char *name, stRECT rectPos, void (*pFunc)(bool bRadioButtonOn));
		void vCreateRadioButton(stHandleGui *handleGroup, char *name, stRECT rectPos, void (*pFunc)(bool bRadioButtonOn), char *textureNameRadioButtonOFF, char *textureNameRadioButtonON);


		// Создание обычного StaticText
		stHandleGui hCreateStaticText(stHandleGui *handleWindow, char *text, int iPosX, int iPosY, int iFontSize);

		// Создание ImageBox
		stHandleGui hCreateImageBox(stHandleGui *handleWindow, stRECT rectPos, char *textureNameImage);
		stHandleGui hCreateImageBox(stHandleGui *handleWindow, stRECT rectPos, stSIZE sizeCadr, int rateInSec, char *textureNameImage);

		// Создание ScrollBar
		stHandleGui hCreateHorizScrollBar(int iWindowID, stRECT rectPos, stSliderInfo info, void (*pFunc)(stSliderInfo *pSliderInfo), int iDelayButton, bool bSlider);
		stHandleGui hCreateHorizScrollBar(int iWindowID, stRECT rectPos, stSliderInfo info, void (*pFunc)(stSliderInfo *pSliderInfo), int iDelayButton, bool bSlider,
												char *pcNameTextureButtonSlider,
												char *pcNameTextureButtonSliderSelection,
												char *pcNameTextureButtonPrev,
												char *pcNameTextureButtonPrevSelection,
												char *pcNameTextureButtonNext,
												char *pcNameTextureButtonNextSelection,
												char *pcNameTextureSliderString);
		stHandleGui hCreateVertScrollBar(int iWindowID, stRECT rectPos, stSliderInfo info, void (*pFunc)(stSliderInfo *pSliderInfo), int iDelayButton, bool bSlider);
		stHandleGui hCreateVertScrollBar(int iWindowID, stRECT rectPos, stSliderInfo info, void (*pFunc)(stSliderInfo *pSliderInfo), int iDelayButton, bool bSlider,
												char *pcNameTextureButtonSlider,
												char *pcNameTextureButtonSliderSelection,
												char *pcNameTextureButtonPrev,
												char *pcNameTextureButtonPrevSelection,
												char *pcNameTextureButtonNext,
												char *pcNameTextureButtonNextSelection,
												char *pcNameTextureSliderString);
		bool bScrollBarOffsetCurValue(int iIndexScrollBar,bool bHoriz, float fDelta); // смещение текущего значения


		// Создание Окон
		stHandleGui hCreateWindowPanel(stRECT rectPos, char *nameTextureFon, enTypeWindowFill typeWindowFill);
		void vRefreshRectWindowPanel(int iIndexWindowPanel, stHandleGui *handleElem, LPDIRECT3DTEXTURE9 *texture);


		// Создание мыши
		stHandleGui hCreateMouse(stSIZE sizeCursor, stSIZE sizeCadr, int rateInSec, char *textureNameImage);  // создаёт мышь
		bool bSetMouse(stHandleGui *handleMouse);  // устанавливает мышь
		void vShowMouse();  // показать текущую мышь
		void vHideMouse();  // спрятать текущую мышь



		//
		void vDrawBox2D(int x,int y,int w,int h,DWORD color);
		void vDrawRectangle(int x,int y,int w,int h,DWORD clr);
		void vDrawRectangle(int x,int y,int w,int h,DWORD clr1,DWORD clr2);
		void vDrawFrameRect(int x, int y, int w, int h, DWORD color_1,DWORD color_2, int widthFrame); // прямоугольник с толщиной рамки
		void vDrawButton(int x,int y,int w,int h,FONT* font,char* str,DWORD colorText);
};
// -----------------------------------  class CManagerGUI ( END ) -----------------------------------


// ==================================  class CButtonGUI ( BEGIN ) ==================================
#define BUTTON_GUI_STATE__PICK_OUT 1 // кнопка выделена
#define BUTTON_GUI_STATE__PRESSED  2 // кнопка была нажата
#define BUTTON_GUI_STATE__JAMMED   3 // кнопка зажата

class CButtonGUI
{
	private:
		// все текстуры находятся в менеджере GUI
		int m_iIndexTextureButton;   // индекс текстуры кнопки
		int m_iIndexTexturePickOut;  // индекс текстуры выделения
		
		RECT m_rectButton;  // прямоугольник в котором находится кнопка
	public:
		CButtonGUI();
		~CButtonGUI();
};
// -----------------------------------  class CButtonGUI ( END ) -----------------------------------
#endif // _CMANAGER_GUI_H_