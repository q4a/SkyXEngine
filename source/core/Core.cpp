#include "Core.h"
#include <common/file_utils.h>
#include "AsyncFileReader.h"

CCore::CCore()
{
	m_pPluginManager = new CPluginManager();
}
CCore::~CCore()
{
	mem_delete(m_pPluginManager);
	for(AssotiativeArray<XGUID, IBaseEventChannel*>::Iterator i = m_mEventChannels.begin(); i; i++)
	{
		mem_delete(*i.second);
	}
}

void CCore::Release()
{
	--m_uRefCount;
	assert(m_uRefCount && "You MUST NOT explicitly call Release on IXCore interface");
}

IPluginManager *CCore::getPluginManager()
{
	return(m_pPluginManager);
}
IFileSystem *CCore::getFileSystem()
{
	return(NULL);
}

IAsyncFileReader *CCore::getAsyncFileReader()
{
	return(new CAsyncFileReader(this));
}
IAsyncTaskRunner *CCore::getAsyncTaskRunner()
{
	return(NULL);
}

IBaseEventChannel *CCore::getEventChannelInternal(const XGUID &guid)
{
	const AssotiativeArray<XGUID, IBaseEventChannel*>::Node *pNode;
	if(m_mEventChannels.KeyExists(guid, &pNode))
	{
		return(*pNode->Val);
	}
	//@HACK
	IBaseEventChannel *pOut = new IEventChannel<int>();
	m_mEventChannels[guid] = pOut;
	return(pOut);
}

void CCore::loadPlugins()
{
#ifdef WIN64
	auto list = FileGetListFiles("../bin64/plugins/*.dll");
#else
	auto list = FileGetListFiles("../bin/plugins/*.dll");
#endif
		
	IXPlugin *pPlugin = NULL;
	for(UINT i = 0, l = list.size(); i < l; ++i)
	{
		printf("Loading plugin '%s'... ", list[i].c_str());
		pPlugin = m_pPluginManager->loadPlugin((String("../bin"
#ifdef WIN64
			"64"
#endif
			"/plugins/") + list[i]).c_str());
		if(pPlugin)
		{
			printf(COLOR_GREEN "DONE!" COLOR_RESET "\n", list[i].c_str());
		}
		else
		{
			printf(COLOR_LRED "ERROR!" COLOR_RESET "\n", list[i].c_str());
		}
	}
	m_pPluginManager->invokeStartup(this);
}

void CCore::getRenderPipeline(IXRenderPipeline **ppRenderPipeline)
{
	if(m_pRenderPipeline)
	{
		m_pRenderPipeline->AddRef();
	}

	*ppRenderPipeline = m_pRenderPipeline;
}

void CCore::setRenderPipeline(IXRenderPipeline *pRenderPipeline)
{
	mem_release(m_pRenderPipeline);
	if(pRenderPipeline)
	{
		pRenderPipeline->AddRef();
	}
	m_pRenderPipeline = pRenderPipeline;
}