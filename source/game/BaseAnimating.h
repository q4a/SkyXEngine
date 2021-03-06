
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Анимированный игровой объект
*/

/*! \defgroup cbaseanimating Моделированные объекты
\ingroup cbaseentity
@{
*/

#ifndef __BASE_ANIMATING_H
#define __BASE_ANIMATING_H

#include "BaseEntity.h"
#include <xcommon/resource/IXModelProvider.h>

#define BLEND_MAX 4

class CAnimationCallback;
//! Анимированный игровой объект
class CBaseAnimating: public CBaseEntity
{
	DECLARE_CLASS(CBaseAnimating, CBaseEntity);
	DECLARE_PROPTABLE();

	friend class CAnimationCallback;
public:
	CBaseAnimating(CEntityManager * pMgr);
	~CBaseAnimating();

	void getMinMax(float3 * min, float3 * max);
	// void getSphere(float3 * center, float * radius);

	virtual void setModel(const char *szMdl);
	virtual void setScale(float fScale);

	float3 getAttachmentPos(int id);
	SMQuaternion getAttachmentRot(int id);

	void onSync() override;

	void playAnimation(const char * name, UINT uFadeTime = 0, UINT slot = 0);
	void playActivity(const char * name, UINT uFadeTime = 0, UINT slot = 0);

	void playAnimationNext(const char * name, UINT uFadeTime = 0, UINT slot = 0);
	void playActivityNext(const char * name, UINT uFadeTime = 0, UINT slot = 0);

	void cancelNextAnimation(int iSlot = -1);

	bool playingAnimations(const char* name);
	void setPos(const float3 & pos);
	void setOrient(const SMQuaternion & q);

	void setSkin(int iSkin);
	
	void setCollisionGroup(COLLISION_GROUP group, COLLISION_GROUP mask = CG_ALL);
	COLLISION_GROUP getCollisionGroup();

	void renderEditor(bool is3D) override;

protected:
	virtual void _initEditorBoxes();
	virtual void _releaseEditorBoxes();

	void inputPlayAnim(inputdata_t * pInputdata);
	void inputPlayAnimNext(inputdata_t * pInputdata);

	void inputPlayActivity(inputdata_t * pInputdata);
	void inputPlayActivityNext(inputdata_t * pInputdata);

	void inputSetSkin(inputdata_t * pInputdata);

	IXModel *m_pModel = NULL;
	const char * m_szModelFile;
	float m_fBaseScale = 1.0f;
	bool m_isStatic = false;
	bool m_useAutoPhysbox = true;

	CBaseEntity *m_pEntColorRef = NULL;
	float3_t m_vGlowColor;

	virtual void initPhysics();
	virtual void releasePhysics();
	virtual void createPhysBody();
	virtual void removePhysBody();

	btCollisionShape *m_pCollideShape = NULL;
	btRigidBody *m_pRigidBody = NULL;

	virtual void _cleanup();

	virtual void onAnimationStart(UINT uLayer);
	virtual void onAnimationStop(UINT uLayer);
	virtual void onAnimationLoop(UINT uLayer);
	virtual void onAnimationProgress(UINT uLayer, float fProgress);

	void onIsStaticChange(bool isStatic);
	void onSetUseAutoPhysbox(bool use);
	int m_iSkin = 0;

	struct
	{
		char szName[XMODEL_MAX_NAME];
		UINT uFadeTime;
		bool isActivity;
	} m_vNextAnim[BLEND_MAX];

private:
	COLLISION_GROUP m_collisionGroup = CG_DEFAULT;
	COLLISION_GROUP m_collisionMask = CG_ALL;
	CAnimationCallback *m_pAnimationCallback;
};

#endif

/*! @} cbaseanimating */
