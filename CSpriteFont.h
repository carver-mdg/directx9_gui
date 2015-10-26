#ifndef _CSPRITEFONT_H_
#define _CSPRITEFONT_H_

/*
#include <stdio.h>
#include <string.h>
#include <vector>
*/
#include "DXData.h"

using namespace std;


struct stLetter
{
	char name[30];

	// Положение на текстуре
	int xpos;
	int ypos;
	int width;
	int height;
};

struct stFont
{
	stFont()
	{
		vectorScale = D3DXVECTOR2(1.0f, 1.0f);
		dwColorFont = D3DCOLOR_XRGB(255,255,255);
		dwColorDraw = D3DCOLOR_ARGB(255, 255,255,255);

		pTexture = NULL;
	};

	char cName[50];  // имя шрифта
	int  ID;

	LPDIRECT3DTEXTURE9 pTexture;
	stLetter letter[224]; // 224 буквы в шрифте
	D3DXVECTOR2  vectorScale;

	DWORD dwColorFont; // цвет шрифта
	DWORD dwColorDraw; // цвет вывода draw ( с альфой )
};



class CSpriteFont
{
	private:
		DXData *m_directx;
		vector<stFont> m_stFont;

		RECT		 m_rect;
		D3DXVECTOR2  m_vectorTrans;
		//D3DXVECTOR2  m_vectorScale;
		RECT m_rectDrawText;

		int m_iQuantFonts; // количество шрифтов
	public:
		CSpriteFont();
		~CSpriteFont();

		void vSetDirectX(DXData *directX);
		int  iAddFont(char *name_file);
		void inline vReplaceColor(LPDIRECT3DTEXTURE9 *texture, DWORD colorSrc, DWORD colorDest);
		bool inline bCompareString(char *str_1, char *str_2); 
		void vSetColor(DWORD dwColorFont, DWORD dwColorDraw, char *nameFont);
		int inline iFindFont(char *name);
		
		void vDrawText(char *text, int x,int y, char *name);  // Просто выводит текст
		void vGetTextSizeRect(char *text, RECT *rect, char *nameFont); // Получает прямоугольник куда вписан текст
};

#endif //_CSPRITEFONT_H_