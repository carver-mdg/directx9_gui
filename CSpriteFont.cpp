#include "CSpriteFont.h"

CSpriteFont::CSpriteFont()
{
	m_iQuantFonts = 0;
}

CSpriteFont::~CSpriteFont()
{
	for(int i =0; i < m_iQuantFonts; i++)
		if(m_stFont[i].pTexture != NULL)
			m_stFont[i].pTexture->Release();
	
	fprintf(m_directx->m_file_report,"CSpriteFont::Деструктор \n");
}

void CSpriteFont::vSetDirectX(DXData *directX)
{
	m_directx = directX;
}


int  CSpriteFont::iAddFont(char *name_file)
{
	stFont font;
	FILE *file;
	int height_letter = 0;

	// считывание из файла (begin)
		int iQuantSymName =0; // количество символов в имени файла
			iQuantSymName = (int)strlen(name_file);
		char *text = new char[iQuantSymName +50];
		sprintf(text,"%s",name_file);

		// подставляем формат файла сопровождения
		text[iQuantSymName -3] = 't'; 
		text[iQuantSymName -2] = 'x';
		text[iQuantSymName -1] = 't';

		file = fopen(text,"r");
			// чтение данных

			// ; Font params:
			fgets(text,50,file);
			fgets(text,50,file);
				sscanf(text,"Font=%s",&font.cName);
			fgets(text,50,file);
			fgets(text,50,file);
			fgets(text,50,file);
			fgets(text,50,file);
				sscanf(text,"Height=%d",&height_letter);
			fgets(text,50,file);
			fgets(text,50,file);
			fgets(text,50,file);

			// ; Texture params:
			fgets(text,50,file);
			fgets(text,50,file);
			fgets(text,50,file);
			fgets(text,50,file);
			fgets(text,50,file);
			fgets(text,50,file);

			// ; Simbol params (x,y,width):
			fgets(text,50,file);
				for(int i =0; i < 224; i++)
				{
					fgets(text,50,file);
					char sym;
					sscanf(text,"%c=%d,%d,%d", &sym, &font.letter[i].xpos, &font.letter[i].ypos, &font.letter[i].width);
					font.letter[i].height = height_letter;
				}
		fclose(file);

		//delete[] text;
	// считывание из файла (end)


	// Загружаем текстуру
	// подставляем формат файла сопровождения
	sprintf(text,"%s",name_file);

	text[iQuantSymName -3] = 'p'; 
	text[iQuantSymName -2] = 'n';
	text[iQuantSymName -1] = 'g';
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,text,&font.pTexture);
	delete[] text;

	font.ID = m_iQuantFonts;

	m_stFont.push_back(font);
	m_iQuantFonts++;
	return m_iQuantFonts -1;   // ID шрифта
}


void inline CSpriteFont::vReplaceColor(LPDIRECT3DTEXTURE9 *texture, DWORD colorSrc, DWORD colorDest)
{
	D3DSURFACE_DESC	d3dsd;
	(*texture)->GetLevelDesc(0,&d3dsd);

	D3DLOCKED_RECT	d3dlr;
	(*texture)->LockRect(0,&d3dlr,0,0);
	DWORD *colorCur = (DWORD*)d3dlr.pBits;

	for(unsigned int j=0; j< d3dsd.Height; j++)
		for(unsigned int i=0; i< d3dsd.Width; i++)
		{
			if(colorCur[i*d3dsd.Height + j] == colorSrc)
				colorCur[i*d3dsd.Height + j] = colorDest;
		}
	(*texture)->UnlockRect(0);
}


void CSpriteFont::vSetColor(DWORD dwColorFont, DWORD dwColorDraw, char *nameFont)
{
	int IDFont = CSpriteFont::iFindFont(nameFont);
	if(IDFont < 0)
		return;

	if( (dwColorFont != 0) && (m_stFont[IDFont].dwColorFont != dwColorFont) )
	{
		CSpriteFont::vReplaceColor(&m_stFont[IDFont].pTexture,m_stFont[IDFont].dwColorFont,dwColorFont);
		m_stFont[IDFont].dwColorFont = dwColorFont;
	}

	if( (dwColorDraw != 0) && (m_stFont[IDFont].dwColorDraw != dwColorDraw) )
	{
		m_stFont[IDFont].dwColorDraw = dwColorDraw;
	}
}


// ---------- FIND ID FONT (begin) -------
bool inline CSpriteFont::bCompareString(char *str_1, char *str_2)
{
	int iMinQuantSym =0;
	(strlen(str_1) < strlen(str_2)) ? (iMinQuantSym = (int)strlen(str_1)) : (iMinQuantSym = (int)strlen(str_2));
	
	for(int i =0; i < iMinQuantSym; i++)
	{
		if( str_1[i] != str_2[i] )
			return false;
	}

	return true;
}

int inline CSpriteFont::iFindFont(char *name)
{
	int iMinQuantSym =0;

	for(int i =0; i < m_iQuantFonts; i++)
	{
		if(bCompareString(m_stFont[i].cName, name))
			return m_stFont[i].ID;
	}
	
	return -1; // false
}
// ---------- FIND ID FONT (end) -------

void CSpriteFont::vDrawText(char *text, int x,int y, char *name )
{
	if( (x > (int)m_directx->m_d3dpp.BackBufferWidth) ||
		(y > (int)m_directx->m_d3dpp.BackBufferHeight) ||
		(x<0) ||
		(y<0)
		)
		return;


	//m_vectorScale = D3DXVECTOR2(1.0f, 1.0f);

	int IDFont = CSpriteFont::iFindFont(name);
	if(IDFont < 0)
		return;


	//m_sprite->Begin();
		for(int i =0, code_sym =0; i < (int)strlen(text);  i++)
		{
			code_sym = (int)text[i];
			if(code_sym < 0)
				code_sym = 256 + code_sym - 32;
			else
				code_sym -= 32;
			
			m_rect.top = m_stFont[IDFont].letter[code_sym].ypos;
			m_rect.left = m_stFont[IDFont].letter[code_sym].xpos;
			m_rect.bottom = m_rect.top + m_stFont[IDFont].letter[code_sym].height;
			m_rect.right = m_rect.left + m_stFont[IDFont].letter[code_sym].width;

			// Позиция очередной буквы
			m_vectorTrans = D3DXVECTOR2((float)x, (float)y);
			x += (int)(m_stFont[IDFont].letter[code_sym].width * m_stFont[IDFont].vectorScale.x);

			m_directx->m_sprite->Draw(m_stFont[IDFont].pTexture,&m_rect,&m_stFont[IDFont].vectorScale,0,0,&m_vectorTrans, m_stFont[IDFont].dwColorDraw);
		}
	//m_sprite->End();
}

void CSpriteFont::vGetTextSizeRect(char *text, RECT *rect, char *nameFont)
{
	int IDFont = CSpriteFont::iFindFont(nameFont);
	if(IDFont < 0)
		return;

	//memset(&rect,0,sizeof(RECT));
	(*rect).bottom =0;
	(*rect).left = 0;
	(*rect).right = 0;
	(*rect).top = 0;


	for(int i =0, code_sym =0; i < (int)strlen(text);  i++)
		{
			code_sym = (int)text[i];
			if(code_sym < 0)
				code_sym = 256 + code_sym - 32;
			else
				code_sym -= 32;


			// Позиция очередной буквы
			(*rect).left = 0;
			(*rect).top = 0;
			(*rect).right += (long)(m_stFont[IDFont].letter[code_sym].width * m_stFont[IDFont].vectorScale.x);
			(*rect).bottom = (long)(m_stFont[IDFont].letter[code_sym].height);
		}
}
