
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Базовый свет
*/

#ifndef __BASELIGHT_H
#define __BASELIGHT_H

#include "PointEntity.h"

#define LIGHT_INITIALLY_DARK ENT_FLAG_0

/*! Базовый источник света
\ingroup clight
*/
class CBaseLight: public CPointEntity
{
	DECLARE_CLASS(CBaseLight, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	~CBaseLight();

	void toggleEnable();

	void setColor(const float3 &vColor)
	{
		m_vColor = vColor;
	}
	const float3_t& getColor() const
	{
		return m_vColor;
	}

	void setDist(float fDist)
	{
		m_fDist = fDist;
	}
	float getDist() const
	{
		return m_fDist;
	}

	/*void setShadowDist(float fShadowDist)
	{
		m_fShadowDist = fShadowDist;
	}
	float getShadowDist() const
	{
		return m_fShadowDist;
	}*/

	void setShadowIntensity(float fShadowIntensity)
	{
		m_fShadowIntensity = fShadowIntensity;
	}
	float getShadowIntensity() const
	{
		return m_fShadowIntensity;
	}

	void setShadowType(int iShadowType)
	{
		m_iShadowType = iShadowType;
	}
	int getShadowType() const
	{
		return m_iShadowType;
	}

	void setEnable(bool isEnable);
	bool getEnable() const
	{
		return m_isEnable;
	}

	bool getMainColor(float3_t *pOut);

	void updateFlags();

protected:
	IXLight *m_pLight = NULL;

	float3_t m_vColor;
	float m_fDist;
	//float m_fShadowDist;
	int m_iShadowType;
	bool m_isEnable;
	float m_fShadowIntensity;
	CBaseEntity *m_pLinkedTo = NULL;

	Array<CBaseLight*> m_vpLinkedLights;

	void setLinkedTo(CBaseEntity *pEnt);

	void onSync();

	void addLinkedLight(CBaseLight *pEnt);
	void removeLinkedLight(CBaseLight *pEnt);

	void turnOn(inputdata_t * pInputdata);
	void turnOff(inputdata_t * pInputdata);

	output_t m_onTurnOn;
	output_t m_onTurnOff;
};

#endif
