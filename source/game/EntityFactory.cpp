
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "EntityFactory.h"
#include "BaseEntity.h"


CEntityFactoryMap::CEntityFactoryMap():
	m_iShowInListCount(0)
{

}

void CEntityFactoryMap::addFactory(IEntityFactory *pFactory, const char *szName)
{
	if(!pFactory->isEditorHidden())
	{
		++m_iShowInListCount;
	}
	m_mFactories[AAString(szName)] = pFactory;
}
CBaseEntity* CEntityFactoryMap::create(const char *szName, CEntityManager *pWorld, bool bDelayPostLoad)
{
	IEntityFactory *pFactory = getFactory(szName);
	if(pFactory)
	{
		EntDefaultsMap *defs = pFactory->getDefaults();
		CBaseEntity *pEnt = pFactory->create(pWorld);
		pEnt->setDefaults();
		pEnt->setClassName(pFactory->getClassName());

		if(defs->Size() > 0)
		{
			proptable_t *pt = pFactory->getPropTable();
			const char *key;
			const EntDefaultsMap::Node *pNode;
			while(pt)
			{
				for(int i = 0, l = pt->numFields; i < l; ++i)
				{
					key = pt->pData[i].szKey;
					if(defs->KeyExists(AAString(key), &pNode))
					{
						pEnt->setKV(key, *(pNode->Val));
					}
				}

				pt = pt->pBaseProptable;
			}
		}
		if(!bDelayPostLoad)
		{
			pEnt->onPostLoad();
		}
		if(pWorld->isEditorMode())
		{
			pEnt->_initEditorBoxes();
		}

		if(pFactory->isSyncable())
		{
			pWorld->regSync(pEnt);
		}

		return(pEnt);
	}
	return(NULL);
}
void CEntityFactoryMap::destroy(CBaseEntity *pEnt)
{
	if(pEnt)
	{
		IEntityFactory *pFactory = getFactory(pEnt->getClassName());
		if(pFactory)
		{
			if(pFactory->isSyncable())
			{
				pEnt->m_pMgr->unregSync(pEnt);
			}
			return(pFactory->destroy(pEnt));
		}
	}
}

IEntityFactory * CEntityFactoryMap::getFactory(const char * szName)
{
	AAString key(szName);
	if(m_mFactories.KeyExists(key))
	{
		return(m_mFactories[key]);
	}
	return(NULL);
}

CEntityFactoryMap * CEntityFactoryMap::GetInstance()
{
	static CEntityFactoryMap map;
	return(&map);
}

proptable_t * CEntityFactoryMap::getPropTable(const char * szClass)
{
	return(getFactory(szClass)->getPropTable());
}

bool CEntityFactoryMap::isEditorHidden(const char * szClass)
{
	return(getFactory(szClass)->isEditorHidden());
}

EntDefaultsMap * CEntityFactoryMap::getDefaults(const char * szClass)
{
	return(getFactory(szClass)->getDefaults());
}

int CEntityFactoryMap::getListCount()
{
	return(m_iShowInListCount);
}

void CEntityFactoryMap::getListing(const char ** pszOut, int size)
{
	int j = 0;
	if(size > m_iShowInListCount)
	{
		size = m_iShowInListCount;
	}
	for(AssotiativeArray<AAString, IEntityFactory*>::Iterator i = m_mFactories.begin(); i && j < size; i++)
	{
		if(!(*i.second)->isEditorHidden())
		{
			pszOut[j++] = i.first->getName();
		}
	}
}

const char *CEntityFactoryMap::getClassNamePtr(const char *szClassName)
{
	IEntityFactory * pFactory = getFactory(szClassName);
	if(pFactory)
	{
		return(pFactory->getClassName());
	}
	return(NULL);
}
