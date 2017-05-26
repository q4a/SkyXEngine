#ifndef _PROPTABLE_H_
#define _PROPTABLE_H_

class SXbaseEntity;

//! ���� ����� ������
enum PDF_TYPE
{
	PDF_INT,
	PDF_FLOAT,
	PDF_VECTOR,
	PDF_BOOL,
	PDF_STRING,
	PDF_ANGLES,
	PDF_ENTITY,
	PDF_PARENT
};

//! ���� ���������� �����
enum PDE_TYPE
{
	PDE_NONE = 0,
	PDE_TEXTFIELD,
	PDE_COMBOBOX
};

typedef int SXbaseEntity::*fieldtype;

struct editor_kv
{
	const char * key;
	const char * value;
};

struct prop_editor_t
{
	PDE_TYPE type;
	void * pData;
};

struct propdata_t
{
	fieldtype pField;
	PDF_TYPE type;
	int flags;
	const char * szKey;
	const char * szEdName;
	prop_editor_t editor;
};

struct proptable_t
{
	propdata_t * pData;
	int numFields;
	proptable_t * pBaseProptable;
};

#define ED_COMBO_MAXELS 256

prop_editor_t _GetEditorCombobox(int start, ...);

#define DECLARE_PROPTABLE() \
	\
	friend class EntityFactoryMap; \
	friend class EntityFactory<ThisClass>; \
	static proptable_t m_pPropTable; \
	\
protected:\
	static void InitPropData(); \
	static proptable_t * GetPropTable(); \
	static void ReleasePropData();\
public:\
	virtual propdata_t * GetField(const char * name);\
private:

#define _BEGIN_PROPTABLE(cls, bclpt) \
proptable_t cls::m_pPropTable = {0,0}; \
void cls::ReleasePropData()\
{\
	for(int i = 0; i < m_pPropTable.numFields; ++i)\
	{\
		if(m_pPropTable.pData[i].editor.type == PDE_COMBOBOX && m_pPropTable.pData[i].editor.pData)\
			delete[] m_pPropTable.pData[i].editor.pData; \
	}\
}\
proptable_t * cls::GetPropTable()\
{\
	if(!m_pPropTable.numFields)\
		InitPropData();\
	return(&m_pPropTable);\
}\
propdata_t * cls::GetField(const char * name)\
{\
	proptable_t * pt = GetPropTable();\
	while(pt)\
	{\
		for(int i = 0; i < pt->numFields; ++i)\
		{\
			if(pt->pData[i].szKey && !strcmp(pt->pData[i].szKey, name))\
			{\
				return(&pt->pData[i]);\
			}\
		}\
		pt = pt->pBaseProptable;\
	}\
	return(NULL);\
}\
void cls::InitPropData() \
{ \
	bclpt \
	typedef cls DataClass; \
	static propdata_t pData[] = {{0}

#define BEGIN_PROPTABLE(cls) \
	_BEGIN_PROPTABLE(cls, m_pPropTable.pBaseProptable = BaseClass::GetPropTable();)

#define BEGIN_PROPTABLE_NOBASE(cls) \
	_BEGIN_PROPTABLE(cls, m_pPropTable.pBaseProptable = NULL;)

#define END_PROPTABLE() \
	}; \
	if(ARRAYSIZE(pData) > 1) \
	{\
		m_pPropTable.pData = &pData[1]; \
		m_pPropTable.numFields = ARRAYSIZE(pData) - 1; \
	} \
	else \
	{ \
		m_pPropTable.pData = pData; \
		m_pPropTable.numFields = 1; \
	} \
} 

#define DECLARE_TRIVIAL_CONSTRUCTOR() ThisClass(EntityManager * pMgr):BaseClass(pMgr){}

const char * GetEmptyString();

#define EDITOR_NONE {PDE_NONE, NULL}}
#define EDITOR_TEXTFIELD {PDE_TEXTFIELD, NULL}}
#define EDITOR_COMBOBOX _GetEditorCombobox(0
#define COMBO_OPTION(name, value) , name, value
#define EDITOR_COMBO_END() , NULL)}


#define DEFINE_FIELD_STRING(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_STRING, flags, keyname, edname, editor
#define DEFINE_FIELD_VECTOR(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_VECTOR, flags, keyname, edname, editor
#define DEFINE_FIELD_ANGLES(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_ANGLES, flags, keyname, edname, editor
#define DEFINE_FIELD_INT(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_INT, flags, keyname, edname, editor
#define DEFINE_FIELD_FLOAT(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_FLOAT, flags, keyname, edname, editor
#define DEFINE_FIELD_BOOL(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_BOOL, flags, keyname, edname, editor
#define DEFINE_FIELD_ENTITY(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_ENTITY, flags, keyname, edname, editor
#define DEFINE_FIELD_PARENT(field, flags, keyname, edname, editor) , {(fieldtype)&DataClass::field, PDF_PARENT, flags, keyname, edname, editor

#endif
