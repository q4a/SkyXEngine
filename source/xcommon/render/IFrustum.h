#ifndef __IFRUSTUM_H
#define __IFRUSTUM_H

#include <gdefines.h>
#include <common/math.h>

//! класс описывающий фрустум
class IFrustum: public IXUnknown
{
public:
	//! обновление фрустума, на вход матрицы по которым необходимо построить фрустум
	virtual void update(
		const float4x4 *pView,	//<! видовая матрица
		const float4x4 *pProj	//<! проекционная матрица
	) = 0;

	virtual void update(const SMPLANE *pPlanes, bool isNormalized = false) = 0;

	//! находится ли точка во фрустуме
	virtual bool pointInFrustum(const float3 *pPoint) const = 0;

	//! находится ли треугольник во фрутстуме
	virtual bool polyInFrustum(const float3 *pPoint1, const float3 *pPoint2, const float3 *pPoint3) const = 0;

	//! находится ли полигон во фрустуме полностью
	virtual bool polyInFrustumAbs(const float3 *pPoint1, const float3 *pPoint2, const float3 *pPoint3) const = 0;


	//! находится ли полигон во фрустуме
	virtual bool sphereInFrustum(const float3 *pPoint, float fRadius) const = 0;

	//! находится ли сфера во фрустуме полностью
	virtual bool sphereInFrustumAbs(const float3 *pPoint, float fRadius) const = 0;

	//! находится ли параллелепипед (описанный точками экстремума) во фрустуме
	virtual bool boxInFrustum(const float3 *pMin, const float3 *pMax) const = 0;

	//! находится ли параллелепипед (описанный точками экстремума) во фрустуме
	virtual bool frustumInFrustum(const IFrustum *pOther) const = 0;


	//! возвращает координаты точки фрустума, iNumPoint = [0,7]
	virtual float3 getPoint(int iNumPoint) const = 0;

	//! возвращает координаты центра фрустума
	virtual float3 getCenter() const = 0;

	////! устанавливает координаты точки фрустума, iNumPoint = [0,7]
	//virtual void setPoint(int iNumPoint, const float3 *pPoint) = 0;

	////! устанавливает координаты центра фрустума
	//virtual void setCenter(const float3 *pCenter) = 0;
};

#endif
