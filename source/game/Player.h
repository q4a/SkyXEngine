
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Класс игрока
*/

/*! \ingroup cbaseanimating
@{
*/

#ifndef __PLAYER_H
#define __PLAYER_H

#include "BaseCharacter.h"
#include "PointCamera.h"
#include "crosshair.h"

//! Класс игрока  \ingroup cbaseanimating
class CPlayer: public CBaseCharacter
{
	DECLARE_CLASS(CPlayer, CBaseCharacter);
	DECLARE_PROPTABLE();
public:
	CPlayer(CEntityManager * pMgr);
	~CPlayer();
	
	//! Возвращает камеру игрока
	CPointCamera * getCamera();

	//! Переключает в режим наблюдателя
	void observe();

	//! Помещает игрока в мир
	void spawn();

	/*! Запускает/останавливает движение в заданном направлении
		@param [in] dir направление движения из #PLAYER_MOVE
	*/
	void move(UINT dir, bool start);

	//! Обновляет инпут от игрока
	virtual void updateInput(float dt);

	void onSync();
	
	//! Получает смещения для задержки движения модели оружия при вращении игрока
	float3_t & getWeaponDeltaAngles();

	//! Команды для подбора положения визуальной модели оружия на экране
	void _ccmd_slot_on(int argc, const char ** argv);
	void _ccmd_slot_off();

	//! Получает объект перекрестия
	CCrosshair * getCrosshair();

	CHUDcontroller * getHUDcontroller();

	void dispatchDamage(CTakeDamageInfo &takeDamageInfo);

	//! Temp method
	void setActiveTool(CBaseTool *pTool)
	{
		m_pActiveTool = pTool;
	}

	void attack(BOOL state);

	void onDeath(CBaseEntity *pAttacker, CBaseEntity *pInflictor);

	void respawn();
	
protected:
	//! Камера
	CPointCamera * m_pCamera;

	//! ID интервала обновления
	ID m_iUpdIval;

	//! Может ли прыгать
	bool m_canJump;

	//! Для качания камеры @{
	float m_fViewbobStep;
	float m_fViewbobY;
	float3_t m_fViewbobStrafe;
	float3_t m_vWpnShakeAngles;
	//! @}

	int m_iDSM;

	//! Перекрестие
	CCrosshair * m_pCrosshair;

	//! Обновляет разброса значение
	virtual void updateSpread(float dt);

	bool m_bCanRespawn;
};

#endif

//! @}
