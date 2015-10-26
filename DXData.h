#ifndef _DXDATA_H_
#define _DXDATA_H_


#include <d3d9.h>
#include <d3dx9.h>

#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include "squall.h"


#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Squall.lib")
#pragma comment(lib,"Squalld.lib")


#define _RELEASE_(p) { if(p) { (p)->Release(); (p)=NULL; };};
#define _DELETE_(p)  { if(p) { delete (p);     (p)=NULL; };};


// Get color in DWORD
#define getA(x) ((x&0xff000000)>>24)
#define getR(x) ((x&0x00ff0000)>>16)
#define getG(x) ((x&0x0000ff00)>>8)
#define getB(x)  (x&0x000000ff)

/* ================== Структуры (begin)===================== */
struct stRECT
{
	stRECT()
	{
		iPosX = 0;
		iPosY = 0;
		iWidth = 0;
		iHeight = 0;
	};
	stRECT(int posX, int posY, int width, int height)
	{
		iPosX = posX;
		iPosY = posY;
		iWidth = width;
		iHeight = height;
	};

	/*stRECT& operator = (stRECT &rect)
	{
	};*/

	int iPosX; // позиция по X
	int iPosY; // позиция по Y
	int iWidth; // Ширина
	int iHeight; // Высота
};

struct stSIZE
{
	stSIZE()
	{
		cx = 0;
		cy = 0;
	};

	stSIZE(int x, int y)
	{
		cx = x;
		cy = y;
	};

	int cx;
	int cy;
};
/* ================== Структуры (end)===================== */

class DXData
{
	public:
		LPDIRECT3D9				m_pD3D;
		LPDIRECT3DDEVICE9		m_pD3DDevice;
		D3DPRESENT_PARAMETERS	m_d3dpp;
		D3DDISPLAYMODE			m_d3ddm;
		LPD3DXSPRITE			m_sprite;
		LPD3DXLINE				m_pLine;

		/*
		D3DPRESENT_PARAMETERS present;
		bool initialized;
		HWND hWindow;
		bool fullscreen;
		LPDIRECT3D9 direct3D;
		LPDIRECT3DDEVICE9 device;
		int width;
		int height;
		*/

		FILE					*m_file_report;


	public:
		DXData();
		~DXData();

		bool bInitD3D(HWND hWnd, int iWidth, int iHeight,bool bFullScreen);
		bool bDestroyDirect3D9();
		void vResetDevice();
		LPDIRECT3DDEVICE9 *GetDevice();
		LPDIRECT3D9 *GetDirect();
		HRESULT hrScreenShot(char *name);
		
		// Для работы с *.ini файлами  ( Считывание из директории *.EXE ФАЙЛА )
		char m_cDirectory_exe [400];  // директория *.exe файла
		void vReadIntegerInIni(char *pcSection, char *pcKey, char *pcPath_ini, int &iValue);
		void vReadFloatInIni(char *pcSection, char *pcKey, char *pcPath_ini, float &fValue);
		void vReadStringInIni(char *pcSection, char *pcKey, char *pcPath_ini, char *pcValue);
		void vReadBoolInIni(char *pcSection, char *pcKey, char *pcPath_ini, bool &bValue);

		// Для работы с строками текста
		bool bCompareString(char *str_1, char *str_2);  // сравнивает строки

		// Для чисел
		void vSortArrayIncrease(int* s_arr,int first, int last);

		// Получает информацию о видеокарте и записывает её в файл "Report_VGA.txt"
		void vGetInfoVideoCard();
		int iConvert_volume_to_squall(int iVolInProz);


		D3DXVECTOR3 vec3RelativePosition(const D3DXVECTOR3 *src,const D3DXVECTOR3 *origin,
			const D3DXVECTOR3 *target,const D3DXVECTOR3 *upvector);

		// Перемещает камеру
		void vSetCameraShift(D3DXVECTOR3 *cam,D3DXVECTOR3 *camtarg,
			const D3DXVECTOR3 *shift,const D3DXVECTOR3 *upvector);
		void vSetCameraRotationX(const D3DXVECTOR3 *cam,D3DXVECTOR3 *camtarg,
			const D3DXVECTOR3 *upvector, const float angle);
		void vSetCameraRotationY(const D3DXVECTOR3 *cam,D3DXVECTOR3 *camtarg,
			const D3DXVECTOR3 *upvector, const float angle);

		void vEffects(LPDIRECT3DTEXTURE9 *texture);
		void vDrawPoint(stSIZE pos, D3DCOLOR color);
		void vDrawCircle(int x0, int y0, int radius, D3DCOLOR color);
		void vDrawLine(int x1, int y1, int x2, int y2, D3DCOLOR color);
		void vDrawEllipse(int x, int y, int a, int b, D3DCOLOR color);
};

#endif // _DXDATA_H_