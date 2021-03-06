#ifndef __RENDERABLE_VISIBILITY_H
#define __RENDERABLE_VISIBILITY_H

#include <xcommon/IXRenderable.h>

class CAnimatedModelProvider;
class CDynamicModelProvider;
class CDynamicModel;
class CRenderableVisibility: public IXRenderableVisibility
{
public:
	CRenderableVisibility(ID idPlugin, CAnimatedModelProvider *m_pProviderAnimated, CDynamicModelProvider *m_pProviderDynamic);

	ID getPluginId();

	void setOcclusionCuller(IXOcclusionCuller *pOcclusionCuller);

	void updateForCamera(ICamera *pCamera, const IXRenderableVisibility *pReference = NULL);

	void updateForFrustum(const IFrustum *pFrustum, const IXRenderableVisibility *pReference = NULL);
	
	struct item_s
	{
		bool isVisible = false;
		UINT uLod = 0;
		bool isTransparent = false;
	};

	struct TransparentModel
	{
		CDynamicModel *pReferenceMdl;
		bool hasPSP;
		SMPLANE psp;
		UINT uLod;
		IXMaterial *pMaterial;
	};

	void setItemCount(UINT uCount);
	item_s* getItem(UINT uIndex);

	void setItemCountDynamic(UINT uCount);
	item_s* getItemDynamic(UINT uIndex, bool forceCreate=true);

	void resetItemTransparentDynamic();
	void addItemTransparentDynamic(const TransparentModel &mdl);
	UINT getItemTransparentDynamicCount();
	TransparentModel* getItemTransparentDynamic(UINT uIndex);
protected:
	ID m_idPlugin;
	CAnimatedModelProvider* m_pProviderAnimated;
	CDynamicModelProvider* m_pProviderDynamic;

	Array<item_s> m_aItems;
	Array<item_s> m_aItemsDynamic;
	Array<TransparentModel> m_aItemsDynamicTransparent;
};

#endif
