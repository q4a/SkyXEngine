
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Базовый игровой объект
*/

/*! \defgroup cbaseentity Объекты. Базовый объект
\ingroup sxgame
Базовый объект, от которого наследуются все остальные классы объектов.
@{
*/

#ifndef __BASE_ENTITY_H
#define __BASE_ENTITY_H

#include <gdefines.h>
#include <common/SXMath.h>
#include <physics/sxphysics.h>

#include "EntityFactory.h"
#include "EntityManager.h"

#include "proptable.h"

#pragma pointers_to_members(full_generality, virtual_inheritance)

#pragma warning(push)
#pragma warning(disable:4251)

//! Базовый объект, от которого наследуются все остальные классы объектов.
class SXGAME_EXPORT CBaseEntity
{
	DECLARE_CLASS_NOBASE(CBaseEntity);
	DECLARE_PROPTABLE();

	friend class CEntityManager;

public:
	//! Конструктор
	CBaseEntity(CEntityManager * pMgr);
	virtual ~CBaseEntity();

	//! Возвращает имя движкового класса объекта
	const char * getClassName();
	
	//! Возвращает имя объекта
	const char * getName();
	
	//! Возвращает баунд объекта
	virtual void getMinMax(float3 * min, float3 * max);
	
	//! Возвращает баунд сферу объекта
	virtual void getSphere(float3 * center, float * radius);

	//! Устанавливает мировую позицию объекта
	virtual void setPos(const float3 & pos);
	//! Получает мировую позицию объекта
	float3 getPos();

	//! Получает ID объекта в системе
	ID getId();

	//! Получает флаги объекта
	UINT getFlags();
	//! Устанавливает флаги объекта
	void setFlags(UINT f);

	//! Получает мировую матрицу трансформации для объекта
	SMMATRIX getWorldTM();

	//! Устанавливает вращение объекта
	virtual void setOrient(const SMQuaternion & q);
	//! Возвращает вращение объекта
	SMQuaternion getOrient();

	//! Устанавливает свойство объекта
	virtual bool setKV(const char * name, const char * value);
	//! Получает свойство объекта
	virtual bool getKV(const char * name, char * out, int bufsize);

	//! Устанавливает родительский объект для объекта
	void setParent(CBaseEntity * pEnt, int attachment = -1);
	//! Возвращает родительский объект объекта
	CBaseEntity * getParent();

	//! Устанавливает владельца
	void setOwner(CBaseEntity * pEnt);
	//! Возвращает владельца объекта
	CBaseEntity * getOwner();

	//! Получает объект менеджера
	CEntityManager * getManager();

	//void updateDiscreteLinearVelocity(int step, float dt);
	//const float3_t & getDiscreteLinearVelocity() const;

private:
	void setClassName(const char * name);
	void setDefaults();

	const char * m_szClassName;

protected:
	CEntityManager * m_pMgr;

	//! Позиция объекта
	float3_t m_vPosition;
	
	/*! Скорость
	\note вероятно, не используется
	*/
	float3_t m_vSpeed;
	
	//! вращение
	SMQuaternion m_vOrientation;
	//float3_t m_vDiscreteLinearVelocity;
	//float3_t m_vOldPosition;

	//! Позиция смещения (для иерархической структуры)
	float3_t m_vOffsetPos;
	//! Вращение смещения (для иерархической структуры)
	SMQuaternion m_vOffsetOrient;

	//! Идентификатор в системе
	ID m_iId;

	//! Флаги
	UINT m_iFlags;

	//! Имя объекта
	const char * m_szName;

	//! Родитель
	CBaseEntity * m_pParent;
	//! Индекс кости родителя
	int m_iParentAttachment;

	//! Владелец
	CBaseEntity * m_pOwner;

	//! Вызывается на стадии синхронизации
	virtual void onSync();
	//! Вызывается при создании после установки всех свойств
	virtual void onPostLoad();

	//! Получает смещение для кости
	virtual float3 getAttachmentPos(int id);
	//! Получает вращение для кости
	virtual SMQuaternion getAttachmentRot(int id);

	/*! Устанавливает значение строкового свойства
	\note только для внутреннего использования
	*/
	void _setStrVal(const char ** to, const char * value);

	/*! Обновляет выходы
	\note только для внутреннего использования
	*/
	void updateOutputs();
};

#pragma warning(pop)

#endif

/*! @} cbaseentity*/