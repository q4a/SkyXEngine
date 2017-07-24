
//#include <physics/sxphysics.h>
#include <common/array.h>
#include <common/memalloc.h>

#define AIGRID_QUAD_PRECOND(id,retval)  if (id < 0 || ArrQuads.size() <= id) {reportf(REPORT_MSG_LEVEL_ERROR, " %s - unresolved index '%d' of quad, sxaigrid", gen_msg_location, id); return retval;}

#define toint100(fnum) (int32_t)((fnum + 0.0001f) * 100)

//long значения позиций, float3 * 100
struct int3
{
	int3(){ this->x = this->y = this->z = 0; }
	int3(long _x, long _y, long _z){ this->x = _x; this->y = _y; this->z = _z; }
	int32_t x,y,z;
};

//структура описывающая квадрат на аи сетке
struct AIQuad
{
	AIQuad()
	{
		axisx = axisy = 0;
		IsClose = false;
		Id = -1;
			for(int i=0;i<8;++i)
				Arr[i] = -1;

		IdSplit = 0;
	}

	//направления по осям
	//если равно 0 то в обоих направлениях, иначе еси 1 то положительное направление, иначе -1 то отрицательное, 2 - хода нет
	char axisx;
	char axisy;

	//установка соседа для квада
	inline void SetNeighbor(int key,ID id)
	{
		Arr[key] = id;

			if(key == 1)
			{
					if(axisy == -1)
						axisy = 0;
					else if(axisy == 2)
						axisy = 1;
			}

			if(key == 6)
			{
					if(axisy == 1)
						axisy = 0;
					else if(axisy == 2)
						axisy = -1;
			}

			if(key == 3)
			{
					if(axisx == 1)
						axisx = 0;
					else if(axisx == 2)
						axisx = -1;
			}

			if(key == 4)
			{
					if(axisx == -1)
						axisx = 0;
					else if(axisx == 2)
						axisx = 1;
			}
	}

	float3_t pos;		//позиция квада
	bool IsClose;		//закрытый ли квад
	ID Arr[8];			//id соседей
	ID Id;				//идентификатор-ключ для обращения к массиву
	uint16_t IdSplit;	//идентификатор сплита к которому отсносится данный квад (в случае если сетка не цельная)
};

//ограничивающий объем для квадов
struct BoundAIQuad
{
	BoundAIQuad(){}
	~BoundAIQuad(){}

	float3_t min,max;
	int3 lmin, lmax;
	Array<ID> ArrIdsQuads;
};

//структура стоимости перехода по кваду
struct CostAIQuad
{
	CostAIQuad()
	{
		F=G=H=0;
	}

	uint32_t F,G,H;
};

struct AIQuadVertexData
{
	AIQuadVertexData(){}
	AIQuadVertexData(float px,float py,float pz,float tcx,float tcy)
	{
		pos.x = px;
		pos.y = py;
		pos.z = pz;

		tc.x = tcx;
		tc.y = tcy;
	}
	float3_t pos;
	float2_t tc;
};

//
struct AIQuadVertexDataInst
{
	float3_t pos;
	float4_t tex;
	uint32_t color;
};


class AIGrid
{
public:
	AIGrid();
	~AIGrid();

	void GraphicsInit();//инициализация графических данных, если надо
	void Clear();		//очистка всех данных

	//сохранение/загрузка
	void GridSave(const char* path);
	void GridLoad(const char* path);
	
	//ограничивающий объем
	//{
	inline void BBCreate(const float3* center, const float3* param);	//создать ограничивающий объем по параметрам
	inline bool BBIsCreated() const;									//создан ли ограничивающий объем
	
	//установка/получение габаритов
	inline void BBSetDimensions(const float3* dim);						
	inline void BBGetDimensions(float3* dim) const;

	//установка/получение позиции
	inline void BBSetPos(const float3* pos);
	inline void BBGetPos(float3* pos) const;

	inline void BBCreateFinish();	//завершение создания ограничивающего объема, после которого изменить какие-либо его данные будет невозможно
	inline bool BBIsCreatedFinish() const;
	//}

	//квады
	//{
	//установка/возвращение состояния квада
	inline void QuadSetState(ID id, AIQUAD_STATE state);
	inline AIQUAD_STATE QuadGetState(ID id) const;
	
	//установка/возвращение позиции по оси Y для квада
	inline void QuadSetPosY(ID id, float posy);
	inline float QuadGetPosY(ID id) const;

	ID QuadAdd(const float3* pos);					//добавление квада в позицию
	bool QuadDelete(ID id);							//удаление квада по его id
	
	void QuadSelect(ID id, bool consider_prev);		//добавление квада к списку выделения, id - идентификатор квада, если -1 то очищает список, consider_prev - учитывать ли предыдущие записанные, если нет то очищает массив и записывает туда, если да то записывает сверху
	inline void QuadSelectedAddPosY(float posy);	//добавление к позиции по оси Y для выделенных квадов
	void QuadSelectedDelete();						//удаление выделенных квадов
	
	inline bool QuadIsFree(ID id, int radius);		//свободен ли квад id в радиусе radius (radius - количество квадов вокруг указанного в id, 1 - значит только указанный, 2 - значит все соседние и т.д.)
	ID QuadGetNear(const float3* pos, bool isfree = false, int raius = 1);	//возвращает id ближайшего квада (если isfree == true то ищет только свободные) С радиусом свободности radius
	inline ID QuadGet(const float3* pos, bool isnear_or_permissible) const;	//получить id квада по позиции, isnear_or_permissible - самый ближний квад, или самый ближний в пределах допустимой разницы начальной точки?
	//}

	//сетка
	//{
	void GridGenerate();											//функция просчетов, ее нужно вызывать чтобы просчитать всю аи сетку
	void GridClear();
	ID GridTraceBeam(const float3* start, const float3* dir) const;	//трассировка луча и проверка его пересечения с каким либо квадом сетки
	void GridTestValidation();										//тест действительности сетки и устранение возможных дефектов
	inline UINT GridGetCountSplits();										//
	inline void GridSetMarkSplits(bool mark);						//выделение сплитов
	inline bool GridGetMarkSplits();								//возвращает состояние выделения сплитов
	inline UINT GridGetCountQuads();								//возвращает количество квадов в сетке
	bool GridFindPath(ID beginq, ID endq);							//поиск пути, (beginq,beginq]
	inline UINT GridGetSizePath();									//размер найденного пути в количестве квадратов
	inline bool GridGetPath(ID * pmem, UINT count);					//запись найденного пути в уже выделенную память

	inline void GridSetColorArr(const ID * pmem, DWORD color, UINT count);
	inline void GridSetNullColor();
	//}

	//графпоинты
	//{
	void GraphPointGenerate();					//автоматическая генерация графпоинтов в центрах играничивающих боксов
	inline UINT GraphPointGetCount();			//возвращает количество графпоинтов
	inline void GraphPointClear();				//очистка списка графпоинтов
	inline void GraphPointAdd(ID id);			//добавление графпоинта (id - идентификатор квада сетки)
	inline void GraphPointDelete(ID id);		//удаление графпоинта (id - идентификатор квада сетки)
	ID GraphPointGetNear(ID beginq, ID endq);	//поиск наиболее близкого графпоинта между стартовой и конецной точкой следования
	//}
	
	//ренер
	//{
	void RenderQuads(const ISXFrustum * frustum, const float3 * viewpos, float dist);	//отрисовка сетки
	void RenderGraphPoints(const float3 * viewpos, float dist);							//отрисовка графпоинтов
	void RenderBB();			//отрисовка ограничивающих объемов
	//}
	
protected:

	bool UseGraphics;	//использовать ли графику

	IDirect3DDevice9* DXDevice;
	IDirect3DVertexDeclaration9* AIQuadDecl;
	ID Tex_AIGrid;
	ID IDVS;
	ID IDPS;
	ID Tex_BB;

	MemAlloc<AIQuad> AllocAIQuad;			//выделенная память для квадов
	MemAlloc<BoundAIQuad> AllocBoundAIQuad;	//выделенная память для баундов

	Array<uint32_t,1000> ArrColor;		//массив цвета для каждого квада сетки (по id квада)
	Array<uint32_t> ArrSplitsColor;		//массив цвета для каждого сплита (если сетка не цельная)
	Array<int3, 1000> ArrLongCoordQuads;//массив long значений координат для каждого квада (по id квада)
	Array<AIQuad*, 1000> ArrQuads;		//массив готовых квадов (порядковый номер и есть id)
	Array<AIQuad*, 1000> ArrQuadsCheck;	//массив квадов на проверку (используется при генерации)
	Array<AIQUAD_STATE> ArrState;		//массив состояний для каждого квада (по id квада)
	Array<bool> ArrPreCondFreeState;	//массив состояния проверка занятости для каждого квада (по id квада)

	Array<BoundAIQuad*> ArrBound;		//массив ограничивающих объемов для разбиения сетки
	Array<ID> ArrGraphPointsIDs;		//массив навигационных квадов, они такие же как и обычные квады, длинные пути удобнее строить через них
	Array<int32_t> ArrCostGPIDs;		//массив со стоимостями перемещения по навигационным квадам

	Array<ID, 1000> ArrTestSplitsIDs;	//массив id квадов которые мы проверяем при тесте целостности сетки
	Array<bool> ArrCloseIDs;			//закрытый список (по id квада)
	Array<ID> ArrOpenIDs;				//закрытый список, ArrIDsInOpen[id квада]
	Array<ID> ArrIDsInOpen;				//значение квада в открытом списке, по id квада
	Array<ID> ArrParentIDs;				//список родителей (по id квада)
	Array<CostAIQuad> ArrCostQuads;		//список стоимостей проходов по каждому кваду (по id квада)
	Array<ID> ArrPathIDs;				//массив с id квадов наденного пути

	Array<ID> ArrSelected;				//массив выделенных в данный момент квадов

	bool IsMarkSplits;		//true - выделить цветом сплиты сетки
	int NumLastKeyOpenList;	//примерный размер открытого списка, бывает чуть больше чем надо, но не выходит за пределы массива
	ID CountInitOpen2;		//номер последнего занесенного квада в открытый список
	uint16_t CountSplits;	//количество сплитов сетки

	//данные гпу, отрисовка по методу hardware instancing
	IDirect3DVertexBuffer9* VertexQuad;
	IDirect3DIndexBuffer9*  IndexQuad;
	IDirect3DVertexBuffer9* TransVertBuf;
	ID3DXMesh* BoundBox;

	//значения ограничивающего объема
	float3_t Max, Min;
	

	inline ID GraphPointGetKey(ID id);			//является ил квад с id графпоинтом
	inline bool IsFreeQuad2(ID id, int radius);	//рекурсивная функция проверки свободности квада

	inline bool QuadSelectedExists(ID id);					//выделен ли квад с id
	inline bool QuadSelectedExistsNeightborDirect(ID id);	//есть ли у квада с id сосед по прямому направлению который сейчас выделен

	void CorrectPosXZ(float3* pos);				//коррекция позиции под центр ближайшего квада (по xz)
	inline bool IsValidIdInArrQuads(ID id);		//валидный ли квад с указанным id
	void QuadDeleteInvalidAll();				//удалить все невалидные квады (это те которые по каким-то причинам оказались закрытыми или не имеют прямого направления)
	void QuadDelete2(ID id);					//удаление информации о кваде из всех возможных источников (с изменением всех идентификаторов которые идут после него)

	void PhyNavigate(AIQuad* quad);	//физическая навигация  квада
	inline bool AIGridCompareHeight(ID id1, ID id2);				//находится ли разница между высотами центров квадов в допустимом пределе
	void ReCreateBuffs();											//пересоздание буферов
	void DefInitBuffs(ID id);										//установка значений по умолчанию для квада id

	inline void AddUniqQuadArrag(const AIQuad* aq);					//добавление квада в подходящий ббокс и запись его позиции в int3
	inline void AddUniqQuadArrag2(const AIQuad* aq, ID idbb);		//добавление квада в указанный ббокс и запись его позиции в int3
	void GenQuad(AIQuad* aq);										//генерация соседей квада по прямым направлениям и полный просчет квада
	void ComputeNeighbor(ID id);									//просчет и назначение соседей для квада
	inline bool IsUniqQuadArrag(AIQuad* aq, ID * idquad, ID * idbb);//является ли квад уникальным
	void SplitBB(int x,int y,int z);								//деление пространства ограничивающего объема на более мелкие части

	//pathfind
	inline bool IsOpen(ID id);				//находится ли квад в открытом списке
	inline int CalcHCost(ID id, ID endid);	//просчет H стоимости, диагональные шаги не в счет
	inline int CalcH2Cost(ID id1, ID id2);	//просчет H стоимости, просчет расстояния между квадами * 100
	inline ID AddInOpenList(ID id);			//добавить квад в открытый список
};
