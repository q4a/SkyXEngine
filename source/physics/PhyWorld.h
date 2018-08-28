
/***********************************************************
Copyright � Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __PHY_WORLD_H
#define __PHY_WORLD_H


#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <common/AssotiativeArray.h>
#include <common/Array.h>
#include <common/file_utils.h>

#include <gdefines.h>

#include <common/SXMath.h>
#include <mtrl/sxmtrl.h>

#define PHY_MAT_FILE_MAGICK 3630267958475905107

#pragma pack(push,1)
struct PhyMatFile
{
	int64_t i64Magick;
	uint32_t uiGeomFaceCount;
	uint32_t uiMatCount;
	//uint32_t uiGreenObjCount;

	PhyMatFile():
		i64Magick(PHY_MAT_FILE_MAGICK)
	{
	}
};
#pragma pack(pop)

class CPhyWorld
{
public:
	CPhyWorld();
	~CPhyWorld();

	void setThreadNum(int tnum);
	void update(int thread = 0);
	void sync();

	void addShape(btRigidBody * pBody);
	void addShape(btRigidBody * pBody, int group, int mask);
	void removeShape(btRigidBody * pBody);


	void loadGeom(const char * file=NULL);
	void unloadGeom();

	bool importGeom(const char * file);
	bool exportGeom(const char * file);

	void disableSimulation();
	void enableSimulation();

	void render();

	MTLTYPE_PHYSIC getMtlType(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo);
	
	ID getMtlID(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo);

	btDiscreteDynamicsWorld * getBtWorld()
	{
		return(m_pDynamicsWorld);
	}

	class CDebugDrawer: public btIDebugDraw
	{
		struct render_point
		{
			float3_t pos;
			DWORD clr;
		};
		int m_iDebugMode;
		Array<render_point, 16384> m_vDrawData;

		bool m_bExpectObject;
	public:
		CDebugDrawer():
			m_bExpectObject(false)
		{
		}

		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

		void reportErrorWarning(const char* warningString);

		void draw3dText(const btVector3& location, const char* textString);

		void setDebugMode(int debugMode);

		int getDebugMode() const;

		void render();
	};

protected:
	btDefaultCollisionConfiguration * m_pCollisionConfiguration;
	btCollisionDispatcher * m_pDispatcher;
	btBroadphaseInterface * m_pBroadphase;
	btSequentialImpulseConstraintSolver * m_pSolver;
	btDiscreteDynamicsWorld * m_pDynamicsWorld;
	btGhostPairCallback * m_pGHostPairCallback;

	const bool * m_bDebugDraw;
	CDebugDrawer * m_pDebugDrawer;

	// ���������� ����� ��� ����������� ��������� ������
	btTriangleMesh * m_pGeomStaticCollideMesh;
	btCollisionShape * m_pGeomStaticCollideShape;
	btRigidBody * m_pGeomStaticRigidBody;
	//MTLTYPE_PHYSIC *m_pGeomMtlTypes;
	ID *m_pGeomMtlIDs;
	int m_iGeomFacesCount;
	int m_iGeomModelCount;

	btCollisionShape ** m_ppGreenStaticCollideShape;
	btRigidBody *** m_pppGreenStaticRigidBody;
	int m_iGreenShapes;
	int * m_piGreenTotal;

	bool m_isRunning;
};

#endif
