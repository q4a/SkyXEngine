
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "crosshair.h"
#include <common/SXMath.h>
#include "GameData.h"

CCrosshair::CCrosshair():
	m_bDirty(true),
	m_bBuildBuff(false),
	m_fSize(0),
	m_idTexture(-1),
	m_bHidden(true),
	m_fMaxSize(0.7f), // 70% of screen height
	m_style(SPLIT_MOVE),
	m_fFixedRadius(0.0f),
	m_fAngle(0.0f),
	m_iNumSegs(4),
	m_u8ActiveBuffer(0),
	m_pTexture(NULL)
{
	m_pDev = SGCore_GetDXDevice();
	int iNumSegs = CROSSHAIR_MAXSEGS * 2;
	int segs = iNumSegs;
	while(segs < CROSSHAIR_MINSEGS)
	{
		segs += iNumSegs;
	}
	int iVC = (iNumSegs + segs + iNumSegs) * 2 + segs;
	int iIC = (segs - 2) * 3 + segs * 6;
	m_pMemoryBlob = new byte[(sizeof(Vertex) * iVC + sizeof(UINT) * iIC) * 2];
	m_pVertices[0] = (Vertex*)&m_pMemoryBlob[0];
	m_pIndices[0] = (UINT*)&m_pMemoryBlob[sizeof(Vertex)* iVC];
	m_pVertices[1] = (Vertex*)&m_pMemoryBlob[(sizeof(Vertex)* iVC + sizeof(UINT)* iIC)];
	m_pIndices[1] = (UINT*)&m_pMemoryBlob[sizeof(Vertex)* iVC + (sizeof(Vertex)* iVC + sizeof(UINT)* iIC)];

	m_pDev->CreateVertexBuffer(sizeof(Vertex)* iVC, D3DUSAGE_WRITEONLY, NULL, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
	m_pDev->CreateIndexBuffer(sizeof(UINT)* iIC, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIndexBuffer, NULL);
}

CCrosshair::~CCrosshair()
{
	mem_delete_a(m_pMemoryBlob);
}

void CCrosshair::setTexInfo(const float2_t & offs, const float2_t & size)
{
	m_bDirty = true;
	m_f2TexOffs = offs;
	m_f2TexSize = size;
}


void CCrosshair::update()
{
	if(m_bDirty && m_idTexture >= 0)
	{
		if(!m_pTexture)
		{
			m_pTexture = SGCore_LoadTexGetTex(m_idTexture);
			if(!m_pTexture)
			{
				return;
			}
		}

		static const int *r_win_width = GET_PCVAR_INT("r_win_width");
		static const int *r_win_height = GET_PCVAR_INT("r_win_height");

		//build new buffer
		float fScreenWidth = (float)*r_win_width;
		float fScreenHeight = (float)*r_win_height;
		float fTexWidth = m_f2TexSize.x;
		float fTexHeight = m_f2TexSize.y;
		float fXradius = fTexWidth / fScreenWidth * 0.5f;
		float fYradius = fTexHeight / fScreenHeight * 0.5f;
		float fYradiusMax = m_fMaxSize * fScreenHeight * 0.5f;
		float fXradiusMax = fYradiusMax * (fTexWidth / fTexHeight);
		float fXoffset = m_fSize * (fXradiusMax - fTexWidth * 0.5f) / fScreenWidth;
		float fYoffset = m_fSize * (fYradiusMax - fTexHeight * 0.5f) / fScreenHeight;

		float fXFixedRadius = m_fFixedRadius / fScreenWidth;
		float fYFixedRadius = m_fFixedRadius / fScreenHeight;

		float fTexLeft = m_f2TexOffs.x;
		float fTexTop = m_f2TexOffs.y;

		D3DSURFACE_DESC _info;
		m_pTexture->GetLevelDesc(0, &_info);
		float fTexFullWidth = (float)_info.Width;
		float fTexFullHeight = (float)_info.Height;

		float fTexFRX = m_fFixedRadius / fTexWidth * 2.0f;
		float fTexFRY = m_fFixedRadius / fTexHeight * 2.0f;

		float2_t f2TexMin = float2_t(fTexLeft / fTexFullWidth, (fTexFullHeight - fTexTop) / fTexFullHeight);
		float2_t f2TexMax = f2TexMin + float2_t(fTexWidth / fTexFullWidth, -fTexHeight / fTexFullHeight);
		//f2TexMin = float2_t(0, 0);
		//f2TexMax = float2_t(1, 1);

		int segs = m_iNumSegs;
		while(segs < CROSSHAIR_MINSEGS)
		{
			segs += m_iNumSegs;
		}

		Vertex * pVertices = m_pVertices[m_u8ActiveBuffer ? 0 : 1];
		UINT * pIndices = m_pIndices[m_u8ActiveBuffer ? 0 : 1];
		int iCurVtx = 0,
			iCurIdx = 0,
			iStopIdx = 0;

		float angle = m_fAngle;
		float fOX = 0.0f, fOY = 0.0f;
		int i = 0;
		bool stop;
		if(m_style == SPLIT_MOVE)
		{
			if(m_fFixedRadius == 0.0f)
			{
				//centered
				//iVC = m_iNumSegs + segs + m_iNumSegs;
				//iIC = segs * 3;

				for(float angleEnd = angle + SM_2PI, angleStep = SM_2PI / (float)segs; 
					angle < angleEnd; 
					angle += angleStep, ++i)
				{
					stop = i % (segs / m_iNumSegs) == 0;
					if(stop)
					{
						if(i != 0)
						{
							pIndices[iCurIdx++] = iStopIdx;
							pIndices[iCurIdx++] = iCurVtx;
							pIndices[iCurIdx++] = iCurVtx - 1;
							pVertices[iCurVtx++] = {
								float3_t(fXradius * sinf(angle) + fOX, fYradius * -cosf(angle) + fOY, 0.0f),
								float2_t(lerpf(f2TexMin.x, f2TexMax.x, sinf(angle) * 0.5f + 0.5f), -lerpf(f2TexMax.y, f2TexMin.y, -cosf(angle) * 0.5f + 0.5f))
							}; // circle
						}

						float avgAngle = angle + (SM_2PI / (float)m_iNumSegs) * 0.5f;
						fOX = sinf(avgAngle) * fXoffset;
						fOY = -cosf(avgAngle) * fYoffset;

						iStopIdx = iCurVtx;
						pVertices[iCurVtx++] = {
							float3_t(fOX, fOY, 0.0f),
							float2_t(lerpf(f2TexMin.x, f2TexMax.x, 0.5f), -lerpf(f2TexMax.y, f2TexMin.y, 0.5f))
						}; // center
					}
					else
					{
						pIndices[iCurIdx++] = iStopIdx;
						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx - 1;
					}
					pVertices[iCurVtx++] = {
						float3_t(fXradius * sinf(angle) + fOX, fYradius * -cosf(angle) + fOY, 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, sinf(angle) * 0.5f + 0.5f), -lerpf(f2TexMax.y, f2TexMin.y, -cosf(angle) * 0.5f + 0.5f))
					}; // circle
					//a=0 x=0 y=-1
				}
			}
			else
			{
				//iVC = (m_iNumSegs + segs + m_iNumSegs) * 2 + segs;
				//iIC = (segs - 2) * 3 + segs * 6;

				for(float angleEnd = angle + SM_2PI, angleStep = SM_2PI / (float)segs;
					angle < angleEnd;
					angle += angleStep, ++i)
				{
					stop = i % (segs / m_iNumSegs) == 0;
					if(stop)
					{
						if(i != 0)
						{
							pIndices[iCurIdx++] = iCurVtx - 2;
							pIndices[iCurIdx++] = iCurVtx;
							pIndices[iCurIdx++] = iCurVtx - 1;

							pIndices[iCurIdx++] = iCurVtx;
							pIndices[iCurIdx++] = iCurVtx + 1;
							pIndices[iCurIdx++] = iCurVtx - 1;

							pVertices[iCurVtx++] = {
								float3_t(fXFixedRadius * sinf(angle) + fOX, fYFixedRadius * -cosf(angle) + fOY, 0.0f),
								float2_t(lerpf(f2TexMin.x, f2TexMax.x, (fTexFRX * sinf(angle) * 0.5f + 0.5f)), -lerpf(f2TexMax.y, f2TexMin.y, (fTexFRY * -cosf(angle) * 0.5f + 0.5f)))
							}; // circle inner
							pVertices[iCurVtx++] = {
								float3_t(fXradius * sinf(angle) + fOX, fYradius * -cosf(angle) + fOY, 0.0f),
								float2_t(lerpf(f2TexMin.x, f2TexMax.x, sinf(angle) * 0.5f + 0.5f), -lerpf(f2TexMax.y, f2TexMin.y, -cosf(angle) * 0.5f + 0.5f))
							}; // circle
						}

						float avgAngle = angle + (SM_2PI / (float)m_iNumSegs) * 0.5f;
						fOX = sinf(avgAngle) * fXoffset;
						fOY = -cosf(avgAngle) * fYoffset;

						/*pVertices[iCurVtx++] = {
							float3_t(fOX, fOY, 0.0f),
							float2_t(lerpf(f2TexMin.x, f2TexMax.x, 0.5f), -lerpf(f2TexMax.y, f2TexMin.y, 0.5f))
						}; // center
						*/
					}
					else
					{
						pIndices[iCurIdx++] = iCurVtx - 2;
						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx - 1;

						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx + 1;
						pIndices[iCurIdx++] = iCurVtx - 1;
					}
					pVertices[iCurVtx++] = {
						float3_t(fXFixedRadius * sinf(angle) + fOX, fYFixedRadius * -cosf(angle) + fOY, 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, (fTexFRX * sinf(angle) * 0.5f + 0.5f)), -lerpf(f2TexMax.y, f2TexMin.y, (fTexFRY * -cosf(angle) * 0.5f + 0.5f)))
					}; // circle inner
					pVertices[iCurVtx++] = {
						float3_t(fXradius * sinf(angle) + fOX, fYradius * -cosf(angle) + fOY, 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, sinf(angle) * 0.5f + 0.5f), -lerpf(f2TexMax.y, f2TexMin.y, -cosf(angle) * 0.5f + 0.5f))
					}; // circle
					//a=0 x=0 y=-1
				}

				i = 0;
				iStopIdx = iCurVtx;
				for(float angleEnd = angle + SM_2PI, angleStep = SM_2PI / (float)segs;
					angle < angleEnd;
					angle += angleStep, ++i)
				{
					if(i > 1)
					{
						pIndices[iCurIdx++] = iStopIdx;
						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx - 1;
					}
					pVertices[iCurVtx++] = {
						float3_t(fXFixedRadius * sinf(angle), fYFixedRadius * -cosf(angle), 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, (fTexFRX * sinf(angle) * 0.5f + 0.5f)), -lerpf(f2TexMax.y, f2TexMin.y, (fTexFRY * -cosf(angle) * 0.5f + 0.5f)))
					}; // circle inner
				}
			}
		}
		else if(m_style == SCALED)
		{
			if(m_fFixedRadius == 0.0f)
			{
				//centered
				//iVC = segs;
				//iIC = (segs - 2) * 3;

				iStopIdx = iCurVtx;
				for(float angleEnd = angle + SM_2PI, angleStep = SM_2PI / (float)segs;
					angle < angleEnd;
					angle += angleStep, ++i)
				{
					if(i > 1)
					{
						pIndices[iCurIdx++] = iStopIdx;
						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx - 1;
					}
					pVertices[iCurVtx++] = {
						float3_t((fXradius + fXoffset) * sinf(angle), (fYradius + fYoffset) * -cosf(angle), 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, sinf(angle) * 0.5f + 0.5f), -lerpf(f2TexMax.y, f2TexMin.y, -cosf(angle) * 0.5f + 0.5f))
					}; // circle
				}

			}
			else
			{
				//iVC = segs * 2;
				//iIC = (segs - 2) * 3 + segs * 2;

				for(float angleEnd = angle + SM_2PI, angleStep = SM_2PI / (float)segs;
					angle < angleEnd;
					angle += angleStep, ++i)
				{
					//if(i > 1)
					{
						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx + 2;
						pIndices[iCurIdx++] = iCurVtx + 1;

						pIndices[iCurIdx++] = iCurVtx + 1;
						pIndices[iCurIdx++] = iCurVtx + 2;
						pIndices[iCurIdx++] = iCurVtx + 3;
					}


					pVertices[iCurVtx++] = {
						float3_t(fXFixedRadius * sinf(angle) + fOX, fYFixedRadius * -cosf(angle) + fOY, 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, (fTexFRX * sinf(angle) * 0.5f + 0.5f)), -lerpf(f2TexMax.y, f2TexMin.y, (fTexFRY * -cosf(angle) * 0.5f + 0.5f)))
					}; // circle inner
					pVertices[iCurVtx++] = {
						float3_t((fXradius + fXoffset) * sinf(angle), (fYradius + fYoffset) * -cosf(angle), 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, sinf(angle) * 0.5f + 0.5f), -lerpf(f2TexMax.y, f2TexMin.y, -cosf(angle) * 0.5f + 0.5f))
					}; // circle
				}

				i = 0;
				iStopIdx = iCurVtx;
				for(float angleEnd = angle + SM_2PI, angleStep = SM_2PI / (float)segs;
					angle < angleEnd;
					angle += angleStep, ++i)
				{
					if(i > 1)
					{
						pIndices[iCurIdx++] = iStopIdx;
						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx - 1;
					}
					pVertices[iCurVtx++] = {
						float3_t(fXFixedRadius * sinf(angle), fYFixedRadius * -cosf(angle), 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, (fTexFRX * sinf(angle) * 0.5f + 0.5f)), -lerpf(f2TexMax.y, f2TexMin.y, (fTexFRY * -cosf(angle) * 0.5f + 0.5f)))
					}; // circle inner
				}
			}
		}

		m_iVertexCount[m_u8ActiveBuffer ? 0 : 1] = iCurVtx;
		m_iIndexCount[m_u8ActiveBuffer ? 0 : 1] = iCurIdx;
	}
}
void CCrosshair::render()
{
	if(m_bHidden || !m_pTexture)
	{
		return;
	}
	if(m_bBuildBuff)
	{
		m_bBuildBuff = false;
		//update buffer
		void * pData;
		if(!FAILED(m_pVertexBuffer->Lock(0, sizeof(Vertex) * m_iVertexCount[m_u8ActiveBuffer], &pData, 0)))
		{
			memcpy(pData, m_pVertices[m_u8ActiveBuffer], sizeof(Vertex)* m_iVertexCount[m_u8ActiveBuffer]);
			m_pVertexBuffer->Unlock();
		}

		if(!FAILED(m_pIndexBuffer->Lock(0, sizeof(UINT) * m_iIndexCount[m_u8ActiveBuffer], &pData, 0)))
		{
			memcpy(pData, m_pIndices[m_u8ActiveBuffer], sizeof(UINT)* m_iIndexCount[m_u8ActiveBuffer]);
			m_pIndexBuffer->Unlock();
		}
	}
	m_pDev->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&SMMatrixIdentity());
	m_pDev->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&SMMatrixIdentity());
	m_pDev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&SMMatrixIdentity());
	m_pDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDev->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
	m_pDev->SetIndices(m_pIndexBuffer);
	m_pDev->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	m_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	SGCore_ShaderUnBind();
	m_pDev->SetTexture(0, m_pTexture);
	m_pDev->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	// Использовать альфа-канал в качестве источника альфа-компонент
	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// Устанавливаем коэффициенты смешивания таким образом,
	// чтобы альфа-компонента определяла прозрачность
	m_pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	//m_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	//m_pDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	//m_pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
	
	//SGCore_ScreenQuadDraw();
	SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount[m_u8ActiveBuffer], 0, m_iIndexCount[m_u8ActiveBuffer] / 3);
	//render
}
void CCrosshair::onSync()
{
	if(m_bDirty)
	{
		m_bDirty = false;

		//swap buffers
		m_u8ActiveBuffer = m_u8ActiveBuffer ? 0 : 1;

		m_bBuildBuff = true;
	}
}

void CCrosshair::enable(bool en)
{
	m_bHidden = !en;
}

void CCrosshair::setNumSegmens(int num)
{
	if(num < 1)
	{
		num = 1;
	}
	else if(num > CROSSHAIR_MAXSEGS)
	{
		num = CROSSHAIR_MAXSEGS;
	}
	if(m_iNumSegs != num)
	{
		m_bDirty = true;
		m_iNumSegs = num;
	}
}
void CCrosshair::setAngle(float a)
{
	if(m_fAngle != a)
	{
		m_bDirty = true;
		m_fAngle = a;
	}
}
void CCrosshair::setFixedRadius(float r)
{
	if(m_fFixedRadius != r)
	{
		m_bDirty = true;
		m_fFixedRadius = r;
	}
}
void CCrosshair::setStyle(STYLE style)
{
	if(m_style != style)
	{
		m_bDirty = true;
		m_style = style;
	}
}
void CCrosshair::setMaxSize(float size)
{
	m_bDirty = true;
	m_fMaxSize = size;
}
void CCrosshair::setTexture(ID id)
{
	if(m_idTexture != id)
	{
		m_bDirty = true;
		m_idTexture = id;
		if(m_idTexture == -1)
		{
			m_pTexture = NULL;
		}
		else
		{
			m_pTexture = SGCore_LoadTexGetTex(m_idTexture);
		}
	}
}

void CCrosshair::setSize(float size)
{
	if(m_fSize != size)
	{
		m_bDirty = true;
		m_fSize = size;
	}
}