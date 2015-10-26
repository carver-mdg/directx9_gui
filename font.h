//font.h
#ifndef _FONT_H_
#define _FONT_H_

#include "DXData.h"

//структура для фонта
class FONT
{
private:
	DXData *m_directx;
	FLOAT height;
	FLOAT width[260];
	LPDIRECT3DTEXTURE9      texture[260];//текстура для фонта
public:
	//длина строки и высота шрифта в пикселах
	SIZE strlong(char* s)
	{
		SIZE size;
		float x=0;

		UINT len = (UINT)strlen(s);
		for(UINT i=0;i<len;++i)
		{
			byte ch;
			ch=s[i];

			if(ch=='\n')x=0;

			if(ch < 0 || ch > 255)continue;

			x+=width[ch];
		}
		size.cx = static_cast<int>(x);//длина строки
		size.cy = static_cast<int>(height);//высота фонта
		return size;//вернуть результат
	}
	//вывод текста
	int print(float x,float y,DWORD color,char* s);
	//вывод строки с клипированием
	void print(float x,float y,DWORD color,char* s,RECT clip);

	void vRelease();

	//конструктор
	FONT(DXData *directx, int SizeFont,DWORD bold,bool italic,char* nameFont);
	//деструктор
	~FONT();
};
//-------------------------------------------------------

#endif  // _FONT_H_