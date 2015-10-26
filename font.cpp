//font.cpp
//--------------------------------------------------------

#include "font.h"

//--------------------------------------------------------
//создание фонта
FONT::FONT(DXData *directx, int SizeFont,DWORD bold,bool italic,char* nameFont)
{
	m_directx = directx;

	TCHAR str[2];//строка ( всего одна буква )
	str[1]=0;//обозначить конец строки
	BYTE c;//номер символа
    
	HDC hDC;
    hDC = CreateCompatibleDC( NULL );
    SetMapMode( hDC, MM_TEXT );

    HBITMAP hbmBitmap = NULL;
    HGDIOBJ hbmOld = NULL;

    // Prepare to create a bitmap
    DWORD*      pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory( &bmi.bmiHeader, sizeof(BITMAPINFOHEADER) );
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = SizeFont;
    bmi.bmiHeader.biHeight      = -SizeFont;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount    = 32;

    // Create a bitmap for the font
    hbmBitmap = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS,(void**)&pBitmapBits, NULL, 0 );

    SelectObject( hDC, hbmBitmap );
    
    // Set text properties
    SetTextColor( hDC, RGB(255,255,255) );
    SetBkColor(   hDC, 0x00000000 );
    SetTextAlign( hDC, TA_TOP );
	
	//создать фонт
 	HFONT hfont=CreateFont(SizeFont,0,0,0,bold,italic,false,false,
			ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,VARIABLE_PITCH,
			nameFont);//задать имя фонта

	if(hfont==NULL) goto fn;//если фонт не создан выйти с ошибкой
		//--------------
	//установить фонт
    SelectObject( hDC, hfont );
    //установить характеристики фонта
    SetTextColor( hDC, RGB(255,255,255) );
    SetBkColor(   hDC, 0x00000000 );
    SetTextAlign( hDC, TA_TOP );

    SIZE size;//структура описания размера символа

	for( c =255; c >0; c-- )//номер символа в фонте
    {
  		//создание текстуры для фонта 
		m_directx->m_pD3DDevice->CreateTexture(SizeFont,SizeFont, 1,0,D3DFMT_A8R8G8B8,//формат текстуры
                                      D3DPOOL_MANAGED, &texture[c], NULL );
		if(texture[c]==NULL)goto fn;
		//-----------------------------------

		str[0] = c;

		if(0==GetTextExtentPoint32( hDC, str, 1, &size ))goto fn;//берем размеры символа
			
		width[c] = (FLOAT)size.cx;//записываем размеры символа
		height = (FLOAT)size.cy;

		if(0==ExtTextOut( hDC, 0, 0, ETO_OPAQUE, NULL, str, 1, NULL ))goto fn;//вывод символа в битмэп

		D3DLOCKED_RECT rect;
		//заблокировать текстуру
		texture[c]->LockRect( 0, &rect, 0, 0 );

		BYTE* pDstRow = (BYTE*)rect.pBits;//адрес текстуры
		DWORD* pDst16;
		BYTE bAlpha; // 4-bit measure of pixel intensity
		DWORD x, y;

		//перенести из битмэпа в текстуру
		for( y=0; y < (DWORD)SizeFont; y++ )
		{
			pDst16 = (DWORD*)pDstRow;//адрес ряда в текструре
			for( x=0; x < (DWORD)SizeFont; x++ )
			{
				//адрес пиксела в битмэпе
				bAlpha = (BYTE)((pBitmapBits[SizeFont*y + x] & 0xff) >> 4);
				//увеличить по Х
				*pDst16++= D3DCOLOR_ARGB(bAlpha*14,255,255,255);//записать пиксел
			}
			pDstRow += rect.Pitch;//увеличить по - Y
		}

		if(texture[c]) texture[c]->UnlockRect(0);//разблокировать
	}
	//удалить ненужное
fn: DeleteObject( hbmBitmap );
    DeleteObject( hfont );
    DeleteDC( hDC );
};
//вывод строки
int FONT::print(float x,float y,DWORD color,char* s)
{
	if(s == NULL)
		return 0;

	byte ch;
	int start = (int)x;
	D3DXVECTOR2 vecScaling(1,1);  // масштаб
	D3DXVECTOR2 vecTrans(0,0);	  // Translation

	RECT rect;//параметры копируемого прямоугольника
	rect.left=0;
	rect.top=0;

	UINT len = (UINT)strlen(s);
	for(UINT i=0;i<len;++i)
	{
		ch=s[i];

		if(ch=='\n')
		{
			x = (float)start;
			y+=height;
			continue;
		}

        if(ch < 0 || ch > 255)continue;

		rect.right = (long)width[ch];
		rect.bottom = (long)height;

		// куда рисовать
		vecTrans.x = x;
		vecTrans.y = y;

		//вывод спрайта
		m_directx->m_sprite->Draw(texture[ch],&rect,&vecScaling,NULL,0,&vecTrans,color);
		x+=rect.right;//сместиться на ширину символа
	}return (int)x-start;//передать длину строки
};
//вывод строки с клипированием
void FONT::print(float x,float y,DWORD color,char* s,RECT clip)
{
	if(s == NULL)
		return;

	byte ch;
	int start = (int)x;
	D3DXVECTOR2 vecScaling(1,1);  // масштаб
	D3DXVECTOR2 vecTrans(0,0);	  // Translation

	RECT rect;//параметры копируемого прямоугольника
	rect.left=0;
	rect.top=0;

	UINT len = (UINT)strlen(s);
	for(UINT i=0;i<len;++i)
	{
		ch=s[i];

		if(ch=='\n')//обработка знака ENTER
		{
			x = (float)start;
			y+=height;//смещаемся вниз
			continue;
		}

        if(ch < 0 || ch > 255)continue;

		rect.right = (long)width[ch];
		rect.bottom = (long)height;

		int px=(int)x,py=(int)y,rx=0,ry=0,w=rect.right,h=rect.bottom;
		int c_x=clip.left,c_y=clip.top,c_w=clip.right,c_h=clip.bottom;
		//клипирование(урезание невидимых частей) выводимого прямоугольника
		if(px<c_x){ rx=rx+(c_x-px);w=w-(c_x-px);px=c_x;}
		if(py<c_y){ ry=ry+(c_y-py);h=h-(c_y-py);py=c_y;}
		if(px+w>c_x+c_w)w=c_w-(px-c_x);
		if(py+h>c_y+c_h)h=c_h-(py-c_y);
		//---------------------
		// куда рисовать
		vecTrans.x = (float)px;
		vecTrans.y = (float)py;

		rect.left=rx,rect.top=ry,rect.right=rx+w,rect.bottom=ry+h;
		//вывод спрайта
		m_directx->m_sprite->Draw(texture[ch],&rect,&vecScaling,NULL,0,&vecTrans,color);
		x+=rect.right;//сместиться на ширину символа
	}
};
FONT::~FONT()
{
	/*height=NULL;
	for(int i=0;i<256;i++)
	{
		width[i]=NULL;
		_RELEASE_(texture[i]);
	}*/

	for(int  c =255; c >0; c-- )//номер символа в фонте
		_RELEASE_(texture[c]);
};

void FONT::vRelease()
{
	//height=NULL;
	for(int i=0; i<1; i++)
	{
		//width[i]=NULL;
		//_RELEASE_(texture[i]);
		if(texture[i])
		{
			//_RELEASE_(texture[i]);
		}
		//SafeExceptionRelease(&texture[i]);
	}

	for(int  c =255; c >0; c-- )//номер символа в фонте
		_RELEASE_(texture[c]);
}
//----------------------------------------
