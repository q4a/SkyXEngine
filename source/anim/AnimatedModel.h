#ifndef __ANIMATEDMODEL_H
#define __ANIMATEDMODEL_H

#include <xcommon/resource/IXModel.h>
#include "AnimatedModelShared.h"

class CAnimatedModel: public IXAnimatedModel
{
public:
	CAnimatedModel(CAnimatedModelProvider *pProvider, CAnimatedModelShared *pShared);
	~CAnimatedModel();

	bool XMETHODCALLTYPE isEnabled() const override;
	void XMETHODCALLTYPE enable(bool yesNo) override;

	IXAnimatedModel * XMETHODCALLTYPE asAnimatedModel() override;
	IXDynamicModel * XMETHODCALLTYPE asDynamicModel() override;
	IXStaticModel * XMETHODCALLTYPE asStaticModel() override;


	float3 XMETHODCALLTYPE getPosition() const override;
	void XMETHODCALLTYPE setPosition(const float3 &vPos) override;

	SMQuaternion XMETHODCALLTYPE getOrientation() const override;
	void XMETHODCALLTYPE setOrientation(const SMQuaternion &qRot) override;

	UINT XMETHODCALLTYPE getSkin() const override;
	void XMETHODCALLTYPE setSkin(UINT uSkin) override;

	float3 XMETHODCALLTYPE getLocalBoundMin() const override;
	float3 XMETHODCALLTYPE getLocalBoundMax() const override;

	float4 XMETHODCALLTYPE getColor() const override;
	void XMETHODCALLTYPE setColor(const float4 &vColor) override;

	UINT XMETHODCALLTYPE getPhysboxCount(UINT uPartIndex = 0) const override;
	const IModelPhysbox * XMETHODCALLTYPE getPhysBox(UINT id, UINT uPartIndex = 0) const override;
	const IXResourceModel * XMETHODCALLTYPE getResource(UINT uIndex = 0) override;



	UINT XMETHODCALLTYPE getPartsCount() const override;
	const char * XMETHODCALLTYPE getPartName(UINT uIndex) const override;
	UINT XMETHODCALLTYPE getPartIndex(const char *szName) override;
	XMODEL_PART_FLAGS XMETHODCALLTYPE getPartFlags(UINT uIndex) const override;
	bool XMETHODCALLTYPE isPartEnabled(UINT uIndex) const override;
	void XMETHODCALLTYPE enablePart(UINT uIndex, bool yesNo) override;

	UINT XMETHODCALLTYPE getHitboxCount(UINT uPartIndex = 0) const override;
	const XResourceModelHitbox * XMETHODCALLTYPE getHitbox(UINT id, UINT uPartIndex = 0) const override;

	void XMETHODCALLTYPE play(const char *szName, UINT uFadeTime = 0, UINT uSlot = 0, bool bReplaceActivity = true) override;
	void XMETHODCALLTYPE stop(UINT uSlot = 0) override;
	void XMETHODCALLTYPE resume(UINT uSlot = 0) override;
	void XMETHODCALLTYPE setProgress(float fProgress, UINT uSlot = 0) override;
	void XMETHODCALLTYPE startActivity(const char *szName, UINT uFadeTime = 0, UINT uSlot = 0) override;
	void XMETHODCALLTYPE stopAll() override;

	float3 XMETHODCALLTYPE getBoneTransformPos(UINT id) override;
	SMQuaternion XMETHODCALLTYPE getBoneTransformRot(UINT id) override;
	SMMATRIX XMETHODCALLTYPE getBoneTransform(UINT id) override;

	UINT XMETHODCALLTYPE getBoneId(const char *szName) override;
	UINT XMETHODCALLTYPE getBoneCount() const override;
	const char * XMETHODCALLTYPE getBoneName(UINT id) const override;

	bool XMETHODCALLTYPE isPlayingAnimations() override;
	bool XMETHODCALLTYPE isPlayingAnimation(const char *szName) override;
	
	void XMETHODCALLTYPE setController(UINT id, float fValue) override;

	UINT XMETHODCALLTYPE getControllersCount() const override;
	const char * XMETHODCALLTYPE getControllerName(UINT id) override;
	UINT XMETHODCALLTYPE getControllerId(const char *szName) override;

	void XMETHODCALLTYPE setCallback(IAnimationCallback *pCallback) override;

	void update(float fDT);

	UINT addLayer();

	void render(UINT uLod);
	void sync();
protected:
	CAnimatedModelProvider *m_pProvider;
	CAnimatedModelShared *m_pShared;
	IGXContext *m_pDevice;

	float3_t m_vPosition;
	SMQuaternion m_qRotation;
	UINT m_uSkin = 0;
	float4_t m_vColor{1.0f, 1.0f, 1.0f, 1.0f};
	bool m_isEnabled = true;

	IGXConstantBuffer *m_pBoneConstantBuffer = NULL;

	IAnimationCallback *m_pCallback = NULL;

	struct ModelBoneShader
	{
		float4 position;   /*!< Позиция */
		SMQuaternion orient; /*!< Вращение */

		SX_ALIGNED_OP_MEM2();
	};

	ModelBoneShader *m_pRenderFrameBones = NULL; //! Используются в текущем кадре }
	ModelBoneShader *m_pNextFrameBones = NULL; //! Рассчет на следующий кадр      } -- переключаются каждый кадр
	
	XResourceModelBone *m_pBoneControllers = NULL;

	bool m_isBoneMatrixReFilled = false;

	struct layer_s
	{
		bool isPlaying = false;
		bool isNewPlaying = false; // for fading
		int iCurrentFrame = 0;
		UINT uFrameCount = 0;
		float fTime = 0;

		UINT uFadeTime = 0;
		UINT uFadeCurTime = 0;
		bool isInFade = false;
		bool bDoAdvance = true;

		XResourceModelBone *pLastFrameBones = NULL;
		XResourceModelBone *pCurrentBones = NULL;
		bool *isBoneWorld = NULL;

		UINT uAnimationId = 0;

		int iActivity = -1;
		UINT uActivityFadeTime = 0;

		bool isDirty = true;
	};
	Array<layer_s, 1> m_aLayers;


	void playActivityNext(UINT uLayer);
	bool validateLayer(UINT uLayer);
	void fillBoneMatrix();
};

#endif