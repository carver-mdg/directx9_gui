#include "CManagerGUI.h"

CManagerGUI::CManagerGUI()
{
	m_pIndexBuffer = NULL;
	m_pVertexBuffer = NULL;

	m_iCurNumMouse = 0;
	m_bCreatedMouse = false;

	m_iQuantWindowsPanel = 0;
	m_iQuantMouse = 0;
	m_iQuantButtons = 0;
	m_iQuantCheckBox = 0;
	m_iQuantRadioButton = 0;
	m_iQuantStaticText = 0;
	m_iQuantImageBox = 0;
	m_iQuantGroup = 0;
	m_iQuantHorizScrollBar = 0;
	m_iQuantVertScrollBar = 0;

	m_colorTextDefault_forRestore = D3DCOLOR_XRGB(0,255,255);;
	m_colorTextDefault = m_colorTextDefault_forRestore;
	m_colorFrameCheckBox = D3DCOLOR_XRGB(0,127,70);
	m_colorFrameRadioButton = D3DCOLOR_XRGB(255,127,70);
}

CManagerGUI::~CManagerGUI()
{
	_RELEASE_(m_pVertexBuffer);
	_RELEASE_(m_pIndexBuffer);
	_RELEASE_(m_boxTexture);

	// элементы которые принадлежат окнам


	// элементы которые НЕ принадлежат окнам
	for(i= 0; i < m_iQuantButtons; i++)
	{
		_RELEASE_(m_stButtons[i].textureButton);
		_RELEASE_(m_stButtons[i].textureSelectionButton);
	}
	for(i= 0; i < m_iQuantCheckBox; i++)
	{
		_RELEASE_(m_stCheckBox[i].textureCheckBoxON);
		_RELEASE_(m_stCheckBox[i].textureCheckBoxOFF);
	}
	for(i= 0; i < m_iQuantRadioButton; i++)
	{
		_RELEASE_(m_stRadioButton[i].textureRadioButtonON);
		_RELEASE_(m_stRadioButton[i].textureRadioButtonOFF);
	}
	for(i= 0; i < m_iQuantStaticText; i++)
	{
		_RELEASE_(m_stStaticText[i].textureStaticText);
	}
	for(i= 0; i < m_iQuantImageBox; i++)
	{
		_RELEASE_(m_stImageBox[i].textureImageBox);
	}
	for(i= 0; i < m_iQuantMouse; i++)
	{
		_RELEASE_(m_stGuiMouse[i].image.textureImageBox);
	}
	for(i= 0; i < m_iQuantHorizScrollBar; i++)
	{
		_RELEASE_(m_stHorizScrollBar[i].textureSliderString);

		_RELEASE_(m_stHorizScrollBar[i].buttonNext.textureButton);
		_RELEASE_(m_stHorizScrollBar[i].buttonNext.textureSelectionButton);

		_RELEASE_(m_stHorizScrollBar[i].buttonPrev.textureButton);
		_RELEASE_(m_stHorizScrollBar[i].buttonPrev.textureSelectionButton);

		_RELEASE_(m_stHorizScrollBar[i].buttonSlider.textureButton);
		_RELEASE_(m_stHorizScrollBar[i].buttonSlider.textureSelectionButton);
	}
	for(i= 0; i < m_iQuantVertScrollBar; i++)
	{
		_RELEASE_(m_stVertScrollBar[i].textureSliderString);

		_RELEASE_(m_stVertScrollBar[i].buttonNext.textureButton);
		_RELEASE_(m_stVertScrollBar[i].buttonNext.textureSelectionButton);

		_RELEASE_(m_stVertScrollBar[i].buttonPrev.textureButton);
		_RELEASE_(m_stVertScrollBar[i].buttonPrev.textureSelectionButton);

		_RELEASE_(m_stVertScrollBar[i].buttonSlider.textureButton);
		_RELEASE_(m_stVertScrollBar[i].buttonSlider.textureSelectionButton);
	}
}
void CManagerGUI::vLoadSamples(char *pcSampleMouseDown, char *pcSampleMouseOver, int iSoundVolume)
{
	squall_sample_default_t sample_info;

	m_iSampleSoundID_MouseDown = SQUALL_Sample_LoadFile(pcSampleMouseDown,true,NULL);	
	SQUALL_Sample_GetDefault(m_iSampleSoundID_MouseDown, &sample_info);
	sample_info.Volume = m_directx->iConvert_volume_to_squall(iSoundVolume);
	SQUALL_Sample_SetDefault(m_iSampleSoundID_MouseDown, &sample_info);	

	m_iSampleSoundID_MouseOver = SQUALL_Sample_LoadFile(pcSampleMouseOver,true,NULL);	
	SQUALL_Sample_GetDefault(m_iSampleSoundID_MouseOver, &sample_info);
	sample_info.Volume = m_directx->iConvert_volume_to_squall(iSoundVolume);
	SQUALL_Sample_SetDefault(m_iSampleSoundID_MouseOver, &sample_info);	
}

void CManagerGUI::vBeginInit(DXData *directx)
{
	m_directx = directx;
	//m_pFontDefault_s20 = new FONT(m_directx, 20,0,false,"Times New Roman Cyr");
	//m_pFontDefault_s25 = new FONT(m_directx, 25,0,false,"Times New Roman Cyr");

//****************************
	//создание текстуры для линий
    m_directx->m_pD3DDevice->CreateTexture( 16, 16, 1,0, D3DFMT_R5G6B5,
                                      D3DPOOL_MANAGED, &m_boxTexture, NULL );
    if(!m_boxTexture)return;
	//-------------------------
	//заполняем текстуру белым цветом
	D3DLOCKED_RECT blockedRect;//структура описания залокированной поверхности
	m_boxTexture->LockRect( 0, &blockedRect, 0, 0 );//блокировка поверхности

	WORD* imageData = (WORD*)blockedRect.pBits;//адресс поверхности
	//залить спрайт белым цветом
	for(int y = 0; y < 16; y++)
	{
		for(int x = 0; x < 16; x++)
		{
			int index = (int)(y * blockedRect.Pitch / 2 + x);
			imageData[index] = (WORD)D3DCOLOR_ARGB(255,255,255,255);//заполнить цветом с прозрачностью
		}
	}
	// завершили пачканье текстуры, разблокировать её
	if( m_boxTexture )m_boxTexture->UnlockRect(0);
//****************************


	/*
	Создание буфферов

	const unsigned short index[] = 
	{
		2,1,0, 2,0,3,
	};

	m_directx->m_pD3DDevice->CreateIndexBuffer(6*sizeof(short),
		0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIndexBuffer,0);

	void *pBI;
	m_pIndexBuffer->Lock(0,6*sizeof(short),&pBI,0);
	memcpy(pBI, index, 6*sizeof(short));
	m_pIndexBuffer->Unlock();


	stVertexGUI vertex[] = 
	{
		D3DXVECTOR3(0.0f,  0.0f, 0.0f),  D3DCOLOR_XRGB(255,255,255), 0.0f, 0.0f,
		D3DXVECTOR3(0.0f,  50.0f, 0.0f),  D3DCOLOR_XRGB(255,255,255), 0.0f, 1.0f,
		D3DXVECTOR3(50.0f, 50.0f, 0.0f),  D3DCOLOR_XRGB(255,255,255), 1.0f, 1.0f,
		D3DXVECTOR3(50.0f,  0.0f, 0.0f),  D3DCOLOR_XRGB(255,255,255), 1.0f, 0.0f,
	};

	if(FAILED(m_directx->m_pD3DDevice->CreateVertexBuffer(
		4*sizeof(stVertexGUI),0,
		D3DFVF_GUI, D3DPOOL_DEFAULT,
		&m_pVertexBuffer, 0)))
		return;

	void *pVB;
	m_pVertexBuffer->Lock(0,4*sizeof(stVertexGUI), &pVB, 0);
	memcpy(pVB, vertex, 4*sizeof(stVertexGUI));
	m_pVertexBuffer->Unlock();

	//m_directx->m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_directx->m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//m_directx->m_pD3DDevice->LightEnable(0,false);
	

	РИСОВАНИЕ
		if(m_stButtons[i].pTextureRender != NULL)
		{
		m_directx->m_pD3DDevice->SetStreamSource(0,m_pVertexBuffer, 0,
			sizeof(stVertexGUI));
		m_directx->m_pD3DDevice->SetFVF(D3DFVF_GUI);
		m_directx->m_pD3DDevice->SetIndices(m_pIndexBuffer);

		m_directx->m_pD3DDevice->SetTexture(0, *m_stButtons[i].pTextureRender);
		m_directx->m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);
		}*/
}

void CManagerGUI::vEndInit()
{
}

void CManagerGUI::vSetMatrix()
{
	// MatrixWorld
	D3DXMatrixTranslation(&m_matWorld, 0.0f, 0.0f, 0.0f);
	m_directx->m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	// MatrixView
	D3DXMatrixLookAtLH(&m_matView, &D3DXVECTOR3(0.0f, 0.0f, -1.0f),
									&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_directx->m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);

	// MatrixProjection
	D3DXMatrixOrthoOffCenterLH ( &m_matProjection ,0.0f , (float)m_directx->m_d3dpp.BackBufferWidth, (float)m_directx->m_d3dpp.BackBufferHeight, 0.0f, -10 , 1000.0f );
	m_directx->m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProjection);
}

void CManagerGUI::vRender()
{
	//CManagerGUI::vSetMatrix();

	m_directx->m_sprite->Begin();
	
	// отрисовка кнопок
	//for(i = 0; i < m_iQuantButtons; i++)
	for(i = 0; i < (int)m_stButtons.size(); i++)
	{
		
		if(m_stButtons[i].pTextureRender != NULL)
		m_directx->m_sprite->Draw(*m_stButtons[i].pTextureRender, 
									0,0,0,0, 
									&m_stButtons[i].vectorTrans,	
									D3DCOLOR_XRGB(255,255,255));		
	}

	// отрисовка CheckBox
	for(i = 0; i < m_iQuantCheckBox; i++)
	{
		if(m_stCheckBox[i].pTextureRender != NULL)
		m_directx->m_sprite->Draw(*m_stCheckBox[i].pTextureRender, 
			0,0,0,0, 
			&m_stCheckBox[i].vectorTrans,
			D3DCOLOR_XRGB(255,255,255));
	}

	// отрисовка GROUP RadioButton
	for(i = 0; i < m_iQuantGroup; i++)
		for(j = 0; j < m_stGroup[i].m_iQuantRadioButton; j++)
			if(m_stGroup[i].m_stRadioButton[j].pTextureRender != NULL)
				m_directx->m_sprite->Draw(*m_stGroup[i].m_stRadioButton[j].pTextureRender, 
				0,0,0,0, 
				&m_stGroup[i].m_stRadioButton[j].vectorTrans,
				D3DCOLOR_XRGB(255,255,255));


	// отрисовка ImageBox
	for(i = 0; i < m_iQuantImageBox; i++)
	{
		if(m_stImageBox[i].bVisible)
			m_directx->m_sprite->Draw(m_stImageBox[i].textureImageBox, 
			&m_stImageBox[i].pRectRender[m_stImageBox[i].iNumCurCadr],
			&m_stImageBox[i].vectorScale,0,0, 
			&m_stImageBox[i].vectorTrans,
			D3DCOLOR_XRGB(255,255,255));
	}

	// ------------------------------------------------ ScrollBar ( begin ) -------------------------------
	// отрисовка Горизонтальных ScrollBar
	for(i = 0; i < m_iQuantHorizScrollBar; i++)
	{
		if(m_stHorizScrollBar[i].bVisible)
		{
			// рисуем строку скролла
			m_directx->m_sprite->Draw(m_stHorizScrollBar[i].textureSliderString, 
					0,
					&m_stHorizScrollBar[i].vectorScale,0,0, 
					&m_stHorizScrollBar[i].vectorTrans,
					D3DCOLOR_XRGB(255,255,255));

			// рисуем кнопки
			// Ползунок
			m_directx->m_sprite->Draw(*m_stHorizScrollBar[i].buttonSlider.pTextureRender, 
				0,
				&m_stHorizScrollBar[i].buttonSlider.vectorScale,0,0, 
				&m_stHorizScrollBar[i].buttonSlider.vectorTrans,
				D3DCOLOR_XRGB(255,255,255));

			// Кнопки Next, Prev
			if(!m_stHorizScrollBar[i].bSlider) // если не Slider значит ScrollBar
			{		
				m_directx->m_sprite->Draw(*m_stHorizScrollBar[i].buttonNext.pTextureRender, 
					0,
					&m_stHorizScrollBar[i].buttonNext.vectorScale,0,0, 
					&m_stHorizScrollBar[i].buttonNext.vectorTrans,
					D3DCOLOR_XRGB(255,255,255));

				m_directx->m_sprite->Draw(*m_stHorizScrollBar[i].buttonPrev.pTextureRender, 
					0,
					&m_stHorizScrollBar[i].buttonPrev.vectorScale,0,0, 
					&m_stHorizScrollBar[i].buttonPrev.vectorTrans,
					D3DCOLOR_XRGB(255,255,255));
			}
		}
	}

	// отрисовка Вертикальных ScrollBar
	for(i = 0; i < m_iQuantVertScrollBar; i++)
	{
		if(m_stVertScrollBar[i].bVisible)
		{
			// рисуем строку скролла
			m_directx->m_sprite->Draw(m_stVertScrollBar[i].textureSliderString, 
				0,
				&m_stVertScrollBar[i].vectorScale,0,0, 
				&m_stVertScrollBar[i].vectorTrans,
				D3DCOLOR_XRGB(255,255,255));

			// рисуем кнопки
			// Ползунок
			m_directx->m_sprite->Draw(*m_stVertScrollBar[i].buttonSlider.pTextureRender, 
				0,
				&m_stVertScrollBar[i].buttonSlider.vectorScale,0,0, 
				&m_stVertScrollBar[i].buttonSlider.vectorTrans,
				D3DCOLOR_XRGB(255,255,255));

			// Кнопки Next, Prev
			if(!m_stVertScrollBar[i].bSlider) // если не Slider значит ScrollBar
			{		
				m_directx->m_sprite->Draw(*m_stVertScrollBar[i].buttonNext.pTextureRender, 
					0,
					&m_stVertScrollBar[i].buttonNext.vectorScale,0,0, 
					&m_stVertScrollBar[i].buttonNext.vectorTrans,
					D3DCOLOR_XRGB(255,255,255));

				m_directx->m_sprite->Draw(*m_stVertScrollBar[i].buttonPrev.pTextureRender, 
					0,
					&m_stVertScrollBar[i].buttonNext.vectorScale,0,0, 
					&m_stVertScrollBar[i].buttonPrev.vectorTrans,
					D3DCOLOR_XRGB(255,255,255));
			}
		}
	}
	// ------------------------------------------------ ScrollBar ( end ) -------------------------------




	// отрисовка StaticText
	for(i = 0; i < m_iQuantStaticText; i++)
	{
		if(m_stStaticText[i].bVisible)
			m_directx->m_sprite->Draw(m_stStaticText[i].textureStaticText, 
			0,0,0,0, 
			&m_stStaticText[i].vectorTrans,
			D3DCOLOR_XRGB(255,255,255));
	}

	// -------------------------- Отрисовка окон  ( begin ) -------------------------------------
	for(i = 0; i < m_iQuantWindowsPanel; i++) // рисуем от дальних к ближним
	{
		switch(m_stHandleWindowRender[i].typeElementGui)
		{
			case WINDOW_PANEL:
				if(m_stWindowPanel[m_stHandleWindowRender[i].iIndex].bVisible)
					m_directx->m_sprite->Draw(m_stWindowPanel[m_stHandleWindowRender[i].iIndex].textureRenderWindow, 
												0,
												0,0,0, 
												&m_stWindowPanel[m_stHandleWindowRender[i].iIndex].vectorTransWindow,
												D3DCOLOR_XRGB(255,255,255));
				break;
		};
	}
		
	/*for(i = 0; i < m_iQuantWindowsPanel; i++)
	{
		if(m_stWindowPanel[i].bVisible)
			m_directx->m_sprite->Draw(m_stWindowPanel[i].textureRenderWindow, 
			0,
			&m_stWindowPanel[i].vectorScaleWindow,0,0, 
			&m_stWindowPanel[i].vectorTransWindow,
			D3DCOLOR_XRGB(255,255,255));
	}*/
	// -------------------------- Отрисовка окон  ( end ) -------------------------------------




	// отрисовка МЫШИ Поставить в самый конец отрисовки
	if(m_bCreatedMouse)
	if(m_stGuiMouse[m_iCurNumMouse].image.bVisible)
			m_directx->m_sprite->Draw(m_stGuiMouse[m_iCurNumMouse].image.textureImageBox, 
			&m_stGuiMouse[m_iCurNumMouse].image.pRectRender[m_stGuiMouse[m_iCurNumMouse].image.iNumCurCadr],
			&m_stGuiMouse[m_iCurNumMouse].image.vectorScale,0,0, 
			&m_stGuiMouse[m_iCurNumMouse].image.vectorTrans,
			D3DCOLOR_XRGB(255,255,255));
	m_directx->m_sprite->End();
}

//-------------------------------------------------------------------------------------
//вывод заполненного прямоугольника
void CManagerGUI::vDrawBox2D(int x,int y,int w,int h,DWORD color)
{
	D3DXVECTOR2 vecScaling(1,1);  // масштаб
	D3DXVECTOR2 vecTrans((float)x,(float)y);	  // Translation

	RECT rect={0,0,w,h};
	m_directx->m_sprite->Draw(m_boxTexture, &rect, &vecScaling,NULL,0,&vecTrans,color );
}
//прямоугольник
void CManagerGUI::vDrawRectangle(int x,int y,int w,int h,DWORD clr)
{
	vDrawBox2D(x,y,w,1,clr);vDrawBox2D(x,y,1,h,clr);vDrawBox2D(x,y+h-1,w,1,clr);vDrawBox2D(x+w-1,y,1,h,clr);
}
void CManagerGUI::vDrawRectangle(int x,int y,int w,int h,DWORD clr1,DWORD clr2)
{
	vDrawBox2D(x,y,w,1,clr1);  vDrawBox2D(x,y,1,h,clr1);
	vDrawBox2D(x,y+h-1,w,1,clr2);  vDrawBox2D(x+w-1,y,1,h,clr2);
}

void CManagerGUI::vDrawFrameRect(int x, int y, int w, int h, DWORD color_1,DWORD color_2, int widthFrame)
{
	vDrawBox2D(x, y, w, widthFrame, color_1); // top
	vDrawBox2D(x, y, widthFrame, h, color_1); // left
	vDrawBox2D(x, y + h - widthFrame, w, widthFrame, color_2); // bottom
	vDrawBox2D(x + w - widthFrame, y, widthFrame, h, color_2); // right
}
//-------------------------------------------------------------------------------------

void CManagerGUI::vDrawButton(int x,int y,int w,int h,FONT* font,char* str,DWORD colorText)
{
	//---------------------------
	DWORD top,bot,surf;
	top=D3DCOLOR_XRGB(156,156,156);
	bot=D3DCOLOR_XRGB(104,84,80);
	surf=D3DCOLOR_XRGB(100,100,100);
	//---------------------------
	
	CManagerGUI::vDrawBox2D(x,y,w,h, surf);
	//CManagerGUI::vDrawRectangle(x,y,w,h,top,bot);
	CManagerGUI::vDrawFrameRect(x,y,w,h,top,bot,2);


	// ----------------------------
	if((str != NULL) && (font != NULL))
	{
		SIZE size=font->strlong(str);
		float sx=x + (float)(w-size.cx)/2;//определить координаты надписи относительно середины кнопки
		float sy=y + (float)(h-size.cy)/2;

		font->print(sx,sy,colorText,str);
	}
}

stHandleGui CManagerGUI::hCreateButton(stHandleGui *handleWindow, char *name, stRECT rectPos, void (*pFunc)(), bool bSelecEnable, int iTypeSelection)
{
	stButton button;
	stHandleGui handleGui;
	handleGui.typeElementGui = BUTTON;

	// ------------------------
	button.pcButtonName = NULL;
	if(name != NULL)
	{

		button.pcButtonName = new char[strlen(name) + 1];
		sprintf(button.pcButtonName, "%s", name);
	}

	button.pFuncExecute = pFunc;
	button.bSelectEnable = bSelecEnable;

	button.rectButton.left = rectPos.iPosX;
	button.rectButton.top = rectPos.iPosY;
	button.rectButton.right = button.rectButton.left + rectPos.iWidth;
	button.rectButton.bottom = button.rectButton.top + rectPos.iHeight;
	button.vectorTrans = D3DXVECTOR2((float)button.rectButton.left, (float)button.rectButton.top);
	// ------------------------

	// =================================== Render To Texture ==============================
	  // Создаём шрифт
		bool bCreateFont = false; 
		bool bCreateBigFont = false;
		FONT *fontButton;
		FONT *fontBigButton;

		if(rectPos.iHeight - 12 > 0)
		{
			bCreateFont = true;
			fontButton  = new FONT(m_directx, rectPos.iHeight - 12,0,false,"Times New Roman Cyr");
		}
		if(rectPos.iHeight - 6 > 0)
		{
			bCreateBigFont = true;
			fontBigButton  = new FONT(m_directx, rectPos.iHeight - 6,0,false,"Times New Roman Cyr");
		}


		LPDIRECT3DSURFACE9 backSurf;
		LPDIRECT3DSURFACE9 surfTexture;
		m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

		// ------ создаёт просто текстуру кнопки  (begin) -----
			m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth, rectPos.iHeight,
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&button.textureButton,0);

			button.textureButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);

			m_directx->m_pD3DDevice->BeginScene();
				if(bCreateFont)
					CManagerGUI::vDrawButton(0,0,rectPos.iWidth, rectPos.iHeight, fontButton, button.pcButtonName, m_colorTextDefault);
				else
					CManagerGUI::vDrawButton(0,0,rectPos.iWidth, rectPos.iHeight, NULL, button.pcButtonName, m_colorTextDefault);
				//CManagerGUI::vDrawBox2D(0,0,rectPos.iWidth,rectPos.iHeight,D3DCOLOR_ARGB(100, 70,70,70));
			m_directx->m_pD3DDevice->EndScene();
			surfTexture->Release();
		// ----- создаёт просто текстуру кнопки  (end) -----



		// ------ создаёт текстуру кнопки с установленым выделением (begin) -----
			m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth, rectPos.iHeight,
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&button.textureSelectionButton,0);

			button.textureSelectionButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);

			m_directx->m_pD3DDevice->BeginScene();

				if(bSelecEnable)
					switch(iTypeSelection)
					{
						case TYPE_SELECTION_COLOR_FONT:
								if(bCreateFont)
									CManagerGUI::vDrawButton(0,0,rectPos.iWidth, rectPos.iHeight, fontButton, button.pcButtonName, D3DCOLOR_XRGB(255,0,255));
								else
									CManagerGUI::vDrawButton(0,0,rectPos.iWidth, rectPos.iHeight, NULL, button.pcButtonName, D3DCOLOR_XRGB(255,0,255));
							break;

						case TYPE_SELECTION_SIZE_FONT:
								if(bCreateBigFont)
									CManagerGUI::vDrawButton(0,0,rectPos.iWidth, rectPos.iHeight, fontBigButton, button.pcButtonName, m_colorTextDefault);
								else
									CManagerGUI::vDrawButton(0,0,rectPos.iWidth, rectPos.iHeight, NULL, button.pcButtonName, m_colorTextDefault);
							break;

						case TYPE_SELECTION_FRAME:
								if(bCreateFont)
									CManagerGUI::vDrawButton(0,0,rectPos.iWidth, rectPos.iHeight, fontButton, button.pcButtonName, m_colorTextDefault);
								else
									CManagerGUI::vDrawButton(0,0,rectPos.iWidth, rectPos.iHeight, NULL, button.pcButtonName, m_colorTextDefault);

										CManagerGUI::vDrawFrameRect(0, 0, rectPos.iWidth, rectPos.iHeight,
											D3DCOLOR_XRGB(0,255,0),
											D3DCOLOR_XRGB(0,255,0),
											3);
							break;
					};
				
			m_directx->m_pD3DDevice->EndScene();
			surfTexture->Release();
		// ----- создаёт текстуру кнопки с установленым выделением  (end) -----


		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
		// =========================================================================



	//m_directx->vEffects(&button.textureButton);
	if(handleWindow == NULL)  // нет окна, поэтому просто создаём и храним в самом классе, а не в окне
	{
		button.iIDWindow = -1;

		handleGui.iIndex = m_iQuantButtons;

		m_stButtons.push_back(button);
		m_iQuantButtons ++;
	}
	else
	{
		handleGui.typeWindow = handleWindow->typeElementGui;
		handleGui.iIDWindow = handleWindow->iIndex;

		// Заполняем структуру для нажатий мышкой в окне
		button.rectForWindowMouse.left = rectPos.iPosX + (int)(m_stWindowPanel[handleWindow->iIndex].vectorTransWindow.x);
		button.rectForWindowMouse.top = rectPos.iPosY  + (int)(m_stWindowPanel[handleWindow->iIndex].vectorTransWindow.y);
		button.rectForWindowMouse.right = button.rectForWindowMouse.left + rectPos.iWidth;
		button.rectForWindowMouse.bottom = button.rectForWindowMouse.top + rectPos.iHeight;

		m_stWindowPanel[handleWindow->iIndex].m_stButtons.push_back(button);
		m_stWindowPanel[handleWindow->iIndex].m_iQuantButtons ++;

		handleGui.iIndex = m_stWindowPanel[handleWindow->iIndex].m_iQuantButtons - 1;
		if(button.bVisible)
			CManagerGUI::vRefreshRectWindowPanel(handleWindow->iIndex, &handleGui, &button.textureButton); // перерисуем всё окно
	}
	
	handleGui.iIDWindow = button.iIDWindow;

	return handleGui;
}

stHandleGui CManagerGUI::hCreateButton(stHandleGui *handleWindow, char *name, stRECT rectPos, void (*pFunc)(), 
								bool bSelecEnable, int iTypeSelection, char *textureButtonName)
{
	stButton button;
	stHandleGui handleGui;
	handleGui.typeElementGui = BUTTON;

	// ------------------------
	button.pcButtonName = NULL;
	if(name != NULL)
	{

		button.pcButtonName = new char[strlen(name) + 1];
		sprintf(button.pcButtonName, "%s", name);
	}

	button.pFuncExecute = pFunc;
	button.bSelectEnable = bSelecEnable;

	button.rectButton.left = rectPos.iPosX;
	button.rectButton.top = rectPos.iPosY;
	button.rectButton.right = button.rectButton.left + rectPos.iWidth;
	button.rectButton.bottom = button.rectButton.top + rectPos.iHeight;
	button.vectorTrans = D3DXVECTOR2((float)button.rectButton.left, (float)button.rectButton.top);
	// ------------------------

	// =================================== Render To Texture ==============================
	  // Создаём шрифт
		bool bCreateFont = false; 
		bool bCreateBigFont = false;
		FONT *fontButton;
		FONT *fontBigButton;

		if(rectPos.iHeight - 12 > 0)
		{
			bCreateFont = true;
			fontButton  = new FONT(m_directx, rectPos.iHeight - 12,0,false,"Times New Roman Cyr");
		}
		if(rectPos.iHeight - 6 > 0)
		{
			bCreateBigFont = true;
			fontBigButton  = new FONT(m_directx, rectPos.iHeight - 6,0,false,"Times New Roman Cyr");
		}



		LPDIRECT3DTEXTURE9 textureButton;
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, textureButtonName, &textureButton);
		float sx,sy;
		D3DSURFACE_DESC	d3dsd;
		textureButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(rectPos.iWidth))/d3dsd.Width;
		sy = (static_cast<float>(rectPos.iHeight))/(float)d3dsd.Height;
		D3DXVECTOR2 vectorScale = D3DXVECTOR2((float)sx, (float)sy);
		D3DXVECTOR2 vectorTrans = D3DXVECTOR2(0,0);


		LPDIRECT3DSURFACE9 backSurf;
		LPDIRECT3DSURFACE9 surfTexture;
		m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

		// ------ создаёт просто текстуру кнопки  (begin) -----
		m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth, rectPos.iHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&button.textureButton,0);

		button.textureButton->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);

		m_directx->m_pD3DDevice->BeginScene();

			m_directx->m_sprite->Draw(textureButton, 
				0,&vectorScale,0,0, 
				&vectorTrans,
				D3DCOLOR_XRGB(255,255,255));


			// ----------------------------
			if((name != NULL) && bCreateFont)
			{
				SIZE size = fontButton->strlong(name);
				float sx =0 + (float)(rectPos.iWidth - size.cx)/2;//определить координаты надписи относительно середины кнопки
				float sy = 0 + (float)(rectPos.iHeight - size.cy)/2;

				fontButton->print(sx,sy, m_colorTextDefault, name);
			}
		m_directx->m_pD3DDevice->EndScene();
		surfTexture->Release();
		// ----- создаёт просто текстуру кнопки  (end) -----



		// ------ создаёт текстуру кнопки с установленым выделением (begin) -----
		m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth, rectPos.iHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&button.textureSelectionButton,0);

		button.textureSelectionButton->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);

		m_directx->m_pD3DDevice->BeginScene();

		if(bSelecEnable)
		{
			m_directx->m_sprite->Draw(textureButton, 
				0,&vectorScale,0,0, 
				&vectorTrans,
				D3DCOLOR_XRGB(255,255,255));

			// выделение рамкой
			if(iTypeSelection == TYPE_SELECTION_FRAME)
				CManagerGUI::vDrawFrameRect(0, 0, rectPos.iWidth, rectPos.iHeight,
					D3DCOLOR_XRGB(0,255,0),
					D3DCOLOR_XRGB(0,255,0),
					3);

			// ----------------------------
			if(name != NULL)
			{
				SIZE size = fontButton->strlong(name);
				float sx =0 + (float)(rectPos.iWidth - size.cx)/2;//определить координаты надписи относительно середины кнопки
				float sy = 0 + (float)(rectPos.iHeight - size.cy)/2;


				if((iTypeSelection == TYPE_SELECTION_COLOR_FONT) && (bCreateFont))
						fontButton->print(sx,sy, D3DCOLOR_XRGB(255,0,255), name);

				else if((iTypeSelection == TYPE_SELECTION_SIZE_FONT) && (bCreateBigFont))
				{
					SIZE size = fontBigButton->strlong(name);
					float sx =0 + (float)(rectPos.iWidth - size.cx)/2;//определить координаты надписи относительно середины кнопки
					float sy = 0 + (float)(rectPos.iHeight - size.cy)/2;
					fontBigButton->print(sx,sy, m_colorTextDefault, name);
				}
				else if((iTypeSelection == TYPE_SELECTION_FRAME) && (bCreateFont))
						fontButton->print(sx,sy, m_colorTextDefault, name);
			}
		}

		m_directx->m_pD3DDevice->EndScene();
		surfTexture->Release();
		//textureButton->Release();
		// ----- создаёт текстуру кнопки с установленым выделением  (end) -----


		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
		// =========================================================================

		if(handleWindow == NULL)  // нет окна, поэтому просто создаём и храним в самом классе, а не в окне
		{
			button.iIDWindow = -1;

			handleGui.iIndex = m_iQuantButtons;

			m_stButtons.push_back(button);
			m_iQuantButtons ++;
		}
		else
		{
			handleGui.typeWindow = handleWindow->typeElementGui;
			handleGui.iIDWindow = handleWindow->iIndex;

			// Заполняем структуру для нажатий мышкой в окне
			button.rectForWindowMouse.left = rectPos.iPosX + (int)(m_stWindowPanel[handleWindow->iIndex].vectorTransWindow.x);
			button.rectForWindowMouse.top = rectPos.iPosY  + (int)(m_stWindowPanel[handleWindow->iIndex].vectorTransWindow.y);
			button.rectForWindowMouse.right = button.rectForWindowMouse.left + rectPos.iWidth;
			button.rectForWindowMouse.bottom = button.rectForWindowMouse.top + rectPos.iHeight;

			m_stWindowPanel[handleWindow->iIndex].m_stButtons.push_back(button);
			m_stWindowPanel[handleWindow->iIndex].m_iQuantButtons ++;

			handleGui.iIndex = m_stWindowPanel[handleWindow->iIndex].m_iQuantButtons - 1;
			if(button.bVisible)
				CManagerGUI::vRefreshRectWindowPanel(handleWindow->iIndex, &handleGui, &button.textureButton); // перерисуем всё окно
		}

		handleGui.iIDWindow = button.iIDWindow;
	return handleGui;
}

stHandleGui CManagerGUI::hCreateButton(stHandleGui *handleWindow, char *name, stRECT rectPos, 
								void (*pFunc)(), char *textureButtonName, char *textureButtonSelectName)
{
	stButton button;
	stHandleGui handleGui;
	handleGui.typeElementGui = BUTTON;

	// ------------------------
	button.pcButtonName = NULL;
	if(name != NULL)
	{

		button.pcButtonName = new char[strlen(name) + 1];
		sprintf(button.pcButtonName, "%s", name);
	}
	if(textureButtonSelectName != NULL)
		button.bSelectEnable = true;

	button.pFuncExecute = pFunc;

	button.rectButton.left = rectPos.iPosX;
	button.rectButton.top = rectPos.iPosY;
	button.rectButton.right = button.rectButton.left + rectPos.iWidth;
	button.rectButton.bottom = button.rectButton.top + rectPos.iHeight;
	button.vectorTrans = D3DXVECTOR2((float)button.rectButton.left, (float)button.rectButton.top);
	// ------------------------


	// =================================== Render To Texture ==============================
	  // Создаём шрифт
		bool bCreateFont = false; 
		bool bCreateBigFont = false;
		FONT *fontButton;
		FONT *fontBigButton;

		if(rectPos.iHeight - 12 > 0)
		{
			bCreateFont = true;
			fontButton  = new FONT(m_directx, rectPos.iHeight - 12,0,false,"Times New Roman Cyr");
		}
		if(rectPos.iHeight - 6 > 0)
		{
			bCreateBigFont = true;
			fontBigButton  = new FONT(m_directx, rectPos.iHeight - 6,0,false,"Times New Roman Cyr");
		}


		LPDIRECT3DTEXTURE9 textureButton;
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, textureButtonName, &textureButton);
		D3DXIMAGE_INFO info;
		D3DXGetImageInfoFromFile (textureButtonName, &info);
		D3DXVECTOR2 vectorScale = D3DXVECTOR2((float)rectPos.iWidth/info.Width, (float)rectPos.iHeight/info.Height);
		D3DXVECTOR2 vectorTrans = D3DXVECTOR2(0,0);


		LPDIRECT3DSURFACE9 backSurf;
		LPDIRECT3DSURFACE9 surfTexture;
		m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

		// ------ создаёт просто текстуру кнопки  (begin) -----
		m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth, rectPos.iHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&button.textureButton,0);

		button.textureButton->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);

		m_directx->m_pD3DDevice->BeginScene();

		m_directx->m_sprite->Draw(textureButton, 
			0,&vectorScale,0,0, 
			&vectorTrans,
			D3DCOLOR_XRGB(255,255,255));

		// ----------------------------
		if((name != NULL) && (bCreateFont))
		{
			SIZE size = fontButton->strlong(name);
			float sx =0 + (float)(rectPos.iWidth - size.cx)/2;//определить координаты надписи относительно середины кнопки
			float sy = 0 + (float)(rectPos.iHeight - size.cy)/2;

			fontButton->print(sx,sy, m_colorTextDefault, name);
		}
		m_directx->m_pD3DDevice->EndScene();
		surfTexture->Release();
		textureButton->Release();
		// ----- создаёт просто текстуру кнопки  (end) -----



		// ------ создаёт текстуру кнопки с установленым выделением (begin) -----
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, textureButtonSelectName, &textureButton);
		D3DXGetImageInfoFromFile (textureButtonSelectName, &info);
		vectorScale = D3DXVECTOR2((float)rectPos.iWidth/info.Width, (float)rectPos.iHeight/info.Height);
		vectorTrans = D3DXVECTOR2(0,0);


		m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth, rectPos.iHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&button.textureSelectionButton,0);

		button.textureSelectionButton->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);

		m_directx->m_pD3DDevice->BeginScene();

		m_directx->m_sprite->Draw(textureButton, 
			0,&vectorScale,0,0, 
			&vectorTrans,
			D3DCOLOR_XRGB(255,255,255));

		// ----------------------------
		if((name != NULL) && bCreateFont)
		{
			SIZE size = fontButton->strlong(name);
			float sx =0 + (float)(rectPos.iWidth - size.cx)/2;//определить координаты надписи относительно середины кнопки
			float sy = 0 + (float)(rectPos.iHeight - size.cy)/2;

			fontButton->print(sx,sy, m_colorTextDefault, name);
		}
		m_directx->m_pD3DDevice->EndScene();
		surfTexture->Release();
		textureButton->Release();
		// ----- создаёт текстуру кнопки с установленым выделением  (end) -----


		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
		// =========================================================================


		if(handleWindow == NULL)  // нет окна, поэтому просто создаём и храним в самом классе, а не в окне
		{
			button.iIDWindow = -1;

			handleGui.iIndex = m_iQuantButtons;

			m_stButtons.push_back(button);
			m_iQuantButtons ++;
		}
		else
		{
			handleGui.typeWindow = handleWindow->typeElementGui;
			handleGui.iIDWindow = handleWindow->iIndex;

			// Заполняем структуру для нажатий мышкой в окне
			button.rectForWindowMouse.left = rectPos.iPosX + (int)(m_stWindowPanel[handleWindow->iIndex].vectorTransWindow.x);
			button.rectForWindowMouse.top = rectPos.iPosY  + (int)(m_stWindowPanel[handleWindow->iIndex].vectorTransWindow.y);
			button.rectForWindowMouse.right = button.rectForWindowMouse.left + rectPos.iWidth;
			button.rectForWindowMouse.bottom = button.rectForWindowMouse.top + rectPos.iHeight;

			m_stWindowPanel[handleWindow->iIndex].m_stButtons.push_back(button);
			m_stWindowPanel[handleWindow->iIndex].m_iQuantButtons ++;

			handleGui.iIndex = m_stWindowPanel[handleWindow->iIndex].m_iQuantButtons - 1;
			if(button.bVisible)
				CManagerGUI::vRefreshRectWindowPanel(handleWindow->iIndex, &handleGui, &button.textureButton); // перерисуем всё окно
		}

		handleGui.iIDWindow = button.iIDWindow;
	return handleGui;
}

stHandleGui CManagerGUI::hCreateCheckBox(stHandleGui *handleWindow, char *name, stRECT rectPos, void (*pFunc)(bool bCheckBoxOn))
{
	stCheckBox checkBox;
	stHandleGui handleGui;
	handleGui.typeElementGui = CHECK_BOX;

	// ------------------------
	checkBox.pcCheckBoxName = NULL;

	FONT *fontCheckBox  = new FONT(m_directx, rectPos.iHeight - 2,0,false,"Times New Roman Cyr");
	SIZE sizeFontText;
	sizeFontText.cx = 0;
	sizeFontText.cy = 0;

	if(name != NULL)
	{

		checkBox.pcCheckBoxName = new char[strlen(name) + 1];
		sprintf(checkBox.pcCheckBoxName, "%s", name);

		sizeFontText = fontCheckBox->strlong(name);
		sizeFontText.cx += 3; // добавляем к рамеру текстуры
	}

	checkBox.pFuncExecute = pFunc;

	checkBox.rectCheckBox.left = rectPos.iPosX;
	checkBox.rectCheckBox.top = rectPos.iPosY;
	checkBox.rectCheckBox.right = checkBox.rectCheckBox.left + rectPos.iWidth;
	checkBox.rectCheckBox.bottom = checkBox.rectCheckBox.top + rectPos.iHeight;
	checkBox.vectorTrans = D3DXVECTOR2((float)checkBox.rectCheckBox.left, (float)checkBox.rectCheckBox.top);
	// ------------------------

	// =================================== Render To Texture ==============================
	LPDIRECT3DSURFACE9 backSurf;
	LPDIRECT3DSURFACE9 surfTexture;
	m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

	// ------------------------- создаёт текстуру CheckBox = FALSE  (begin) ------------------

	// создаём текстуру рамера CheckBox + место для текста

	m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth + sizeFontText.cx, rectPos.iHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, 
		&checkBox.textureCheckBoxOFF,0);


	checkBox.textureCheckBoxOFF->GetSurfaceLevel(0, &surfTexture);
	m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

	//   следующую сточку можно коментировать для ускорения
	m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 0.0f, 0);

	m_directx->m_pD3DDevice->BeginScene();
		CManagerGUI::vDrawBox2D(0,0,rectPos.iWidth, rectPos.iHeight, D3DCOLOR_XRGB(0,0,0));
		CManagerGUI::vDrawFrameRect(0,0,rectPos.iWidth, rectPos.iHeight, m_colorFrameCheckBox, m_colorFrameCheckBox,3);

	// ----------------------------
	if(name != NULL)
		fontCheckBox->print((float)rectPos.iWidth + 3,2.0f, m_colorTextDefault, name);

	m_directx->m_pD3DDevice->EndScene();
	surfTexture->Release();
	// --------------------- создаёт текстуру CheckBox = FALSE  (end) ---------------------------------



	// ---------------------------- создаёт текстуру CheckBox = TRUE (begin) ---------------------------------

	// создаём текстуру рамера CheckBox + место для текста
	if(name != NULL)
	{
		sizeFontText = fontCheckBox->strlong(name);
		sizeFontText.cx += 3;
	}

	m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth + sizeFontText.cx, rectPos.iHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, 
		&checkBox.textureCheckBoxON,0);


	checkBox.textureCheckBoxON->GetSurfaceLevel(0, &surfTexture);
	m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

	//   следующую сточку можно коментировать для ускорения
	m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);

	m_directx->m_pD3DDevice->BeginScene();
		CManagerGUI::vDrawBox2D(0, 0, rectPos.iWidth, rectPos.iHeight, D3DCOLOR_XRGB(0,0,0));
		CManagerGUI::vDrawFrameRect(0,0,rectPos.iWidth, rectPos.iHeight, m_colorFrameCheckBox, m_colorFrameCheckBox,3);
		if(name != NULL)
		{
			sizeFontText = fontCheckBox->strlong("X");
			fontCheckBox->print((float)(rectPos.iWidth - sizeFontText.cx)/2,
				(float)(rectPos.iHeight - sizeFontText.cy)/2
				, D3DCOLOR_XRGB(100,255,100), "X");
		}
		//CManagerGUI::vDrawBox2D(6,6,rectPos.iWidth - 12, rectPos.iHeight - 12, D3DCOLOR_XRGB(100,100,100));

	// ----------------------------
	if(name != NULL)
		fontCheckBox->print((float)rectPos.iWidth + 3,2.0f, m_colorTextDefault, name);

	m_directx->m_pD3DDevice->EndScene();
	surfTexture->Release();
	// ---------------------------- создаёт текстуру CheckBox = TRUE  (end) ------------------------


	m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
	//checkBox.pTextureRender = &checkBox.textureCheckBoxOFF;

	//surfTexture->Release();
	// =========================================================================


	if(handleWindow == NULL)  // нет окна, поэтому просто создаём и храним в самом классе, а не в окне
	{
		checkBox.iIDWindow = -1;

		handleGui.iIndex = m_iQuantCheckBox;

		m_stCheckBox.push_back(checkBox);
		m_iQuantCheckBox ++;
	}
	else
	{
		handleGui.typeWindow = handleWindow->typeElementGui;
		handleGui.iIDWindow = handleWindow->iIndex;

		// Заполняем структуру для нажатий мышкой в окне
		checkBox.rectForWindowMouse.left = rectPos.iPosX + (int)(m_stWindowPanel[handleWindow->iIndex].vectorTransWindow.x);
		checkBox.rectForWindowMouse.top = rectPos.iPosY  + (int)(m_stWindowPanel[handleWindow->iIndex].vectorTransWindow.y);
		checkBox.rectForWindowMouse.right = checkBox.rectForWindowMouse.left + rectPos.iWidth;
		checkBox.rectForWindowMouse.bottom = checkBox.rectForWindowMouse.top + rectPos.iHeight;

		handleGui.iIndex = m_stWindowPanel[handleWindow->iIndex].m_iQuantCheckBox;

		m_stWindowPanel[handleWindow->iIndex].m_stCheckBox.push_back(checkBox);
		m_stWindowPanel[handleWindow->iIndex].m_iQuantCheckBox ++;

		if(checkBox.bVisible)
		{
			if(checkBox.bCheckBoxState)
				CManagerGUI::vRefreshRectWindowPanel(handleWindow->iIndex, &handleGui, &checkBox.textureCheckBoxON);
			else
				CManagerGUI::vRefreshRectWindowPanel(handleWindow->iIndex, &handleGui, &checkBox.textureCheckBoxOFF);
		}
	}

	handleGui.iIDWindow = checkBox.iIDWindow;
	return handleGui;
}

stHandleGui CManagerGUI::hCreateCheckBox(stHandleGui *handleWindow, char *name, stRECT rectPos, 
								  void (*pFunc)(bool bCheckBoxOn), char *textureNameCheckBoxOFF, char *textureNameCheckBoxON)
{
	stCheckBox checkBox;
	stHandleGui handleGui;
	handleGui.typeElementGui = CHECK_BOX;

	// ------------------------
	checkBox.pcCheckBoxName = NULL;
	
	FONT *fontCheckBox  = new FONT(m_directx, rectPos.iHeight - 2,0,false,"Times New Roman Cyr");
	SIZE sizeFontText;
		sizeFontText.cx = 0;
		sizeFontText.cy = 0;

	if(name != NULL)
	{

		checkBox.pcCheckBoxName = new char[strlen(name) + 1];
		sprintf(checkBox.pcCheckBoxName, "%s", name);

		sizeFontText = fontCheckBox->strlong(name);
		sizeFontText.cx += 3; // добавляем к рамеру текстуры
	}

	checkBox.pFuncExecute = pFunc;

	checkBox.rectCheckBox.left = rectPos.iPosX;
	checkBox.rectCheckBox.top = rectPos.iPosY;
	checkBox.rectCheckBox.right = checkBox.rectCheckBox.left + rectPos.iWidth;
	checkBox.rectCheckBox.bottom = checkBox.rectCheckBox.top + rectPos.iHeight;
	checkBox.vectorTrans = D3DXVECTOR2((float)checkBox.rectCheckBox.left, (float)checkBox.rectCheckBox.top);
	// ------------------------

		// =================================== Render To Texture ==============================
		LPDIRECT3DSURFACE9 backSurf;
		LPDIRECT3DSURFACE9 surfTexture;
		m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

		// ------------------------- создаёт текстуру CheckBox = FALSE  (begin) ------------------

		LPDIRECT3DTEXTURE9 textureCheckBox;
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, textureNameCheckBoxOFF, &textureCheckBox);
		//D3DXIMAGE_INFO info;
		//D3DXGetImageInfoFromFile (textureNameCheckBoxOFF, &info);
		float sx,sy;
		D3DSURFACE_DESC	d3dsd;
		textureCheckBox->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(rectPos.iWidth))/d3dsd.Width;
		sy = (static_cast<float>(rectPos.iHeight))/(float)d3dsd.Height;
		D3DXVECTOR2 vectorScale = D3DXVECTOR2(sx,sy);
		//D3DXVECTOR2 vectorScale = D3DXVECTOR2((float)rectPos.iWidth/info.Width, (float)rectPos.iHeight/info.Height);
		D3DXVECTOR2 vectorTrans = D3DXVECTOR2(0,0);

		// создаём текстуру рамера CheckBox + место для текста
		
		m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth + sizeFontText.cx, rectPos.iHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&checkBox.textureCheckBoxOFF,0);


		checkBox.textureCheckBoxOFF->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 0.0f, 0);

		m_directx->m_pD3DDevice->BeginScene();
			m_directx->m_sprite->Draw(textureCheckBox, 
				0,&vectorScale,0,0, 
				&vectorTrans,
				D3DCOLOR_XRGB(255,255,255));

		// ----------------------------
		if(name != NULL)
			fontCheckBox->print((float)rectPos.iWidth + 3,2.0f, m_colorTextDefault, name);

		m_directx->m_pD3DDevice->EndScene();
		surfTexture->Release();
		textureCheckBox->Release();
		// --------------------- создаёт текстуру CheckBox = FALSE  (end) ---------------------------------



		// ---------------------------- создаёт текстуру CheckBox = TRUE (begin) ---------------------------------

		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, textureNameCheckBoxON, &textureCheckBox);
		//D3DXGetImageInfoFromFile (textureNameCheckBoxON, &info);
		//vectorScale = D3DXVECTOR2((float)rectPos.iWidth/info.Width, (float)rectPos.iHeight/info.Height);
		textureCheckBox->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(rectPos.iWidth))/d3dsd.Width;
		sy = (static_cast<float>(rectPos.iHeight))/(float)d3dsd.Height;
		vectorScale = D3DXVECTOR2(sx,sy);
		vectorTrans = D3DXVECTOR2(0,0);


		// создаём текстуру рамера CheckBox + место для текста
		if(name != NULL)
		{
			sizeFontText = fontCheckBox->strlong(name);
			sizeFontText.cx += 3; // добавляем к рамеру текстуры
		}
		
		m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth + sizeFontText.cx, rectPos.iHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&checkBox.textureCheckBoxON,0);


		checkBox.textureCheckBoxON->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);

		m_directx->m_pD3DDevice->BeginScene();
			m_directx->m_sprite->Draw(textureCheckBox, 
				0,&vectorScale,0,0, 
				&vectorTrans,
				D3DCOLOR_XRGB(255,255,255));

		// ----------------------------
		if(name != NULL)
			fontCheckBox->print((float)rectPos.iWidth + 3,(float)2, m_colorTextDefault, name);

		m_directx->m_pD3DDevice->EndScene();
		surfTexture->Release();
		textureCheckBox->Release();
		// ---------------------------- создаёт текстуру CheckBox = TRUE  (end) ------------------------


		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
		//checkBox.pTextureRender = &checkBox.textureCheckBoxOFF;

		//surfTexture->Release();
		// =========================================================================


		if(handleWindow == NULL)  // нет окна, поэтому просто создаём и храним в самом классе, а не в окне
		{
			checkBox.iIDWindow = -1;

			handleGui.iIndex = m_iQuantCheckBox;

			m_stCheckBox.push_back(checkBox);
			m_iQuantCheckBox ++;
		}
		else
		{
			handleGui.typeWindow = handleWindow->typeElementGui;
			handleGui.iIDWindow = handleWindow->iIndex;

			// Заполняем структуру для нажатий мышкой в окне
			checkBox.rectForWindowMouse.left = rectPos.iPosX + (int)(m_stWindowPanel[handleWindow->iIndex].vectorTransWindow.x);
			checkBox.rectForWindowMouse.top = rectPos.iPosY  + (int)(m_stWindowPanel[handleWindow->iIndex].vectorTransWindow.y);
			checkBox.rectForWindowMouse.right = checkBox.rectForWindowMouse.left + rectPos.iWidth;
			checkBox.rectForWindowMouse.bottom = checkBox.rectForWindowMouse.top + rectPos.iHeight;

			handleGui.iIndex = m_stWindowPanel[handleWindow->iIndex].m_iQuantCheckBox;

			m_stWindowPanel[handleWindow->iIndex].m_stCheckBox.push_back(checkBox);
			m_stWindowPanel[handleWindow->iIndex].m_iQuantCheckBox ++;

			if(checkBox.bVisible)
			{
				if(checkBox.bCheckBoxState)
					CManagerGUI::vRefreshRectWindowPanel(handleWindow->iIndex, &handleGui, &checkBox.textureCheckBoxON);
				else
					CManagerGUI::vRefreshRectWindowPanel(handleWindow->iIndex, &handleGui, &checkBox.textureCheckBoxOFF);
			}
		}

	handleGui.iIDWindow = checkBox.iIDWindow;
	return handleGui;
}

stHandleGui CManagerGUI::hCreateStaticText(stHandleGui *handleWindow, char *text, int iPosX, int iPosY, int iFontSize)
{
	stStaticText staticText;
	stHandleGui handleGui;
	handleGui.typeElementGui = STATIC_TEXT;

	staticText.vectorTrans.x = (float)iPosX;
	staticText.vectorTrans.y = (float)iPosY;
	
	staticText.iFontSize = iFontSize;
	staticText.text = NULL;

	SIZE sizeFont;
	sizeFont.cx = 0;
	sizeFont.cy = 0;

	if(text != NULL)
	{
		staticText.text = new char[strlen(text) + 1];
		sprintf(staticText.text, "%s", text);

		FONT *fontStaticText  = new FONT(m_directx, iFontSize,0,false,"Times New Roman Cyr");
		sizeFont = fontStaticText->strlong(staticText.text);

		// =================================== Рендер в текстуру (begin) =============================
		LPDIRECT3DSURFACE9 backSurf;
		LPDIRECT3DSURFACE9 surfTexture;
		m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

		m_directx->m_pD3DDevice->CreateTexture(sizeFont.cx, sizeFont.cy,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&staticText.textureStaticText,0);

		staticText.textureStaticText->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 0.0f, 0);
			m_directx->m_pD3DDevice->BeginScene();
				fontStaticText->print(0,0,m_colorTextDefault, staticText.text);
			m_directx->m_pD3DDevice->EndScene();

		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
		// =================================== Рендер в текстуру (end) =============================
	}

	handleGui.iIndex = m_iQuantStaticText;
	if(handleWindow == NULL)  // нет окна, поэтому просто создаём и храним в самом классе, а не в окне
	{
		staticText.iIDWindow = -1;

		m_stStaticText.push_back(staticText);
		m_iQuantStaticText ++;
	}
	else
	{
		handleGui.typeWindow = handleWindow->typeElementGui;
		handleGui.iIDWindow = handleWindow->iIndex;

		// Заполняем структуру для нажатий мышкой в окне
		staticText.rectStaticText.left = iPosX;
		staticText.rectStaticText.top = iPosY;
		staticText.rectStaticText.right = staticText.rectStaticText.left + sizeFont.cx;
		staticText.rectStaticText.bottom = staticText.rectStaticText.top + sizeFont.cy;

		handleGui.iIndex = m_stWindowPanel[handleWindow->iIndex].m_iQuantStaticText;

		m_stWindowPanel[handleWindow->iIndex].m_stStaticText.push_back(staticText);
		m_stWindowPanel[handleWindow->iIndex].m_iQuantStaticText ++;

		if(staticText.bVisible)
			CManagerGUI::vRefreshRectWindowPanel(handleWindow->iIndex, &handleGui, &staticText.textureStaticText);
	}

	//handleGui.iIDWindow = staticText.iIDWindow;
	//handleGui.iIndex = m_iQuantStaticText - 1;

	return handleGui;
}

bool CManagerGUI::bReplaceStaticText(stHandleGui *handleGui, char *newText)
{
	if(handleGui->typeElementGui == STATIC_TEXT)
	{
		//if(handleGui->iIDWindow < 0)
		if(handleGui->typeWindow == NOT_ELEM)
		{
			_RELEASE_(m_stStaticText[handleGui->iIndex].textureStaticText);
			_DELETE_(m_stStaticText[handleGui->iIndex].text);

			m_stStaticText[handleGui->iIndex].text = new char[strlen(newText) + 1];
			sprintf(m_stStaticText[handleGui->iIndex].text, "%s", newText);

			FONT *fontStaticText  = new FONT(m_directx, m_stStaticText[handleGui->iIndex].iFontSize,0,false,"Times New Roman Cyr");
			SIZE sizeFont = fontStaticText->strlong(m_stStaticText[handleGui->iIndex].text);

			// =================================== Рендер в текстуру (begin) =============================
			LPDIRECT3DSURFACE9 backSurf;
			LPDIRECT3DSURFACE9 surfTexture;
			m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

			m_directx->m_pD3DDevice->CreateTexture(sizeFont.cx, sizeFont.cy,
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&m_stStaticText[handleGui->iIndex].textureStaticText,0);

			m_stStaticText[handleGui->iIndex].textureStaticText->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 0.0f, 0);
			m_directx->m_pD3DDevice->BeginScene();
				fontStaticText->print(0,0,m_colorTextDefault, m_stStaticText[handleGui->iIndex].text);
			m_directx->m_pD3DDevice->EndScene();

			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// =================================== Рендер в текстуру (end) =============================
		
			//fontStaticText->vRelease();
			_DELETE_(fontStaticText);
		}
		else if(handleGui->typeWindow == WINDOW_PANEL)
		{
			// для того чтобы коректно удалить пред. текст с поверхности окна
			bool bVisibleState = m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].bVisible;
			m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].bVisible = false;
				CManagerGUI::vRefreshRectWindowPanel(handleGui->iIDWindow, handleGui, &m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].textureStaticText);
			m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].bVisible = bVisibleState;


			_RELEASE_(m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].textureStaticText);
			_DELETE_(m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].text);

			m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].text = new char[strlen(newText) + 1];
			sprintf(m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].text, "%s", newText);

			FONT *fontStaticText  = new FONT(m_directx, m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].iFontSize,0,false,"Times New Roman Cyr");
			SIZE sizeFont = fontStaticText->strlong(m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].text);

			// =================================== Рендер в текстуру (begin) =============================
			LPDIRECT3DSURFACE9 backSurf;
			LPDIRECT3DSURFACE9 surfTexture;
			m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

			m_directx->m_pD3DDevice->CreateTexture(sizeFont.cx, sizeFont.cy,
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].textureStaticText,0);

			m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].textureStaticText->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);
			m_directx->m_pD3DDevice->BeginScene();
				fontStaticText->print(0,0,m_colorTextDefault, m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].text);
			m_directx->m_pD3DDevice->EndScene();

			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// =================================== Рендер в текстуру (end) =============================

			//fontStaticText->vRelease();
			_DELETE_(fontStaticText);

			
			// -------------------------------------------- Регистрация в окне -----------------------------------
			//stHandleGui handleElem;
			//handleElem.typeWindow = handleGui->typeWindow;
			//handleElem.iIDWindow = handleGui->iIDWindow;

			// Заполняем структуру для коректного затирания в окне
			m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].rectStaticText.left = (long)m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].vectorTrans.x;
			m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].rectStaticText.top = (long)m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].vectorTrans.y;
			m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].rectStaticText.right = m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].rectStaticText.left + sizeFont.cx;
			m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].rectStaticText.bottom = m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].rectStaticText.top + sizeFont.cy;

			if(m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].bVisible)
				CManagerGUI::vRefreshRectWindowPanel(handleGui->iIDWindow, handleGui, &m_stWindowPanel[handleGui->iIDWindow].m_stStaticText[handleGui->iIndex].textureStaticText);
		}
	}

	return true;
}

stHandleGui CManagerGUI::hCreateImageBox(stHandleGui *handleWindow, stRECT rectPos, char *textureNameImage)
{
	stImageBox imageBox;
	stHandleGui handleGui;

	handleGui.typeElementGui = IMAGE_BOX;

	imageBox.bAnimEnable = false;

	// позиция
	imageBox.vectorTrans.x = (float)rectPos.iPosX;
	imageBox.vectorTrans.y = (float)rectPos.iPosY;

	imageBox.m_stRectPos = rectPos;


	// =================================== Рендер в текстуру (begin) =============================
	//LPDIRECT3DTEXTURE9 textureImageBox;
	D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, textureNameImage, &imageBox.textureImageBox);

	//D3DXIMAGE_INFO info;
	//D3DXGetImageInfoFromFile (textureNameImage, &info);
	//D3DXVECTOR2 vectorScale = D3DXVECTOR2((float)iWidth/(float)info.Width, (float)iHeight/(float)info.Height);

	//D3DXVECTOR2 vectorScale;
	float sx,sy;
	D3DSURFACE_DESC	d3dsd;
	imageBox.textureImageBox->GetLevelDesc(0,&d3dsd);
	sx = (static_cast<float>(rectPos.iWidth))/d3dsd.Width;
	sy = (static_cast<float>(rectPos.iHeight))/(float)d3dsd.Height;
	imageBox.vectorScale = D3DXVECTOR2(sx, sy);

	// какую часть изображения выводмть
	imageBox.pRectRender = new RECT[1];
	imageBox.pRectRender[0].top = 1;
	imageBox.pRectRender[0].left = 0;
	imageBox.pRectRender[0].bottom = d3dsd.Height;
	imageBox.pRectRender[0].right = d3dsd.Width;


	//m_directx->vEffects(&imageBox.textureImageBox);
	if(handleWindow == NULL)  // нет окна, поэтому просто создаём и храним в самом классе, а не в окне
	{
		/*imageBox.rectFrameForWindow.top = 1;
		imageBox.rectFrameForWindow.left = 0;
		imageBox.rectFrameForWindow.bottom = d3dsd.Height;
		imageBox.rectFrameForWindow.right = d3dsd.Width;*/

		handleGui.typeWindow = NOT_ELEM;
		imageBox.iIDWindow = -1;

		m_stImageBox.push_back(imageBox);
		m_iQuantImageBox ++;
	}
	else
	{
		imageBox.rectFrameForWindow.top = rectPos.iPosY;
		imageBox.rectFrameForWindow.left = rectPos.iPosX;
		imageBox.rectFrameForWindow.bottom = imageBox.rectFrameForWindow.top + rectPos.iHeight;
		imageBox.rectFrameForWindow.right = imageBox.rectFrameForWindow.left + rectPos.iWidth;

		handleGui.typeWindow = handleWindow->typeElementGui;
		handleGui.iIDWindow = handleWindow->iIndex;

		m_stWindowPanel[handleWindow->iIndex].m_stImageBox.push_back(imageBox);
		m_stWindowPanel[handleWindow->iIndex].m_iQuantImageBox ++;

		if(imageBox.bVisible)
			CManagerGUI::vRefreshRectWindowPanel(handleWindow->iIndex, NULL, NULL); // перерисуем всё окно
	}

	handleGui.iIDWindow = imageBox.iIDWindow;
	handleGui.iIndex = m_iQuantImageBox - 1;
	return handleGui;
}

stHandleGui CManagerGUI::hCreateImageBox(stHandleGui *handleWindow, stRECT rectPos, stSIZE sizeCadr, int rateInSec, char *textureNameImage)
{
	stImageBox imageBox;
	stHandleGui handleGui;

	handleGui.typeElementGui = IMAGE_BOX;

	imageBox.bAnimEnable = true;
	imageBox.iRateInSec = rateInSec;

	// позиция
	imageBox.vectorTrans.x = (float)rectPos.iPosX;
	imageBox.vectorTrans.y = (float)rectPos.iPosY;

	imageBox.m_stRectPos = rectPos;


	// =================================== Рендер в текстуру (begin) =============================
	//LPDIRECT3DTEXTURE9 textureImageBox;
	//D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, textureNameImage, &imageBox.textureImageBox);

	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile (textureNameImage, &info);
	D3DXCreateTextureFromFileEx(m_directx->m_pD3DDevice,
		textureNameImage,
		info.Width,
		info.Height,
		0,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		0, // or NULL
		NULL,
		&imageBox.textureImageBox);
	//D3DXVECTOR2 vectorScale = D3DXVECTOR2((float)iWidth/(float)info.Width, (float)iHeight/(float)info.Height);

	//D3DXVECTOR2 vectorScale;
	float sx,sy;
	D3DSURFACE_DESC	d3dsd;
	imageBox.textureImageBox->GetLevelDesc(0,&d3dsd);
	sx = (static_cast<float>(rectPos.iWidth))/sizeCadr.cx;
	sy = (static_cast<float>(rectPos.iHeight))/(float)sizeCadr.cy;
	imageBox.vectorScale = D3DXVECTOR2(sx, sy);

	/*
		 какую часть изображения выводмть
		 Определяем количество кадров
		 Заполняем под каждый кадр структуру RECT
	*/
	int iNumIndex = 0;
	int iQuantCadr = (info.Width/sizeCadr.cx) * (info.Height/sizeCadr.cy);
	imageBox.iQuantCadr = iQuantCadr;
	imageBox.iDelayFrame = 1000/imageBox.iRateInSec;
	imageBox.pRectRender = new RECT[iQuantCadr];
	//for( int i =0; i < ((float)info.Height/sizeCadr.cy); i++)
		//for( int j =0; j < ((float)info.Width/sizeCadr.cx); j++)
	for( int i =0; i < ((float)info.Height/sizeCadr.cy); i++)
		for( int j =0; j < ((float)info.Width/sizeCadr.cx); j++)
		{
			
			imageBox.pRectRender[iNumIndex].left = j*sizeCadr.cx;
			imageBox.pRectRender[iNumIndex].top = i*sizeCadr.cy;
			imageBox.pRectRender[iNumIndex].right = imageBox.pRectRender[iNumIndex].left + sizeCadr.cx;
			imageBox.pRectRender[iNumIndex].bottom = imageBox.pRectRender[iNumIndex].top + sizeCadr.cy;
			
			iNumIndex++;
		}

	//m_directx->vEffects(&imageBox.textureImageBox);
	if(handleWindow == NULL)  // нет окна, поэтому просто создаём и храним в самом классе, а не в окне
	{
		handleGui.typeWindow = NOT_ELEM;
		imageBox.iIDWindow = -1;

		m_stImageBox.push_back(imageBox);
		m_iQuantImageBox ++;
	}
	else
	{
		imageBox.rectFrameForWindow.top = rectPos.iPosY;
		imageBox.rectFrameForWindow.left = rectPos.iPosX;
		imageBox.rectFrameForWindow.bottom = imageBox.rectFrameForWindow.top + rectPos.iHeight;
		imageBox.rectFrameForWindow.right = imageBox.rectFrameForWindow.left + rectPos.iWidth;

		handleGui.typeWindow = handleWindow->typeElementGui;
		handleGui.iIDWindow = handleWindow->iIndex;

		m_stWindowPanel[handleWindow->iIndex].m_stImageBox.push_back(imageBox);
		m_stWindowPanel[handleWindow->iIndex].m_iQuantImageBox ++;

		if(imageBox.bVisible)
			CManagerGUI::vRefreshRectWindowPanel(handleWindow->iIndex, NULL, NULL); // перерисуем всё окно
	}

	handleGui.iIDWindow = imageBox.iIDWindow;
	handleGui.iIndex = m_iQuantImageBox - 1;
	return handleGui;
}

stHandleGui CManagerGUI::hCreateGroup(stHandleGui *handleWindow)
{
	stHandleGui handle;

	if(handleWindow == NULL)
	{
		handle.typeElementGui = GROUP;
		handle.iIDWindow = -1;
		handle.iIndex = m_iQuantGroup;
		m_iQuantGroup ++;

		stGroup group;
		m_stGroup.push_back(group);
	}
	else if(handleWindow->typeElementGui == WINDOW_PANEL)
	{
		handle.typeWindow = handleWindow->typeElementGui;
		handle.iIDWindow = handleWindow->iIndex;

		handle.typeElementGui = GROUP;
		handle.iIndex = m_stWindowPanel[handleWindow->iIndex].m_iQuantGroup;

		stGroup group;

		m_stWindowPanel[handleWindow->iIndex].m_stGroup.push_back(group);
		m_stWindowPanel[handleWindow->iIndex].m_iQuantGroup ++;
	}

	return handle;
}

void CManagerGUI::vCreateRadioButton(stHandleGui *handleGroup, char *name, stRECT rectPos, void (*pFunc)(bool bRadioButtonOn))
{
	stRadioButton radioButton;

	// ------------------------
	radioButton.pcRadioButtonName = NULL;

	FONT *fontRadioButton  = new FONT(m_directx, rectPos.iHeight - 2,0,false,"Times New Roman Cyr");
	SIZE sizeFontText;
	sizeFontText.cx = 0;
	sizeFontText.cy = 0;

	if(name != NULL)
	{

		radioButton.pcRadioButtonName = new char[strlen(name) + 1];
		sprintf(radioButton.pcRadioButtonName, "%s", name);

		sizeFontText = fontRadioButton->strlong(name);
		sizeFontText.cx += 3; // добавляем к рамеру текстуры
	}

	radioButton.pFuncExecute = pFunc;

	radioButton.rectRadioButton.left = rectPos.iPosX;
	radioButton.rectRadioButton.top = rectPos.iPosY;
	radioButton.rectRadioButton.right = radioButton.rectRadioButton.left + rectPos.iWidth;
	radioButton.rectRadioButton.bottom = radioButton.rectRadioButton.top + rectPos.iHeight;
	radioButton.vectorTrans = D3DXVECTOR2((float)radioButton.rectRadioButton.left, (float)radioButton.rectRadioButton.top);
	// ------------------------

	// =================================== Render To Texture ==============================
	LPDIRECT3DSURFACE9 backSurf;
	LPDIRECT3DSURFACE9 surfTexture;
	m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

	// ------------------------- создаёт текстуру RadioButton = FALSE  (begin) ------------------

	// создаём текстуру рамера RadioButton + место для текста

	m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth + sizeFontText.cx, rectPos.iHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, 
		&radioButton.textureRadioButtonOFF,0);


	radioButton.textureRadioButtonOFF->GetSurfaceLevel(0, &surfTexture);
	m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

	//   следующую сточку можно коментировать для ускорения
	m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 0.0f, 0);

	m_directx->m_pD3DDevice->BeginScene();
		CManagerGUI::vDrawBox2D(0,0,rectPos.iWidth, rectPos.iHeight, D3DCOLOR_XRGB(0,0,0));
		CManagerGUI::vDrawFrameRect(0,0,rectPos.iWidth, rectPos.iHeight, m_colorFrameRadioButton, m_colorFrameRadioButton,3);

	// ----------------------------
	if(name != NULL)
		fontRadioButton->print((float)rectPos.iWidth + 3,2.0f, m_colorTextDefault, name);

	m_directx->m_pD3DDevice->EndScene();
	surfTexture->Release();
	// --------------------- создаёт текстуру RadioButton = FALSE  (end) ---------------------------------



	// ---------------------------- создаёт текстуру RadioButton = TRUE (begin) ---------------------------------

	// создаём текстуру рамера RadioButton + место для текста
	if(name != NULL)
	{
		sizeFontText = fontRadioButton->strlong(name);
		sizeFontText.cx += 3;
	}

	m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth + sizeFontText.cx, rectPos.iHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, 
		&radioButton.textureRadioButtonON,0);


	radioButton.textureRadioButtonON->GetSurfaceLevel(0, &surfTexture);
	m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

	//   следующую сточку можно коментировать для ускорения
	m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);

	m_directx->m_pD3DDevice->BeginScene();
		CManagerGUI::vDrawBox2D(0, 0, rectPos.iWidth, rectPos.iHeight, D3DCOLOR_XRGB(0,0,0));
		CManagerGUI::vDrawFrameRect(0,0,rectPos.iWidth, rectPos.iHeight, m_colorFrameRadioButton, m_colorFrameRadioButton,3);
		CManagerGUI::vDrawBox2D(6,6,rectPos.iWidth - 12, rectPos.iHeight - 12, m_colorFrameRadioButton);

	// ----------------------------
	if(name != NULL)
		fontRadioButton->print((float)rectPos.iWidth + 3,2.0f, m_colorTextDefault, name);

	m_directx->m_pD3DDevice->EndScene();
	surfTexture->Release();
	// ---------------------------- создаёт текстуру CheckBox = TRUE  (end) ------------------------


	m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);

	//surfTexture->Release();
	// =========================================================================

	if(handleGroup->typeWindow == NOT_ELEM)
	{
		if(m_stGroup[handleGroup->iIndex].m_iQuantRadioButton < 1)  // если кнопка первая в списке группы то state = true
			radioButton.bRadioButtonState = true;

		m_stGroup[handleGroup->iIndex].m_stRadioButton.push_back(radioButton);
		m_stGroup[handleGroup->iIndex].m_iQuantRadioButton++;
	}
	else if(handleGroup->typeWindow == WINDOW_PANEL)
	{
		if(m_stWindowPanel[handleGroup->iIDWindow].m_stGroup[handleGroup->iIndex].m_iQuantRadioButton < 1)  // если кнопка первая в списке группы то state = true
			radioButton.bRadioButtonState = true;

		// Заполняем структуру для нажатий мышкой в окне
		radioButton.rectForWindowMouse.left = rectPos.iPosX + (int)(m_stWindowPanel[handleGroup->iIDWindow].vectorTransWindow.x);
		radioButton.rectForWindowMouse.top = rectPos.iPosY  + (int)(m_stWindowPanel[handleGroup->iIDWindow].vectorTransWindow.y);
		radioButton.rectForWindowMouse.right = radioButton.rectForWindowMouse.left + rectPos.iWidth;
		radioButton.rectForWindowMouse.bottom = radioButton.rectForWindowMouse.top + rectPos.iHeight;


		m_stWindowPanel[handleGroup->iIDWindow].m_stGroup[handleGroup->iIndex].m_stRadioButton.push_back(radioButton);
		m_stWindowPanel[handleGroup->iIDWindow].m_stGroup[handleGroup->iIndex].m_iQuantRadioButton++;


		if(radioButton.bVisible)
		{
			if(radioButton.bRadioButtonState) // состояние = true
				CManagerGUI::vRefreshRectWindowPanel(handleGroup->iIDWindow, handleGroup, NULL); // перерисуем область элемента
			else
				CManagerGUI::vRefreshRectWindowPanel(handleGroup->iIDWindow, handleGroup, NULL); // перерисуем область элемента
		}
	}
}

void CManagerGUI::vCreateRadioButton(stHandleGui *handleGroup, char *name, stRECT rectPos, void (*pFunc)(bool bRadioButtonOn), char *textureNameRadioButtonOFF, char *textureNameRadioButtonON)
{
	stRadioButton radioButton;

	// ------------------------
	radioButton.pcRadioButtonName = NULL;
	
	FONT *fontRadioButton  = new FONT(m_directx, rectPos.iHeight - 2,0,false,"Times New Roman Cyr");
	SIZE sizeFontText;
		sizeFontText.cx = 0;
		sizeFontText.cy = 0;

	if(name != NULL)
	{

		radioButton.pcRadioButtonName = new char[strlen(name) + 1];
		sprintf(radioButton.pcRadioButtonName, "%s", name);

		sizeFontText = fontRadioButton->strlong(name);
		sizeFontText.cx += 3; // добавляем к рамеру текстуры
	}

	radioButton.pFuncExecute = pFunc;

	radioButton.rectRadioButton.left = rectPos.iPosX;
	radioButton.rectRadioButton.top = rectPos.iPosY;
	radioButton.rectRadioButton.right = radioButton.rectRadioButton.left + rectPos.iWidth;
	radioButton.rectRadioButton.bottom = radioButton.rectRadioButton.top + rectPos.iHeight;
	radioButton.vectorTrans = D3DXVECTOR2((float)radioButton.rectRadioButton.left, (float)radioButton.rectRadioButton.top);
	// ------------------------

		// =================================== Render To Texture ==============================
		LPDIRECT3DSURFACE9 backSurf;
		LPDIRECT3DSURFACE9 surfTexture;
		m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

		// ------------------------- создаёт текстуру RadioButton = FALSE  (begin) ------------------

		LPDIRECT3DTEXTURE9 textureRadioButton;
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, textureNameRadioButtonOFF, &textureRadioButton);
		//D3DXIMAGE_INFO info;
		//D3DXGetImageInfoFromFile (textureNameCheckBoxOFF, &info);
		float sx,sy;
		D3DSURFACE_DESC	d3dsd;
		textureRadioButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(rectPos.iWidth))/d3dsd.Width;
		sy = (static_cast<float>(rectPos.iHeight))/(float)d3dsd.Height;
		D3DXVECTOR2 vectorScale = D3DXVECTOR2(sx,sy);
		//D3DXVECTOR2 vectorScale = D3DXVECTOR2((float)rectPos.iWidth/info.Width, (float)rectPos.iHeight/info.Height);
		D3DXVECTOR2 vectorTrans = D3DXVECTOR2(0,0);

		// создаём текстуру рамера RadioButton + место для текста
		
		m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth + sizeFontText.cx, rectPos.iHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&radioButton.textureRadioButtonOFF,0);


		radioButton.textureRadioButtonOFF->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 0.0f, 0);

		m_directx->m_pD3DDevice->BeginScene();
			m_directx->m_sprite->Draw(textureRadioButton, 
				0,&vectorScale,0,0, 
				&vectorTrans,
				D3DCOLOR_XRGB(255,255,255));

		// ----------------------------
		if(name != NULL)
			fontRadioButton->print((float)rectPos.iWidth + 3,2.0f, m_colorTextDefault, name);

		m_directx->m_pD3DDevice->EndScene();
		surfTexture->Release();
		textureRadioButton->Release();
		// --------------------- создаёт текстуру RadioButton = FALSE  (end) ---------------------------------



		// ---------------------------- создаёт текстуру RadioButton = TRUE (begin) ---------------------------------

		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, textureNameRadioButtonON, &textureRadioButton);
		//D3DXGetImageInfoFromFile (textureNameCheckBoxON, &info);
		//vectorScale = D3DXVECTOR2((float)rectPos.iWidth/info.Width, (float)rectPos.iHeight/info.Height);
		textureRadioButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(rectPos.iWidth))/d3dsd.Width;
		sy = (static_cast<float>(rectPos.iHeight))/(float)d3dsd.Height;
		vectorScale = D3DXVECTOR2(sx,sy);
		vectorTrans = D3DXVECTOR2(0,0);


		// создаём текстуру рамера RadioButton + место для текста
		if(name != NULL)
		{
			sizeFontText = fontRadioButton->strlong(name);
			sizeFontText.cx += 3; // добавляем к рамеру текстуры
		}
		
		m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth + sizeFontText.cx, rectPos.iHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&radioButton.textureRadioButtonON,0);


		radioButton.textureRadioButtonON->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);

		m_directx->m_pD3DDevice->BeginScene();
			m_directx->m_sprite->Draw(textureRadioButton, 
				0,&vectorScale,0,0, 
				&vectorTrans,
				D3DCOLOR_XRGB(255,255,255));

		// ----------------------------
		if(name != NULL)
			fontRadioButton->print((float)rectPos.iWidth + 3,(float)2, m_colorTextDefault, name);

		m_directx->m_pD3DDevice->EndScene();
		surfTexture->Release();
		textureRadioButton->Release();
		// ---------------------------- создаёт текстуру RadioButton = TRUE  (end) ------------------------


		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
		//checkBox.pTextureRender = &checkBox.textureCheckBoxOFF;

		//surfTexture->Release();
		// =========================================================================


		if(handleGroup->typeWindow == NOT_ELEM)
		{
			if(m_stGroup[handleGroup->iIndex].m_iQuantRadioButton < 1)  // если кнопка первая в списке группы то state = true
				radioButton.bRadioButtonState = true;

			m_stGroup[handleGroup->iIndex].m_stRadioButton.push_back(radioButton);
			m_stGroup[handleGroup->iIndex].m_iQuantRadioButton++;
		}
		else if(handleGroup->typeWindow == WINDOW_PANEL)
		{
			if(m_stWindowPanel[handleGroup->iIDWindow].m_stGroup[handleGroup->iIndex].m_iQuantRadioButton < 1)  // если кнопка первая в списке группы то state = true
				radioButton.bRadioButtonState = true;

			// Заполняем структуру для нажатий мышкой в окне
			radioButton.rectForWindowMouse.left = rectPos.iPosX + (int)(m_stWindowPanel[handleGroup->iIDWindow].vectorTransWindow.x);
			radioButton.rectForWindowMouse.top = rectPos.iPosY  + (int)(m_stWindowPanel[handleGroup->iIDWindow].vectorTransWindow.y);
			radioButton.rectForWindowMouse.right = radioButton.rectForWindowMouse.left + rectPos.iWidth;
			radioButton.rectForWindowMouse.bottom = radioButton.rectForWindowMouse.top + rectPos.iHeight;


			m_stWindowPanel[handleGroup->iIDWindow].m_stGroup[handleGroup->iIndex].m_stRadioButton.push_back(radioButton);
			m_stWindowPanel[handleGroup->iIDWindow].m_stGroup[handleGroup->iIndex].m_iQuantRadioButton++;


			if(radioButton.bVisible)
			{
				if(radioButton.bRadioButtonState) // состояние = true
					CManagerGUI::vRefreshRectWindowPanel(handleGroup->iIDWindow, handleGroup, NULL); // перерисуем область элемента
				else
					CManagerGUI::vRefreshRectWindowPanel(handleGroup->iIDWindow, handleGroup, NULL); // перерисуем область элемента
			}
		}
}

stHandleGui CManagerGUI::hCreateHorizScrollBar(int iWindowID, stRECT rectPos, stSliderInfo info,
											  void (*pFunc)(stSliderInfo *pSliderInfo), int iDelayButton, bool bSlider)
{
	stScrollBar scrollBar;
	stHandleGui hanlde;
	hanlde.typeElementGui = HSCROLL_BAR;

	D3DXVECTOR2 *vectorLinePos = new D3DXVECTOR2[3]; // для рисования значков
	//D3DCOLOR colorLineSign = D3DCOLOR_XRGB(100,74,127);
	D3DCOLOR colorLineSign = D3DCOLOR_XRGB(0,255,255);


	scrollBar.bSlider = bSlider;
	if(iDelayButton > 0)
		scrollBar.iDelayScroll = iDelayButton;

		scrollBar.m_stSliderInfo = info;
		scrollBar.m_stSliderInfo.m_fRaznizaMinMax = fabs(info.m_fMaxValue - (info.m_fMinValue));
		    
			// Если задано CurValue > MaxValue then CurValue = MaxValue, так же и с минимальным
			if(scrollBar.m_stSliderInfo.m_fCurValue > scrollBar.m_stSliderInfo.m_fMaxValue)
				scrollBar.m_stSliderInfo.m_fCurValue = scrollBar.m_stSliderInfo.m_fMaxValue;
			else if(scrollBar.m_stSliderInfo.m_fCurValue < scrollBar.m_stSliderInfo.m_fMinValue)
				scrollBar.m_stSliderInfo.m_fCurValue = scrollBar.m_stSliderInfo.m_fMinValue;

	scrollBar.pFuncExecute = pFunc;

	

	// =================================== Render To Texture ==============================
	LPDIRECT3DSURFACE9 backSurf;
	LPDIRECT3DSURFACE9 surfTexture;
	m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

	// ------------------------- создаёт текстуру RadioButton = FALSE  (begin) ------------------

	if(bSlider) // значит строка слайдер на весь размер stRECT
	{
		int iWidthButtonSlider = rectPos.iHeight;
		scrollBar.iSliderButtonWidth = iWidthButtonSlider; // ширина рычажка (SliderButton)

		// позиция на экране
		scrollBar.vectorTrans.x = (float)rectPos.iPosX;
		scrollBar.vectorTrans.y = (float)rectPos.iPosY;

		//  -------------------------- Рисуем SliderString ----------------------------------
		m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth, rectPos.iHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&scrollBar.textureSliderString,0);


		scrollBar.textureSliderString->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);
		m_directx->m_pD3DDevice->BeginScene();
			CManagerGUI::vDrawFrameRect(0,0,rectPos.iWidth,rectPos.iHeight,D3DCOLOR_XRGB(128,128,128),D3DCOLOR_XRGB(128,128,128),3);
		m_directx->m_pD3DDevice->EndScene();

		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf); 


			//  -------------------------- Рисуем SliderButton ----------------------------------

		// ширина столки по которой ходит SliderButton
		scrollBar.iButtonSliderBegPosX = rectPos.iPosX;  // от какой точки он будет двмгатся (он же должен двигатся только в прямоугольнике)
		scrollBar.iSliderStringWidth = rectPos.iWidth - iWidthButtonSlider;

		scrollBar.fPixelInValue = (scrollBar.iSliderStringWidth)/scrollBar.m_stSliderInfo.m_fRaznizaMinMax;  // узнаём сколько в единице значения пикселей

	// позиция на экране
		scrollBar.buttonSlider.vectorTrans.x = (float)(scrollBar.iButtonSliderBegPosX + 
														((scrollBar.m_stSliderInfo.m_fCurValue) + fabs(scrollBar.m_stSliderInfo.m_fMinValue)) * scrollBar.fPixelInValue);
		scrollBar.buttonSlider.vectorTrans.y = (float)rectPos.iPosY;

		// Заполняем структуру RECT
		scrollBar.buttonSlider.rectButton.top = (int)scrollBar.buttonSlider.vectorTrans.y;
		scrollBar.buttonSlider.rectButton.left = (int)scrollBar.buttonSlider.vectorTrans.x;
		scrollBar.buttonSlider.rectButton.bottom = scrollBar.buttonSlider.rectButton.top + scrollBar.iSliderButtonWidth;
		scrollBar.buttonSlider.rectButton.right = scrollBar.buttonSlider.rectButton.left + scrollBar.iSliderButtonWidth;



			// ---------- Кнопка в обычном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonSlider, iWidthButtonSlider, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonSlider.textureButton,0);

			scrollBar.buttonSlider.textureButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (70, 70, 70), 1.0f, 0);
			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonSlider,iWidthButtonSlider,
										D3DCOLOR_XRGB(128,128,128),D3DCOLOR_XRGB(128,128,128),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonSlider, iWidthButtonSlider, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonSlider.textureSelectionButton,0);

			scrollBar.buttonSlider.textureSelectionButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (70, 70, 70), 1.0f, 0);
			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonSlider,iWidthButtonSlider,
										D3DCOLOR_XRGB(255,128,128),D3DCOLOR_XRGB(255,128,128),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в выделеном состоянии (End) ----------*/
	}
	else // это ScrollBar и в эти координаты надо втиснуть и две квадратные кнопки
	{
		// ширина одной кнопки = высоте => что из ширины SliderString отнимаем 2*iWidthButtonNextPrev
		int iWidthButtonNextPrev = rectPos.iHeight;
		scrollBar.iSliderStringWidth = rectPos.iWidth - 2*iWidthButtonNextPrev;


		//  -------------------------- Рисуем SliderString ----------------------------------
		// позиция на экране
		scrollBar.vectorTrans.x = (float)(rectPos.iPosX + iWidthButtonNextPrev);
		scrollBar.vectorTrans.y = (float)rectPos.iPosY;

		m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth - 2*iWidthButtonNextPrev, rectPos.iHeight, 
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&scrollBar.textureSliderString,0);

		scrollBar.textureSliderString->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);
		m_directx->m_pD3DDevice->BeginScene();
			CManagerGUI::vDrawFrameRect(0,0,rectPos.iWidth - 2*iWidthButtonNextPrev,
							rectPos.iHeight,D3DCOLOR_XRGB(150,150,150),D3DCOLOR_XRGB(150,150,150),3);
		m_directx->m_pD3DDevice->EndScene();
		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);


		//  -------------------------- Рисуем SliderButton ----------------------------------
		scrollBar.iSliderButtonWidth = iWidthButtonNextPrev; // ширина рычажка (SliderButton)

		//static float sx =  0; 
		scrollBar.fPixelInValue = (scrollBar.iSliderStringWidth - scrollBar.iSliderButtonWidth)/scrollBar.m_stSliderInfo.m_fRaznizaMinMax;  // узнаём сколько в единице значения пикселей
		/*scrollBar.buttonSlider.vectorTrans.x = scrollBar.iButtonSliderBegPosX + 
			scrollBar.m_stSliderInfo.m_fCurValue*sx;*/

		// ширина столки по которой ходит SliderButton
		scrollBar.iButtonSliderBegPosX = rectPos.iPosX + iWidthButtonNextPrev;  // от какой точки он будет двмгатся (он же должен двигатся только в прямоугольнике)
		//scrollBar.iButtonSliderBegPosX = rectPos.iPosX + scrollBar.m_stSliderInfo.m_fCurValue * sx;
		scrollBar.iSliderStringWidth = rectPos.iWidth - 3*iWidthButtonNextPrev;

	// позиция на экране
		scrollBar.buttonSlider.vectorTrans.x = (float)(scrollBar.iButtonSliderBegPosX + 
														((scrollBar.m_stSliderInfo.m_fCurValue) + fabs(scrollBar.m_stSliderInfo.m_fMinValue)) * scrollBar.fPixelInValue);
		scrollBar.buttonSlider.vectorTrans.y = (float)rectPos.iPosY;

		// Заполняем структуру RECT
		scrollBar.buttonSlider.rectButton.top = (int)scrollBar.buttonSlider.vectorTrans.y;
		scrollBar.buttonSlider.rectButton.left = (int)scrollBar.buttonSlider.vectorTrans.x;
		scrollBar.buttonSlider.rectButton.bottom = scrollBar.buttonSlider.rectButton.top + scrollBar.iSliderButtonWidth;
		scrollBar.buttonSlider.rectButton.right = scrollBar.buttonSlider.rectButton.left + scrollBar.iSliderButtonWidth;



			// ---------- Кнопка в обычном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonNextPrev, iWidthButtonNextPrev, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonSlider.textureButton,0);

			scrollBar.buttonSlider.textureButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (70, 70, 70), 1.0f, 0);
			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonNextPrev,iWidthButtonNextPrev,
										D3DCOLOR_XRGB(128,128,128),D3DCOLOR_XRGB(128,128,128),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonNextPrev, iWidthButtonNextPrev, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonSlider.textureSelectionButton,0);

			scrollBar.buttonSlider.textureSelectionButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (70, 70, 70), 1.0f, 0);
			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonNextPrev,iWidthButtonNextPrev,
										D3DCOLOR_XRGB(255,128,128),D3DCOLOR_XRGB(255,128,128),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в выделеном состоянии (End) ----------


		//  -------------------------- Рисуем SliderNEXT ----------------------------------
		// позиция на экране
		scrollBar.buttonNext.vectorTrans.x = (float)(rectPos.iPosX + rectPos.iWidth - iWidthButtonNextPrev);
		scrollBar.buttonNext.vectorTrans.y = (float)rectPos.iPosY;

		// Заполняем структуру RECT
		scrollBar.buttonNext.rectButton.top = (int)scrollBar.buttonNext.vectorTrans.y;
		scrollBar.buttonNext.rectButton.left = (int)scrollBar.buttonNext.vectorTrans.x;
		scrollBar.buttonNext.rectButton.bottom = scrollBar.buttonNext.rectButton.top + iWidthButtonNextPrev;
		scrollBar.buttonNext.rectButton.right = scrollBar.buttonNext.rectButton.left + iWidthButtonNextPrev;


			// ---------- Кнопка в обычном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonNextPrev, iWidthButtonNextPrev, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonNext.textureButton,0);

			scrollBar.buttonNext.textureButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (0,74,127), 1.0f, 0);
			
			// Рисование  значка линиями
			m_directx->m_pLine->SetWidth(2);
			m_directx->m_pLine->Begin();				
				vectorLinePos[0] = D3DXVECTOR2(3.0f, 3.0f);
				vectorLinePos[1] = D3DXVECTOR2((float)(iWidthButtonNextPrev-3),(float)(iWidthButtonNextPrev/2));
				vectorLinePos[2] = D3DXVECTOR2(3.0f, (float)(iWidthButtonNextPrev-3));					
				m_directx->m_pLine->Draw(vectorLinePos,3,colorLineSign);				
			m_directx->m_pLine->End();


			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonNextPrev,iWidthButtonNextPrev,
										D3DCOLOR_XRGB(0,127,127),D3DCOLOR_XRGB(0,127,127),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonNextPrev, iWidthButtonNextPrev, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonNext.textureSelectionButton,0);

			scrollBar.buttonNext.textureSelectionButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (0,74,127), 1.0f, 0);
			
			// Рисование  значка линиями
			m_directx->m_pLine->SetWidth(2);
			m_directx->m_pLine->Begin();
				vectorLinePos[0] = D3DXVECTOR2(3,3);
				vectorLinePos[1] = D3DXVECTOR2((float)(iWidthButtonNextPrev-3), (float)(iWidthButtonNextPrev/2));
				vectorLinePos[2] = D3DXVECTOR2(3.0f, (float)(iWidthButtonNextPrev-3));				
				m_directx->m_pLine->Draw(vectorLinePos,3,D3DCOLOR_XRGB(255,128,128));
			m_directx->m_pLine->End();

			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonNextPrev,iWidthButtonNextPrev,
										D3DCOLOR_XRGB(255,128,128),D3DCOLOR_XRGB(255,128,128),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в выделеном состоянии (End) ----------


		//  -------------------------- Рисуем SliderPREV ----------------------------------
		// позиция на экране
		scrollBar.buttonPrev.vectorTrans.x = (float)rectPos.iPosX;
		scrollBar.buttonPrev.vectorTrans.y = (float)rectPos.iPosY;

		// Заполняем структуру RECT
		scrollBar.buttonPrev.rectButton.top = (int)scrollBar.buttonPrev.vectorTrans.y;
		scrollBar.buttonPrev.rectButton.left = (int)scrollBar.buttonPrev.vectorTrans.x;
		scrollBar.buttonPrev.rectButton.bottom = scrollBar.buttonPrev.rectButton.top + iWidthButtonNextPrev;
		scrollBar.buttonPrev.rectButton.right = scrollBar.buttonPrev.rectButton.left + iWidthButtonNextPrev;


			// ---------- Кнопка в обычном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonNextPrev, iWidthButtonNextPrev, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonPrev.textureButton,0);

			scrollBar.buttonPrev.textureButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (0,74,127), 1.0f, 0);

			// Рисование  значка линиями
			m_directx->m_pLine->SetWidth(2);
			m_directx->m_pLine->Begin();				
				vectorLinePos[0] = D3DXVECTOR2((float)(iWidthButtonNextPrev-3), 3.0f);
				vectorLinePos[1] = D3DXVECTOR2(3.0f, (float)(iWidthButtonNextPrev/2));
				vectorLinePos[2] = D3DXVECTOR2((float)(iWidthButtonNextPrev-3), (float)(iWidthButtonNextPrev-3));					
				m_directx->m_pLine->Draw(vectorLinePos,3,colorLineSign);				
			m_directx->m_pLine->End();

			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonNextPrev,iWidthButtonNextPrev,
										D3DCOLOR_XRGB(0,127,127),D3DCOLOR_XRGB(0,127,127),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonNextPrev, iWidthButtonNextPrev, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonPrev.textureSelectionButton,0);

			scrollBar.buttonPrev.textureSelectionButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (0,74,127), 1.0f, 0);

			// Рисование  значка линиями
			m_directx->m_pLine->SetWidth(2);
			m_directx->m_pLine->Begin();				
				vectorLinePos[0] = D3DXVECTOR2((float)(iWidthButtonNextPrev-3), 3.0f);
				vectorLinePos[1] = D3DXVECTOR2(3.0f, (float)(iWidthButtonNextPrev/2));
				vectorLinePos[2] = D3DXVECTOR2((float)(iWidthButtonNextPrev-3), (float)(iWidthButtonNextPrev-3));					
				m_directx->m_pLine->Draw(vectorLinePos,3,colorLineSign);				
			m_directx->m_pLine->End();

			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonNextPrev,iWidthButtonNextPrev,
										D3DCOLOR_XRGB(255,128,128),D3DCOLOR_XRGB(255,128,128),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в выделеном состоянии (End) ----------
		//----------------------------------------------------------------------------------


		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
	}
	_DELETE_(vectorLinePos);
	// ===========================================================================


	if(iWindowID < 0)  // нет окна, поэтому просто создаём и храним в самом классе, а не в окне
	{
		m_stHorizScrollBar.push_back(scrollBar);
		m_iQuantHorizScrollBar ++;
	}

	hanlde.iIDWindow = iWindowID;
	hanlde.iIndex = m_iQuantHorizScrollBar - 1;
	return hanlde;
}

stHandleGui CManagerGUI::hCreateHorizScrollBar(int iWindowID, stRECT rectPos, stSliderInfo info, void (*pFunc)(stSliderInfo *pSliderInfo), int iDelayButton, bool bSlider,
								  char *pcNameTextureButtonSlider,
								  char *pcNameTextureButtonSliderSelection,
								  char *pcNameTextureButtonPrev,
								  char *pcNameTextureButtonPrevSelection,
								  char *pcNameTextureButtonNext,
								  char *pcNameTextureButtonNextSelection,
								  char *pcNameTextureSliderString)
{
		stScrollBar scrollBar;
	stHandleGui hanlde;
	hanlde.typeElementGui = HSCROLL_BAR;

	//D3DXVECTOR2 *vectorLinePos = new D3DXVECTOR2[3]; // для рисования значков
	//D3DCOLOR colorLineSign = D3DCOLOR_XRGB(100,74,127);
	//D3DCOLOR colorLineSign = D3DCOLOR_XRGB(0,255,255);


	scrollBar.bSlider = bSlider;
	if(iDelayButton > 0)
		scrollBar.iDelayScroll = iDelayButton;

		scrollBar.m_stSliderInfo = info;
		scrollBar.m_stSliderInfo.m_fRaznizaMinMax = fabs(info.m_fMaxValue - (info.m_fMinValue));
		    
			// Если задано CurValue > MaxValue then CurValue = MaxValue, так же и с минимальным
			if(scrollBar.m_stSliderInfo.m_fCurValue > scrollBar.m_stSliderInfo.m_fMaxValue)
				scrollBar.m_stSliderInfo.m_fCurValue = scrollBar.m_stSliderInfo.m_fMaxValue;
			else if(scrollBar.m_stSliderInfo.m_fCurValue < scrollBar.m_stSliderInfo.m_fMinValue)
				scrollBar.m_stSliderInfo.m_fCurValue = scrollBar.m_stSliderInfo.m_fMinValue;

	scrollBar.pFuncExecute = pFunc;

	

	// ========================== Загрузка текстур кнопок ===============================
	float sx,sy;
	D3DSURFACE_DESC	d3dsd;

	if(bSlider) // значит строка слайдер на весь размер stRECT
	{
		int iWidthButtonSlider = rectPos.iHeight;
		scrollBar.iSliderButtonWidth = iWidthButtonSlider; // ширина рычажка (SliderButton)

		// позиция на экране
		scrollBar.vectorTrans.x = (float)rectPos.iPosX;
		scrollBar.vectorTrans.y = (float)rectPos.iPosY;

		//  -------------------------- Рисуем SliderString ----------------------------------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureSliderString, &scrollBar.textureSliderString);		
		
		scrollBar.textureSliderString->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(rectPos.iWidth))/d3dsd.Width;
		sy = (static_cast<float>(rectPos.iHeight))/(float)d3dsd.Height;
		scrollBar.vectorScale = D3DXVECTOR2(sx, sy);


			//  -------------------------- Рисуем SliderButton ----------------------------------

		// ширина столки по которой ходит SliderButton
		scrollBar.iButtonSliderBegPosX = rectPos.iPosX;  // от какой точки он будет двмгатся (он же должен двигатся только в прямоугольнике)
		scrollBar.iSliderStringWidth = rectPos.iWidth - iWidthButtonSlider;

		scrollBar.fPixelInValue = (scrollBar.iSliderStringWidth)/scrollBar.m_stSliderInfo.m_fRaznizaMinMax;  // узнаём сколько в единице значения пикселей

	// позиция на экране
		scrollBar.buttonSlider.vectorTrans.x = (float)(scrollBar.iButtonSliderBegPosX + 
														((scrollBar.m_stSliderInfo.m_fCurValue) + fabs(scrollBar.m_stSliderInfo.m_fMinValue)) * scrollBar.fPixelInValue);
		scrollBar.buttonSlider.vectorTrans.y = (float)rectPos.iPosY;

		// Заполняем структуру RECT
		scrollBar.buttonSlider.rectButton.top = (int)scrollBar.buttonSlider.vectorTrans.y;
		scrollBar.buttonSlider.rectButton.left = (int)scrollBar.buttonSlider.vectorTrans.x;
		scrollBar.buttonSlider.rectButton.bottom = scrollBar.buttonSlider.rectButton.top + scrollBar.iSliderButtonWidth;
		scrollBar.buttonSlider.rectButton.right = scrollBar.buttonSlider.rectButton.left + scrollBar.iSliderButtonWidth;



			// ---------- Кнопка в обычном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, pcNameTextureButtonSlider, &scrollBar.buttonSlider.textureButton);		

		scrollBar.buttonSlider.textureButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonSlider))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonSlider))/(float)d3dsd.Height;
		scrollBar.buttonSlider.vectorScale = D3DXVECTOR2(sx, sy);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureButtonSliderSelection, &scrollBar.buttonSlider.textureSelectionButton);		

		scrollBar.buttonSlider.textureSelectionButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonSlider))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonSlider))/(float)d3dsd.Height;
		scrollBar.buttonSlider.vectorScale = D3DXVECTOR2(sx, sy);

			// ---------- Кнопка в выделеном состоянии (End) ----------*/

	}
	else // это ScrollBar и в эти координаты надо втиснуть и две квадратные кнопки
	{
		// ширина одной кнопки = высоте => что из ширины SliderString отнимаем 2*iWidthButtonNextPrev
		int iWidthButtonNextPrev = rectPos.iHeight;
		scrollBar.iSliderStringWidth = rectPos.iWidth - 2*iWidthButtonNextPrev;


		//  -------------------------- Рисуем SliderString ----------------------------------
		// позиция на экране
		scrollBar.vectorTrans.x = (float)(rectPos.iPosX + iWidthButtonNextPrev);
		scrollBar.vectorTrans.y = (float)rectPos.iPosY;


		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureSliderString, &scrollBar.textureSliderString);		

		scrollBar.textureSliderString->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(rectPos.iWidth - 2*iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(rectPos.iHeight))/(float)d3dsd.Height;
		scrollBar.vectorScale = D3DXVECTOR2(sx, sy);


		//  -------------------------- Рисуем SliderButton ----------------------------------
		scrollBar.iSliderButtonWidth = iWidthButtonNextPrev; // ширина рычажка (SliderButton)

		//static float sx =  0; 
		scrollBar.fPixelInValue = (scrollBar.iSliderStringWidth - scrollBar.iSliderButtonWidth)/scrollBar.m_stSliderInfo.m_fRaznizaMinMax;  // узнаём сколько в единице значения пикселей

		// ширина столки по которой ходит SliderButton
		scrollBar.iButtonSliderBegPosX = rectPos.iPosX + iWidthButtonNextPrev;  // от какой точки он будет двмгатся (он же должен двигатся только в прямоугольнике)
		//scrollBar.iButtonSliderBegPosX = rectPos.iPosX + scrollBar.m_stSliderInfo.m_fCurValue * sx;
		scrollBar.iSliderStringWidth = rectPos.iWidth - 3*iWidthButtonNextPrev;

	// позиция на экране
		scrollBar.buttonSlider.vectorTrans.x = (float)(scrollBar.iButtonSliderBegPosX + 
														((scrollBar.m_stSliderInfo.m_fCurValue) + fabs(scrollBar.m_stSliderInfo.m_fMinValue)) * scrollBar.fPixelInValue);
		scrollBar.buttonSlider.vectorTrans.y = (float)rectPos.iPosY;

		// Заполняем структуру RECT
		scrollBar.buttonSlider.rectButton.top = (int)scrollBar.buttonSlider.vectorTrans.y;
		scrollBar.buttonSlider.rectButton.left = (int)scrollBar.buttonSlider.vectorTrans.x;
		scrollBar.buttonSlider.rectButton.bottom = scrollBar.buttonSlider.rectButton.top + scrollBar.iSliderButtonWidth;
		scrollBar.buttonSlider.rectButton.right = scrollBar.buttonSlider.rectButton.left + scrollBar.iSliderButtonWidth;



			// ---------- Кнопка в обычном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureButtonSlider, &scrollBar.buttonSlider.textureButton);		

		scrollBar.buttonSlider.textureButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.buttonSlider.vectorScale = D3DXVECTOR2(sx, sy);

			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureButtonSliderSelection, &scrollBar.buttonSlider.textureSelectionButton);		

		scrollBar.buttonSlider.textureSelectionButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.buttonSlider.vectorScale = D3DXVECTOR2(sx, sy);
			// ---------- Кнопка в выделеном состоянии (End) ----------


		//  -------------------------- Рисуем SliderNEXT ----------------------------------
		// позиция на экране
		scrollBar.buttonNext.vectorTrans.x = (float)(rectPos.iPosX + rectPos.iWidth - iWidthButtonNextPrev);
		scrollBar.buttonNext.vectorTrans.y = (float)rectPos.iPosY;

		// Заполняем структуру RECT
		scrollBar.buttonNext.rectButton.top = (int)scrollBar.buttonNext.vectorTrans.y;
		scrollBar.buttonNext.rectButton.left = (int)scrollBar.buttonNext.vectorTrans.x;
		scrollBar.buttonNext.rectButton.bottom = scrollBar.buttonNext.rectButton.top + iWidthButtonNextPrev;
		scrollBar.buttonNext.rectButton.right = scrollBar.buttonNext.rectButton.left + iWidthButtonNextPrev;


			// ---------- Кнопка в обычном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, pcNameTextureButtonNext, &scrollBar.buttonNext.textureButton);		

		scrollBar.buttonNext.textureButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.buttonNext.vectorScale = D3DXVECTOR2(sx, sy);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, pcNameTextureButtonNextSelection, &scrollBar.buttonNext.textureSelectionButton);		

		scrollBar.buttonNext.textureSelectionButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.buttonNext.vectorScale = D3DXVECTOR2(sx, sy);
			// ---------- Кнопка в выделеном состоянии (End) ----------


		//  -------------------------- Рисуем SliderPREV ----------------------------------
		// позиция на экране
		scrollBar.buttonPrev.vectorTrans.x = (float)rectPos.iPosX;
		scrollBar.buttonPrev.vectorTrans.y = (float)rectPos.iPosY;

		// Заполняем структуру RECT
		scrollBar.buttonPrev.rectButton.top = (int)scrollBar.buttonPrev.vectorTrans.y;
		scrollBar.buttonPrev.rectButton.left = (int)scrollBar.buttonPrev.vectorTrans.x;
		scrollBar.buttonPrev.rectButton.bottom = scrollBar.buttonPrev.rectButton.top + iWidthButtonNextPrev;
		scrollBar.buttonPrev.rectButton.right = scrollBar.buttonPrev.rectButton.left + iWidthButtonNextPrev;


			// ---------- Кнопка в обычном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureButtonPrev, &scrollBar.buttonPrev.textureButton);		

		scrollBar.buttonPrev.textureButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.buttonPrev.vectorScale = D3DXVECTOR2(sx, sy);

			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureButtonPrevSelection, &scrollBar.buttonPrev.textureSelectionButton);		

		scrollBar.buttonPrev.textureSelectionButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.buttonPrev.vectorScale = D3DXVECTOR2(sx, sy);

			// ---------- Кнопка в выделеном состоянии (End) ----------
		//----------------------------------------------------------------------------------

	}

	// ===========================================================================


	if(iWindowID < 0)  // нет окна, поэтому просто создаём и храним в самом классе, а не в окне
	{
		m_stHorizScrollBar.push_back(scrollBar);
		m_iQuantHorizScrollBar ++;
	}

	hanlde.iIDWindow = iWindowID;
	hanlde.iIndex = m_iQuantHorizScrollBar - 1;
	return hanlde;
}

stHandleGui CManagerGUI::hCreateVertScrollBar(int iWindowID, stRECT rectPos, stSliderInfo info, void (*pFunc)(stSliderInfo *pSliderInfo), int iDelayButton, bool bSlider,
								 char *pcNameTextureButtonSlider,
								 char *pcNameTextureButtonSliderSelection,
								 char *pcNameTextureButtonPrev,
								 char *pcNameTextureButtonPrevSelection,
								 char *pcNameTextureButtonNext,
								 char *pcNameTextureButtonNextSelection,
								 char *pcNameTextureSliderString)
{
	stScrollBar scrollBar;
	stHandleGui hanlde;
	hanlde.typeElementGui = VSCROLL_BAR;

	//D3DXVECTOR2 *vectorLinePos = new D3DXVECTOR2[3]; // для рисования значков
	//D3DCOLOR colorLineSign = D3DCOLOR_XRGB(100,74,127);
	//D3DCOLOR colorLineSign = D3DCOLOR_XRGB(0,255,255);


	scrollBar.bSlider = bSlider;
	if(iDelayButton > 0)
		scrollBar.iDelayScroll = iDelayButton;

		scrollBar.m_stSliderInfo = info;
		scrollBar.m_stSliderInfo.m_fRaznizaMinMax = fabs(info.m_fMaxValue - (info.m_fMinValue));
		    
			// Если задано CurValue > MaxValue then CurValue = MaxValue, так же и с минимальным
			if(scrollBar.m_stSliderInfo.m_fCurValue > scrollBar.m_stSliderInfo.m_fMaxValue)
				scrollBar.m_stSliderInfo.m_fCurValue = scrollBar.m_stSliderInfo.m_fMaxValue;
			else if(scrollBar.m_stSliderInfo.m_fCurValue < scrollBar.m_stSliderInfo.m_fMinValue)
				scrollBar.m_stSliderInfo.m_fCurValue = scrollBar.m_stSliderInfo.m_fMinValue;

	scrollBar.pFuncExecute = pFunc;

	

	// ========================== Загрузка текстур кнопок ===============================
	float sx,sy;
	D3DSURFACE_DESC	d3dsd;

	// ------------------------- создаёт текстуру RadioButton = FALSE  (begin) ------------------

	if(bSlider) // значит строка слайдер на весь размер stRECT
	{
		int iWidthButtonSlider = rectPos.iWidth;
		scrollBar.iSliderButtonWidth = iWidthButtonSlider; // ширина рычажка (SliderButton)

		// позиция на экране
		scrollBar.vectorTrans.x = (float)rectPos.iPosX;
		scrollBar.vectorTrans.y = (float)rectPos.iPosY;

		//  -------------------------- Рисуем SliderString ----------------------------------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureSliderString, &scrollBar.textureSliderString);		

		scrollBar.textureSliderString->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(rectPos.iWidth))/d3dsd.Width;
		sy = (static_cast<float>(rectPos.iHeight))/(float)d3dsd.Height;
		scrollBar.vectorScale = D3DXVECTOR2(sx, sy);


			//  -------------------------- Рисуем SliderButton ----------------------------------

		// ширина столки по которой ходит SliderButton
		scrollBar.iButtonSliderBegPosY = rectPos.iPosY;  // от какой точки он будет двмгатся (он же должен двигатся только в прямоугольнике)
		scrollBar.iSliderStringWidth = rectPos.iHeight - iWidthButtonSlider;

		scrollBar.fPixelInValue = (scrollBar.iSliderStringWidth)/scrollBar.m_stSliderInfo.m_fRaznizaMinMax;  // узнаём сколько в единице значения пикселей

	// позиция на экране
		scrollBar.buttonSlider.vectorTrans.y = (float)(scrollBar.iButtonSliderBegPosY + 
														((scrollBar.m_stSliderInfo.m_fCurValue) + fabs(scrollBar.m_stSliderInfo.m_fMinValue)) * scrollBar.fPixelInValue);
		scrollBar.buttonSlider.vectorTrans.x = (float)rectPos.iPosX;

		// Заполняем структуру RECT
		scrollBar.buttonSlider.rectButton.top = (int)scrollBar.buttonSlider.vectorTrans.y;
		scrollBar.buttonSlider.rectButton.left = (int)scrollBar.buttonSlider.vectorTrans.x;
		scrollBar.buttonSlider.rectButton.bottom = scrollBar.buttonSlider.rectButton.top + scrollBar.iSliderButtonWidth;
		scrollBar.buttonSlider.rectButton.right = scrollBar.buttonSlider.rectButton.left + scrollBar.iSliderButtonWidth;



			// ---------- Кнопка в обычном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, pcNameTextureButtonSlider, &scrollBar.buttonSlider.textureButton);		

		scrollBar.buttonSlider.textureButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(scrollBar.iSliderButtonWidth))/(float)d3dsd.Width;
		sy = (static_cast<float>(scrollBar.iSliderButtonWidth))/(float)d3dsd.Height;
		scrollBar.buttonSlider.vectorScale = D3DXVECTOR2(sx, sy);

			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureButtonSliderSelection, &scrollBar.buttonSlider.textureSelectionButton);		

		scrollBar.buttonSlider.textureSelectionButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonSlider))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonSlider))/(float)d3dsd.Height;
		scrollBar.buttonSlider.vectorScale = D3DXVECTOR2(sx, sy);
			// ---------- Кнопка в выделеном состоянии (End) ----------*/
	}
	else // это ScrollBar и в эти координаты надо втиснуть и две квадратные кнопки
	{
		// ширина одной кнопки = высоте => что из ширины SliderString отнимаем 2*iWidthButtonNextPrev
		int iWidthButtonNextPrev = rectPos.iWidth;
		scrollBar.iSliderStringWidth = rectPos.iHeight - 2*iWidthButtonNextPrev;


		//  -------------------------- Рисуем SliderString ----------------------------------
		// позиция на экране
		scrollBar.vectorTrans.y = (float)(rectPos.iPosY + iWidthButtonNextPrev);
		scrollBar.vectorTrans.x = (float)rectPos.iPosX;

		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureSliderString, &scrollBar.textureSliderString);		

		scrollBar.textureSliderString->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(rectPos.iWidth))/d3dsd.Width;
		sy = (static_cast<float>(rectPos.iHeight - 2*iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.vectorScale = D3DXVECTOR2(sx, sy);


		//  -------------------------- Рисуем SliderButton ----------------------------------
		scrollBar.iSliderButtonWidth = iWidthButtonNextPrev; // ширина рычажка (SliderButton)

		//static float sx =  0; 
		scrollBar.fPixelInValue = (scrollBar.iSliderStringWidth - scrollBar.iSliderButtonWidth)/scrollBar.m_stSliderInfo.m_fRaznizaMinMax;  // узнаём сколько в единице значения пикселей

		// ширина столки по которой ходит SliderButton
		scrollBar.iButtonSliderBegPosY = rectPos.iPosY + iWidthButtonNextPrev;  // от какой точки он будет двмгатся (он же должен двигатся только в прямоугольнике)
		//scrollBar.iButtonSliderBegPosX = rectPos.iPosX + scrollBar.m_stSliderInfo.m_fCurValue * sx;
		scrollBar.iSliderStringWidth = rectPos.iHeight - 3*iWidthButtonNextPrev;

	// позиция на экране
		scrollBar.buttonSlider.vectorTrans.y = (float)(scrollBar.iButtonSliderBegPosY + 
														((scrollBar.m_stSliderInfo.m_fCurValue) + fabs(scrollBar.m_stSliderInfo.m_fMinValue)) * scrollBar.fPixelInValue);
		scrollBar.buttonSlider.vectorTrans.x = (float)rectPos.iPosX;

		// Заполняем структуру RECT
		scrollBar.buttonSlider.rectButton.top = (int)scrollBar.buttonSlider.vectorTrans.y;
		scrollBar.buttonSlider.rectButton.left = (int)scrollBar.buttonSlider.vectorTrans.x;
		scrollBar.buttonSlider.rectButton.bottom = scrollBar.buttonSlider.rectButton.top + scrollBar.iSliderButtonWidth;
		scrollBar.buttonSlider.rectButton.right = scrollBar.buttonSlider.rectButton.left + scrollBar.iSliderButtonWidth;



			// ---------- Кнопка в обычном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureButtonSlider, &scrollBar.buttonSlider.textureButton);		

		scrollBar.buttonSlider.textureButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.buttonSlider.vectorScale = D3DXVECTOR2(sx, sy);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureButtonSliderSelection, &scrollBar.buttonSlider.textureSelectionButton);		

		scrollBar.buttonSlider.textureSelectionButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.buttonSlider.vectorScale = D3DXVECTOR2(sx, sy);
			// ---------- Кнопка в выделеном состоянии (End) ----------


		//  -------------------------- Рисуем SliderNEXT ----------------------------------
		// позиция на экране
		scrollBar.buttonNext.vectorTrans.y = (float)(rectPos.iPosY + rectPos.iHeight - iWidthButtonNextPrev);
		scrollBar.buttonNext.vectorTrans.x = (float)rectPos.iPosX;

		// Заполняем структуру RECT
		scrollBar.buttonNext.rectButton.top = (int)scrollBar.buttonNext.vectorTrans.y;
		scrollBar.buttonNext.rectButton.left = (int)scrollBar.buttonNext.vectorTrans.x;
		scrollBar.buttonNext.rectButton.bottom = scrollBar.buttonNext.rectButton.top + iWidthButtonNextPrev;
		scrollBar.buttonNext.rectButton.right = scrollBar.buttonNext.rectButton.left + iWidthButtonNextPrev;


			// ---------- Кнопка в обычном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, pcNameTextureButtonNext, &scrollBar.buttonNext.textureButton);		

		scrollBar.buttonNext.textureButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.buttonNext.vectorScale = D3DXVECTOR2(sx, sy);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, pcNameTextureButtonNextSelection, &scrollBar.buttonNext.textureSelectionButton);		

		scrollBar.buttonNext.textureSelectionButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.buttonNext.vectorScale = D3DXVECTOR2(sx, sy);
			// ---------- Кнопка в выделеном состоянии (End) ----------


		//  -------------------------- Рисуем SliderPREV ----------------------------------
		// позиция на экране
		scrollBar.buttonPrev.vectorTrans.y = (float)rectPos.iPosY;
		scrollBar.buttonPrev.vectorTrans.x = (float)rectPos.iPosX;

		// Заполняем структуру RECT
		scrollBar.buttonPrev.rectButton.top = (int)scrollBar.buttonPrev.vectorTrans.y;
		scrollBar.buttonPrev.rectButton.left = (int)scrollBar.buttonPrev.vectorTrans.x;
		scrollBar.buttonPrev.rectButton.bottom = scrollBar.buttonPrev.rectButton.top + iWidthButtonNextPrev;
		scrollBar.buttonPrev.rectButton.right = scrollBar.buttonPrev.rectButton.left + iWidthButtonNextPrev;


			// ---------- Кнопка в обычном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureButtonPrev, &scrollBar.buttonPrev.textureButton);		

		scrollBar.buttonPrev.textureButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.buttonPrev.vectorScale = D3DXVECTOR2(sx, sy);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice,pcNameTextureButtonPrevSelection, &scrollBar.buttonPrev.textureSelectionButton);		

		scrollBar.buttonPrev.textureSelectionButton->GetLevelDesc(0,&d3dsd);
		sx = (static_cast<float>(iWidthButtonNextPrev))/d3dsd.Width;
		sy = (static_cast<float>(iWidthButtonNextPrev))/(float)d3dsd.Height;
		scrollBar.buttonPrev.vectorScale = D3DXVECTOR2(sx, sy);
			// ---------- Кнопка в выделеном состоянии (End) ----------
		//----------------------------------------------------------------------------------

	}
	// ===========================================================================


	if(iWindowID < 0)  // нет окна, поэтому просто создаём и храним в самом классе, а не в окне
	{
		m_stVertScrollBar.push_back(scrollBar);
		m_iQuantVertScrollBar ++;
	}

	hanlde.iIDWindow = iWindowID;
	hanlde.iIndex = m_iQuantVertScrollBar - 1;
	return hanlde;
}

stHandleGui CManagerGUI::hCreateVertScrollBar(int iWindowID, stRECT rectPos, stSliderInfo info,
											  void (*pFunc)(stSliderInfo *pSliderInfo), int iDelayButton, bool bSlider)
{
	stScrollBar scrollBar;
	stHandleGui hanlde;
	hanlde.typeElementGui = VSCROLL_BAR;

	D3DXVECTOR2 *vectorLinePos = new D3DXVECTOR2[3]; // для рисования значков
	//D3DCOLOR colorLineSign = D3DCOLOR_XRGB(100,74,127);
	D3DCOLOR colorLineSign = D3DCOLOR_XRGB(0,255,255);


	scrollBar.bSlider = bSlider;
	if(iDelayButton > 0)
		scrollBar.iDelayScroll = iDelayButton;

		scrollBar.m_stSliderInfo = info;
		scrollBar.m_stSliderInfo.m_fRaznizaMinMax = fabs(info.m_fMaxValue - (info.m_fMinValue));
		    
			// Если задано CurValue > MaxValue then CurValue = MaxValue, так же и с минимальным
			if(scrollBar.m_stSliderInfo.m_fCurValue > scrollBar.m_stSliderInfo.m_fMaxValue)
				scrollBar.m_stSliderInfo.m_fCurValue = scrollBar.m_stSliderInfo.m_fMaxValue;
			else if(scrollBar.m_stSliderInfo.m_fCurValue < scrollBar.m_stSliderInfo.m_fMinValue)
				scrollBar.m_stSliderInfo.m_fCurValue = scrollBar.m_stSliderInfo.m_fMinValue;

	scrollBar.pFuncExecute = pFunc;

	

	// =================================== Render To Texture ==============================
	LPDIRECT3DSURFACE9 backSurf;
	LPDIRECT3DSURFACE9 surfTexture;
	m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

	// ------------------------- создаёт текстуру RadioButton = FALSE  (begin) ------------------

	if(bSlider) // значит строка слайдер на весь размер stRECT
	{
		int iWidthButtonSlider = rectPos.iWidth;
		scrollBar.iSliderButtonWidth = iWidthButtonSlider; // ширина рычажка (SliderButton)

		// позиция на экране
		scrollBar.vectorTrans.x = (float)rectPos.iPosX;
		scrollBar.vectorTrans.y = (float)rectPos.iPosY;

		//  -------------------------- Рисуем SliderString ----------------------------------
		m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth, rectPos.iHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&scrollBar.textureSliderString,0);


		scrollBar.textureSliderString->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);
		m_directx->m_pD3DDevice->BeginScene();
			CManagerGUI::vDrawFrameRect(0,0,rectPos.iWidth,rectPos.iHeight,D3DCOLOR_XRGB(128,128,128),D3DCOLOR_XRGB(128,128,128),3);
		m_directx->m_pD3DDevice->EndScene();

		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf); 


			//  -------------------------- Рисуем SliderButton ----------------------------------

		// ширина столки по которой ходит SliderButton
		scrollBar.iButtonSliderBegPosY = rectPos.iPosY;  // от какой точки он будет двмгатся (он же должен двигатся только в прямоугольнике)
		scrollBar.iSliderStringWidth = rectPos.iHeight - iWidthButtonSlider;

		scrollBar.fPixelInValue = (scrollBar.iSliderStringWidth)/scrollBar.m_stSliderInfo.m_fRaznizaMinMax;  // узнаём сколько в единице значения пикселей

	// позиция на экране
		scrollBar.buttonSlider.vectorTrans.y = (float)(scrollBar.iButtonSliderBegPosY + 
														((scrollBar.m_stSliderInfo.m_fCurValue) + fabs(scrollBar.m_stSliderInfo.m_fMinValue)) * scrollBar.fPixelInValue);
		scrollBar.buttonSlider.vectorTrans.x = (float)rectPos.iPosX;

		// Заполняем структуру RECT
		scrollBar.buttonSlider.rectButton.top = (int)scrollBar.buttonSlider.vectorTrans.y;
		scrollBar.buttonSlider.rectButton.left = (int)scrollBar.buttonSlider.vectorTrans.x;
		scrollBar.buttonSlider.rectButton.bottom = scrollBar.buttonSlider.rectButton.top + scrollBar.iSliderButtonWidth;
		scrollBar.buttonSlider.rectButton.right = scrollBar.buttonSlider.rectButton.left + scrollBar.iSliderButtonWidth;



			// ---------- Кнопка в обычном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonSlider, iWidthButtonSlider, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonSlider.textureButton,0);

			scrollBar.buttonSlider.textureButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (70, 70, 70), 1.0f, 0);
			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonSlider,iWidthButtonSlider,
										D3DCOLOR_XRGB(128,128,128),D3DCOLOR_XRGB(128,128,128),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonSlider, iWidthButtonSlider, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonSlider.textureSelectionButton,0);

			scrollBar.buttonSlider.textureSelectionButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (70, 70, 70), 1.0f, 0);
			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonSlider,iWidthButtonSlider,
										D3DCOLOR_XRGB(255,128,128),D3DCOLOR_XRGB(255,128,128),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в выделеном состоянии (End) ----------*/
	}
	else // это ScrollBar и в эти координаты надо втиснуть и две квадратные кнопки
	{
		// ширина одной кнопки = высоте => что из ширины SliderString отнимаем 2*iWidthButtonNextPrev
		int iWidthButtonNextPrev = rectPos.iWidth;
		scrollBar.iSliderStringWidth = rectPos.iHeight - 2*iWidthButtonNextPrev;


		//  -------------------------- Рисуем SliderString ----------------------------------
		// позиция на экране
		scrollBar.vectorTrans.y = (float)(rectPos.iPosY + iWidthButtonNextPrev);
		scrollBar.vectorTrans.x = (float)rectPos.iPosX;

		m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth, rectPos.iHeight - 2*iWidthButtonNextPrev, 
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, 
			&scrollBar.textureSliderString,0);

		scrollBar.textureSliderString->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);
		m_directx->m_pD3DDevice->BeginScene();
			CManagerGUI::vDrawFrameRect(0,0,rectPos.iWidth,
							rectPos.iHeight - 2*iWidthButtonNextPrev,D3DCOLOR_XRGB(150,150,150),D3DCOLOR_XRGB(150,150,150),3);
		m_directx->m_pD3DDevice->EndScene();
		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);


		//  -------------------------- Рисуем SliderButton ----------------------------------
		scrollBar.iSliderButtonWidth = iWidthButtonNextPrev; // ширина рычажка (SliderButton)

		//static float sx =  0; 
		scrollBar.fPixelInValue = (scrollBar.iSliderStringWidth - scrollBar.iSliderButtonWidth)/scrollBar.m_stSliderInfo.m_fRaznizaMinMax;  // узнаём сколько в единице значения пикселей

		// ширина столки по которой ходит SliderButton
		scrollBar.iButtonSliderBegPosY = rectPos.iPosY + iWidthButtonNextPrev;  // от какой точки он будет двмгатся (он же должен двигатся только в прямоугольнике)
		//scrollBar.iButtonSliderBegPosX = rectPos.iPosX + scrollBar.m_stSliderInfo.m_fCurValue * sx;
		scrollBar.iSliderStringWidth = rectPos.iHeight - 3*iWidthButtonNextPrev;

	// позиция на экране
		scrollBar.buttonSlider.vectorTrans.y = (float)(scrollBar.iButtonSliderBegPosY + 
														((scrollBar.m_stSliderInfo.m_fCurValue) + fabs(scrollBar.m_stSliderInfo.m_fMinValue)) * scrollBar.fPixelInValue);
		scrollBar.buttonSlider.vectorTrans.x = (float)rectPos.iPosX;

		// Заполняем структуру RECT
		scrollBar.buttonSlider.rectButton.top = (int)scrollBar.buttonSlider.vectorTrans.y;
		scrollBar.buttonSlider.rectButton.left = (int)scrollBar.buttonSlider.vectorTrans.x;
		scrollBar.buttonSlider.rectButton.bottom = scrollBar.buttonSlider.rectButton.top + scrollBar.iSliderButtonWidth;
		scrollBar.buttonSlider.rectButton.right = scrollBar.buttonSlider.rectButton.left + scrollBar.iSliderButtonWidth;



			// ---------- Кнопка в обычном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonNextPrev, iWidthButtonNextPrev, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonSlider.textureButton,0);

			scrollBar.buttonSlider.textureButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (70, 70, 70), 1.0f, 0);
			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonNextPrev,iWidthButtonNextPrev,
										D3DCOLOR_XRGB(128,128,128),D3DCOLOR_XRGB(128,128,128),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonNextPrev, iWidthButtonNextPrev, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonSlider.textureSelectionButton,0);

			scrollBar.buttonSlider.textureSelectionButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (70, 70, 70), 1.0f, 0);
			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonNextPrev,iWidthButtonNextPrev,
										D3DCOLOR_XRGB(255,128,128),D3DCOLOR_XRGB(255,128,128),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в выделеном состоянии (End) ----------


		//  -------------------------- Рисуем SliderNEXT ----------------------------------
		// позиция на экране
		scrollBar.buttonNext.vectorTrans.y = (float)(rectPos.iPosY + rectPos.iHeight - iWidthButtonNextPrev);
		scrollBar.buttonNext.vectorTrans.x = (float)rectPos.iPosX;

		// Заполняем структуру RECT
		scrollBar.buttonNext.rectButton.top = (int)scrollBar.buttonNext.vectorTrans.y;
		scrollBar.buttonNext.rectButton.left = (int)scrollBar.buttonNext.vectorTrans.x;
		scrollBar.buttonNext.rectButton.bottom = scrollBar.buttonNext.rectButton.top + iWidthButtonNextPrev;
		scrollBar.buttonNext.rectButton.right = scrollBar.buttonNext.rectButton.left + iWidthButtonNextPrev;


			// ---------- Кнопка в обычном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonNextPrev, iWidthButtonNextPrev, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonNext.textureButton,0);

			scrollBar.buttonNext.textureButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (0,74,127), 1.0f, 0);
			
			// Рисование  значка линиями
			m_directx->m_pLine->SetWidth(2);
			m_directx->m_pLine->Begin();				
				vectorLinePos[0] = D3DXVECTOR2(3.0f, 3.0f);
				vectorLinePos[1] = D3DXVECTOR2((float)(iWidthButtonNextPrev/2), (float)(iWidthButtonNextPrev-3));
				vectorLinePos[2] = D3DXVECTOR2((float)(iWidthButtonNextPrev-3), 3.0f);					
				m_directx->m_pLine->Draw(vectorLinePos,3,colorLineSign);				
			m_directx->m_pLine->End();


			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonNextPrev,iWidthButtonNextPrev,
										D3DCOLOR_XRGB(0,127,127),D3DCOLOR_XRGB(0,127,127),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonNextPrev, iWidthButtonNextPrev, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonNext.textureSelectionButton,0);

			scrollBar.buttonNext.textureSelectionButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (0,74,127), 1.0f, 0);
			
			// Рисование  значка линиями
			m_directx->m_pLine->SetWidth(2);
			m_directx->m_pLine->Begin();
				vectorLinePos[0] = D3DXVECTOR2(3.0f, 3.0f);
				vectorLinePos[1] = D3DXVECTOR2((float)(iWidthButtonNextPrev/2), (float)(iWidthButtonNextPrev-3));
				vectorLinePos[2] = D3DXVECTOR2((float)(iWidthButtonNextPrev-3), 3.0f);				
				m_directx->m_pLine->Draw(vectorLinePos,3,D3DCOLOR_XRGB(255,128,128));
			m_directx->m_pLine->End();

			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonNextPrev,iWidthButtonNextPrev,
										D3DCOLOR_XRGB(255,128,128),D3DCOLOR_XRGB(255,128,128),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в выделеном состоянии (End) ----------


		//  -------------------------- Рисуем SliderPREV ----------------------------------
		// позиция на экране
		scrollBar.buttonPrev.vectorTrans.y = (float)rectPos.iPosY;
		scrollBar.buttonPrev.vectorTrans.x = (float)rectPos.iPosX;

		// Заполняем структуру RECT
		scrollBar.buttonPrev.rectButton.top = (int)scrollBar.buttonPrev.vectorTrans.y;
		scrollBar.buttonPrev.rectButton.left = (int)scrollBar.buttonPrev.vectorTrans.x;
		scrollBar.buttonPrev.rectButton.bottom = scrollBar.buttonPrev.rectButton.top + iWidthButtonNextPrev;
		scrollBar.buttonPrev.rectButton.right = scrollBar.buttonPrev.rectButton.left + iWidthButtonNextPrev;


			// ---------- Кнопка в обычном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonNextPrev, iWidthButtonNextPrev, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonPrev.textureButton,0);

			scrollBar.buttonPrev.textureButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (0,74,127), 1.0f, 0);

			// Рисование  значка линиями
			m_directx->m_pLine->SetWidth(2);
			m_directx->m_pLine->Begin();				
				vectorLinePos[0] = D3DXVECTOR2(3.0f, (float)(iWidthButtonNextPrev-3));
				vectorLinePos[1] = D3DXVECTOR2((float)(iWidthButtonNextPrev/2), 3.0f);
				vectorLinePos[2] = D3DXVECTOR2((float)(iWidthButtonNextPrev-3), (float)(iWidthButtonNextPrev-3));				
				m_directx->m_pLine->Draw(vectorLinePos,3,colorLineSign);				
			m_directx->m_pLine->End();

			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonNextPrev,iWidthButtonNextPrev,
										D3DCOLOR_XRGB(0,127,127),D3DCOLOR_XRGB(0,127,127),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в обычном состоянии (End) ----------


			// ---------- Кнопка в выделеном состоянии (Begin) ----------
			m_directx->m_pD3DDevice->CreateTexture(iWidthButtonNextPrev, iWidthButtonNextPrev, 
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT, 
				&scrollBar.buttonPrev.textureSelectionButton,0);

			scrollBar.buttonPrev.textureSelectionButton->GetSurfaceLevel(0, &surfTexture);
			m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

			//   следующую сточку можно коментировать для ускорения
			m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (0,74,127), 1.0f, 0);

			// Рисование  значка линиями
			m_directx->m_pLine->SetWidth(2);
			m_directx->m_pLine->Begin();				
				vectorLinePos[0] = D3DXVECTOR2(3.0f, (float)(iWidthButtonNextPrev-3));
				vectorLinePos[1] = D3DXVECTOR2((float)(iWidthButtonNextPrev/2), 3.0f);
				vectorLinePos[2] = D3DXVECTOR2((float)(iWidthButtonNextPrev-3), (float)(iWidthButtonNextPrev-3));					
				m_directx->m_pLine->Draw(vectorLinePos,3,colorLineSign);				
			m_directx->m_pLine->End();

			m_directx->m_pD3DDevice->BeginScene();
				CManagerGUI::vDrawFrameRect(0,0,iWidthButtonNextPrev,iWidthButtonNextPrev,
										D3DCOLOR_XRGB(255,128,128),D3DCOLOR_XRGB(255,128,128),3);
			m_directx->m_pD3DDevice->EndScene();
			m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			// ---------- Кнопка в выделеном состоянии (End) ----------
		//----------------------------------------------------------------------------------


		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
	}
	_DELETE_(vectorLinePos);
	// ===========================================================================


	if(iWindowID < 0)  // нет окна, поэтому просто создаём и храним в самом классе, а не в окне
	{
		m_stVertScrollBar.push_back(scrollBar);
		m_iQuantVertScrollBar ++;
	}

	hanlde.iIDWindow = iWindowID;
	hanlde.iIndex = m_iQuantVertScrollBar - 1;
	return hanlde;
}

bool CManagerGUI::bScrollBarOffsetCurValue(int iIndexScrollBar, bool bHoriz, float fDelta) // смещение текущего значения
{
	if(bHoriz)
	{
	if(iIndexScrollBar < m_iQuantHorizScrollBar)
	{
		static float summa = 0;
		summa = m_stHorizScrollBar[iIndexScrollBar].m_stSliderInfo.m_fCurValue + fDelta;

		if(summa >= m_stHorizScrollBar[iIndexScrollBar].m_stSliderInfo.m_fMaxValue)
			m_stHorizScrollBar[iIndexScrollBar].m_stSliderInfo.m_fCurValue = m_stHorizScrollBar[iIndexScrollBar].m_stSliderInfo.m_fMaxValue;
		else if(summa <= m_stHorizScrollBar[iIndexScrollBar].m_stSliderInfo.m_fMinValue)
			m_stHorizScrollBar[iIndexScrollBar].m_stSliderInfo.m_fCurValue = m_stHorizScrollBar[iIndexScrollBar].m_stSliderInfo.m_fMinValue;
		else
			m_stHorizScrollBar[iIndexScrollBar].m_stSliderInfo.m_fCurValue = summa;

		//----------------------
		/*
		static float raz = 0;
		static float sx =  0;
		raz = fabs(m_stScrollBar[iIndexScrollBar].m_stSliderInfo.m_fMaxValue - fabs(m_stScrollBar[iIndexScrollBar].m_stSliderInfo.m_fMinValue));
		sx = m_stScrollBar[iIndexScrollBar].iSliderStringWidth/raz;
		m_stScrollBar[iIndexScrollBar].buttonSlider.vectorTrans.x = m_stScrollBar[iIndexScrollBar].iButtonSliderBegPosX + 
																		m_stScrollBar[iIndexScrollBar].m_stSliderInfo.m_fCurValue*sx;

		// Перезаполняем структуру RECT	
		m_stScrollBar[iIndexScrollBar].buttonSlider.rectButton.top = (int)m_stScrollBar[iIndexScrollBar].buttonSlider.vectorTrans.y;
		m_stScrollBar[iIndexScrollBar].buttonSlider.rectButton.left = (int)m_stScrollBar[iIndexScrollBar].buttonSlider.vectorTrans.x;
		m_stScrollBar[iIndexScrollBar].buttonSlider.rectButton.bottom = m_stScrollBar[iIndexScrollBar].buttonSlider.rectButton.top + 
																								m_stScrollBar[iIndexScrollBar].iSliderButtonWidth;
		m_stScrollBar[iIndexScrollBar].buttonSlider.rectButton.right = m_stScrollBar[iIndexScrollBar].buttonSlider.rectButton.left +
																								m_stScrollBar[iIndexScrollBar].iSliderButtonWidth;
		*/

		return true;
	}
	}
	else
	{
		if(iIndexScrollBar < m_iQuantVertScrollBar)
		{
			static float summa = 0;
			summa = m_stVertScrollBar[iIndexScrollBar].m_stSliderInfo.m_fCurValue + fDelta;

			if(summa >= m_stVertScrollBar[iIndexScrollBar].m_stSliderInfo.m_fMaxValue)
				m_stVertScrollBar[iIndexScrollBar].m_stSliderInfo.m_fCurValue = m_stVertScrollBar[iIndexScrollBar].m_stSliderInfo.m_fMaxValue;
			else if(summa <= m_stVertScrollBar[iIndexScrollBar].m_stSliderInfo.m_fMinValue)
				m_stVertScrollBar[iIndexScrollBar].m_stSliderInfo.m_fCurValue = m_stVertScrollBar[iIndexScrollBar].m_stSliderInfo.m_fMinValue;
			else
				m_stVertScrollBar[iIndexScrollBar].m_stSliderInfo.m_fCurValue = summa;
		}
	}

	return false;
}

void CManagerGUI::vCalculate(stMouseInfoGUI *mouseInfoGui)
{
	// Вызывать в главном цикле, передаём позицию мыши, где была нажата и отпущена клавиша мыши
	// А также обрабатываем внутрение функции, например счётчик кадров
	DWORD dwTickCount = GetTickCount();
	static bool bEnableCalculate = true; /* С помощью этой переменной можно запреть вычисление некоторых инструкций 
											Например если мышь находится над определённым окном значит запретить 
											необходимо обрабатывать только это окно и не обрабатывать то что за ним */
	
	
	// ----------------------- Обработка окон (begin) -------------------------------------------------------
	bEnableCalculate = bCalculateWindowPanels(mouseInfoGui);
	// ----------------------- Обработка окон (end) -------------------------------------------------------



	// ------------------------------------------------------------------------------------------------------
	// Проверка кнопок
	for(i = 0; i < m_iQuantButtons; i++)
	{
		if(m_stButtons[i].bVisible)  // Кнопка видима
		{
			// выделение кнопки
			if(PtInRect(&m_stButtons[i].rectButton, mouseInfoGui->pointMousePos) && (bEnableCalculate)) // лежит ли точка в прямоугольнике
			{
				if(m_stButtons[i].bSelectEnable)
					m_stButtons[i].pTextureRender = &m_stButtons[i].textureSelectionButton;
			}
			else // мышка находится не над кнопкой
				m_stButtons[i].pTextureRender = &m_stButtons[i].textureButton;

			// проверяем нажатия на элементе
			if((mouseInfoGui->bLButtonClick) && (bEnableCalculate))
			if(PtInRect(&m_stButtons[i].rectButton, mouseInfoGui->pointLBDownPos))
				if(PtInRect(&m_stButtons[i].rectButton, mouseInfoGui->pointLBUpPos))
				{
					// значит прошёл цикл нажатия и отпускания левой клавиши
					m_stButtons[i].pFuncExecute();
				}
		}
		else if(!m_stButtons[i].bVisible)  // Кнопка не видима
			m_stButtons[i].pTextureRender = NULL;

	} // Проверка кнопок
	// ------------------------------------------------------------------------------------------------------




	// ------------------------------------------------------------------------------------------------------
	// Проверка CheckBox
	for(i = 0; i < m_iQuantCheckBox; i++)
	{
		if(m_stCheckBox[i].bVisible)  // CheckBox видимый
		{
			// проверяем нажатия на элементе
			if(mouseInfoGui->bLButtonClick)
			{
				if(PtInRect(&m_stCheckBox[i].rectCheckBox, mouseInfoGui->pointLBDownPos) &&(bEnableCalculate))
					if(PtInRect(&m_stCheckBox[i].rectCheckBox, mouseInfoGui->pointLBUpPos) &&(bEnableCalculate))
					{
						// значит прошёл цикл нажатия и отпускания левой клавиши
						m_stCheckBox[i].bCheckBoxState = !m_stCheckBox[i].bCheckBoxState;  // меняем состояние на противоположное
						m_stCheckBox[i].bRunFunc = true;
						//m_stCheckBox[i].pFuncExecute(m_stCheckBox[i].bCheckBoxState); // что-бы функции запускались при изменении внутреннего состояния, А НЕ КАЖДУЮ ИТЕРАЦИЮ
					}
					if(m_stCheckBox[i].bRunFunc) // Чтобы при первом запуске выполнились функции
					{
						m_stCheckBox[i].bRunFunc = false;
						m_stCheckBox[i].pFuncExecute(m_stCheckBox[i].bCheckBoxState); // что-бы функции запускались при изменении внутреннего состояния, А НЕ КАЖДУЮ ИТЕРАЦИЮ
					}
			}

			if(m_stCheckBox[i].bCheckBoxState)
				m_stCheckBox[i].pTextureRender = &m_stCheckBox[i].textureCheckBoxON;
			else //if(!m_stCheckBox[i].bCheckBoxState)
				m_stCheckBox[i].pTextureRender = &m_stCheckBox[i].textureCheckBoxOFF;
		}
		else if(!m_stCheckBox[i].bVisible)  // CheckBox не видима
			m_stCheckBox[i].pTextureRender = NULL;
	} // Проверка CheckBox
	// ------------------------------------------------------------------------------------------------------


	// ------------------------------------------------------------------------------------------------------
	// Проверка RadioButton
	for(i = 0; i < m_iQuantGroup; i++)
	for(j = 0; j < m_stGroup[i].m_iQuantRadioButton; j++)
	{
		if(m_stGroup[i].m_stRadioButton[j].bVisible)  // RadioButton видимый
		{
			// проверяем нажатия на элементе
			if((mouseInfoGui->bLButtonClick) && (bEnableCalculate))
			{
				if(PtInRect(&m_stGroup[i].m_stRadioButton[j].rectRadioButton, mouseInfoGui->pointLBDownPos))
					if(PtInRect(&m_stGroup[i].m_stRadioButton[j].rectRadioButton, mouseInfoGui->pointLBUpPos))
					{
						// значит прошёл цикл нажатия и отпускания левой клавиши

						if(!m_stGroup[i].m_stRadioButton[j].bRadioButtonState)  // Кнопка уже выбрана ничего не делаем
						{
							m_stGroup[i].m_stRadioButton[j].bRadioButtonState = !m_stGroup[i].m_stRadioButton[j].bRadioButtonState;  // меняем состояние на противоположное
							m_stGroup[i].m_stRadioButton[m_stGroup[i].iCurIndexRadioButton].bRadioButtonState = false;  // предыдущее кнопка = false
							m_stGroup[i].iCurIndexRadioButton = j;  // теперь текщая кнопка выбрана
							m_stGroup[i].bRunRadioButtonFunc = true;
							//m_stGroup[i].m_stRadioButton[j].pFuncExecute(m_stGroup[i].m_stRadioButton[j].bRadioButtonState); // что-бы функции запускались при изменении внутреннего состояния, А НЕ КАЖДУЮ ИТЕРАЦИЮ
						}
					}
			}

				if(m_stGroup[i].bRunRadioButtonFunc) // Чтобы при первом запуске выполнились функции
				{
					m_stGroup[i].bRunRadioButtonFunc = false;
					m_stGroup[i].m_stRadioButton[j].pFuncExecute(m_stGroup[i].m_stRadioButton[j].bRadioButtonState); // что-бы функции запускались при изменении внутреннего состояния, А НЕ КАЖДУЮ ИТЕРАЦИЮ
				}

				if(m_stGroup[i].m_stRadioButton[j].bRadioButtonState)
					m_stGroup[i].m_stRadioButton[j].pTextureRender = &m_stGroup[i].m_stRadioButton[j].textureRadioButtonON;
				else //if(!bCheckBoxState)
					m_stGroup[i].m_stRadioButton[j].pTextureRender = &m_stGroup[i].m_stRadioButton[j].textureRadioButtonOFF;
		}
		else if(!m_stGroup[i].m_stRadioButton[j].bVisible)  // RadioButton не видима
			m_stGroup[i].m_stRadioButton[j].pTextureRender = NULL;
	} // Проверка RadioButton
	// ------------------------------------------------------------------------------------------------------



	// ------------------------------------------------------------------------------------------------------
	// Проверка ImageBox
	// Проверяем счёткик кадров, если надо то меняем кадры
	for(i = 0; i < m_iQuantImageBox; i++)
		if((m_stImageBox[i].bAnimEnable) && (m_stImageBox[i].bVisible))
		{		
			if(dwTickCount - m_stImageBox[i].dwLastUpdate >= (DWORD)m_stImageBox[i].iDelayFrame)
			{
				//m_stImageBox[i].dwLastUpdate = dwTickCount;
				//m_stImageBox[i].iNumCurCadr++;
				
				// теперь будет пропускать кадры которые должны были отрисоваться но не хватило времени
				m_stImageBox[i].iNumCurCadr += (dwTickCount - m_stImageBox[i].dwLastUpdate)/(DWORD)m_stImageBox[i].iDelayFrame;
				m_stImageBox[i].dwLastUpdate = dwTickCount;

				if(m_stImageBox[i].iNumCurCadr >= m_stImageBox[i].iQuantCadr)
					m_stImageBox[i].iNumCurCadr = 0;
			}
		}// Проверка ImageBox
	// ------------------------------------------------------------------------------------------------------


	// ----------------------------------------------------------------------------------------------------------------------
	// Проверка ГОРИЗОНТАЛЬНЫХ  ScrollBar
		for(i = 0; i < m_iQuantHorizScrollBar; i++)
		{
			if(m_stHorizScrollBar[i].bVisible)  // ScrollBar видима
			{
				// =================================== Проверяем Выделение (begin) ================================

				if(!m_stHorizScrollBar[i].bSlider)
				{
					// ------------------------------- ButtonNext -------------------------------------------
					if(PtInRect(&m_stHorizScrollBar[i].buttonNext.rectButton, mouseInfoGui->pointMousePos) && bEnableCalculate) // лежит ли точка в прямоугольнике
					{
						if(m_stHorizScrollBar[i].buttonNext.bSelectEnable)
							m_stHorizScrollBar[i].buttonNext.pTextureRender = &m_stHorizScrollBar[i].buttonNext.textureSelectionButton;
					}
					else // мышка находится не над кнопкой
						m_stHorizScrollBar[i].buttonNext.pTextureRender = &m_stHorizScrollBar[i].buttonNext.textureButton;

					// ------------------------------- ButtonPrev -------------------------------------------
					if(PtInRect(&m_stHorizScrollBar[i].buttonPrev.rectButton, mouseInfoGui->pointMousePos) && bEnableCalculate) // лежит ли точка в прямоугольнике
					{
						if(m_stHorizScrollBar[i].buttonPrev.bSelectEnable)
							m_stHorizScrollBar[i].buttonPrev.pTextureRender = &m_stHorizScrollBar[i].buttonPrev.textureSelectionButton;
					}
					else // мышка находится не над кнопкой
						m_stHorizScrollBar[i].buttonPrev.pTextureRender = &m_stHorizScrollBar[i].buttonPrev.textureButton;
				}

				// ------------------------------- ButtonSlider -------------------------------------------
				if(PtInRect(&m_stHorizScrollBar[i].buttonSlider.rectButton, mouseInfoGui->pointMousePos) && bEnableCalculate) // лежит ли точка в прямоугольнике
				{
					if(m_stHorizScrollBar[i].buttonSlider.bSelectEnable)
						m_stHorizScrollBar[i].buttonSlider.pTextureRender = &m_stHorizScrollBar[i].buttonSlider.textureSelectionButton;
				}
				else // мышка находится не над кнопкой
					m_stHorizScrollBar[i].buttonSlider.pTextureRender = &m_stHorizScrollBar[i].buttonSlider.textureButton;
				// =================================== Проверяем Выделение (end) ================================



				// =================================== Проверяем нажатие на каждой кнопке(begin) ================================				
				if(mouseInfoGui->bLButtonDown) // Левая клавиша зажата
				{
					if(!m_stHorizScrollBar[i].bFollowMouse)
					{
						if(!m_stHorizScrollBar[i].bSlider)
						{
							// ------------------------------- ButtonNext -------------------------------------------
							if(PtInRect(&m_stHorizScrollBar[i].buttonNext.rectButton, mouseInfoGui->pointMousePos) && bEnableCalculate)
							{
								m_stHorizScrollBar[i].bFollowMouse = false;
								if(dwTickCount - m_stHorizScrollBar[i].dwLastUpdate >= (DWORD)m_stHorizScrollBar[i].iDelayScroll)
								{					
									m_stHorizScrollBar[i].dwLastUpdate = dwTickCount;
									CManagerGUI::bScrollBarOffsetCurValue(i, true, m_stHorizScrollBar[i].m_stSliderInfo.fDelta);

									// перемещаем рычажок и перезаполняем структуру RECT  SliderButton	
									m_stHorizScrollBar[i].buttonSlider.vectorTrans.x = (float)(m_stHorizScrollBar[i].iButtonSliderBegPosX + 
										(m_stHorizScrollBar[i].m_stSliderInfo.m_fCurValue
											+ fabs(m_stHorizScrollBar[i].m_stSliderInfo.m_fMinValue)) * m_stHorizScrollBar[i].fPixelInValue);

									// Перезаполняем структуру RECT	
									m_stHorizScrollBar[i].buttonSlider.rectButton.top = (int)m_stHorizScrollBar[i].buttonSlider.vectorTrans.y;
									m_stHorizScrollBar[i].buttonSlider.rectButton.left = (int)m_stHorizScrollBar[i].buttonSlider.vectorTrans.x;
									m_stHorizScrollBar[i].buttonSlider.rectButton.bottom = m_stHorizScrollBar[i].buttonSlider.rectButton.top + 
										m_stHorizScrollBar[i].iSliderButtonWidth;
									m_stHorizScrollBar[i].buttonSlider.rectButton.right = m_stHorizScrollBar[i].buttonSlider.rectButton.left +
										m_stHorizScrollBar[i].iSliderButtonWidth;
								}	
							}
				
							// ------------------------------- ButtonPrev -------------------------------------------
							else if(PtInRect(&m_stHorizScrollBar[i].buttonPrev.rectButton, mouseInfoGui->pointMousePos) && bEnableCalculate)
							{
								m_stHorizScrollBar[i].bFollowMouse = false;
								if(dwTickCount - m_stHorizScrollBar[i].dwLastUpdate >= (DWORD)m_stHorizScrollBar[i].iDelayScroll)
								{					
									m_stHorizScrollBar[i].dwLastUpdate = dwTickCount;
									CManagerGUI::bScrollBarOffsetCurValue(i,true, -m_stHorizScrollBar[i].m_stSliderInfo.fDelta);

									// перемещаем рычажок и перезаполняем структуру RECT  SliderButton
									m_stHorizScrollBar[i].buttonSlider.vectorTrans.x = (float)(m_stHorizScrollBar[i].iButtonSliderBegPosX + 
																			(m_stHorizScrollBar[i].m_stSliderInfo.m_fCurValue
																			+ fabs(m_stHorizScrollBar[i].m_stSliderInfo.m_fMinValue)) * m_stHorizScrollBar[i].fPixelInValue);

									// Перезаполняем структуру RECT	
									m_stHorizScrollBar[i].buttonSlider.rectButton.top = (int)m_stHorizScrollBar[i].buttonSlider.vectorTrans.y;
									m_stHorizScrollBar[i].buttonSlider.rectButton.left = (int)m_stHorizScrollBar[i].buttonSlider.vectorTrans.x;
									m_stHorizScrollBar[i].buttonSlider.rectButton.bottom = m_stHorizScrollBar[i].buttonSlider.rectButton.top + 
										m_stHorizScrollBar[i].iSliderButtonWidth;
									m_stHorizScrollBar[i].buttonSlider.rectButton.right = m_stHorizScrollBar[i].buttonSlider.rectButton.left +
										m_stHorizScrollBar[i].iSliderButtonWidth;
								}
							}
						}

						if(PtInRect(&m_stHorizScrollBar[i].buttonSlider.rectButton, mouseInfoGui->pointLBDownPos) && bEnableCalculate)
								m_stHorizScrollBar[i].bFollowMouse = true;
					
						}//if(!m_stScrollBar[i].bFollowMouse) Чтобы при скролинге за рычажок не мешали кнопки next, prev

					// ------------------------------- ButtonSlider -------------------------------------------
					if(m_stHorizScrollBar[i].bFollowMouse)
					{
						// перемещаем рычажок и перезаполняем структуру RECT  SliderButton
						if((mouseInfoGui->pointMousePos.x >= m_stHorizScrollBar[i].iButtonSliderBegPosX) && 
														(mouseInfoGui->pointMousePos.x <= m_stHorizScrollBar[i].iButtonSliderBegPosX + m_stHorizScrollBar[i].iSliderStringWidth ))
							m_stHorizScrollBar[i].buttonSlider.vectorTrans.x = (float)mouseInfoGui->pointMousePos.x;
						
						//float cur;
						m_stHorizScrollBar[i].m_stSliderInfo.m_fCurValue = ((m_stHorizScrollBar[i].buttonSlider.vectorTrans.x - m_stHorizScrollBar[i].iButtonSliderBegPosX)/
							m_stHorizScrollBar[i].fPixelInValue) - fabs(m_stHorizScrollBar[i].m_stSliderInfo.m_fMinValue);

						// Перезаполняем структуру RECT	
						m_stHorizScrollBar[i].buttonSlider.rectButton.top = (int)m_stHorizScrollBar[i].buttonSlider.vectorTrans.y;
						m_stHorizScrollBar[i].buttonSlider.rectButton.left = (int)m_stHorizScrollBar[i].buttonSlider.vectorTrans.x;
						m_stHorizScrollBar[i].buttonSlider.rectButton.bottom = m_stHorizScrollBar[i].buttonSlider.rectButton.top + 
							m_stHorizScrollBar[i].iSliderButtonWidth;
						m_stHorizScrollBar[i].buttonSlider.rectButton.right = m_stHorizScrollBar[i].buttonSlider.rectButton.left +
							m_stHorizScrollBar[i].iSliderButtonWidth;
					}
				} //if(mouseInfoGui->bLButtonDown) // Левая клавиша зажата
				else if(!mouseInfoGui->bLButtonDown)  // Левая клавиша отпущена
					m_stHorizScrollBar[i].bFollowMouse = false; 
				// =================================== Проверяем нажатие на каждой кнопке(end) ================================
			}
			//else if(!m_stButtons[i].bVisible)  // Кнопка не видима
				//m_stButtons[i].pTextureRender = NULL;

			m_stHorizScrollBar[i].pFuncExecute(&m_stHorizScrollBar[i].m_stSliderInfo);
		}// Проверка ScrollBar
		//----------------------------------------------------------------------------------------------------------------------


		//----------------------------------------------------------------------------------------------------------------------
		// Проверка ВЕРТИКАЛЬНЫХ  ScrollBar-
		for(i = 0; i < m_iQuantVertScrollBar; i++)
		{
			if(m_stVertScrollBar[i].bVisible)  // ScrollBar видима
			{
				// =================================== Проверяем Выделение (begin) ================================

				if(!m_stVertScrollBar[i].bSlider)
				{
					// ------------------------------- ButtonNext -------------------------------------------
					if(PtInRect(&m_stVertScrollBar[i].buttonNext.rectButton, mouseInfoGui->pointMousePos) && bEnableCalculate) // лежит ли точка в прямоугольнике
					{
						if(m_stVertScrollBar[i].buttonNext.bSelectEnable)
							m_stVertScrollBar[i].buttonNext.pTextureRender = &m_stVertScrollBar[i].buttonNext.textureSelectionButton;
					}
					else // мышка находится не над кнопкой
						m_stVertScrollBar[i].buttonNext.pTextureRender = &m_stVertScrollBar[i].buttonNext.textureButton;

					// ------------------------------- ButtonPrev -------------------------------------------
					if(PtInRect(&m_stVertScrollBar[i].buttonPrev.rectButton, mouseInfoGui->pointMousePos) && bEnableCalculate) // лежит ли точка в прямоугольнике
					{
						if(m_stVertScrollBar[i].buttonPrev.bSelectEnable)
							m_stVertScrollBar[i].buttonPrev.pTextureRender = &m_stVertScrollBar[i].buttonPrev.textureSelectionButton;
					}
					else // мышка находится не над кнопкой
						m_stVertScrollBar[i].buttonPrev.pTextureRender = &m_stVertScrollBar[i].buttonPrev.textureButton;
				}

				// ------------------------------- ButtonSlider -------------------------------------------
				if(PtInRect(&m_stVertScrollBar[i].buttonSlider.rectButton, mouseInfoGui->pointMousePos) && bEnableCalculate) // лежит ли точка в прямоугольнике
				{
					if(m_stVertScrollBar[i].buttonSlider.bSelectEnable)
						m_stVertScrollBar[i].buttonSlider.pTextureRender = &m_stVertScrollBar[i].buttonSlider.textureSelectionButton;
				}
				else // мышка находится не над кнопкой
					m_stVertScrollBar[i].buttonSlider.pTextureRender = &m_stVertScrollBar[i].buttonSlider.textureButton;
				// =================================== Проверяем Выделение (end) ================================



				// =================================== Проверяем нажатие на каждой кнопке(begin) ================================				
				if(mouseInfoGui->bLButtonDown) // Левая клавиша зажата
				{
					if(!m_stVertScrollBar[i].bFollowMouse)
					{
						if(!m_stVertScrollBar[i].bSlider)
						{
							// ------------------------------- ButtonNext -------------------------------------------
							if(PtInRect(&m_stVertScrollBar[i].buttonNext.rectButton, mouseInfoGui->pointMousePos) && bEnableCalculate)
							{
								m_stVertScrollBar[i].bFollowMouse = false;
								if(dwTickCount - m_stVertScrollBar[i].dwLastUpdate >= (DWORD)m_stVertScrollBar[i].iDelayScroll)
								{					
									m_stVertScrollBar[i].dwLastUpdate = dwTickCount;
									CManagerGUI::bScrollBarOffsetCurValue(i, false, m_stVertScrollBar[i].m_stSliderInfo.fDelta);

									// перемещаем рычажок и перезаполняем структуру RECT  SliderButton	
									m_stVertScrollBar[i].buttonSlider.vectorTrans.y = (float)(m_stVertScrollBar[i].iButtonSliderBegPosY + 
										(m_stVertScrollBar[i].m_stSliderInfo.m_fCurValue
											+ fabs(m_stVertScrollBar[i].m_stSliderInfo.m_fMinValue)) * m_stVertScrollBar[i].fPixelInValue);

									// Перезаполняем структуру RECT	
									m_stVertScrollBar[i].buttonSlider.rectButton.top = (int)m_stVertScrollBar[i].buttonSlider.vectorTrans.y;
									m_stVertScrollBar[i].buttonSlider.rectButton.left = (int)m_stVertScrollBar[i].buttonSlider.vectorTrans.x;
									m_stVertScrollBar[i].buttonSlider.rectButton.bottom = m_stVertScrollBar[i].buttonSlider.rectButton.top + 
										m_stVertScrollBar[i].iSliderButtonWidth;
									m_stVertScrollBar[i].buttonSlider.rectButton.right = m_stVertScrollBar[i].buttonSlider.rectButton.left +
										m_stVertScrollBar[i].iSliderButtonWidth;
								}	
							}
				
							// ------------------------------- ButtonPrev -------------------------------------------
							else if(PtInRect(&m_stVertScrollBar[i].buttonPrev.rectButton, mouseInfoGui->pointMousePos) && bEnableCalculate)
							{
								m_stVertScrollBar[i].bFollowMouse = false;
								if(dwTickCount - m_stVertScrollBar[i].dwLastUpdate >= (DWORD)m_stVertScrollBar[i].iDelayScroll)
								{					
									m_stVertScrollBar[i].dwLastUpdate = dwTickCount;
									CManagerGUI::bScrollBarOffsetCurValue(i,false, -m_stVertScrollBar[i].m_stSliderInfo.fDelta);

									// перемещаем рычажок и перезаполняем структуру RECT  SliderButton
									m_stVertScrollBar[i].buttonSlider.vectorTrans.y = (float)(m_stVertScrollBar[i].iButtonSliderBegPosY + 
																			(m_stVertScrollBar[i].m_stSliderInfo.m_fCurValue
																			+ fabs(m_stVertScrollBar[i].m_stSliderInfo.m_fMinValue)) * m_stVertScrollBar[i].fPixelInValue);

									// Перезаполняем структуру RECT	
									m_stVertScrollBar[i].buttonSlider.rectButton.top = (int)m_stVertScrollBar[i].buttonSlider.vectorTrans.y;
									m_stVertScrollBar[i].buttonSlider.rectButton.left = (int)m_stVertScrollBar[i].buttonSlider.vectorTrans.x;
									m_stVertScrollBar[i].buttonSlider.rectButton.bottom = m_stVertScrollBar[i].buttonSlider.rectButton.top + 
										m_stVertScrollBar[i].iSliderButtonWidth;
									m_stVertScrollBar[i].buttonSlider.rectButton.right = m_stVertScrollBar[i].buttonSlider.rectButton.left +
										m_stVertScrollBar[i].iSliderButtonWidth;
								}
							}
						}

						if(PtInRect(&m_stVertScrollBar[i].buttonSlider.rectButton, mouseInfoGui->pointLBDownPos) && bEnableCalculate)
								m_stVertScrollBar[i].bFollowMouse = true;
					
						}//if(!m_stScrollBar[i].bFollowMouse) Чтобы при скролинге за рычажок не мешали кнопки next, prev

					// ------------------------------- ButtonSlider -------------------------------------------
					if(m_stVertScrollBar[i].bFollowMouse)
					{
						// перемещаем рычажок и перезаполняем структуру RECT  SliderButton
						if((mouseInfoGui->pointMousePos.y >= m_stVertScrollBar[i].iButtonSliderBegPosY) && 
														(mouseInfoGui->pointMousePos.y <= m_stVertScrollBar[i].iButtonSliderBegPosY + m_stVertScrollBar[i].iSliderStringWidth ))
							m_stVertScrollBar[i].buttonSlider.vectorTrans.y = (float)mouseInfoGui->pointMousePos.y;
						
						//float cur;
						m_stVertScrollBar[i].m_stSliderInfo.m_fCurValue = ((m_stVertScrollBar[i].buttonSlider.vectorTrans.y - m_stVertScrollBar[i].iButtonSliderBegPosY)/
							m_stVertScrollBar[i].fPixelInValue) - fabs(m_stVertScrollBar[i].m_stSliderInfo.m_fMinValue);

						// Перезаполняем структуру RECT	
						m_stVertScrollBar[i].buttonSlider.rectButton.top = (int)m_stVertScrollBar[i].buttonSlider.vectorTrans.y;
						m_stVertScrollBar[i].buttonSlider.rectButton.left = (int)m_stVertScrollBar[i].buttonSlider.vectorTrans.x;
						m_stVertScrollBar[i].buttonSlider.rectButton.bottom = m_stVertScrollBar[i].buttonSlider.rectButton.top + 
							m_stVertScrollBar[i].iSliderButtonWidth;
						m_stVertScrollBar[i].buttonSlider.rectButton.right = m_stVertScrollBar[i].buttonSlider.rectButton.left +
							m_stVertScrollBar[i].iSliderButtonWidth;
					}
				} //if(mouseInfoGui->bLButtonDown) // Левая клавиша зажата
				else if(!mouseInfoGui->bLButtonDown)  // Левая клавиша отпущена
					m_stVertScrollBar[i].bFollowMouse = false; 
				// =================================== Проверяем нажатие на каждой кнопке(end) ================================
			}
			//else if(!m_stButtons[i].bVisible)  // Кнопка не видима
				//m_stButtons[i].pTextureRender = NULL;

			m_stVertScrollBar[i].pFuncExecute(&m_stVertScrollBar[i].m_stSliderInfo);
		}// Проверка ScrollBar
		// ----------------------------------------------------------------------------------------------------------------------



	// ------------------------------------------------------------------------------------------------------
	// Устанавливаем координаты мышки и если необходимо меняем кадр
		if(m_bCreatedMouse)
		if(m_stGuiMouse[m_iCurNumMouse].image.bVisible)
		{
			m_stGuiMouse[m_iCurNumMouse].image.vectorTrans = D3DXVECTOR2((float)mouseInfoGui->pointMousePos.x, 
																		 (float)mouseInfoGui->pointMousePos.y);

			if(m_stGuiMouse[m_iCurNumMouse].image.bAnimEnable)	
				if(dwTickCount - m_stGuiMouse[m_iCurNumMouse].image.dwLastUpdate >= (DWORD)m_stGuiMouse[m_iCurNumMouse].image.iDelayFrame)
				{
					//m_stGuiMouse[m_iCurNumMouse].image.dwLastUpdate = dwTickCount;
					//m_stGuiMouse[m_iCurNumMouse].image.iNumCurCadr++;
					
					// теперь будет пропускать кадры которые должны были отрисоваться но не хватило времени
					m_stGuiMouse[m_iCurNumMouse].image.iNumCurCadr += (dwTickCount - m_stGuiMouse[m_iCurNumMouse].image.dwLastUpdate)/(DWORD)m_stGuiMouse[m_iCurNumMouse].image.iDelayFrame;
					m_stGuiMouse[m_iCurNumMouse].image.dwLastUpdate = dwTickCount;

					if(m_stGuiMouse[m_iCurNumMouse].image.iNumCurCadr >= m_stGuiMouse[m_iCurNumMouse].image.iQuantCadr)
						m_stGuiMouse[m_iCurNumMouse].image.iNumCurCadr = 0;
				}
		}// Устанавливаем координаты мышки и если необходимо меняем кадр
	// ------------------------------------------------------------------------------------------------------

	// Обработали элементы сбрасываем сообщения о нажатии
	mouseInfoGui->bLButtonClick = false;
	
	/*mouseInfoGui->pointLBUpPos.x = -10;
	mouseInfoGui->pointLBUpPos.y = -10;
	mouseInfoGui->pointLBDownPos.x = -10;
	mouseInfoGui->pointLBDownPos.y = -10;*/
}


bool CManagerGUI::bShow(stHandleGui *handleGui)
{
	switch(handleGui->typeElementGui)
	{
		case BUTTON:
				if(handleGui->typeWindow == NOT_ELEM)
				{
					m_stButtons[handleGui->iIndex].bVisible = true;
				}
			break;

		case CHECK_BOX:
			if(handleGui->typeWindow == NOT_ELEM)
			{
				m_stCheckBox[handleGui->iIndex].bVisible = true;
			}
			break;

		case STATIC_TEXT:
			if(handleGui->typeWindow == NOT_ELEM)
			{
				m_stStaticText[handleGui->iIndex].bVisible = true;
			}
			break;

		case IMAGE_BOX:
			if(handleGui->typeWindow == NOT_ELEM)
			{
				m_stImageBox[handleGui->iIndex].bVisible = true;
			}
			break;

		case GROUP:
			if(handleGui->typeWindow == NOT_ELEM)
			{
				for(i = 0; i < m_stGroup[handleGui->iIndex].m_iQuantRadioButton; i++)
					m_stGroup[handleGui->iIndex].m_stRadioButton[i].bVisible = true;
			}
			break;

		case HSCROLL_BAR:
				if(handleGui->typeWindow == NOT_ELEM)
				{
					m_stHorizScrollBar[handleGui->iIndex].bVisible = true;
				}
			break;

		case VSCROLL_BAR:
			if(handleGui->typeWindow == NOT_ELEM)
			{
				m_stVertScrollBar[handleGui->iIndex].bVisible = true;
			}
			break;

		default:
			return false;
	};
	
	return true;
}

bool CManagerGUI::bHide(stHandleGui *handleGui)
{
	switch(handleGui->typeElementGui)
	{
		case BUTTON:
				if(handleGui->typeWindow == NOT_ELEM)
				{
					m_stButtons[handleGui->iIndex].bVisible = false;
				}
			break;

		case CHECK_BOX:
			if(handleGui->typeWindow == NOT_ELEM)
			{
				m_stCheckBox[handleGui->iIndex].bVisible = false;
			}
			break;

		case STATIC_TEXT:
			if(handleGui->typeWindow == NOT_ELEM)
			{
				m_stStaticText[handleGui->iIndex].bVisible = false;
			}
			break;

		case IMAGE_BOX:
			if(handleGui->typeWindow == NOT_ELEM)
			{
				m_stImageBox[handleGui->iIndex].bVisible = false;
			}
			break;

		case GROUP:
			if(handleGui->typeWindow == NOT_ELEM)
			{
				for(i = 0; i < m_stGroup[handleGui->iIndex].m_iQuantRadioButton; i++)
					m_stGroup[handleGui->iIndex].m_stRadioButton[i].bVisible = false;
			}
			break;

		case HSCROLL_BAR:
			if(handleGui->typeWindow == NOT_ELEM)
			{
				m_stHorizScrollBar[handleGui->iIndex].bVisible = false;
			}
			break;

		case VSCROLL_BAR:
			if(handleGui->typeWindow == NOT_ELEM)
			{
				m_stVertScrollBar[handleGui->iIndex].bVisible = false;
			}
			break;

		default:
			return false;
	};

	return true;
}

bool CManagerGUI::bSetObjectPos(D3DXVECTOR2 pos, stHandleGui *handleGui)
{
	switch(handleGui->typeElementGui)
	{
		case BUTTON:
				if(handleGui->iIDWindow < 0)
				{
					m_stButtons[handleGui->iIndex].vectorTrans = pos;
				}
			break;

		case CHECK_BOX:
			if(handleGui->iIDWindow < 0)
			{
				m_stCheckBox[handleGui->iIndex].vectorTrans = pos;
			}
			break;

		case STATIC_TEXT:
			if(handleGui->iIDWindow < 0)
			{
				m_stStaticText[handleGui->iIndex].vectorTrans = pos;
			}
			break;

		case IMAGE_BOX:
			if(handleGui->iIDWindow < 0)
			{
				m_stImageBox[handleGui->iIndex].vectorTrans = pos;
			}
			break;
		default:
			return false;
	};

	return true;
}

stHandleGui CManagerGUI::hCreateMouse(stSIZE sizeCursor,stSIZE sizeCadr, int rateInSec, char *textureNameImage)  // создаёт мышь
{
	stImageBox imageBox;
	stHandleGui handleGui;

	handleGui.typeElementGui = MOUSE;

	imageBox.bAnimEnable = true;
	imageBox.iRateInSec = rateInSec;


	// =================================== Рендер в текстуру (begin) =============================
	//LPDIRECT3DTEXTURE9 textureImageBox;
	//D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, textureNameImage, &imageBox.textureImageBox);
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile (textureNameImage, &info);

	D3DXCreateTextureFromFileEx(m_directx->m_pD3DDevice,
		textureNameImage,
		info.Width,
		info.Height,
		0,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		0, // or NULL
		NULL,
		&imageBox.textureImageBox);

	float sx,sy;
	D3DSURFACE_DESC	d3dsd;
	imageBox.textureImageBox->GetLevelDesc(0,&d3dsd);
	sx = (static_cast<float>(sizeCursor.cx))/sizeCadr.cx;
	sy = (static_cast<float>(sizeCursor.cy))/(float)sizeCadr.cy;
	imageBox.vectorScale = D3DXVECTOR2(sx, sy);

	/*
		 какую часть изображения выводмть
		 Определяем количество кадров
		 Заполняем под каждый кадр структуру RECT
	*/
	int iNumIndex = 0;
	int iQuantCadr = (info.Width/sizeCadr.cx) * (info.Height/sizeCadr.cy);
	imageBox.iQuantCadr = iQuantCadr;
	imageBox.iDelayFrame = 1000/imageBox.iRateInSec;
	imageBox.pRectRender = new RECT[iQuantCadr];
	for( int i =0; i < static_cast<int>(info.Height/sizeCadr.cy); i++)
		for( int j =0; j < static_cast<int>(info.Width/sizeCadr.cx); j++)
		{
			
			imageBox.pRectRender[iNumIndex].left = j*sizeCadr.cx;
			imageBox.pRectRender[iNumIndex].top = i*sizeCadr.cy;
			imageBox.pRectRender[iNumIndex].right = imageBox.pRectRender[iNumIndex].left + sizeCadr.cx;
			imageBox.pRectRender[iNumIndex].bottom = imageBox.pRectRender[iNumIndex].top + sizeCadr.cy;
			
			iNumIndex++;
		}


	imageBox.iIDWindow = -1;
	stGuiMouse mouse;
	mouse.image = imageBox;
	m_bCreatedMouse = true;

	m_stGuiMouse.push_back(mouse);
	m_iQuantMouse ++;

	handleGui.iIndex = m_iQuantMouse - 1;
	return handleGui;
}

bool CManagerGUI::bSetMouse(stHandleGui *handleMouse)  // устанавливает мышь
{
	if(handleMouse->typeElementGui == MOUSE)
	{
		// прячем все мышки
		/*for(i = 0; i < m_iQuantMouse; i++)
			m_stGuiMouse[i].image.bVisible = false;*/

		// прячем текущую мышку
		m_stGuiMouse[m_iCurNumMouse].image.bVisible = false;

		// и показываем нужную
		m_iCurNumMouse = handleMouse->iIndex;
		m_stGuiMouse[m_iCurNumMouse].image.bVisible = true;

		return true;
	}

	return false;
}

void CManagerGUI::vShowMouse()  // показать текущую мышь
{
	if(m_bCreatedMouse)
		m_stGuiMouse[m_iCurNumMouse].image.bVisible = true;
}
void CManagerGUI::vHideMouse()  // спрятать текущую мышь
{
	if(m_bCreatedMouse)
		m_stGuiMouse[m_iCurNumMouse].image.bVisible = false;
}

void CManagerGUI::vSetCurTextColor(D3DCOLOR color)  // устанавливает текущий цвет текста
{
	m_colorTextDefault = color;
}

void CManagerGUI::vRestoreTextColor() // востанавливает цвет текста который прописан в классе по-умолчанию
{
	m_colorTextDefault = m_colorTextDefault_forRestore;
}

inline bool CManagerGUI::bCalculateWindowPanels(stMouseInfoGUI *mouseInfoGui)
{
	DWORD dwTickCount = GetTickCount();
	static bool bMouseOverWindows; // мышка над каким-то окном
	static stHandleGui handleElem;
	bMouseOverWindows = false;

	if(m_iQuantWindowsPanel <= 0) return true;

	// Проверяем находиться ли мышка хоть над одним окном
	for(i = m_iQuantWindowsPanel - 1; i >= 0; i--)
		if(PtInRect(&m_stWindowPanel[m_stHandleWindowRender[i].iIndex].RectWindow, mouseInfoGui->pointMousePos))
		{
			bMouseOverWindows = true;
			break;
		}

	// Ищем на каком окне произошло нажатие и делаем его активным (меняем с предыдущим)
	if(mouseInfoGui->bLButtonClick)
	{
		for(i = m_iQuantWindowsPanel - 1; i >= 0; i--)
		{
			if(m_stWindowPanel[m_stHandleWindowRender[i].iIndex].bVisible)
				if(PtInRect(&m_stWindowPanel[m_stHandleWindowRender[i].iIndex].RectWindow, mouseInfoGui->pointMousePos))
				{
					stHandleGui handle;
					handle = m_stHandleWindowRender[i];
					m_stHandleWindowRender[i] = m_stHandleWindowRender[m_iQuantWindowsPanel - 1];
					m_stHandleWindowRender[m_iQuantWindowsPanel - 1] = handle;

					break;
			}
		}
	}

	static enElemGuiState enElemState;  // хранит состояние в котором хранится объект

	// ================================= Обрабатываем элементы которые содержат окна =================================================
	// ------------------------------------------------------------------------------------------------------
	// Проверка ImageBox
	// Проверяем счёткик кадров, если надо то меняем кадры
	//for(j = m_iQuantWindowsPanel - 1; j >= 0; j--)
	int iIndexWindow = m_stHandleWindowRender[m_iQuantWindowsPanel - 1].iIndex; // Обрабатываем только активное окно, иначе раскоментировать пред. строчку
		if(m_stWindowPanel[iIndexWindow].bVisible)
		{ 
			// -------------------------------------------------- Проверка кнопок -------------------------------------------------- 
			for(i = 0; i < m_stWindowPanel[iIndexWindow].m_iQuantButtons; i++)
			{
				if(m_stWindowPanel[iIndexWindow].m_stButtons[i].bVisible)  // Кнопка видима
				{
					// выделение кнопки
					if(PtInRect(&m_stWindowPanel[iIndexWindow].m_stButtons[i].rectForWindowMouse, mouseInfoGui->pointMousePos)) // лежит ли точка в прямоугольнике
					{
						if(m_stWindowPanel[iIndexWindow].m_stButtons[i].bSelectEnable)
							enElemState = ELEM_STATE_SELECTED;
					}
					else // мышка находится не над кнопкой
						enElemState = ELEM_STATE_NORMAL;


					// Если состояние объекта изменилось - перерисовываем на текстуре
					if(m_stWindowPanel[iIndexWindow].m_stButtons[i].enState != enElemState)
					{
						handleElem.typeElementGui = BUTTON;
						handleElem.iIndex = i;
						
						m_stWindowPanel[iIndexWindow].m_stButtons[i].enState = enElemState;

						switch(enElemState)
						{
							case ELEM_STATE_NORMAL:
									CManagerGUI::vRefreshRectWindowPanel(iIndexWindow,&handleElem, &m_stWindowPanel[iIndexWindow].m_stButtons[i].textureButton);									
								break;

							case ELEM_STATE_SELECTED:
									CManagerGUI::vRefreshRectWindowPanel(iIndexWindow,&handleElem, &m_stWindowPanel[iIndexWindow].m_stButtons[i].textureSelectionButton);									
									SQUALL_Sample_Play(m_iSampleSoundID_MouseOver,false,0,true);
								break;
						};
					}

					// проверяем нажатия на элементе
					if((mouseInfoGui->bLButtonClick))
						if(PtInRect(&m_stWindowPanel[iIndexWindow].m_stButtons[i].rectForWindowMouse, mouseInfoGui->pointLBDownPos))
							if(PtInRect(&m_stWindowPanel[iIndexWindow].m_stButtons[i].rectForWindowMouse, mouseInfoGui->pointLBUpPos))
							{
								// значит прошёл цикл нажатия и отпускания левой клавиши
								SQUALL_Sample_Play(m_iSampleSoundID_MouseDown,false,0,true);
								m_stWindowPanel[iIndexWindow].m_stButtons[i].pFuncExecute();
							}
				}

			} // -------------------------------------------------- Проверка кнопок -------------------------------------------------- 

			// -------------------------------------------------- Проверка ImageBox -------------------------------------------------- 
			// Проверяем счёткик кадров, если надо то меняем кадры
			for(i = 0; i < m_stWindowPanel[iIndexWindow].m_iQuantImageBox; i++)
				if((m_stWindowPanel[iIndexWindow].m_stImageBox[i].bAnimEnable) && (m_stWindowPanel[iIndexWindow].m_stImageBox[i].bVisible))
				{		
					if(dwTickCount - m_stWindowPanel[iIndexWindow].m_stImageBox[i].dwLastUpdate >= (DWORD)m_stWindowPanel[iIndexWindow].m_stImageBox[i].iDelayFrame)
					{
						//m_stImageBox[i].dwLastUpdate = dwTickCount;
						//m_stImageBox[i].iNumCurCadr++;

						// теперь будет пропускать кадры которые должны были отрисоваться но не хватило времени
						m_stWindowPanel[iIndexWindow].m_stImageBox[i].iNumCurCadr += (dwTickCount - m_stWindowPanel[iIndexWindow].m_stImageBox[i].dwLastUpdate)/
																							(DWORD)m_stWindowPanel[iIndexWindow].m_stImageBox[i].iDelayFrame;
						m_stWindowPanel[iIndexWindow].m_stImageBox[i].dwLastUpdate = dwTickCount;

						if(m_stWindowPanel[iIndexWindow].m_stImageBox[i].iNumCurCadr >= m_stWindowPanel[iIndexWindow].m_stImageBox[i].iQuantCadr)
							m_stWindowPanel[iIndexWindow].m_stImageBox[i].iNumCurCadr = 0;

						handleElem.typeElementGui = IMAGE_BOX;
						handleElem.iIndex = i;
						CManagerGUI::vRefreshRectWindowPanel(iIndexWindow,&handleElem, &m_stWindowPanel[iIndexWindow].m_stImageBox[i].textureImageBox);
					}
				}// --------------------------------------------------  Проверка ImageBox -------------------------------------------------- 

				// -------------------------------------------------- Проверка CheckBox --------------------------------------------------
				for(i = 0; i < m_stWindowPanel[iIndexWindow].m_iQuantCheckBox; i++)
				{
					if(m_stWindowPanel[iIndexWindow].m_stCheckBox[i].bVisible)  // CheckBox видимый
					{
						handleElem.typeElementGui = CHECK_BOX;
						handleElem.iIndex = i;

						// Находится ли над элементом
						if(PtInRect(&m_stWindowPanel[iIndexWindow].m_stCheckBox[i].rectForWindowMouse, mouseInfoGui->pointMousePos))
						{
							enElemState = ELEM_STATE_SELECTED;							
						}

						// проверяем нажатия на элементе
						if(mouseInfoGui->bLButtonClick)
						{
							if(PtInRect(&m_stWindowPanel[iIndexWindow].m_stCheckBox[i].rectForWindowMouse, mouseInfoGui->pointLBDownPos))
								if(PtInRect(&m_stWindowPanel[iIndexWindow].m_stCheckBox[i].rectForWindowMouse, mouseInfoGui->pointLBUpPos))
								{
									// значит прошёл цикл нажатия и отпускания левой клавиши
									m_stWindowPanel[iIndexWindow].m_stCheckBox[i].bCheckBoxState = !m_stWindowPanel[iIndexWindow].m_stCheckBox[i].bCheckBoxState;  // меняем состояние на противоположное
									m_stWindowPanel[iIndexWindow].m_stCheckBox[i].bRunFunc = true;
									//m_stCheckBox[i].pFuncExecute(m_stCheckBox[i].bCheckBoxState); // что-бы функции запускались при изменении внутреннего состояния, А НЕ КАЖДУЮ ИТЕРАЦИЮ
								}
								if(m_stWindowPanel[iIndexWindow].m_stCheckBox[i].bRunFunc) // Чтобы при первом запуске выполнились функции
								{
									SQUALL_Sample_Play(m_iSampleSoundID_MouseDown,false,0,true);

									m_stWindowPanel[iIndexWindow].m_stCheckBox[i].bRunFunc = false;
									m_stWindowPanel[iIndexWindow].m_stCheckBox[i].pFuncExecute(m_stWindowPanel[iIndexWindow].m_stCheckBox[i].bCheckBoxState); // что-бы функции запускались при изменении внутреннего состояния, А НЕ КАЖДУЮ ИТЕРАЦИЮ
								}


								if(m_stWindowPanel[iIndexWindow].m_stCheckBox[i].bCheckBoxState)
									enElemState = ELEM_STATE_ON;
								else
									enElemState = ELEM_STATE_OFF;
						}
					}


					// Если состояние объекта изменилось - перерисовываем на текстуре
					if(m_stWindowPanel[iIndexWindow].m_stCheckBox[i].enState != enElemState)
					{												
						m_stWindowPanel[iIndexWindow].m_stCheckBox[i].enState = enElemState;

						switch(enElemState)
						{
							case ELEM_STATE_ON:
								CManagerGUI::vRefreshRectWindowPanel(iIndexWindow,&handleElem, &m_stWindowPanel[iIndexWindow].m_stCheckBox[i].textureCheckBoxON);
							break;

							case ELEM_STATE_OFF:
								CManagerGUI::vRefreshRectWindowPanel(iIndexWindow,&handleElem, &m_stWindowPanel[iIndexWindow].m_stCheckBox[i].textureCheckBoxOFF);
							break;

							case ELEM_STATE_SELECTED:
									//SQUALL_Sample_Play(m_iSampleSoundID_MouseOver,false,0,true);
								break;
						};
					}				
				} // -------------------------------------------------- Проверка CheckBox --------------------------------------------------

				// ---------------------------------------------------- Проверка RadioButton --------------------------------------------------
				// Проверка RadioButton
				for(i = 0; i < m_stWindowPanel[iIndexWindow].m_iQuantGroup; i++)
				{
					handleElem.typeElementGui = GROUP;
					handleElem.iIndex = i;

					for(int iNumButton = 0; iNumButton < m_stWindowPanel[iIndexWindow].m_stGroup[i].m_iQuantRadioButton; iNumButton++)
					{
						if(m_stWindowPanel[iIndexWindow].m_stGroup[i].m_stRadioButton[iNumButton].bVisible)  // RadioButton видимый
						{
							// Находится ли над элементом
							if(PtInRect(&m_stWindowPanel[iIndexWindow].m_stGroup[i].m_stRadioButton[iNumButton].rectForWindowMouse, mouseInfoGui->pointMousePos))
							{
								enElemState = ELEM_STATE_SELECTED;							
							}

							// проверяем нажатия на элементе
							if(mouseInfoGui->bLButtonClick)
							{
								if(PtInRect(&m_stWindowPanel[iIndexWindow].m_stGroup[i].m_stRadioButton[iNumButton].rectForWindowMouse, mouseInfoGui->pointLBDownPos))
									if(PtInRect(&m_stWindowPanel[iIndexWindow].m_stGroup[i].m_stRadioButton[iNumButton].rectForWindowMouse, mouseInfoGui->pointLBUpPos))
									{
										// значит прошёл цикл нажатия и отпускания левой клавиши

										if(!m_stWindowPanel[iIndexWindow].m_stGroup[i].m_stRadioButton[iNumButton].bRadioButtonState)  // Кнопка уже выбрана ничего не делаем
										{
											m_stWindowPanel[iIndexWindow].m_stGroup[i].m_stRadioButton[iNumButton].bRadioButtonState = !m_stWindowPanel[iIndexWindow].m_stGroup[i].m_stRadioButton[iNumButton].bRadioButtonState;  // меняем состояние на противоположное
											m_stWindowPanel[iIndexWindow].m_stGroup[i].m_stRadioButton[m_stWindowPanel[iIndexWindow].m_stGroup[i].iCurIndexRadioButton].bRadioButtonState = false;  // предыдущее кнопка = false
											m_stWindowPanel[iIndexWindow].m_stGroup[i].iCurIndexRadioButton = iNumButton;  // теперь текщая кнопка выбрана
											m_stWindowPanel[iIndexWindow].m_stGroup[i].bRunRadioButtonFunc = true;
											//m_stGroup[i].m_stRadioButton[iIndexWindow].pFuncExecute(m_stGroup[i].m_stRadioButton[iIndexWindow].bRadioButtonState); // что-бы функции запускались при изменении внутреннего состояния, А НЕ КАЖДУЮ ИТЕРАЦИЮ
										}

										enElemState = ELEM_STATE_OFF;
									}
							}

							if(m_stWindowPanel[iIndexWindow].m_stGroup[i].bRunRadioButtonFunc) // Чтобы при первом запуске выполнились функции
							{
								m_stWindowPanel[iIndexWindow].m_stGroup[i].bRunRadioButtonFunc = false;
								SQUALL_Sample_Play(m_iSampleSoundID_MouseDown,false,0,true);
								m_stWindowPanel[iIndexWindow].m_stGroup[i].m_stRadioButton[iNumButton].pFuncExecute(m_stWindowPanel[iIndexWindow].m_stGroup[i].m_stRadioButton[iNumButton].bRadioButtonState); // что-бы функции запускались при изменении внутреннего состояния, А НЕ КАЖДУЮ ИТЕРАЦИЮ

								CManagerGUI::vRefreshRectWindowPanel(iIndexWindow,&handleElem, NULL);
							}


							// Если состояние объекта изменилось - перерисовываем на текстуре
							if(m_stWindowPanel[iIndexWindow].m_stGroup[i].m_stRadioButton[iNumButton].enState != enElemState)
							{												
								m_stWindowPanel[iIndexWindow].m_stGroup[i].m_stRadioButton[iNumButton].enState = enElemState;

								switch(enElemState)
								{
									case ELEM_STATE_OFF:
									break;

									case ELEM_STATE_SELECTED:
										//SQUALL_Sample_Play(m_iSampleSoundID_MouseOver,false,0,true);
									break;
								};
							}

							/*if(m_stWindowPanel[j].m_stGroup[i].m_stRadioButton[iNumButton].bRadioButtonState)
								enElemState = ELEM_STATE_ON;
							else
								enElemState = ELEM_STATE_OFF;

							// Если состояние объекта изменилось - перерисовываем на текстуре
							if(m_stWindowPanel[j].m_stGroup[i].m_stRadioButton[iNumButton].enState != enElemState)
							{						
								m_stWindowPanel[j].m_stGroup[i].m_stRadioButton[iNumButton].enState = enElemState;

								switch(enElemState)
								{
								case ELEM_STATE_ON:
									CManagerGUI::vRefreshRectWindowPanel(j,&handleElem, &m_stWindowPanel[j].m_stCheckBox[i].textureCheckBoxON);
									break;

								case ELEM_STATE_OFF:
									CManagerGUI::vRefreshRectWindowPanel(j,&handleElem, &m_stWindowPanel[j].m_stCheckBox[i].textureCheckBoxOFF);
									break;
								};
							}	*/
						}
						//else if(!m_stWindowPanel[j].m_stGroup[i].m_stRadioButton[iNumButton].bVisible)  // RadioButton не видима
							//m_stWindowPanel[j].m_stGroup[i].m_stRadioButton[iNumButton].pTextureRender = NULL;
					} // ---------------------------------------------------- Проверка RadioButton --------------------------------------------------
				}
		}
	// ------------------------------------------------------------------------------------------------------

	return !bMouseOverWindows;
}

stHandleGui CManagerGUI::hCreateWindowPanel(stRECT rectPos, char *nameTextureFon, enTypeWindowFill typeWindowFill)
{
	stWindowPanel windowPanel;
	stHandleGui handle;
	handle.typeElementGui = WINDOW_PANEL;

	float sx,sy;
	D3DSURFACE_DESC	d3dsd;
	LPDIRECT3DTEXTURE9 textureFon;
	LPDIRECT3DSURFACE9 backSurf;
	LPDIRECT3DSURFACE9 surfTexture;
	D3DXVECTOR2 vectorTrans;
	D3DXVECTOR2 vectorScale;
	int iRepeatImageX;  // сколько раз повторять изображение по X и Y
	int iRepeatImageY;


	windowPanel.vectorTransWindow = D3DXVECTOR2((float)rectPos.iPosX, (float)rectPos.iPosY);
	windowPanel.typeWindowFill = typeWindowFill;
	windowPanel.stRect = rectPos;

	windowPanel.RectWindow.left = rectPos.iPosX;
	windowPanel.RectWindow.top = rectPos.iPosY;
	windowPanel.RectWindow.right = windowPanel.RectWindow.left + rectPos.iWidth;
	windowPanel.RectWindow.bottom = windowPanel.RectWindow.top + rectPos.iHeight;

	// --------------------- Загрузка и расчёт фонового изображения (begin) ----------------------
	D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, nameTextureFon, &textureFon);

	// Создаём текстуру под всё окно
	m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth, rectPos.iHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, 
		&windowPanel.textureRenderWindow,0);

	m_directx->m_pD3DDevice->CreateTexture(rectPos.iWidth, rectPos.iHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, 
		&windowPanel.textureRenderFon,0);


		// -------------------------------- Заливаем поверхность окна окна фоновым изображением ----------------------
		switch(windowPanel.typeWindowFill)
		{
			case FILL_NO:
				break;

			case FILL_REPEAT:
				m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

				windowPanel.textureRenderFon->GetSurfaceLevel(0, &surfTexture);
				m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);


				textureFon->GetLevelDesc(0,&d3dsd);
				iRepeatImageX = (int)ceil((float)windowPanel.stRect.iWidth/(float)d3dsd.Width);
				iRepeatImageY = (int)ceil((float)windowPanel.stRect.iHeight/(float)d3dsd.Height);


				m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 1.0f, 0);
				m_directx->m_sprite->Begin();
				
				for(int j = 0; j < iRepeatImageY; j++)
					for(int i = 0; i < iRepeatImageX; i++)					
					{						
						vectorTrans.x = (float)(i * d3dsd.Width);
						vectorTrans.y =  (float)(j * d3dsd.Height);

						m_directx->m_sprite->Draw(textureFon, 
							0,
							0,0,0, 
							&vectorTrans,
							D3DCOLOR_XRGB(255,255,255));
					}
					m_directx->m_sprite->End();
					m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
				break;

		case FILL_SCALE:
				textureFon->GetLevelDesc(0,&d3dsd);
				sx = (static_cast<float>(windowPanel.stRect.iWidth))/(float)d3dsd.Width;
				sy = (static_cast<float>(windowPanel.stRect.iHeight))/(float)d3dsd.Height;
				vectorScale = D3DXVECTOR2(sx, sy);


				// нарисуем загруженой текстурой по фону
				m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);
				windowPanel.textureRenderFon->GetSurfaceLevel(0, &surfTexture);
				m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

				m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (255, 0, 0, 0), 1.0f, 0);
					m_directx->m_sprite->Begin();
						m_directx->m_sprite->Draw(textureFon, 
													0,
													&vectorScale,0,0, 
													0,
													D3DCOLOR_XRGB(255,255,255));
					m_directx->m_sprite->End();
				m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
			break;
		};

		textureFon->Release();
	// --------------------- Загрузка и расчёт фонового изображения (end) ----------------------


	// ----------------------------------- Дополняем в массив окон -------------------
	m_stWindowPanel.push_back(windowPanel);
	m_iQuantWindowsPanel ++;
	handle.iIndex = m_iQuantWindowsPanel - 1; //m_iQuantWindowsPanel - 1;
	CManagerGUI::vRefreshRectWindowPanel(handle.iIndex, NULL, NULL); // перерисуем всё окно


	// Регистрируем в массиве handl'ow отрисовки и обработки Окон
	stHandleGui handleRender;
	handleRender.typeElementGui = WINDOW_PANEL;
	handleRender.iIndex = handle.iIndex;
	m_stHandleWindowRender.push_back(handleRender);

	return handle;
}

void CManagerGUI::vRefreshRectWindowPanel(int iIndexWindowPanel, stHandleGui *handleElem, LPDIRECT3DTEXTURE9 *texture)
{
	static float sx,sy;
	static D3DSURFACE_DESC	d3dsd;
	static LPDIRECT3DSURFACE9 backSurf;
	static LPDIRECT3DSURFACE9 surfTexture;
	static D3DXVECTOR2 vectorTrans;
	static D3DXVECTOR2 vectorScale;

	if(handleElem == NULL)  // Перерисовать всё окно
	{
		// -------------------------------- Перезаливаем поверхность окна фоновым изображением --------------------------
		m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);
		m_stWindowPanel[iIndexWindowPanel].textureRenderWindow->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		m_directx->m_sprite->Begin();
			m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].textureRenderFon, 
									0,
									0,0,0, 
									0,
									D3DCOLOR_XRGB(255,255,255));
		m_directx->m_sprite->End();
		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);



		// =================================== Перерисовываем на текстуре все элементы ==================================
		m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);

		m_stWindowPanel[iIndexWindowPanel].textureRenderWindow->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);
		m_directx->m_sprite->Begin();

			// ------------------------------------- отрисовка ImageBox ----------------------------------------------------
			for(int i = 0; i < (int)m_stWindowPanel[iIndexWindowPanel].m_stImageBox.size(); i++)
			{
				if(m_stWindowPanel[iIndexWindowPanel].m_stImageBox[i].bVisible)
					m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].m_stImageBox[i].textureImageBox, 
												&m_stWindowPanel[iIndexWindowPanel].m_stImageBox[i].pRectRender[m_stWindowPanel[iIndexWindowPanel].m_stImageBox[i].iNumCurCadr],
												&m_stWindowPanel[iIndexWindowPanel].m_stImageBox[i].vectorScale,0,0, 
												&m_stWindowPanel[iIndexWindowPanel].m_stImageBox[i].vectorTrans,
												D3DCOLOR_XRGB(255,255,255));
			}
			// ------------------------------------- отрисовка Buttons ----------------------------------------------------
			for(int i = 0; i < (int)m_stWindowPanel[iIndexWindowPanel].m_stButtons.size(); i++)
			{
				if(m_stWindowPanel[iIndexWindowPanel].m_stButtons[i].bVisible)
					m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].m_stButtons[i].textureButton, 
					0,
					&m_stWindowPanel[iIndexWindowPanel].m_stButtons[i].vectorScale,0,0, 
					&m_stWindowPanel[iIndexWindowPanel].m_stButtons[i].vectorTrans,
					D3DCOLOR_XRGB(255,255,255));
			}
			// ------------------------------------- отрисовка CheckBox ----------------------------------------------------
			for(int i = 0; i < (int)m_stWindowPanel[iIndexWindowPanel].m_stCheckBox.size(); i++)
			{
				if(m_stWindowPanel[iIndexWindowPanel].m_stCheckBox[i].bVisible)
				{
					if(m_stWindowPanel[iIndexWindowPanel].m_stCheckBox[i].bCheckBoxState)
						m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].m_stCheckBox[i].textureCheckBoxON, 
												0,
												0,0,0, 
												&m_stWindowPanel[iIndexWindowPanel].m_stCheckBox[i].vectorTrans,
												D3DCOLOR_XRGB(255,255,255));
					else
						m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].m_stCheckBox[i].textureCheckBoxOFF, 
												0,
												0,0,0, 
												&m_stWindowPanel[iIndexWindowPanel].m_stCheckBox[i].vectorTrans,
												D3DCOLOR_XRGB(255,255,255));
				}
			}
			// ------------------------------------- отрисовка StaticText ----------------------------------------------------
			for(int i = 0; i < (int)m_stWindowPanel[iIndexWindowPanel].m_stStaticText.size(); i++)
			{
				if(m_stWindowPanel[iIndexWindowPanel].m_stStaticText[i].bVisible)
				{
					m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].m_stStaticText[i].textureStaticText, 
												0,
												0,0,0, 
												&m_stWindowPanel[iIndexWindowPanel].m_stStaticText[i].vectorTrans,
												D3DCOLOR_XRGB(255,255,255));
				}
			}
			// ------------------------------------- отрисовка GroupRadioButton ----------------------------------------------------
			for(int i = 0; i < m_stWindowPanel[iIndexWindowPanel].m_iQuantGroup; i++)
			for(int j = 0; j < m_stWindowPanel[iIndexWindowPanel].m_stGroup[i].m_iQuantRadioButton; j++)
			{								
				// Рисуем элемент
				if(m_stWindowPanel[iIndexWindowPanel].m_stGroup[i].m_stRadioButton[j].bVisible)
				{	
					if(m_stWindowPanel[iIndexWindowPanel].m_stGroup[i].m_stRadioButton[j].bRadioButtonState)
						m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].m_stGroup[i].m_stRadioButton[j].textureRadioButtonON, 
						0,
						0,0,0, 
						&m_stWindowPanel[iIndexWindowPanel].m_stGroup[i].m_stRadioButton[j].vectorTrans,
						D3DCOLOR_XRGB(255,255,255));
					else
						m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].m_stGroup[i].m_stRadioButton[j].textureRadioButtonOFF, 
						0,
						0,0,0, 
						&m_stWindowPanel[iIndexWindowPanel].m_stGroup[i].m_stRadioButton[j].vectorTrans,
						D3DCOLOR_XRGB(255,255,255));
				}
			}

		m_directx->m_sprite->End();
		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
	}
	else // перерисовать только часть окна
	{
		// получаем предварительно заполненый handle  элемента, вытаскиваем из него координаты и в них рисуем

			switch(handleElem->typeElementGui)
			{
				case BUTTON:
						// перед перерисовкой элемента область заливаем фоном
							m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);
							m_stWindowPanel[iIndexWindowPanel].textureRenderWindow->GetSurfaceLevel(0, &surfTexture);
							m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

							m_directx->m_sprite->Begin();
									m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].textureRenderFon, 
																&m_stWindowPanel[iIndexWindowPanel].m_stButtons[handleElem->iIndex].rectButton,
																0,0,0, 
																&m_stWindowPanel[iIndexWindowPanel].m_stButtons[handleElem->iIndex].vectorTrans,
																D3DCOLOR_XRGB(255,255,255));


							// Рисуем элемент
						if(m_stWindowPanel[iIndexWindowPanel].m_stButtons[handleElem->iIndex].bVisible)
						{	
							m_directx->m_sprite->Draw(*texture, 
											0,
											&m_stWindowPanel[iIndexWindowPanel].m_stButtons[handleElem->iIndex].vectorScale,0,0, 
											&m_stWindowPanel[iIndexWindowPanel].m_stButtons[handleElem->iIndex].vectorTrans,
											D3DCOLOR_XRGB(255,255,255));
						}
						m_directx->m_sprite->End();
						m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
					break;

				case CHECK_BOX:
							// перед перерисовкой элемента область заливаем фоном
							m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);
							m_stWindowPanel[iIndexWindowPanel].textureRenderWindow->GetSurfaceLevel(0, &surfTexture);
							m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

							m_directx->m_sprite->Begin();
									m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].textureRenderFon, 
																&m_stWindowPanel[iIndexWindowPanel].m_stCheckBox[handleElem->iIndex].rectCheckBox,
																0,0,0, 
																&m_stWindowPanel[iIndexWindowPanel].m_stCheckBox[handleElem->iIndex].vectorTrans,
																D3DCOLOR_XRGB(255,255,255));


							// Рисуем элемент
						if(m_stWindowPanel[iIndexWindowPanel].m_stCheckBox[handleElem->iIndex].bVisible)
						{	
							m_directx->m_sprite->Draw(*texture, 
											0,
											0,0,0, 
											&m_stWindowPanel[iIndexWindowPanel].m_stCheckBox[handleElem->iIndex].vectorTrans,
											D3DCOLOR_XRGB(255,255,255));
						}
						m_directx->m_sprite->End();
						m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
					break;

				case GROUP:
					// Для RadioButtons передаваемая текстура не нужна, так как перерисовываются все кнопки группы

							// перед перерисовкой элемента область заливаем фоном
							m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);
							m_stWindowPanel[iIndexWindowPanel].textureRenderWindow->GetSurfaceLevel(0, &surfTexture);
							m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

							m_directx->m_sprite->Begin();
								
							// для данной группы отрисовываем все кнопки
									for(int j = 0; j < m_stWindowPanel[iIndexWindowPanel].m_stGroup[handleElem->iIndex].m_iQuantRadioButton; j++)
									{								
										m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].textureRenderFon, 
																&m_stWindowPanel[iIndexWindowPanel].m_stGroup[handleElem->iIndex].m_stRadioButton[j].rectRadioButton,
																0,0,0, 
																&m_stWindowPanel[iIndexWindowPanel].m_stGroup[handleElem->iIndex].m_stRadioButton[j].vectorTrans,
																D3DCOLOR_XRGB(255,255,255));


										// Рисуем элемент
										if(m_stWindowPanel[iIndexWindowPanel].m_stGroup[handleElem->iIndex].m_stRadioButton[j].bVisible)
										{	
											if(m_stWindowPanel[iIndexWindowPanel].m_stGroup[handleElem->iIndex].m_stRadioButton[j].bRadioButtonState)
												m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].m_stGroup[handleElem->iIndex].m_stRadioButton[j].textureRadioButtonON, 
													0,
													0,0,0, 
													&m_stWindowPanel[iIndexWindowPanel].m_stGroup[handleElem->iIndex].m_stRadioButton[j].vectorTrans,
													D3DCOLOR_XRGB(255,255,255));
											else
												m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].m_stGroup[handleElem->iIndex].m_stRadioButton[j].textureRadioButtonOFF, 
													0,
													0,0,0, 
													&m_stWindowPanel[iIndexWindowPanel].m_stGroup[handleElem->iIndex].m_stRadioButton[j].vectorTrans,
													D3DCOLOR_XRGB(255,255,255));
										}
									}
							m_directx->m_sprite->End();
							m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
					break;

				case STATIC_TEXT:
							// перед перерисовкой элемента область заливаем фоном
							m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);
							m_stWindowPanel[iIndexWindowPanel].textureRenderWindow->GetSurfaceLevel(0, &surfTexture);
							m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

							m_directx->m_sprite->Begin();
								m_directx->m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
								m_directx->m_pD3DDevice->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_SRCCOLOR );
								m_directx->m_pD3DDevice->SetRenderState( D3DRS_DESTBLENDALPHA,D3DBLEND_INVSRCCOLOR );

									m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].textureRenderFon, 
																&m_stWindowPanel[iIndexWindowPanel].m_stStaticText[handleElem->iIndex].rectStaticText,
																0,0,0, 
																&m_stWindowPanel[iIndexWindowPanel].m_stStaticText[handleElem->iIndex].vectorTrans,
																D3DCOLOR_XRGB(255,255,255));


							// Рисуем элемент
						if(m_stWindowPanel[iIndexWindowPanel].m_stStaticText[handleElem->iIndex].bVisible)
						{	
							m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].m_stStaticText[handleElem->iIndex].textureStaticText, 
											0,
											0,0,0, 
											&m_stWindowPanel[iIndexWindowPanel].m_stStaticText[handleElem->iIndex].vectorTrans,
											D3DCOLOR_XRGB(255,255,255));
						}
						m_directx->m_sprite->End();
						m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
					break;

				case IMAGE_BOX:		
						// перед перерисовкой элемента область заливаем фоном
							m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);
							m_stWindowPanel[iIndexWindowPanel].textureRenderWindow->GetSurfaceLevel(0, &surfTexture);
							m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

							m_directx->m_sprite->Begin();
									m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].textureRenderFon, 
																&m_stWindowPanel[iIndexWindowPanel].m_stImageBox[handleElem->iIndex].rectFrameForWindow,
																0,0,0, 
																&m_stWindowPanel[iIndexWindowPanel].m_stImageBox[handleElem->iIndex].vectorTrans,
																D3DCOLOR_XRGB(255,255,255));


							// Рисуем элемент
						if(m_stWindowPanel[iIndexWindowPanel].m_stImageBox[handleElem->iIndex].bVisible)
						{	
							m_directx->m_sprite->Draw(m_stWindowPanel[iIndexWindowPanel].m_stImageBox[handleElem->iIndex].textureImageBox, 
											&m_stWindowPanel[iIndexWindowPanel].m_stImageBox[handleElem->iIndex].pRectRender[m_stWindowPanel[iIndexWindowPanel].m_stImageBox[handleElem->iIndex].iNumCurCadr],
											&m_stWindowPanel[iIndexWindowPanel].m_stImageBox[handleElem->iIndex].vectorScale,0,0, 
											&m_stWindowPanel[iIndexWindowPanel].m_stImageBox[handleElem->iIndex].vectorTrans,
											D3DCOLOR_XRGB(255,255,255));
						}
						m_directx->m_sprite->End();
						m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
					break;

				default:
					break;
			};
	}
}