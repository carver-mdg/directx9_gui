#include <vector>
#include <stack>
#include "DXData.h"

using namespace std;


/* ошибки ( begin ) */
#define ANIM_OK					0  /* */ 
#define ERR_TYPE_WAS_CREATED	-1 /* Тип уже есть */
#define ERR_TEXTURE_LOAD		-2 /* Проблемы с загрузкой текстуры */
#define ERR_ID_NOT_ARRAY		-3 /* ID выходит за границы массива */
#define ERR_TYPE_NOT_FIND		(-4) /* не найден указаный тип */
/* ошибки ( end ) */


/* iFlagsAnimation */
#define ANIM_QUANT_ONE 0x00000001  /* анимировать один раз */
#define ANIM_QUANT_INF 0x00000010  /* анимировать много раз */
#define ANIM_FORWARD   0x00000100  /* анимировать вперёд */
#define ANIM_BACK      0x00001000  /* анимировать назад */
#define ANIM_NO_ANIM   0x00010000  /* НЕ анимировать */
#define ANIM_DELETE    0x00100000  /* после анимации удалить */


/* iFlagsCadr */
#define CADR_BEG	   -1 /* начальный кадр */
#define CADR_END	   -2 /* последний кадр */
#define CADR_NEXT	   -3 /* след кадр */
#define CADR_PREV	   -4 /* пред кадр */

/* События ( begin ) */
#define ANIM_CADR_END_WAS  1 /* конечный кадр был отрисован */
#define ANIM_CADR_BEG_WAS  2 /* начальный кадр был отрисован */
/* События ( end ) */


struct stType
{
	stType()
	{
		iDelayAnim = 100;
		pTexture = NULL;
		rect_draw = NULL;
		iQuantCadrs = 0;
		iQuantTextureFiles = 0;

		//ID = -1;  // не создан или удалён
		bDeleted = false;
		bFewFiles = false;
	};

	bool bDeleted;
	bool bFewFiles; // Несколько файлов ??
	int ID;
	char name[50];  // имя типа
	int iDelayAnim;  // задержка между кадрами
	
	int iQuantCadrs;
	int iQuantTextureFiles;
	LPDIRECT3DTEXTURE9 *pTexture;
	RECT *rect_draw; // части одной текстуры ( если в одной текстуре несколько кадров )
	
	// Для VectorRot
	int iCadrWidth;
	int iCadrHeight;
};

struct stObject
{
	stObject()
	{
		iNumCadr = 0;
		iNumFile = 0;
		iBegTimeAnim = 0;
		bDeleted = false;

		vectorScale = D3DXVECTOR2(1.0f, 1.0f);
		vectorRot = D3DXVECTOR2(0.0f, 0.0f);
		fAngle = 0;

		bRender = true;
		name[0] = '\0';

		iPriority = 0;
	};

	bool bDeleted;
	int ID;		// ID самого объекта
	int IDType;  // ID типа к которому принадлежит объект
	
	char name[50];
	D3DXVECTOR2 vectorPos;
	D3DXVECTOR2 vectorScale;
	D3DXVECTOR2 vectorRot; // вектор вращения
	float fAngle; // Угол вращения

	int iNumCadr;
	int iNumFile;
	int iBegTimeAnim;

	int iFlagsAnimation; // Как анимировать (один раз, цикл)
	bool bRender;  // рисовать ли ?

	int iPriority; // приоритет отрисовки
};



class CAnimation
{
	private:
		DXData *m_directx;
		int m_iQuantType; // количество зарегестрированых типов
		int m_iQuantObject; // количество зарегестрированых Объектов
		int m_iSizeArrayType;  // размер массива
		int m_iSizeArrayObject;
		
		int  m_iSizeArrayRender;  // размер массива отрисовки
		bool m_bRestAssumeArrayRender;  //  нужно ли пересчитывать индексы массива отрисовки
		int *m_piIndexRender;  // индексы массива отрисовки
		LPDIRECT3DTEXTURE9 m_lpRenderTexture;

		vector<stType> m_stType;
		vector<stObject> m_stObject;

		stack <int, vector<int> > m_stackFreeIndexObjects;  //  стек свобоных елементов массива ОБЪЕКТОВ

	public:
		CAnimation();
		~CAnimation();

		void vSetDirectX(DXData *directX)
		{
			m_directx = directX;
			m_directx->m_pD3DDevice->CreateTexture(m_directx->m_d3dpp.BackBufferWidth,
														m_directx->m_d3dpp.BackBufferHeight,
														1,
														D3DUSAGE_RENDERTARGET,
														D3DFMT_A8R8G8B8,
														D3DPOOL_DEFAULT, 
														&m_lpRenderTexture,0);
			/*
			D3DXCreateTexture(m_directx->m_pD3DDevice,
						1024,
						1024,
						D3DUSAGE_RENDERTARGET,
						0,
						D3DFMT_A8R8G8B8,
						D3DPOOL_MANAGED, // D3DPOOL_MANAGED
						&m_lpRenderTexture);
			*/
		};

		int iAddType(char *pcType, char *pcFiles, int iQuantFiles);
		int iAddType(char *pcType, char *pcFile, int iQuantCadr, int iSizeCadrX, int iSizeCadrY);
		bool bDeleteType(char *cType);
		//int  inline iGetTypeID(char *pcType);


		int iAddObject(char *pcNameObject, char *pcType, D3DXVECTOR2 vectorPos, int iFlagsAnimation);
		bool bDeleteObject(char *pcNameObject, int ID);  // Можно обращаться по ID или Имени
		void vSetObjectPos(char *pcNameObject, int ID, D3DXVECTOR2 vectorPos);  // Можно обращаться по ID или Имени
		void vSetObjectScale(char *pcNameObject, int ID, D3DXVECTOR2 vectorScale);  // Можно обращаться по ID или Имени
		void vSetObjectAngle(char *pcNameObject, int ID, float fAngle);  // Можно обращаться по ID или Имени
		int iSetObjectCadr(char *pcNameObject, int ID, int iFlagsOrCadr);  // Можно обращаться по ID или Имени
		bool bSetObjectPriority(char *pcNameObject, int ID, int iPriority);  // установить приоритет прорисовки



		void vRender(); // Рисует
		void vDraw();   // Пересчитывает (счётчик, ...)



		int iGetSizeArrayObject()
		{
			return m_iSizeArrayObject;
		};
		int iGetQuantObject()
		{
			return m_iQuantObject;
		};
		int iGetQuantType()
		{
			return m_iQuantType;
		};
		int iGetSizeArrayType()
		{
			return m_iSizeArrayType;
		};
};