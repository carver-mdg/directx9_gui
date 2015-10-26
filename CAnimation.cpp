#include "CAnimation.h"

CAnimation::CAnimation()
{
	m_iQuantType = 0;
	m_iQuantObject = 0;

	m_iSizeArrayType = 0;
	m_iSizeArrayObject = 0;

	m_bRestAssumeArrayRender = true;
	m_iSizeArrayRender = 0;
	m_piIndexRender = NULL;

	//m_stType.reserve(1000);
}

CAnimation::~CAnimation()
{
	int i,j;

	// Удаляет текстуры
	for(i =0; i < m_iSizeArrayType; i++)
		if(!m_stType[i].bDeleted)
			for(j =0; j < m_stType[i].iQuantTextureFiles; j++)
				if(m_stType[i].pTexture[j] != NULL)
					m_stType[i].pTexture[j]->Release();

	if(m_lpRenderTexture != NULL)
		m_lpRenderTexture->Release();
	fprintf(m_directx->m_file_report,"CAnimation::Деструктор \n");
}

// Загрузка из цикла текстур ( кадр = файл )
int CAnimation::iAddType(char *pcType, char *pcFiles, int iQuantFiles)
{
	fprintf(m_directx->m_file_report,"CAnimation::Добавление типа \"%s\"\n",pcType);


	int i =0;
	int j =0;

	// если тип уже был зарегестрирован - Выходим
	for(i =0; i < m_iSizeArrayType; i++)
		if(m_directx->bCompareString(m_stType[i].name, pcType))
		{
			fprintf(m_directx->m_file_report,"CAnimation::Ошибка тип \"%s\" уже существует\n\n",pcType);
			return ERR_TYPE_WAS_CREATED;
		}


	// иначе регистрируем
	stType type;
	
	// Основные данные
	sprintf(type.name,"%s",pcType);
	type.bDeleted = false;
	type.iQuantTextureFiles = iQuantFiles;
	type.bFewFiles = true;
	type.iQuantCadrs = 1;


	type.pTexture = new LPDIRECT3DTEXTURE9[type.iQuantTextureFiles];
	type.rect_draw = new RECT[type.iQuantTextureFiles];
	
	D3DXIMAGE_INFO info;
	char *pcNameFile = new char[strlen(pcFiles) + 30];

	
	for(i =0; i < type.iQuantTextureFiles; i++)
	{
		sprintf(pcNameFile,pcFiles,i);


		D3DXGetImageInfoFromFile (pcNameFile, &info);

		type.iCadrWidth = info.Width;
		type.iCadrHeight = info.Height;


		if(FAILED(D3DXCreateTextureFromFileEx(m_directx->m_pD3DDevice,
			                            pcNameFile,
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
										&type.pTexture[i])))
		{
			fprintf(m_directx->m_file_report,"CAnimation::Ошибка загрузки текстуры \"%s\"\n\n",pcNameFile);
			delete[] type.pTexture;
			return ERR_TEXTURE_LOAD;
		}

		fprintf(m_directx->m_file_report,"CAnimation::Текстура \"%s\" загружена\n",pcNameFile);
		type.rect_draw[i].left = 0;
		type.rect_draw[i].top = 0;
		type.rect_draw[i].right = info.Width;
		type.rect_draw[i].bottom = info.Height;
	}
	delete[]pcNameFile;




	// поиск "свободных" (удалёных) участков массива типов
	if(m_iSizeArrayType == 0)  // если впервые
	{
		fprintf(m_directx->m_file_report,"CAnimation::Создаю новый элемент массива\n\n");

		type.ID = 0;
		m_stType.push_back(type);
		m_iSizeArrayType++;

		m_iQuantType++;
	}
	else
	{
		// поиск "свободных" (удалёных) участков массива типов
		for(i =0; i < m_iSizeArrayType; i++)
			if(m_stType[i].bDeleted)
			{
				fprintf(m_directx->m_file_report,"CAnimation::Найден удалённый тип - заменяю\n\n");
			
				m_stType[i].bDeleted = false;
				memcpy(&m_stType[i],&type,sizeof(stType));

				m_iQuantType++;
				return ANIM_OK;
			}

		// иначе создание нового элемента массива
		fprintf(m_directx->m_file_report,"CAnimation::Создаю новый элемент массива\n\n");

		type.ID = m_iSizeArrayType;
		m_stType.push_back(type);
		m_iSizeArrayType++;

		m_iQuantType++;
		return ANIM_OK;
	}

	return ANIM_OK;


	





	//  ФУНКЦИИ НЕТ


	/*
	int i =0;

	// если тип уже был зарегестрирован - Выходим
	for(i =0; i < m_iQuantType; i++)
		if(m_directx->bCompareString(m_stType[i].name, pcType))
			return ERR_TYPE_WAS_CREATED;


	// иначе регистрируем
	stType type;
	
	sprintf(type.name,"%s",pcType);
	type.IDType = m_iQuantType;
	type.iQuantCadrs = iQuantCadr;



	int iSizeString = strlen(pcFile);
	char *name_file = new char[iSizeString + 50];
				
	// узнаём формат файла
	char format_file[3];
	format_file[0] = cFile[iSizeString - 3];
	format_file[1] = cFile[iSizeString - 2];
	format_file[2] = cFile[iSizeString - 1];
	

	//sprintf(name_file

	for(i =0; i < iQuantCadr; i ++)
		D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, "NAME", &type.pTexture[i]);

	delete[] name_file;
	m_stType.push_back(type);
	*/

	return ANIM_OK;
}

/*
	загрузка кадров из одной текстуры
	if( iQuantCadr ) == -1 определять автоматически
*/
int CAnimation::iAddType(char *pcType, char *pcFile, int iQuantCadr, int iSizeCadrX, int iSizeCadrY)
{
	fprintf(m_directx->m_file_report,"CAnimation::Добавление типа \"%s\"\n",pcType);


	int i =0;
	int j =0;

	// если тип уже был зарегестрирован - Выходим
	for(i =0; i < m_iSizeArrayType; i++)
		if(m_directx->bCompareString(m_stType[i].name, pcType))
		{
			fprintf(m_directx->m_file_report,"CAnimation::Ошибка тип \"%s\" уже существует\n\n",pcType);
			return ERR_TYPE_WAS_CREATED;
		}


	// иначе регистрируем
	stType type;
	
	// Основные данные
	sprintf(type.name,"%s",pcType);
	type.bFewFiles = false;
	type.bDeleted = false;
	type.iQuantTextureFiles = 1;


	type.pTexture = new LPDIRECT3DTEXTURE9[type.iQuantTextureFiles];
	//if(FAILED(D3DXCreateTextureFromFile(m_directx->m_pD3DDevice, pcFile, &type.pTexture[0])))
	
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile (pcFile, &info); 
	
	type.iCadrWidth = info.Width;
	type.iCadrHeight = info.Height;

	// int iQuantCadrX = 0; // количество кадров по X,Y
	// int iQuantCadrY = 0;
	
	if(FAILED(D3DXCreateTextureFromFileEx(m_directx->m_pD3DDevice,
                                    pcFile,
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
                                    &type.pTexture[0])))
	{
		fprintf(m_directx->m_file_report,"CAnimation::Ошибка загрузка текстуры \"%s\"\n\n",pcFile);
		delete[] type.pTexture;
		return ERR_TEXTURE_LOAD;
	}
	fprintf(m_directx->m_file_report,"CAnimation::Текстура \"%s\" загружена\n",pcFile);


	// Определяем количество кадров
	//D3DXIMAGE_INFO info;
	//D3DXGetImageInfoFromFile (pcFile, &info); 
	if( (iSizeCadrX <= 0) || (iSizeCadrY <= 0) )  // Если размеры кадра <= 0 то  - это  1 кадр
	{
		type.iQuantCadrs = 1;
		fprintf(m_directx->m_file_report,"CAnimation::Количество кадров \"%d\"\n", type.iQuantCadrs);

		type.rect_draw = new RECT[type.iQuantCadrs];
		type.rect_draw[0].left = 0;
		type.rect_draw[0].top = 0;
		type.rect_draw[0].right = info.Width;
		type.rect_draw[0].bottom = info.Height;
	}
	else
	{
		if(iQuantCadr <= 0) // Значит определяем автоматически количество кадров
		{
			// Узнаём размеры текстуры - файла
			//D3DXGetImageInfoFromFile (pcFile, &info); 

			// Узнаём количество кадров
			type.iQuantCadrs = (info.Width/iSizeCadrX) * (info.Height/iSizeCadrY);
		}
		else
			type.iQuantCadrs = iQuantCadr;
		fprintf(m_directx->m_file_report,"CAnimation::Количество кадров \"%d\"\n", type.iQuantCadrs);


		// заполняем RECT
		int iNumIndex = 0;
		type.rect_draw = new RECT[(info.Width/iSizeCadrX) * (info.Height/iSizeCadrY)];
		for( i =0; i < ((float)info.Height/iSizeCadrY); i++)
			for( j =0; j < ((float)info.Width/iSizeCadrX); j++)
			{
				type.rect_draw[iNumIndex].left = j*iSizeCadrX;
				type.rect_draw[iNumIndex].top = i*iSizeCadrY;
				type.rect_draw[iNumIndex].right = type.rect_draw[iNumIndex].left + iSizeCadrX;
				type.rect_draw[iNumIndex].bottom = type.rect_draw[iNumIndex].top + iSizeCadrY;

				fprintf(m_directx->m_file_report,"RECT:: (%d, %d)  (%d, %d)\n", 
					type.rect_draw[iNumIndex].left,
					type.rect_draw[iNumIndex].top,
					type.rect_draw[iNumIndex].right,
					type.rect_draw[iNumIndex].bottom);
				iNumIndex++;
			}
	}


	// поиск "свободных" (удалёных) участков массива типов
	if(m_iSizeArrayType == 0)  // если впервые
	{
		fprintf(m_directx->m_file_report,"CAnimation::Создаю новый элемент массива\n\n");

		type.ID = 0;
		m_stType.push_back(type);
		m_iSizeArrayType++;

		m_iQuantType++;
	}
	else
	{
		// поиск "свободных" (удалёных) участков массива типов
		for(i =0; i < m_iSizeArrayType; i++)
			if(m_stType[i].bDeleted)
			{
				fprintf(m_directx->m_file_report,"CAnimation::Найден удалённый тип - заменяю\n\n");
			
				m_stType[i].bDeleted = false;
				memcpy(&m_stType[i],&type,sizeof(stType));

				m_iQuantType++;
				return ANIM_OK;
			}

		// иначе создание нового элемента массива
		fprintf(m_directx->m_file_report,"CAnimation::Создаю новый элемент массива\n\n");

		type.ID = m_iSizeArrayType;
		m_stType.push_back(type);
		m_iSizeArrayType++;

		m_iQuantType++;
		return ANIM_OK;
	}

	return ANIM_OK;
}

/*
int inline CAnimation::iGetTypeID(char *pcType)
{
	int i;
	for(i =0; i < m_iQuantType; i++)
		if(m_directx->bCompareString(m_stType[i].name, pcType))
			return m_stType[i].ID;

	return -1;
}
*/

bool CAnimation::bDeleteType(char *pcType)
{
	fprintf(m_directx->m_file_report,"CAnimation::Удаление типа \"%s\" \n",pcType);

	int i;
	for(i =0; i < m_iSizeArrayType; i++)
		if(m_directx->bCompareString(m_stType[i].name, pcType))
		{
			fprintf(m_directx->m_file_report,"CAnimation::Найден тип \"%s\" удаляю\n\n", m_stType[i].name);

			m_stType[i].bDeleted = true;
			sprintf( m_stType[i].name, "\0");
			//fprintf(m_directx->m_file_report,"CAnimation::String = %s\n", m_stType[i].name);
			
			if(m_stType[i].rect_draw != NULL)
				delete[] m_stType[i].rect_draw;
			
			// Удаляем тектстуры
			for(int j =0; j < m_stType[i].iQuantTextureFiles; j++)
				if(m_stType[i].pTexture[j] != NULL)
					m_stType[i].pTexture[j]->Release();

			m_iQuantType--;
			
			// если нашёл true
			return true;
		}

	fprintf(m_directx->m_file_report,"CAnimation::Тип \"%s\" не найден\n\n", pcType);
	return false;
}

int CAnimation::iAddObject(char *pcNameObject, char *pcType, D3DXVECTOR2 vectorPos, int iFlagsAnimation)
{
	int i;
	int iTypeID = -1;
	stObject object;


	fprintf(m_directx->m_file_report,"CAnimation::Добавляю объект без имени\n");

	// Записываем имя объекта
	if(pcNameObject != NULL)
	{
		sprintf(object.name,"%s",pcNameObject);
		fprintf(m_directx->m_file_report,"CAnimation::Добавляю объект \"%s\"\n", pcNameObject);
	}

	// Находим ID типа
	for(i =0; i < m_iSizeArrayType; i++)
		if(m_directx->bCompareString(m_stType[i].name, pcType))
		{
			iTypeID = m_stType[i].ID;
			break;
		}

	// Тип не найден
	if(iTypeID < 0)
	{
		fprintf(m_directx->m_file_report,"CAnimation::Ошибка тип \"%s\" не найден\n", pcType);
		return ERR_TYPE_NOT_FIND;
	}

	object.vectorPos = vectorPos;
	object.vectorRot = D3DXVECTOR2( (float)m_stType[iTypeID].iCadrWidth/2, (float)m_stType[iTypeID].iCadrHeight/2 );
	object.iFlagsAnimation = iFlagsAnimation;
	object.iNumFile = 0;
	object.IDType = iTypeID;


	// если анимировать вперёд (iNumCadr == CadrBeg), если назад (iNumCadr == CadrEnd)
	if(iFlagsAnimation & ANIM_BACK) 
	{
		if(m_stType[object.IDType].bFewFiles)
			object.iNumFile = m_stType[object.IDType].iQuantTextureFiles - 1;
		else
			object.iNumCadr = m_stType[object.IDType].iQuantCadrs - 1;
	}
	else if( (iFlagsAnimation & ANIM_FORWARD) ||
			 (iFlagsAnimation & ANIM_NO_ANIM) )
				object.iNumCadr = 0;



	// Если есть свободные елементы массива используем их
	if(m_stackFreeIndexObjects.size() > 0)
	{
		object.ID = m_stackFreeIndexObjects.top();
		m_stackFreeIndexObjects.pop();

		memcpy(&m_stObject[object.ID], &object, sizeof(stObject));
	}
	else
	{
		object.ID = m_iSizeArrayObject;
		m_stObject.push_back(object);
		m_iSizeArrayObject++;
	}

	m_bRestAssumeArrayRender = true;
	m_iQuantObject++;
	return object.ID;
}

bool CAnimation::bDeleteObject(char *pcNameObject, int ID)
{
	if( (ID >= 0) && (ID < m_iSizeArrayObject) )
		if(!m_stObject[ID].bDeleted)
	{
		fprintf(m_directx->m_file_report,"CAnimation::Удаляю объект \"%s\", ID = %d\n", m_stObject[ID].name,m_stObject[ID].ID);

		m_bRestAssumeArrayRender = true;
		m_stObject[ID].bDeleted = true;
		m_stackFreeIndexObjects.push(ID);
		m_iQuantObject--;

		fprintf(m_directx->m_file_report,"CAnimation::Найден объект \"%s\" удаляю\n\n", m_stObject[ID].name);
		return true;
	}

	fprintf(m_directx->m_file_report,"CAnimation::Объект не найден\n\n");
	return false;
}

void CAnimation::vSetObjectPos(char *pcNameObject, int ID, D3DXVECTOR2 vectorPos)
{
	if( (ID >=0 ) && (ID < m_iSizeArrayObject) )
		m_stObject[ID].vectorPos = vectorPos;
}

void CAnimation::vSetObjectAngle(char *pcNameObject, int ID, float fAngle)
{
	if( (ID >=0 ) && (ID < m_iSizeArrayObject) )
		m_stObject[ID].fAngle = fAngle;
}

void CAnimation::vSetObjectScale(char *pcNameObject, int ID, D3DXVECTOR2 vectorScale)
{
	if( (ID >=0 ) && (ID < m_iSizeArrayObject) )
		m_stObject[ID].vectorScale = vectorScale;
}


bool CAnimation::bSetObjectPriority(char *pcNameObject, int ID, int iPriority)
{
	if( (ID >=0 ) && (ID < m_iSizeArrayObject) )
	{
		m_stObject[ID].iPriority = iPriority;
		m_bRestAssumeArrayRender = true;
		return true;
	}

	return false;
}

int CAnimation::iSetObjectCadr(char *pcNameObject, int ID, int iFlagsOrCadr)
{
	if( (ID < 0 ) || (ID >= m_iSizeArrayObject) )
		return 0;


	// if(iFlagsCadr) < 0 Значит передали флаг
	if( iFlagsOrCadr < 0 )	
	{
		switch(iFlagsOrCadr)
		{
			case CADR_BEG:
						if(m_stType[m_stObject[ID].IDType].bFewFiles)
							m_stObject[ID].iNumFile = 0;
						else
							m_stObject[ID].iNumCadr = 0;
					return ANIM_OK;
				break;

			case CADR_END:
						if(m_stType[m_stObject[ID].IDType].bFewFiles)
							m_stObject[ID].iNumFile = m_stType[m_stObject[ID].IDType].iQuantTextureFiles - 1;
						else
							m_stObject[ID].iNumCadr = m_stType[m_stObject[ID].IDType].iQuantCadrs - 1;
					return ANIM_OK;
				break;

			case CADR_NEXT:
					if(m_stType[m_stObject[ID].IDType].bFewFiles)
					{
						if( m_stObject[ID].iNumFile + 1 < m_stType[m_stObject[ID].IDType].iQuantTextureFiles)
							m_stObject[ID].iNumFile++;
						else
						{
							m_stObject[ID].iNumFile = 0;
							return ANIM_CADR_END_WAS;
						}
					}

					else
					{
						if( m_stObject[ID].iNumCadr + 1 < m_stType[m_stObject[ID].IDType].iQuantCadrs)
							m_stObject[ID].iNumCadr++;
						else
						{
							m_stObject[ID].iNumCadr = 0;
							return ANIM_CADR_END_WAS;
						}
					}

					return ANIM_OK;
				break;

			case CADR_PREV:
					if(m_stType[m_stObject[ID].IDType].bFewFiles)
					{
						if( m_stObject[ID].iNumFile >= 1)
							m_stObject[ID].iNumFile --;
						else
						{
							m_stObject[ID].iNumFile = m_stType[m_stObject[ID].IDType].iQuantTextureFiles - 1;
							return ANIM_CADR_END_WAS;
						}
					}
					else
					{
						if( m_stObject[ID].iNumCadr >= 1 )
							m_stObject[ID].iNumCadr--;
						else
						{
							m_stObject[ID].iNumCadr = m_stType[m_stObject[ID].IDType].iQuantCadrs - 1;
							return ANIM_CADR_BEG_WAS;
						}
					}

					return ANIM_OK;
				break;
		};
	}

	// Иначе номер кадра
	if(m_stType[m_stObject[ID].IDType].bFewFiles)
	{
		if( iFlagsOrCadr < m_stType[m_stObject[ID].IDType].iQuantTextureFiles )
			m_stObject[ID].iNumFile = iFlagsOrCadr;
	}
	else
	{
		if( iFlagsOrCadr < m_stType[m_stObject[ID].IDType].iQuantCadrs )
			m_stObject[ID].iNumCadr = iFlagsOrCadr;
	}

	return ANIM_OK;
}


void CAnimation::vRender()
{
	//m_directx->m_sprite->Begin();
	/*
		for(int i = 0; i < m_iSizeArrayObject; i++)
			if( (m_stObject[i].bRender) && (!m_stObject[i].bDeleted) )
				m_directx->m_sprite->Draw(m_stType[m_stObject[i].IDType].pTexture[m_stObject[i].iNumFile],
										&m_stType[m_stObject[i].IDType].rect_draw[m_stObject[i].iNumCadr],
										&m_stObject[i].vectorScale,
										&m_stObject[i].vectorRot,
										m_stObject[i].fAngle,
										&m_stObject[i].vectorPos,
										D3DCOLOR_XRGB(255,255,255));
		*/

	D3DXVECTOR2 vectorPos(0,0);
				m_directx->m_sprite->Draw(m_lpRenderTexture,
										0,
										0,
										0,
										0,
										&vectorPos,
										D3DCOLOR_XRGB(255,255,255));

	
		int iIndex;
		for(int i = 0; i < m_iSizeArrayRender; i++)
		{
			iIndex = m_piIndexRender[i];
				m_directx->m_sprite->Draw(m_stType[m_stObject[iIndex].IDType].pTexture[m_stObject[iIndex].iNumFile],
										&m_stType[m_stObject[iIndex].IDType].rect_draw[m_stObject[iIndex].iNumCadr],
										&m_stObject[iIndex].vectorScale,
										&m_stObject[iIndex].vectorRot,
										m_stObject[iIndex].fAngle,
										&m_stObject[iIndex].vectorPos,
										D3DCOLOR_ARGB(255,255,255,255));
		}
	//m_directx->m_sprite->End();
}

void CAnimation::vDraw()
{
	int iBegTime = GetTickCount();
	int i;

	for(i = 0; i < m_iSizeArrayObject; i++)
		if( (!m_stObject[i].bDeleted) && 
			!(m_stObject[i].iFlagsAnimation & ANIM_NO_ANIM) )
		{
			if( (iBegTime - m_stObject[i].iBegTimeAnim) >= m_stType[m_stObject[i].IDType].iDelayAnim )
			{
				m_stObject[i].iBegTimeAnim = iBegTime;

				// в какую сторону крутить кадры
				if(m_stObject[i].iFlagsAnimation & ANIM_FORWARD)
				{
					if(m_stType[m_stObject[i].IDType].bFewFiles)
						m_stObject[i].iNumFile++;
					else
						m_stObject[i].iNumCadr++;
				}
				else if(m_stObject[i].iFlagsAnimation & ANIM_BACK)
				{
					if(m_stType[m_stObject[i].IDType].bFewFiles)
						m_stObject[i].iNumFile--;
					else
						m_stObject[i].iNumCadr--;
				}


				// Не выходит ли кадр за рамки ?
				if( (m_stObject[i].iNumCadr >= m_stType[m_stObject[i].IDType].iQuantCadrs) ||
					(m_stObject[i].iNumCadr < 0) ||
					(m_stObject[i].iNumFile >= m_stType[m_stObject[i].IDType].iQuantTextureFiles) ||
					(m_stObject[i].iNumFile < 0) )
				{
					if(m_stObject[i].iFlagsAnimation & ANIM_DELETE)
					{
						CAnimation::bDeleteObject(0,i);
						break;
					}

					// если бесконечная анимация ( begin )
					else if(m_stObject[i].iFlagsAnimation & ANIM_QUANT_INF)
					{
						if(m_stObject[i].iFlagsAnimation & ANIM_FORWARD)
						{
							m_stObject[i].iNumCadr = 0;
							m_stObject[i].iNumFile = 0;
						}

						if(m_stObject[i].iFlagsAnimation & ANIM_BACK)
						{
							if(m_stType[m_stObject[i].IDType].bFewFiles)
								m_stObject[i].iNumFile = m_stType[m_stObject[i].IDType].iQuantTextureFiles - 1;
							else
								m_stObject[i].iNumCadr = m_stType[m_stObject[i].IDType].iQuantCadrs - 1;
						}
					}
					// если бесконечная анимация ( end )


					// если цикл анимации прошёл устанавливаем нулевой кадр и Запрещяем анимацию ( begin )
					else if( (m_stObject[i].iFlagsAnimation & ANIM_QUANT_ONE) )
					{
						if(m_stObject[i].iFlagsAnimation & ANIM_FORWARD)
						{
							m_stObject[i].bRender = false;
							m_stObject[i].iNumCadr = 0;
							m_stObject[i].iNumFile = 0;
						}

						else if(m_stObject[i].iFlagsAnimation & ANIM_BACK)
						{
							m_stObject[i].bRender = false;
							
							if(m_stType[m_stObject[i].IDType].bFewFiles)
								m_stObject[i].iNumFile = m_stType[m_stObject[i].IDType].iQuantTextureFiles - 1;
							else
								m_stObject[i].iNumCadr = m_stType[m_stObject[i].IDType].iQuantCadrs - 1;
						}
					}
					// если цикл анимации прошёл устанавливаем нулевой кадр и Запрещяем анимацию ( end )
				}
			} // if( (iBegTime - m_stObject[i].iBegTimeAnim) >= m_stType[m_stObject[i].IDType].iDelayAnim )
		}


	
	// Нужно ли пересчитывать индексы массива отрисовки
	if(m_bRestAssumeArrayRender)
	{
		/*
			В будующем переписать
		*/


		if(m_piIndexRender != NULL)
			delete[] m_piIndexRender;
		m_piIndexRender = new int[m_iSizeArrayObject];
		m_iSizeArrayRender = 0;


		//m_piIndexRender[0] = m_stObject[0].iPriority;
		int iter = 0;

		// Копируем разрешёные объекты
		for(i = 0; i < m_iSizeArrayObject; i++)
			if( (m_stObject[i].bRender) && (!m_stObject[i].bDeleted) && !(m_stObject[i].iFlagsAnimation & ANIM_NO_ANIM))
			{
				m_piIndexRender[iter] = m_stObject[i].ID;
				iter++;
			}
		m_iSizeArrayRender = iter;

		// Сортируем полученный массив по возрастанию
		//m_directx->vSortArrayIncrease(m_piIndexRender,0,m_iSizeArrayRender -1);
		///*
		int iTemp;
		for(i = 0; i < m_iSizeArrayRender; i++)
		{
			for(int j = i; j < m_iSizeArrayRender; j++)
			{
				if(m_stObject[m_piIndexRender[j]].iPriority < m_stObject[m_piIndexRender[i]].iPriority)
				{
					iTemp = m_piIndexRender[i];
					m_piIndexRender[i] = m_piIndexRender[j];
					m_piIndexRender[j] = iTemp;
				}
			}
		}
		//*/
		m_bRestAssumeArrayRender = false;

		// ---------- Render To Texture ----------
		LPDIRECT3DSURFACE9 backSurf;
		LPDIRECT3DSURFACE9 surfTexture;
		m_directx->m_pD3DDevice->GetRenderTarget(0, &backSurf);
			m_lpRenderTexture->GetSurfaceLevel(0, &surfTexture);
		m_directx->m_pD3DDevice->SetRenderTarget(0, surfTexture);

		//   следующую сточку можно коментировать для ускорения
		m_directx->m_pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB (0, 0, 0, 0), 0.0f, 0);
		
		m_directx->m_pD3DDevice->BeginScene();
		m_directx->m_sprite->Begin();
		
		for(int i = 0; i < m_iSizeArrayObject; i++)
			if( (m_stObject[i].bRender) && (!m_stObject[i].bDeleted) && (m_stObject[i].iFlagsAnimation & ANIM_NO_ANIM))
				m_directx->m_sprite->Draw(m_stType[m_stObject[i].IDType].pTexture[m_stObject[i].iNumFile],
										&m_stType[m_stObject[i].IDType].rect_draw[m_stObject[i].iNumCadr],
										&m_stObject[i].vectorScale,
										&m_stObject[i].vectorRot,
										m_stObject[i].fAngle,
										&m_stObject[i].vectorPos,
										D3DCOLOR_XRGB(255,255,255));
		m_directx->m_sprite->End();
		m_directx->m_pD3DDevice->EndScene();
		m_directx->m_pD3DDevice->SetRenderTarget(0, backSurf);
	}
}