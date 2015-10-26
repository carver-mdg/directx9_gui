#include "DXData.h"

DXData::DXData()
{
	SQUALL_Init(0);

	m_pD3D = NULL;
	m_pD3DDevice = NULL;

	m_file_report = fopen("report.txt","w");
}

DXData::~DXData()
{
	SQUALL_Free();
	DXData::bDestroyDirect3D9();
	
	if(m_file_report != NULL)
	{
		fprintf(m_file_report,"DXData::Деструктор \n");
		fclose(m_file_report);
	}
}

LPDIRECT3DDEVICE9 *DXData::GetDevice()
{
	return &m_pD3DDevice;
}

LPDIRECT3D9 *DXData::GetDirect()
{
	return &m_pD3D;
}

bool DXData::bDestroyDirect3D9()
{
	_RELEASE_ (m_pD3DDevice);
	_RELEASE_ (m_pD3D);

	return true;
}

bool DXData::bInitD3D(HWND hWnd, int iWidth, int iHeight,bool bFullScreen)
{
	RECT windowRect; 
	DWORD windowStyle, windowExStyle;

	if( (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	//D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&m_d3dpp,sizeof(m_d3dpp));

	m_d3dpp.BackBufferCount = 3;
	m_d3dpp.BackBufferWidth   = iWidth;
	m_d3dpp.BackBufferHeight  = iHeight;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//d3dpp.BackBufferCount = 1;

	if(bFullScreen)
	{
		m_d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
		m_d3dpp.Windowed = false;
		m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		windowExStyle = WS_EX_APPWINDOW;              
		windowStyle = WS_POPUP; 

		//SetWindowPos(*hWnd,HWND_TOPMOST,0, 0, iWidth, iHeight,SWP_SHOWWINDOW | SWP_FRAMECHANGED);
		//SetWindowLong(*hWnd,GWL_STYLE,WS_POPUP);
	}

	else
	{
		// Устанавливаем размер клиентской области
		RECT wndRect;
		RECT clientRect;
			GetWindowRect(hWnd, &wndRect);
			GetClientRect(hWnd, &clientRect);	  

		iWidth  = iWidth + (wndRect.right-wndRect.left)  - (clientRect.right-clientRect.left);
		iHeight = iHeight + (wndRect.bottom-wndRect.top) - (clientRect.bottom-clientRect.top);

		//MoveWindow(*hWnd, wndRect.left, wndRect.top, iWidth, iHeight, TRUE);

		m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&m_d3ddm);
		
		//m_d3dpp.BackBufferFormat = m_d3ddm.Format;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.Windowed = true;

		windowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;    
		windowStyle = WS_OVERLAPPEDWINDOW; 
	}

	windowRect.left=(long)0;  
	windowRect.right=(long)iWidth;  
	windowRect.top=(long)0;      
	windowRect.bottom=(long)iHeight;      
	AdjustWindowRectEx(&windowRect, windowStyle, false, windowExStyle);    
	SetWindowLong(hWnd, GWL_STYLE, windowStyle); 
	SetWindowLong(hWnd, GWL_EXSTYLE, windowExStyle);

	SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 
		windowRect.right - windowRect.left , windowRect.bottom - windowRect.top , 
		SWP_SHOWWINDOW ); 

	DWORD Flags = D3DCREATE_MIXED_VERTEXPROCESSING;
	if(FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,Flags,
										&m_d3dpp,&m_pD3DDevice) ))
		return false;



	// 2D
	D3DXCreateSprite(m_pD3DDevice,&m_sprite);
	D3DXCreateLine(m_pD3DDevice,&m_pLine);

	//ShowWindow(hWnd,SW_NORMAL);
	//UpdateWindow(hWnd);

	return true;
}

void DXData::vResetDevice()
{
	HRESULT hr;
	hr = m_pD3DDevice->TestCooperativeLevel();
	if(hr == D3DERR_DEVICELOST)
		return;

	if(hr == D3DERR_DEVICENOTRESET)
		m_pD3DDevice->Reset(&m_d3dpp);
}

HRESULT DXData::hrScreenShot(char *name)
{
	HRESULT hr; 

	LPDIRECT3DSURFACE9 surf; 
	if (FAILED(hr=m_pD3DDevice->CreateOffscreenPlainSurface(m_d3ddm.Width,m_d3ddm.Height, D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&surf,NULL))) 
		return hr; 

	if (FAILED(hr = m_pD3DDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&surf))) 
	{
		surf->Release(); 
		return hr; 
	}	

	hr= D3DXSaveSurfaceToFile(name,D3DXIFF_BMP,surf,NULL,NULL); 
	surf->Release(); 

	return hr; 
}

void DXData::vReadIntegerInIni(char *pcSection, char *pcKey, char *pcPath_ini, int &iValue)
{
	GetCurrentDirectory(sizeof(m_cDirectory_exe), m_cDirectory_exe);
	
	char *pc_name_ini_file = new char[ strlen(m_cDirectory_exe) + 40];
	sprintf(pc_name_ini_file, "%s\\%s", m_cDirectory_exe,pcPath_ini);
	

	char cIntValue[10]; // Для чисел
	GetPrivateProfileString(pcSection,pcKey,
								"not_ready",cIntValue, sizeof(cIntValue),pc_name_ini_file);
	sscanf(cIntValue,"%d",&iValue);

	delete[] pc_name_ini_file;
}

void DXData::vReadFloatInIni(char *pcSection, char *pcKey, char *pcPath_ini, float &fValue)
{
	GetCurrentDirectory(sizeof(m_cDirectory_exe), m_cDirectory_exe);
	
	char *pc_name_ini_file = new char[ strlen(m_cDirectory_exe) + 40];
	sprintf(pc_name_ini_file, "%s\\%s", m_cDirectory_exe,pcPath_ini);
	

	char cFloatValue[10]; // Для чисел
	GetPrivateProfileString(pcSection,pcKey,
								"not_ready",cFloatValue, sizeof(cFloatValue),pc_name_ini_file);
	sscanf(cFloatValue,"%f",&fValue);

	delete[] pc_name_ini_file;
}

void DXData::vReadStringInIni(char *pcSection, char *pcKey, char *pcPath_ini, char *pcValue)
{
	GetCurrentDirectory(sizeof(m_cDirectory_exe), m_cDirectory_exe);

	char *pc_name_ini_file = new char[ strlen(m_cDirectory_exe) + 40];
	sprintf(pc_name_ini_file, "%s\\%s", m_cDirectory_exe,pcPath_ini);


	//char *temp = new char[length]; //Для временного хранения строки
	char cText[100];

	GetPrivateProfileString(pcSection,pcKey,
								"not_ready",cText, sizeof(cText),pc_name_ini_file);

	sprintf(pcValue,"%s",cText);

	delete[] pc_name_ini_file;
}

void DXData::vReadBoolInIni(char *pcSection, char *pcKey, char *pcPath_ini, bool &bValue)
{
	GetCurrentDirectory(sizeof(m_cDirectory_exe), m_cDirectory_exe);
	
	char *pc_name_ini_file = new char[ strlen(m_cDirectory_exe) + 40];
	sprintf(pc_name_ini_file, "%s\\%s", m_cDirectory_exe,pcPath_ini);
	

	char cBoolValue[10], *pcBool; // Для bool
	GetPrivateProfileString(pcSection,pcKey,
								"not_ready",cBoolValue, sizeof(cBoolValue),pc_name_ini_file);
	pcBool = strlwr(cBoolValue);
	if(strstr(pcBool,"true"))
		bValue = true;
	if(strstr(pcBool,"false"))
		bValue = false;

	delete[] pc_name_ini_file;
}

// Сравнивает строки
bool DXData::bCompareString(char *str_1, char *str_2)
{
	if( (str_1 == NULL) || (str_2 == NULL) )
		return false;

	if( strlen(str_1) != strlen(str_2) )
		return  false;

	int iMinQuantSym =0;
	(strlen(str_1) < strlen(str_2)) ? (iMinQuantSym = (int)strlen(str_1)) : (iMinQuantSym = (int)strlen(str_2));
	
	for(int i =0; i < iMinQuantSym; i++)
	{
		if( str_1[i] != str_2[i] )
			return false;
	}

	return true;
}

template <class T>
inline void swap(T &val1, T &val2)
{
    T temp = val1;

    val1 = val2;
    val2 = temp;
}

// Сортирует массив целых чисел по возрастанию
void DXData::vSortArrayIncrease(int* s_arr,int first, int last)
{
    int i = first, j = last, x = s_arr[(first + last) / 2];
 
    do {
        while (s_arr[i] < x) i++;
        while (s_arr[j] > x) j--;
 
        if(i <= j) {
            if (i < j) swap(s_arr[i], s_arr[j]);
            i++;
            j--;
        }
    } while (i <= j);
 
    if (i < last)
        vSortArrayIncrease(s_arr, i, last);
    if (first < j)
        vSortArrayIncrease(s_arr, first,j);
}

//------------------------------------------
// Имя: vGetInfoVideoCard()
// Описание: Получает информацию о видеокарте и записывает её в файл "Report_VGA.txt"
//------------------------------------------
void DXData::vGetInfoVideoCard()
{
	D3DCAPS9 caps;
	m_pD3DDevice->GetDeviceCaps(&caps);

	/*
	fstream file;
	file.open("Report_VGA.txt", ios::out);
	file<< "MaxAnisotropy = " << caps.MaxAnisotropy << endl;
	file<< "MaxPointSize = " << caps.MaxPointSize << endl;
	file<< "MaxPrimitiveCount = " << caps.MaxPrimitiveCount << endl;
	file<< "MaxVertexIndex = " << caps.MaxVertexIndex << endl << endl;

	file<< "MaxActiveLights = " << caps.MaxActiveLights << endl << endl;

	file<< "VertexShaderVersion = " << caps.VertexShaderVersion << endl;
	file<< "PixelShaderVersion = " << caps.PixelShaderVersion << endl;
	file<< "MaxPixelShaderInstructionsExecuted = " << caps.MaxPShaderInstructionsExecuted << endl;
	file<< "MaxVertexShaderInstructionsExecuted = " << caps.MaxVShaderInstructionsExecuted << endl << endl;

	file<< "MaxTextureSize = " << caps.MaxTextureWidth << " x " << caps.MaxTextureHeight << endl;
	file.close();
	*/
}

//Перевод громкости от 0..1 в 0..100 
int DXData::iConvert_volume_to_squall(int iVolInProz) 
{ 
	float fVol = (float)iVolInProz/100;
	if (fVol <= 0) 
		return 0;

	const float steepness = 2.f; 

	float res = (log10f(fVol * 10.f) + steepness) / (steepness + 1.f); 
	//saturate(res); // clamp 0..1
	if(res > 1.0f)
		res = 1.0f;

	return int(res * 100.f); 
}

//------------------------------------------
// Имя: RelativePosition()
// Описание: Возвращает положение точки src относительно вектора origin,
//       направленного к target
//      (Исп. для перевода из мировой СК в СК игрока/камеры/радара)
//------------------------------------------
D3DXVECTOR3 DXData::vec3RelativePosition(const D3DXVECTOR3 *src,const D3DXVECTOR3 *origin,
							 const D3DXVECTOR3 *target,const D3DXVECTOR3 *upvector)
{
	D3DXVECTOR3 dirn;
	D3DXVec3Normalize(&dirn,&D3DXVECTOR3(*target-*origin));
	D3DXVECTOR3 u;
	D3DXVec3Cross(&u, &dirn, upvector);
	D3DXVec3Normalize(&u,&u);
	D3DXVECTOR3 v;
	D3DXVec3Cross(&v,&u,&dirn);
	D3DXVECTOR3 originToSrc=*origin-*src;
	return D3DXVECTOR3( D3DXVec3Dot(&u,&originToSrc), D3DXVec3Dot(&v,&originToSrc),
		D3DXVec3Dot(&dirn,&originToSrc) );
}

//------------------------------------------
// Имя: SetCameraShift()
// Описание: Сдвигает камеру и ее цель в определенном направлении
//      (Исп. для перемещения вперед/назад/влево/вправо)
//------------------------------------------
void DXData::vSetCameraShift(D3DXVECTOR3 *cam,D3DXVECTOR3 *camtarg,
					const D3DXVECTOR3 *shift,const D3DXVECTOR3 *upvector)
{
	D3DXVECTOR3 dirn;
	D3DXVec3Normalize(&dirn,&D3DXVECTOR3(*camtarg-*cam));
	D3DXVECTOR3 u;
	D3DXVec3Cross(&u, &dirn, upvector);
	D3DXVec3Normalize(&u,&u);
	D3DXVECTOR3 v;
	D3DXVec3Cross(&v,&u,&dirn);
	*cam += u*(shift->x) + v*(shift->y) + dirn*(shift->z);
	*camtarg += u*(shift->x) + v*(shift->y) + dirn*(shift->z);
}

//------------------------------------------
// Имя: SetCameraRotationX()
// Описание: Поворачивает камеру по оси X, изменяя координаты ее цели
//      (Исп. для поворота вверх/вниз)
//------------------------------------------
void DXData::vSetCameraRotationX(const D3DXVECTOR3 *cam,D3DXVECTOR3 *camtarg,
						const D3DXVECTOR3 *upvector, const float angle)
{
	D3DXVECTOR3 dirn;
	D3DXVec3Normalize(&dirn,&D3DXVECTOR3(*camtarg-*cam));
	D3DXVECTOR3 u;
	D3DXVec3Cross(&u, &dirn, upvector);
	D3DXVec3Normalize(&u,&u);
	D3DXMATRIX rotmatrix;
	D3DXMatrixRotationAxis(&rotmatrix,&u,angle);
	D3DXVec3TransformCoord(&dirn,&dirn,&rotmatrix);
	*camtarg=*cam+dirn;
}

//------------------------------------------
// Имя: SetCameraRotationY()
// Описание: Поворачивает камеру по оси Y, изменяя координаты ее цели
//      (Исп. для поворота влево/вправо)
//------------------------------------------
void DXData::vSetCameraRotationY(const D3DXVECTOR3 *cam,D3DXVECTOR3 *camtarg,
						const D3DXVECTOR3 *upvector, const float angle)
{
	D3DXVECTOR3 dirn;
	D3DXVec3Normalize(&dirn,&D3DXVECTOR3(*camtarg-*cam));
	D3DXVECTOR3 u;
	D3DXVec3Cross(&u, &dirn, &D3DXVECTOR3(0.0f,1.0f,0.0f));
	D3DXVec3Normalize(&u,&u);
	D3DXVECTOR3 v;
	D3DXVec3Cross(&v,&u,&dirn);
	D3DXMATRIX rotmatrix;
	D3DXMatrixRotationAxis(&rotmatrix,&v,angle);
	D3DXVec3TransformCoord(&dirn,&dirn,&rotmatrix);
	*camtarg=*cam+dirn;
}

void DXData::vEffects(LPDIRECT3DTEXTURE9 *texture)
{
	D3DSURFACE_DESC	d3dsd;
	(*texture)->GetLevelDesc(0,&d3dsd);

	D3DLOCKED_RECT blockedRect;//структура описания залокированной поверхности
	(*texture)->LockRect( 0, &blockedRect, 0, 0 );//блокировка поверхности

	DWORD* imageData = (DWORD*)blockedRect.pBits;//адресс поверхности
	int color = 0;
	int alpha = 255;

	//залить спрайт белым цветом
	//if((d3dsd.Format == D3DFMT_R5G6B5) || (d3dsd.Format == D3DFMT_X8R8G8B8))
	//if(blockedRect.Pitch = 16)
	for(int y = 0; y < (int)d3dsd.Height; y++)
		for(int x = 0; x < (int)d3dsd.Width; x++)
		{
			//int index = (int)(y * blockedRect.Pitch / 4 + x);
			//int index = x*4 + y*blockedRect.Pitch;
			//int index = y*d3dsd.Width + x;
			//DWORD index=(x*4+(y*(blockedRect.Pitch)));

			//BYTE b=imageData[index];

			//BYTE g=imageData[index+1];

			//BYTE r=imageData[index+2];

			//BYTE a=imageData[index+3];
			//imageData[index] = D3DCOLOR_ARGB(255,255,255,255);//заполнить цветом с прозрачностью
			
			//color = (0.30*getR(imageData[index]) + 0.59*getG(imageData[index]) + 0.11*getB(imageData[index]));
			//alpha = getA(imageData[index]);
			//imageData[index] = D3DCOLOR_ARGB(150,color,color,color);
			
			/*static int index = 0;
			index = (int)(y * d3dsd.Width + x);
			D3DCOLOR colorTex = imageData[index];
			//m_pD3DDevice->GetRenderTargetData()
			D3DCOLOR color = (0.30*getR(colorTex) + 0.59*getG(colorTex) + 0.11*getB(colorTex));
			color = D3DCOLOR_XRGB(255,0,0);
			DXData::vDrawPoint(stSIZE(x,y+300), color);*/
		}
	/*else if(blockedRect.Pitch = 32)
	for(int y = 0; y < d3dsd.Height; y++)
		for(int x = 0; x < d3dsd.Width; x++)
		{
			//int index = (int)(y * blockedRect.Pitch / 4 + x);
			int index = y*d3dsd.Width + x;
			//imageData[index] = (WORD)D3DCOLOR_ARGB(255,255,255,255);//заполнить цветом с прозрачностью
			
			//color = (0.30*getR(imageData[index]) + 0.59*getG(imageData[index]) + 0.11*getB(imageData[index]));
			//imageData[index] = D3DCOLOR_ARGB(color,color,color,color);
			DWORD dwcolor = imageData[index];
			//imageData[index] = 0.5*((imageData[index]&0x0000ff00)>>8); 
			//imageData[index] = D3DCOLOR_COLORVALUE(255,0,0,200);
			imageData[index] = D3DCOLOR_XRGB(0,0,0);
		}*/

	// завершили пачканье текстуры, разблокировать её
	(*texture)->UnlockRect(0);
}

void DXData::vDrawPoint(stSIZE pos, D3DCOLOR color)
{
	LPDIRECT3DSURFACE9 backsurf;
	m_pD3DDevice->GetRenderTarget(0,&backsurf);

	RECT rect;
	rect.left = pos.cx;
	rect.top = pos.cy;
	rect.right = pos.cx + 1;
	rect.bottom = pos.cy + 1;
	m_pD3DDevice->ColorFill(backsurf,&rect,color);
}

void DXData::vDrawCircle(int x0, int y0, int radius, D3DCOLOR color) 
{
	int x = 0;
	int y = radius;
	int delta = 2 - 2 * radius;
	int error = 0;
	while(y >= 0) 
	{
		DXData::vDrawPoint(stSIZE(x0 + x, y0 + y),color);
		DXData::vDrawPoint(stSIZE(x0 + x, y0 - y),color);
		DXData::vDrawPoint(stSIZE(x0 - x, y0 + y),color);
		DXData::vDrawPoint(stSIZE(x0 - x, y0 - y),color);
		error = 2 * (delta + y) - 1;
		if(delta < 0 && error <= 0) 
		{
			++x;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
		if(delta > 0 && error > 0) 
		{
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
}

void DXData::vDrawLine(int x1, int y1, int x2, int y2, D3DCOLOR color) 
{
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;
	//
	int error = deltaX - deltaY;
	//
	DXData::vDrawPoint(stSIZE(x2, y2), color);
	while(x1 != x2 || y1 != y2) 
	{
		DXData::vDrawPoint(stSIZE(x1, y1), color);
		const int error2 = error * 2;
		//
		if(error2 > -deltaY) 
		{
			error -= deltaY;
			x1 += signX;
		}
		if(error2 < deltaX) 
		{
			error += deltaX;
			y1 += signY;
		}
	}
}

void DXData::vDrawEllipse(int x, int y, int a, int b, D3DCOLOR color)
{
	int col,i,row,bnew;
	long a_square,b_square,two_a_square,two_b_square,four_a_square,four_b_square,d;

	b_square=b*b;
	a_square=a*a;
	row=b;
	col=0;
	two_a_square=a_square<<1;
	four_a_square=a_square<<2;
	four_b_square=b_square<<2;
	two_b_square=b_square<<1;
	d=two_a_square*((row-1)*(row))+a_square+two_b_square*(1-a_square);
	while(a_square*(row)>b_square*(col))
	{
		DXData::vDrawPoint(stSIZE(col+x,row+y), color);
		DXData::vDrawPoint(stSIZE(col+x,y-row), color);
		DXData::vDrawPoint(stSIZE(x-col,row+y), color);
		DXData::vDrawPoint(stSIZE(x-col,y-row), color);
		if (d>=0)
		{
			row--;
			d-=four_a_square*(row);
		}
		d+=two_b_square*(3+(col<<1));
		col++;
	}
	d=two_b_square*(col+1)*col+two_a_square*(row*(row-2)+1)+(1-two_a_square)*b_square;
	while ((row) + 1)
	{
		DXData::vDrawPoint(stSIZE(col+x,row+y), color);
		DXData::vDrawPoint(stSIZE(col+x,y-row), color);
		DXData::vDrawPoint(stSIZE(x-col,row+y), color);
		DXData::vDrawPoint(stSIZE(x-col,y-row), color);
		if (d<=0)
		{
			col++;
			d+=four_b_square*col;
		}
		row--;
		d+=two_a_square*(3-(row <<1));
	}
}