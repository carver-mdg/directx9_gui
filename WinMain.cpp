/*
	WinMain
*/

#define APPNAME  "SnakeDX (Direct3D9)"
#define APPTITLE "SnakeDX v0.7 (Direct3D9)"

#define _RELEASE_(p) { if(p) { (p)->Release(); (p)=NULL; };};
#define _DELETE_(p)  { if(p) { delete (p);     (p)=NULL; };};
 

// include
#include <windows.h>
#include "DXData.h"
#include "CAnimation.h"
#include "CSpriteFont.h"
#include "CManagerGUI.h"
//#include "font.h"

HWND hWnd;
BOOL g_bActivate = true;


// ---------- Structures (begin) --------
struct stFPS
{
	stFPS()
	{
		iFPS = 0;
		iBegTime = 0;

		memset(&rect,0,sizeof(RECT));
	};

	int iFPS;
	char cFPS[10];
	int iBegTime;
	RECT rect;
	char nameFont[30]; // имя шрифта которым выводить
};

struct stMouse
{
	stMouse()
	{
		//vectorPos = D3DXVECTOR2(0.0f, 0.0f);
		fSensitivity_x = 0.8f;
		fSensitivity_y = 0.8f;

		pointScreenOldPos.x = 0;
		pointScreenOldPos.y = 0;
		pointScreenNewPos.x = 0;
		pointScreenNewPos.y = 0;

		fDeltaX = 0;
		fDeltaY = 0;
	};

	// Координаты на всём экране
	POINT pointScreenOldPos;
	POINT pointScreenNewPos;

	// координаты в окне
	D3DXVECTOR2 vectorPos;

	POINT pointLBDownPos;   // где была нажата левая клавиша
	POINT pointLBUpPos; // где была отпущена левая клавиша


	// чуствительность мыши
	float fSensitivity_x;
	float fSensitivity_y;

	// Разница при изменении координат (для внутреннего пользования)
	float fDeltaX;
	float fDeltaY;

	// текстуры
	//int IDTextureTypeMouse_1;  // CAnimation
	stHandleGui mouseType_1;
	stHandleGui mouseType_2;
	stHandleGui mouseType_3;
};
// ------------ Structures (end) --------
// ------------ Variable (begin) --------
bool g_bExit = false;
bool g_bPauseProgram = false; // если не надо обрабатывать игру ( например при потери фокуса )
int g_iFrameWindowWidth = 0; // размеры елементов окна ( таскбар, полоска с именем...)
int g_iFrameWindowHeight = 0;

int g_iScreenWidth = 640;
int g_iScreenHeight = 480;
bool g_bFullScreen = false;

//bool g_bKeyBuf[256];  // буфер клавиатуры


stFPS   g_stFPS;
stMouse g_stMouse;

DXData directx;
CSpriteFont g_font;
CAnimation g_anim;
CManagerGUI gui;
FONT *font;

char text[200];
int g_iColorClearRed = 0;
int g_iColorClearGreen = 0;
int g_iColorClearBlue = 0;

stMouseInfoGUI mouseInfoGui; // передаём заполненую структуру в GUI
stHandleGui button_1;
stHandleGui button_2;
stHandleGui button_3;
stHandleGui checkBox_1;
stHandleGui checkBox_2;
stHandleGui checkBox_3;
stHandleGui staticText_1;
stHandleGui staticText_in_window_3_1;
stHandleGui staticText_FPS;
stHandleGui imageBox_1;
stHandleGui handleGroup_1;
stHandleGui handleGroup_2;
stHandleGui handleScrollBar_1;
stHandleGui handleScrollBar_2;
stHandleGui handleScrollBar_3;
stHandleGui handleScrollBar_4;
stHandleGui handleScrollBar_5;
stHandleGui handleScrollBar_6;
stHandleGui handleWindowPanel_1;
stHandleGui handleWindowPanel_2;
stHandleGui handleWindowPanel_3;
stHandleGui handleRadioButton_win_1;
stHandleGui handleRadioButton_win_2;
stHandleGui handleRadioButton_win_3;

// ------------ Variable (end) ------------
// -------------- Prototype (begin) -----------------
bool bLoadResource();
void vFPS();
void vKeyboard();
void vMouse();
void vButton_1();
void vButton_2();
void vButton_3();
void vButton_4();
void vButton_5();
void vButton_6();
void vButton_7();
void vButton_8();
void vCheckBox_1(bool bCheckBoxOn);
void vCheckBox_2(bool bCheckBoxOn);
void vCheckBox_3(bool bCheckBoxOn);
void vRadioButton_1(bool bRadioButtonOn);
void vRadioButton_2(bool bRadioButtonOn);
void vRadioButton_3(bool bRadioButtonOn);
void vRadioButton_4(bool bRadioButtonOn);
void vRadioButton_5(bool bRadioButtonOn);
void vRadioButton_6(bool bRadioButtonOn);
void vSliderFunc_RED(stSliderInfo *pSliderInfo);
void vSliderFunc_GREEN(stSliderInfo *pSliderInfo);
void vSliderFunc_BLUE(stSliderInfo *pSliderInfo);
void vSliderFunc_SenMouseX(stSliderInfo *pSliderInfo);
void vSliderFunc_SenMouseY(stSliderInfo *pSliderInfo);
void vSliderFunc_Vert_1(stSliderInfo *pSliderInfo);
void vSliderFunc_Horiz_texture_1(stSliderInfo *pSliderInfo);
void vSliderFunc_Vert_texture_1(stSliderInfo *pSliderInfo);
void vButton_in_Window_3_1();
void vRadioButton_win_1(bool bRadioButtonOn);
void vRadioButton_win_2(bool bRadioButtonOn);
void vRadioButton_win_3(bool bRadioButtonOn);
void vRadioButton_win_4(bool bRadioButtonOn);
void vRadioButton_win_5(bool bRadioButtonOn);
void vRadioButton_win_6(bool bRadioButtonOn);
void vRadioButton_win_7(bool bRadioButtonOn);
// -------------- Prototype (end) -----------------



//---------------------------------
// Name: WindowProc ()
// Desc: 
//---------------------------------
LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage (0);
		break;

	case WM_SETCURSOR:
			SetCursor (NULL);
		break; 

	case WM_ACTIVATE:
		g_bActivate = LOWORD(wParam);
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_KILLFOCUS:
			g_bPauseProgram = true;
		break;

	case WM_KEYUP:
			switch(wParam)
			{
				case VK_HOME:
						//directx.bInitD3D(hWnd, 800, 600, false);
						/*gui.vBeginInit(&directx);
						button_1 = gui.hCreateButton(-1,"Button_1",100,100,100,32,vButton_1, true, TYPE_SELECTION_COLOR_FONT);
						gui.hCreateButton(-1,"Button_2",100,142,100,32,vButton_2, true, TYPE_SELECTION_SIZE_FONT);
						gui.hCreateButton(-1,"Button_3",100,184,100,32,vButton_3, true, TYPE_SELECTION_FRAME);

						button_2 = gui.hCreateButton(-1,"Img But_1",250,100,100,32,vButton_4, true, TYPE_SELECTION_COLOR_FONT, "gfx\\gui\\Button_3.png");
						gui.hCreateButton(-1,"Img But_2",250,142,100,32,vButton_5, true, TYPE_SELECTION_SIZE_FONT, "gfx\\gui\\Button_3.png");
						gui.hCreateButton(-1,"Img But_3",250,184,100,32,vButton_6, true, TYPE_SELECTION_FRAME, "gfx\\gui\\Button_3.png");
						gui.hCreateButton(-1,"TanksBut",250,226,100,32,vButton_7, true, TYPE_SELECTION_COLOR_FONT, "gfx\\gui\\i_button.png");

						button_3 = gui.hCreateButton(-1,NULL,400,100,32,32,vButton_7, "gfx\\gui\\button_2.png","gfx\\gui\\button_2_Sel.png");


						checkBox_1 = gui.hCreateCheckBox(-1,"Спрятать Button_1 & checkBox_3",550,100,24,24,vCheckBox_1, "gfx\\gui\\Check_OFF_1.png", "gfx\\gui\\Check_ON_1.png");
						checkBox_2 = gui.hCreateCheckBox(-1,"Спрятать Img But_1",550,124,24,24,vCheckBox_2, "gfx\\gui\\Check_OFF_1.png", "gfx\\gui\\Check_ON_1.png");
						checkBox_3 = gui.hCreateCheckBox(-1,"Спрятать texture Button",550,148,24,24,vCheckBox_3, "gfx\\gui\\Check_OFF_1.png", "gfx\\gui\\Check_ON_1.png");

						staticText_1 = gui.hCreateStaticText(-1,"static text", 0,0,30);
						staticText_FPS = gui.hCreateStaticText(-1,NULL, g_iScreenWidth - 150,0,30);
						gui.vEndInit();*/
					break;

				case VK_END:					
					//directx.bInitD3D(hWnd, 1024, 768, true);
					/*gui.vBeginInit(&directx);
					button_1 = gui.hCreateButton(-1,"Button_1",100,100,100,32,vButton_1, true, TYPE_SELECTION_COLOR_FONT);
					gui.hCreateButton(-1,"Button_2",100,142,100,32,vButton_2, true, TYPE_SELECTION_SIZE_FONT);
					gui.hCreateButton(-1,"Button_3",100,184,100,32,vButton_3, true, TYPE_SELECTION_FRAME);

					button_2 = gui.hCreateButton(-1,"Img But_1",250,100,100,32,vButton_4, true, TYPE_SELECTION_COLOR_FONT, "gfx\\gui\\Button_3.png");
					gui.hCreateButton(-1,"Img But_2",250,142,100,32,vButton_5, true, TYPE_SELECTION_SIZE_FONT, "gfx\\gui\\Button_3.png");
					gui.hCreateButton(-1,"Img But_3",250,184,100,32,vButton_6, true, TYPE_SELECTION_FRAME, "gfx\\gui\\Button_3.png");
					gui.hCreateButton(-1,"TanksBut",250,226,100,32,vButton_7, true, TYPE_SELECTION_COLOR_FONT, "gfx\\gui\\i_button.png");

					button_3 = gui.hCreateButton(-1,NULL,400,100,32,32,vButton_7, "gfx\\gui\\button_2.png","gfx\\gui\\button_2_Sel.png");


					checkBox_1 = gui.hCreateCheckBox(-1,"Спрятать Button_1 & checkBox_3",550,100,24,24,vCheckBox_1, "gfx\\gui\\Check_OFF_1.png", "gfx\\gui\\Check_ON_1.png");
					checkBox_2 = gui.hCreateCheckBox(-1,"Спрятать Img But_1",550,124,24,24,vCheckBox_2, "gfx\\gui\\Check_OFF_1.png", "gfx\\gui\\Check_ON_1.png");
					checkBox_3 = gui.hCreateCheckBox(-1,"Спрятать texture Button",550,148,24,24,vCheckBox_3, "gfx\\gui\\Check_OFF_1.png", "gfx\\gui\\Check_ON_1.png");

					staticText_1 = gui.hCreateStaticText(-1,"static text", 0,0,30);
					staticText_FPS = gui.hCreateStaticText(-1,NULL, g_iScreenWidth - 150,0,30);
					gui.vEndInit();*/
					break;
			};
		break;

	case WM_SETFOCUS:
			g_bPauseProgram = false;

			/*
			RECT rect;

			RECT wndRect;
			RECT clientRect;
			GetWindowRect(hWnd, &wndRect);
			GetClientRect(hWnd, &clientRect);	  

			g_iFrameWindowWidth  = (wndRect.right-wndRect.left)  - (clientRect.right-clientRect.left);
			g_iFrameWindowHeight = (wndRect.bottom-wndRect.top) - (clientRect.bottom-clientRect.top);
			
			g_iFrameWindowWidth /= 2;
			g_iFrameWindowHeight -= g_iFrameWindowWidth;

			rect.left = g_iFrameWindowWidth;
			rect.top = g_iFrameWindowHeight;
			rect.right = g_iScreenWidth + g_iFrameWindowWidth;
			rect.bottom = g_iScreenHeight + g_iFrameWindowHeight;

			ClipCursor(&rect);
			*/
		break;
	};
	return DefWindowProc(hWnd, message, wParam, lParam);
};

//---------------------------------
// Name: WindowInit ()
// Desc: 
//---------------------------------
bool WindowInit (HINSTANCE hThisInst, int nCmdShow)
{
	WNDCLASS		    wcl;
		
	wcl.hInstance		= hThisInst;
	wcl.lpszClassName	= APPNAME;
	wcl.lpfnWndProc		= WindowProc;
	wcl.style			= 0;

	wcl.hIcon			= LoadIcon (hThisInst, IDC_ICON);
	wcl.hCursor			= LoadCursor (hThisInst, IDC_ARROW);
	wcl.lpszMenuName	= NULL;

	wcl.cbClsExtra		= 0;
	wcl.cbWndExtra		= 0;
	wcl.hbrBackground	= (HBRUSH) GetStockObject (BLACK_BRUSH);

	RegisterClass (&wcl);

	hWnd = CreateWindowEx (
		WS_EX_TOPMOST,
		APPNAME,
		APPTITLE,
		WS_OVERLAPPEDWINDOW,
		0, 0, 
		640,
		480,
		NULL,
		NULL,
		hThisInst,
		NULL);

	if(!hWnd) return false;

	return true;
};

//---------------------------------
// Name: DestroyDirect3D9()
// Desc: Destroy Direct3D 
//---------------------------------

void DestroyDirect3D9()
{
}

//---------------------------------
// Name: AppInit()
// Desc: Initializes Aplication
//---------------------------------
bool AppInit (HINSTANCE hThisInst, int nCmdShow)
{
	if(!WindowInit (hThisInst, nCmdShow)) return false;
	ShowWindow (hWnd, nCmdShow);
	UpdateWindow (hWnd);

	return true;
};

//---------------------------------
// Name: RenderScene ()
// Desc: 
//---------------------------------
void vRender()
{
	//directx.m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB (128, 128, 128), 1.0f, 0);
	directx.m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB (g_iColorClearRed, g_iColorClearGreen, g_iColorClearBlue), 1.0f, 0);
	directx.m_pD3DDevice->BeginScene();

		//g_font.vDrawText(g_stFPS.cFPS,g_iScreenWidth - (g_stFPS.rect.right),0, g_stFPS.nameFont);
		//font->print(g_iScreenWidth - (g_stFPS.rect.right), 0, D3DCOLOR_XRGB(255,0,255), g_stFPS.cFPS);
		
		//gui.vDrawBox2D(100, 100, 100, 30, D3DCOLOR_XRGB(100,100,100));
		//gui.vDrawRectangle(100,100,100,30,D3DCOLOR_XRGB(128,128,128), D3DCOLOR_XRGB(150,150,150));
		//g_anim.vRender();
		gui.vRender();
		font->print((float)(g_iScreenWidth - (g_stFPS.rect.right)), 0, D3DCOLOR_XRGB(30,255,30), g_stFPS.cFPS);
		//font->print(0, 0, D3DCOLOR_XRGB(255,0,255), text);
		//gui.vDrawButton(100,100,100,30,font, "Button_1", D3DCOLOR_XRGB(0,255,255));

		//g_anim.vRender();
	directx.m_pD3DDevice->EndScene();
	directx.m_pD3DDevice->Present(0,0,0,0);
}

//---------------------------------
// Name: WinMain ()
// Desc: 
//---------------------------------
int APIENTRY WinMain (HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	if(!AppInit (hThisInst, nCmdShow)) return false;

	if(!bLoadResource())
	{
		//MessageBox(hWnd,"Error Load resource","FATAL ERROR",MB_OK);
		g_bExit = true;
	}

	// -------- Main -------
	while (!g_bExit)
	{
		if(PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage (&msg, NULL, 0, 0)) break;
			TranslateMessage (&msg); 
			DispatchMessage (&msg);
		}

		if(!g_bPauseProgram)
		{
			vMouse();
			vKeyboard();
			gui.vCalculate(&mouseInfoGui);	
			//g_anim.vDraw();
			vFPS();

			vRender();
		}
	};

	directx.bDestroyDirect3D9();

	return 0;
};

//---------------------------------
// EOF
//---------------------------------

bool bLoadResource()
{
	directx.vReadIntegerInIni("VideoSettings","ScreenWidth","DATA.ini", g_iScreenWidth);
	directx.vReadIntegerInIni("VideoSettings","ScreenHeight","DATA.ini", g_iScreenHeight);
	directx.vReadBoolInIni("VideoSettings","FullScreen","DATA.ini",g_bFullScreen);

	if(!directx.bInitD3D(hWnd, g_iScreenWidth, g_iScreenHeight, g_bFullScreen))
	{
		MessageBox(hWnd,"Directx not init","FATAL ERROR",MB_OK);
		return false;
	}

	// -------- Loads fonts (begin) --------
	g_font.vSetDirectX(&directx);
	g_font.iAddFont("Fonts\\Arial.***");
		
	// Load font text FPS
		sprintf(g_stFPS.nameFont,"%s","Arial");
		g_font.vSetColor(D3DCOLOR_XRGB(255,255,255),D3DCOLOR_XRGB(20,219,40),"Arial");
	// -------- Loads fonts (end) --------


	g_anim.vSetDirectX(&directx);
	

	// -------- Load and Init cursor ( begin ) -------
	//g_anim.iAddType("CURSOR_MAIN_1","gfx\\gui\\cursor_main.png",0,32,32);
	//g_stMouse.IDTextureTypeMouse_1 = g_anim.iAddObject(0,"CURSOR_MAIN_1",g_stMouse.vectorPos, ANIM_QUANT_INF | ANIM_FORWARD);
	//g_anim.bSetObjectPriority(0,g_stMouse.IDTextureTypeMouse_1, 10000);
		
	// Init mouse
		SetCursorPos(g_iScreenWidth/2 - g_iFrameWindowWidth, g_iScreenHeight/2 + g_iFrameWindowHeight);
	g_stMouse.vectorPos.x = (float)g_iScreenWidth/2 + g_iFrameWindowWidth;
	g_stMouse.vectorPos.y = (float)g_iScreenHeight/2 + g_iFrameWindowHeight;
		//g_anim.vSetObjectPos(0,g_stMouse.IDTextureTypeMouse_1, g_stMouse.vectorPos);
	g_stMouse.pointScreenOldPos.x = (long)g_stMouse.vectorPos.x;
	g_stMouse.pointScreenOldPos.y = (long)g_stMouse.vectorPos.y;

		// загружаем чуствительность мыши
	directx.vReadFloatInIni("MOUSE","sensitivity_x","DATA.ini", g_stMouse.fSensitivity_x);
	directx.vReadFloatInIni("MOUSE","sensitivity_y","DATA.ini", g_stMouse.fSensitivity_y);
	// -------- Load and Init cursor ( end ) -------



	// -------  LOAD game data (begin) --------
	gui.vBeginInit(&directx);
	gui.vLoadSamples("snd\\gui\\mousedown2.wav", "snd\\gui\\mouseover.wav", 100);

	   // ------------------- Buttons ----------------
		button_1 = gui.hCreateButton(NULL,"Button_1",stRECT(100,100,100,32),vButton_1, true, TYPE_SELECTION_COLOR_FONT);
		gui.hCreateButton(NULL,"Button_2",stRECT(100,142,100,32),vButton_2, true, TYPE_SELECTION_SIZE_FONT);
		gui.hCreateButton(NULL,"Button_3",stRECT(100,184,100,32),vButton_3, true, TYPE_SELECTION_FRAME);

		button_2 = gui.hCreateButton(NULL,"Img But_1",stRECT(250,100,100,32),vButton_4, true, TYPE_SELECTION_COLOR_FONT, "gfx\\gui\\Button\\Button_3.png");
		gui.hCreateButton(NULL,"Img But_2",stRECT(250,142,100,32),vButton_5, true, TYPE_SELECTION_SIZE_FONT, "gfx\\gui\\Button\\Button_3.png");
		gui.hCreateButton(NULL,"Img But_3",stRECT(250,184,100,32),vButton_6, true, TYPE_SELECTION_FRAME, "gfx\\gui\\Button\\Button_3.png");
		gui.hCreateButton(NULL,"TanksBut",stRECT(250,226,100,32),vButton_7, true, TYPE_SELECTION_COLOR_FONT, "gfx\\gui\\Button\\i_button.png");
		gui.hCreateButton(NULL,"SpaceRanger",stRECT(250,270,128,32),vButton_8, true, TYPE_SELECTION_COLOR_FONT, "gfx\\gui\\Button\\2BUTA.png");

		button_3 = gui.hCreateButton(NULL,NULL,stRECT(400,100,32,32),vButton_7, "gfx\\gui\\Button\\button_2.png","gfx\\gui\\Button\\button_2_Sel.png");


		// ------------------- CheckBox ----------------
		checkBox_1 = gui.hCreateCheckBox(NULL,"Спрятать Button_1 & checkBox_3",stRECT(550,100,24,24),vCheckBox_1, "gfx\\gui\\CheckBox\\Check_OFF_1.png", "gfx\\gui\\CheckBox\\Check_ON_1.png");
		checkBox_2 = gui.hCreateCheckBox(NULL,"Спрятать Img But_1",stRECT(550,124,24,24),vCheckBox_2, "gfx\\gui\\CheckBox\\Check_OFF_1.png", "gfx\\gui\\CheckBox\\Check_ON_1.png");
		checkBox_3 = gui.hCreateCheckBox(NULL,"Спрятать texture Button",stRECT(550,148,24,24),vCheckBox_3, "gfx\\gui\\CheckBox\\Check_OFF_1.png", "gfx\\gui\\CheckBox\\Check_ON_1.png");

		gui.hCreateCheckBox(NULL,"CheckBox Auto",stRECT(400,300,24,24),vCheckBox_2);


		// ------------------- RadioButton ----------------
		handleGroup_1 = gui.hCreateGroup(NULL);
		gui.vCreateRadioButton(&handleGroup_1,"RadioButton_1",stRECT(550,200,24,24),vRadioButton_1);
		gui.vCreateRadioButton(&handleGroup_1,"RadioButton_2",stRECT(550,230,24,24),vRadioButton_2);
		gui.vCreateRadioButton(&handleGroup_1,"RadioButton_3",stRECT(550,260,24,24),vRadioButton_3);

		handleGroup_2 = gui.hCreateGroup(NULL);
		gui.vCreateRadioButton(&handleGroup_2,"RadioButton_4",stRECT(750,200,24,24),vRadioButton_4,"gfx\\gui\\RadioButton\\RadioButton_1_OFF.png", "gfx\\gui\\RadioButton\\RadioButton_1_ON.png");
		gui.vCreateRadioButton(&handleGroup_2,"RadioButton_5",stRECT(750,230,24,24),vRadioButton_5,"gfx\\gui\\RadioButton\\RadioButton_1_OFF.png", "gfx\\gui\\RadioButton\\RadioButton_1_ON.png");
		gui.vCreateRadioButton(&handleGroup_2,"RadioButton_6",stRECT(750,260,24,24),vRadioButton_6,"gfx\\gui\\RadioButton\\RadioButton_1_OFF.png", "gfx\\gui\\RadioButton\\RadioButton_1_ON.png");



		// ------------------- StaticText ----------------
		staticText_1 = gui.hCreateStaticText(NULL,"static text", 0,0,30);
		staticText_FPS = gui.hCreateStaticText(NULL,NULL, g_iScreenWidth - 150,0,30);

		// ------------------- ImageBox ----------------
		// 187 = 800x600
		// 299 = 1024x768
		// 491 = 1680x1050
		imageBox_1 = gui.hCreateImageBox(NULL,stRECT(0,g_iScreenHeight - 299,g_iScreenWidth,299), "gfx\\gui\\Images\\Terain_Menu.png");
		gui.hCreateImageBox(NULL,stRECT(0,0,g_iScreenWidth,32), "gfx\\gui\\Images\\panel.png");

		gui.hCreateImageBox(NULL,stRECT(300,300,64,64), stSIZE(32,32), 10, "gfx\\gui\\Mouse\\cursor_main.png");
		gui.hCreateImageBox(NULL,stRECT(200,300,46,46), stSIZE(64,64), 25, "gfx\\explosion.png");
		gui.hCreateImageBox(NULL,stRECT(200,350,80,80), stSIZE(80,80), 10, "gfx\\gnomish_flying_machine.png");
		gui.hCreateImageBox(NULL,stRECT(100,350,80,80), stSIZE(72,72), 10, "gfx\\footman.png");


		// ------------------- Mouse ----------------
		g_stMouse.mouseType_1 = gui.hCreateMouse(stSIZE(32,32),stSIZE(32,32),50, "gfx\\gui\\Mouse\\cursor_main.png");
		g_stMouse.mouseType_2 = gui.hCreateMouse(stSIZE(32,32),stSIZE(32,32),10, "gfx\\gui\\Mouse\\Cur_1.png");
		g_stMouse.mouseType_3 = gui.hCreateMouse(stSIZE(32,32),stSIZE(32,32),10, "gfx\\gui\\Mouse\\arrow000.png");


		// ------------------- ScrollBar ----------------
		//Horizontal
		handleScrollBar_1 = gui.hCreateHorizScrollBar(-1,stRECT(550,300,400,20),stSliderInfo(0.0f, 0.0f, 255.0f, 5.0f),vSliderFunc_RED,-1, false);
			gui.vSetCurTextColor(D3DCOLOR_XRGB(127,0,0));
			gui.hCreateStaticText(NULL,"Red Value",620,300,20);

		handleScrollBar_2 = gui.hCreateHorizScrollBar(-1,stRECT(550,330,400,20),stSliderInfo(0.0f, 74.0f, 255.0f, 5.0f),vSliderFunc_GREEN,-1, false);
			gui.vSetCurTextColor(D3DCOLOR_XRGB(0,170,0));
			gui.hCreateStaticText(NULL,"Green Value",620,330,20);

		handleScrollBar_3 = gui.hCreateHorizScrollBar(-1,stRECT(550,360,400,20),stSliderInfo(0.0f, 127.0f, 255.0f, 5.0f),vSliderFunc_BLUE,-1, false);
			gui.vSetCurTextColor(D3DCOLOR_XRGB(0,0,255));
			gui.hCreateStaticText(NULL,"Blue Value",620,360,20);

			gui.vSetCurTextColor(D3DCOLOR_XRGB(0,255,255));
		handleScrollBar_4 = gui.hCreateHorizScrollBar(-1,stRECT(550,420,100,20),stSliderInfo(0.1f, 1.0f, 10.0f, 0.1f),vSliderFunc_SenMouseX,400, false);
			gui.hCreateStaticText(NULL,"Sens Mouse X",550,400,17);
		handleScrollBar_5 = gui.hCreateHorizScrollBar(-1,stRECT(700,420,100,20),stSliderInfo(0.1f, 1.0f, 10.0f, 0.1f),vSliderFunc_SenMouseY,400, false);
			gui.hCreateStaticText(NULL,"Sens Mouse Y",700,400,17);

		// Vertical
			handleScrollBar_6 = gui.hCreateVertScrollBar(-1,stRECT(1000,300,20,200),stSliderInfo(-100.0f, 0.0f, 100.0f, 1.0f),vSliderFunc_Vert_1,-1,false);
			gui.hCreateHorizScrollBar(-1,stRECT(10,250,200,25),stSliderInfo(-100, 1.0f, 100, 1.0f),vSliderFunc_Horiz_texture_1,-1, false,
										"gfx\\gui\\Scroll\\slider_knob_normal.png",
										"gfx\\gui\\Scroll\\slider_knob_selection.png",
										"gfx\\gui\\Scroll\\left_arrow_normal.png",
										"gfx\\gui\\Scroll\\left_arrow_selection.png",
										"gfx\\gui\\Scroll\\right_arrow_normal.png",
										"gfx\\gui\\Scroll\\right_arrow_selection.png",
										"gfx\\gui\\Scroll\\hslider_bar_normal.png");
			gui.hCreateVertScrollBar(-1,stRECT(220,100,25,200),stSliderInfo(-100, 1.0f, 100, 1.0f),vSliderFunc_Vert_texture_1,-1, false,
										"gfx\\gui\\Scroll\\slider_knob_normal.png",
										"gfx\\gui\\Scroll\\slider_knob_selection.png",
										"gfx\\gui\\Scroll\\up_arrow_normal.png",
										"gfx\\gui\\Scroll\\up_arrow_selection.png",
										"gfx\\gui\\Scroll\\down_arrow_normal.png",
										"gfx\\gui\\Scroll\\down_arrow_selection.png",
										"gfx\\gui\\Scroll\\vslider_bar_normal.png");

			// --------------- Создаём окна -------------------
			handleWindowPanel_1 = gui.hCreateWindowPanel(stRECT(130,130,100,100), "gfx\\gui\\Window\\buttonpanel.png", FILL_SCALE);
			handleWindowPanel_2 = gui.hCreateWindowPanel(stRECT(200,150,150,100), "gfx\\gui\\Window\\block_1.bmp", FILL_REPEAT);
			handleWindowPanel_3 = gui.hCreateWindowPanel(stRECT(365,412,400,200), "gfx\\gui\\Window\\1.png", FILL_SCALE);


			// -------------- Назначаем окнам элементы -----------------
			gui.hCreateImageBox(&handleWindowPanel_1,stRECT(0,0,80,80), stSIZE(80,80), 10, "gfx\\gnomish_flying_machine.png");
			gui.hCreateButton(&handleWindowPanel_1,"Button_1",stRECT(15,80,100,32),vButton_in_Window_3_1, true, TYPE_SELECTION_FRAME);	


			gui.vSetCurTextColor(D3DCOLOR_XRGB(0,0,0));
			gui.hCreateImageBox(&handleWindowPanel_2,stRECT(0,0,80,80), stSIZE(72,72), 10, "gfx\\footman.png");
			handleRadioButton_win_3 = gui.hCreateGroup(&handleWindowPanel_2);
			gui.vCreateRadioButton(&handleRadioButton_win_3,"M_1",stRECT(80,0,24,24),vRadioButton_win_5);
			gui.vCreateRadioButton(&handleRadioButton_win_3,"M_2",stRECT(80,30,24,24),vRadioButton_win_6);
			gui.vCreateRadioButton(&handleRadioButton_win_3,"M_3",stRECT(80,60,24,24),vRadioButton_win_7);



			gui.vSetCurTextColor(D3DCOLOR_XRGB(128,255,100));

			gui.hCreateImageBox(&handleWindowPanel_3,stRECT(0,0,80,80), stSIZE(72,72), 10, "gfx\\footman.png");
			gui.hCreateImageBox(&handleWindowPanel_3,stRECT(200,0,80,80), stSIZE(32,32), 10, "gfx\\diamond.tga");
			gui.hCreateImageBox(&handleWindowPanel_3,stRECT(80,10,32,32), "gfx\\8.bmp");
			gui.hCreateImageBox(&handleWindowPanel_3,stRECT(80,42,32,32), "gfx\\brick_1.png");
			gui.hCreateButton(&handleWindowPanel_3,"Button_1",stRECT(15,80,100,32),vButton_in_Window_3_1, true, TYPE_SELECTION_FRAME);
			gui.hCreateButton(&handleWindowPanel_3,"Img But_1",stRECT(15,120,100,32),vButton_4, true, TYPE_SELECTION_COLOR_FONT, "gfx\\gui\\Button\\Button_3.png");
			gui.hCreateButton(&handleWindowPanel_3,NULL,stRECT(15,160,32,32),vButton_7, "gfx\\gui\\Button\\button_2.png","gfx\\gui\\Button\\button_2_Sel.png");
			gui.hCreateCheckBox(&handleWindowPanel_3,"Спрятать texture Button",stRECT(50,160,24,24),vCheckBox_3, "gfx\\gui\\CheckBox\\Check_OFF_1.png", "gfx\\gui\\CheckBox\\Check_ON_1.png");
			gui.hCreateCheckBox(&handleWindowPanel_3,"CheckBox Auto",stRECT(120,120,24,24),vCheckBox_2);
			staticText_in_window_3_1 = gui.hCreateStaticText(&handleWindowPanel_3,"StaticText in Window", 120,10,20);
			
			handleRadioButton_win_1 = gui.hCreateGroup(&handleWindowPanel_3);
			gui.vCreateRadioButton(&handleRadioButton_win_1,"_1",stRECT(280,120,24,24),vRadioButton_win_1,"gfx\\gui\\RadioButton\\RadioButton_1_OFF.png", "gfx\\gui\\RadioButton\\RadioButton_1_ON.png");
			gui.vCreateRadioButton(&handleRadioButton_win_1,"_2",stRECT(340,120,24,24),vRadioButton_win_2,"gfx\\gui\\RadioButton\\RadioButton_1_OFF.png", "gfx\\gui\\RadioButton\\RadioButton_1_ON.png");
			
			handleRadioButton_win_2 = gui.hCreateGroup(&handleWindowPanel_3);
			gui.vCreateRadioButton(&handleRadioButton_win_2,"_3",stRECT(280,150,24,24),vRadioButton_win_3);
			gui.vCreateRadioButton(&handleRadioButton_win_2,"_4",stRECT(340,150,24,24),vRadioButton_win_4);

			gui.vRestoreTextColor();
	gui.vEndInit();

	font = new FONT(&directx, 20,0,false,"Times New Roman Cyr");
	// -------  LOAD game data (end) ----------

	//SendMessage(hWnd,WM_SETFOCUS,0,0);

	return true;
}

void vFPS()
{
	//g_stFPS;
		g_stFPS.iFPS++;
		if(GetTickCount() - g_stFPS.iBegTime >= 1000)
		{
			g_stFPS.iBegTime = GetTickCount();
			sprintf(g_stFPS.cFPS,"%d",g_stFPS.iFPS);
			g_stFPS.iFPS =0;
			
			g_font.vGetTextSizeRect(g_stFPS.cFPS, &g_stFPS.rect,g_stFPS.nameFont);

			//gui.bReplaceStaticText(&staticText_FPS, g_stFPS.cFPS);
		}
}

void vKeyboard()
{
	if(GetAsyncKeyState(VK_ESCAPE))
		g_bExit = true;
}
 // ==================================  Mouse (begin) ===============================
// Нажатие левой клавиши
inline bool bMouseLBUp()
{
	return false;
}

// Отпускание левой клавиши
inline bool bMouseLBDown()
{
	if(GetAsyncKeyState(VK_LBUTTON))
		return true;
	
	return false;
}

// Проверяет был ли клик левой клавишей мыши
inline bool bMouseLBClick()
{
	static bool bRunLB = false;

	// Обработка нажатий 
	if(!GetAsyncKeyState(VK_LBUTTON) && bRunLB)
	{
		bRunLB = false;
		g_stMouse.pointLBUpPos.x = (long)g_stMouse.vectorPos.x;
		g_stMouse.pointLBUpPos.y = (long)g_stMouse.vectorPos.y;
		return true;
	}
	if(GetAsyncKeyState(VK_LBUTTON) && (!bRunLB))
	{
		bRunLB = true;
		g_stMouse.pointLBDownPos.x = (long)g_stMouse.vectorPos.x;
		g_stMouse.pointLBDownPos.y = (long)g_stMouse.vectorPos.y;
	}
	return false;
}

void vMouse()
{	
	// ----------------- Получем Позицию мыши ---------------------------
	GetCursorPos(&g_stMouse.pointScreenNewPos);
	g_stMouse.fDeltaX = (float)g_stMouse.pointScreenNewPos.x - g_stMouse.pointScreenOldPos.x;
	g_stMouse.fDeltaY = (float)g_stMouse.pointScreenNewPos.y - g_stMouse.pointScreenOldPos.y;

	if( (g_stMouse.fDeltaX != 0) || (g_stMouse.fDeltaY != 0) )
	{
		//  если координаты мышки изменились считаем

		g_stMouse.vectorPos.x = g_stMouse.vectorPos.x + g_stMouse.fDeltaX * g_stMouse.fSensitivity_x;
		g_stMouse.vectorPos.y = g_stMouse.vectorPos.y + g_stMouse.fDeltaY * g_stMouse.fSensitivity_y;

		
		//g_stMouse.vectorPos = D3DXVECTOR2(x,y);

		//g_anim.vSetObjectPos(0,g_stMouse.IDTextureTypeMouse_1, g_stMouse.vectorPos);

		// перемещаем мышку (абсолютные координаты) в середину окна
		SetCursorPos(g_iScreenWidth/2 + g_iFrameWindowWidth, g_iScreenHeight/2 + g_iFrameWindowHeight);
		
		GetCursorPos(&g_stMouse.pointScreenNewPos);
		g_stMouse.pointScreenOldPos.x = g_stMouse.pointScreenNewPos.x;
		g_stMouse.pointScreenOldPos.y = g_stMouse.pointScreenNewPos.y;

		// если выходит за края екрана
		if(g_stMouse.vectorPos.x > g_iScreenWidth)
			g_stMouse.vectorPos.x -= g_stMouse.fDeltaX * g_stMouse.fSensitivity_x;
		if(g_stMouse.vectorPos.x < 0 )
			g_stMouse.vectorPos.x -= g_stMouse.fDeltaX * g_stMouse.fSensitivity_x;
		if(g_stMouse.vectorPos.y > g_iScreenHeight)
			g_stMouse.vectorPos.y -= g_stMouse.fDeltaY * g_stMouse.fSensitivity_y;
		if(g_stMouse.vectorPos.y < 0)
			g_stMouse.vectorPos.y -= g_stMouse.fDeltaY * g_stMouse.fSensitivity_y;

		//g_anim.vSetObjectPos(0,g_stMouse.IDTextureTypeMouse_1, g_stMouse.vectorPos);
		//gui.bSetObjectPos(g_stMouse.vectorPos, &g_stMouse.mouseType_1);
		//sprintf(text,"X = %f, Y = %f",g_stMouse.vectorPos.x,g_stMouse.vectorPos.y);
	}


	// ----------------- Обрабатываем сообщения  мыши ---------------------------

	//  заполняем и передаём структуру  в GUI
	if(bMouseLBClick())
	{
		/*mouseInfoGui.pointLBDownPos.x = g_stMouse.pointLBDownPos.x;
		mouseInfoGui.pointLBDownPos.y = g_stMouse.pointLBDownPos.y;
		mouseInfoGui.pointLBUpPos.x = g_stMouse.pointLBUpPos.x;
		mouseInfoGui.pointLBUpPos.y = g_stMouse.pointLBUpPos.y;*/

		mouseInfoGui.bLButtonClick = true;
	}
	if(bMouseLBDown())
		mouseInfoGui.bLButtonDown = true;
	else
		mouseInfoGui.bLButtonDown = false;

	// Заполняем структуру и передаём GUI
	mouseInfoGui.pointLBDownPos.x = g_stMouse.pointLBDownPos.x;
	mouseInfoGui.pointLBDownPos.y = g_stMouse.pointLBDownPos.y;
	mouseInfoGui.pointLBUpPos.x = g_stMouse.pointLBUpPos.x;
	mouseInfoGui.pointLBUpPos.y = g_stMouse.pointLBUpPos.y;
	mouseInfoGui.pointMousePos.x = (long)g_stMouse.vectorPos.x;
	mouseInfoGui.pointMousePos.y = (long)g_stMouse.vectorPos.y;
}
// ==================================  Mouse (end) ===============================



// ============================== BUTTONS ================
void vButton_1()
{
	static int n_1 = 0;
	sprintf(text, "Click Button_1 = %d", n_1);
	gui.bReplaceStaticText(&staticText_1, text);
	n_1++;
}
void vButton_2()
{
	static int n_2 = 0;
	sprintf(text, "Click Button_2 = %d", n_2);
	gui.bReplaceStaticText(&staticText_1, text);
	n_2++;
}
void vButton_3()
{
	static int n_3 = 0;
	sprintf(text, "Click Button_3 = %d", n_3);
	gui.bReplaceStaticText(&staticText_1, text);
	n_3++;
}
void vButton_4()
{
	static int n_4 = 0;
	sprintf(text, "Click Button_4 = %d", n_4);
	gui.bReplaceStaticText(&staticText_1, text);
	n_4++;
}
void vButton_5()
{
	static int n_5 = 0;
	sprintf(text, "Click Button_5 = %d", n_5);
	gui.bReplaceStaticText(&staticText_1, text);
	n_5++;
}
void vButton_6()
{
	static int n_6 = 0;
	sprintf(text, "Click Button_6 = %d", n_6);
	gui.bReplaceStaticText(&staticText_1, text);
	n_6++;
}
void vButton_7()
{
	static int n_7 = 0;
	sprintf(text, "Click Button_7 = %d", n_7);
	gui.bReplaceStaticText(&staticText_1, text);
	n_7++;
}
void vButton_8()
{
	static int n_8 = 0;
	sprintf(text, "Click Button_8 = %d", n_8);
	gui.bReplaceStaticText(&staticText_1, text);
	n_8++;
}
void vCheckBox_1(bool bCheckBoxOn)
{
	if(bCheckBoxOn)
	{
		gui.bHide(&button_1);
		gui.bHide(&checkBox_3);
		gui.bHide(&imageBox_1);
		gui.bSetMouse(&g_stMouse.mouseType_2);
		gui.bHide(&handleGroup_1);
		gui.bHide(&handleScrollBar_1);
		gui.bHide(&handleScrollBar_6);
	}
	else
	{
		gui.bShow(&button_1);
		gui.bShow(&checkBox_3);
		gui.bShow(&imageBox_1);
		gui.bSetMouse(&g_stMouse.mouseType_1);
		gui.bShow(&handleGroup_1);
		gui.bShow(&handleScrollBar_1);
		gui.bShow(&handleScrollBar_6);
	}
}
void vCheckBox_2(bool bCheckBoxOn)
{
	if(bCheckBoxOn)
	{
		gui.bHide(&button_2);
	}
	else
	{
		gui.bShow(&button_2);
	}
}
void vCheckBox_3(bool bCheckBoxOn)
{
	if(bCheckBoxOn)
	{
		gui.bHide(&button_3);
	}
	else
	{
		gui.bShow(&button_3);
	}
}

void vRadioButton_1(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		sprintf(text, "RadioButton_1");
		gui.bReplaceStaticText(&staticText_1, text);
	}
	else
	{
	}
}

void vRadioButton_2(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		sprintf(text, "RadioButton_2");
		gui.bReplaceStaticText(&staticText_1, text);
	}
	else
	{
	}
}

void vRadioButton_3(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		sprintf(text, "RadioButton_3");
		gui.bReplaceStaticText(&staticText_1, text);
	}
	else
	{
	}
}

void vRadioButton_4(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		sprintf(text, "RadioButton_4");
		gui.bReplaceStaticText(&staticText_1, text);
	}
	else
	{
	}
}

void vRadioButton_5(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		sprintf(text, "RadioButton_5");
		gui.bReplaceStaticText(&staticText_1, text);
	}
	else
	{
	}
}

void vRadioButton_6(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		sprintf(text, "RadioButton_6");
		gui.bReplaceStaticText(&staticText_1, text);
	}
	else
	{
	}
}
void vSliderFunc_RED(stSliderInfo *pSliderInfo)
{
	g_iColorClearRed = (int)pSliderInfo->m_fCurValue;
	
	//sprintf(text, "RED = %d", g_iColorClearRed);
	//gui.bReplaceStaticText(&staticText_1, text);
}
void vSliderFunc_GREEN(stSliderInfo *pSliderInfo)
{
	g_iColorClearGreen = (int)pSliderInfo->m_fCurValue;
}
void vSliderFunc_BLUE(stSliderInfo *pSliderInfo)
{
	g_iColorClearBlue = (int)pSliderInfo->m_fCurValue;
}
void vSliderFunc_SenMouseX(stSliderInfo *pSliderInfo)
{
	g_stMouse.fSensitivity_x = pSliderInfo->m_fCurValue;
}
void vSliderFunc_SenMouseY(stSliderInfo *pSliderInfo)
{
	g_stMouse.fSensitivity_y = pSliderInfo->m_fCurValue;
}
void vSliderFunc_Vert_1(stSliderInfo *pSliderInfo)
{
	//sprintf(text, "SliderVert = %d", int(pSliderInfo->m_fCurValue));
	//gui.bReplaceStaticText(&staticText_1, text);
}
void vSliderFunc_Horiz_texture_1(stSliderInfo *pSliderInfo)
{
	/*sprintf(text, "min = %d, cur = %d, max = %d", int(pSliderInfo->m_fMinValue),
												int(pSliderInfo->m_fCurValue), 
												int(pSliderInfo->m_fMaxValue));
	gui.bReplaceStaticText(&staticText_1, text);
	*/
}
void vSliderFunc_Vert_texture_1(stSliderInfo *pSliderInfo)
{
	/*sprintf(text, "min = %d, cur = %d, max = %d", int(pSliderInfo->m_fMinValue),
		int(pSliderInfo->m_fCurValue), 
		int(pSliderInfo->m_fMaxValue));
	gui.bReplaceStaticText(&staticText_1, text);
	*/
}

void vButton_in_Window_3_1()
{
	static int n_3_1 = 0;
	sprintf(text, "Click Button_1 in window = %d", n_3_1);
	gui.bReplaceStaticText(&staticText_in_window_3_1, text);
	n_3_1++;
}
void vRadioButton_win_1(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		sprintf(text, "RadioButton_1");
		gui.bReplaceStaticText(&staticText_in_window_3_1, text);
	}
	else
	{
	}
}

void vRadioButton_win_2(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		sprintf(text, "RadioButton_2");
		gui.bReplaceStaticText(&staticText_in_window_3_1, text);
	}
	else
	{
	}
}
void vRadioButton_win_3(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		sprintf(text, "RadioButton_3");
		gui.bReplaceStaticText(&staticText_in_window_3_1, text);
	}
	else
	{
	}
}
void vRadioButton_win_4(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		sprintf(text, "RadioButton_4");
		gui.bReplaceStaticText(&staticText_in_window_3_1, text);
	}
	else
	{
	}
}

void vRadioButton_win_5(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		gui.bSetMouse(&g_stMouse.mouseType_1);
	}
	else
	{
	}
}
void vRadioButton_win_6(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		gui.bSetMouse(&g_stMouse.mouseType_2);
	}
	else
	{
	}
}
void vRadioButton_win_7(bool bRadioButtonOn)
{
	if(bRadioButtonOn)
	{
		gui.bSetMouse(&g_stMouse.mouseType_3);
	}
	else
	{
	}
}