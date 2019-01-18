#ifndef _GRAPHIX_H_
#define _GRAPHIX_H_

#include <common/sxtypes.h>
#include <common/SXMath.h>

typedef DWORD GXCOLOR;

enum GXBUFFERLOCK
{
	GXBL_READ,
	GXBL_WRITE
};

enum GXTEXLOCK
{
	GXTL_READ,
	GXTL_WRITE
};

enum
{
	GX_BUFFER_USAGE_STATIC = 0x01, // данные будут очень редко обновляться
	GX_BUFFER_USAGE_DYNAMIC = 0x02, // данные будут обновляться, но не каждый кадр
	GX_BUFFER_USAGE_STREAM = 0x04, // данные будут обновляться каждый кадр
	GX_BUFFER_WRITEONLY = 0x08
};


#define GXDECL_END() {0xFF,0,GXDECLTYPE_UNUSED,(GXDECLUSAGE)0, GXDECLSPEC_PER_VERTEX_DATA}


typedef enum _GXDECLTYPE
{
	GXDECLTYPE_FLOAT1 = 0,  // 1D float expanded to (value, 0., 0., 1.)
	GXDECLTYPE_FLOAT2 = 1,  // 2D float expanded to (value, value, 0., 1.)
	GXDECLTYPE_FLOAT3 = 2,  // 3D float expanded to (value, value, value, 1.)
	GXDECLTYPE_FLOAT4 = 3,  // 4D float
	GXDECLTYPE_GXCOLOR = 4,  // 4D packed unsigned bytes mapped to 0. to 1. range
	// Input is in GXCOLOR format (ARGB) expanded to (R, G, B, A)
	GXDECLTYPE_UBYTE4 = 5,  // 4D unsigned byte
	GXDECLTYPE_SHORT2 = 6,  // 2D signed short expanded to (value, value, 0., 1.)
	GXDECLTYPE_SHORT4 = 7,  // 4D signed short

	// The following types are valid only with vertex shaders >= 2.0


	GXDECLTYPE_UBYTE4N = 8,  // Each of 4 bytes is normalized by dividing to 255.0
	GXDECLTYPE_SHORT2N = 9,  // 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
	GXDECLTYPE_SHORT4N = 10,  // 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
	GXDECLTYPE_USHORT2N = 11,  // 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
	GXDECLTYPE_USHORT4N = 12,  // 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
	GXDECLTYPE_UDEC3 = 13,  // 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
	GXDECLTYPE_DEC3N = 14,  // 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
	GXDECLTYPE_FLOAT16_2 = 15,  // Two 16-bit floating point values, expanded to (value, value, 0, 1)
	GXDECLTYPE_FLOAT16_4 = 16,  // Four 16-bit floating point values
	GXDECLTYPE_UNUSED = 17,  // When the type field in a decl is unused.
} GXDECLTYPE;



typedef enum _GXDECLSPEC
{
	GXDECLSPEC_PER_VERTEX_DATA = 0,	 // Input data is per-vertex data.
	GXDECLSPEC_PER_INSTANCE_DATA // Input data is per-instance data.
} GXDECLSPEC;

// Vertex element semantics
//
typedef enum _GXDECLUSAGE
{
	GXDECLUSAGE_POSITION = 0,
	GXDECLUSAGE_TEXCOORD,
	GXDECLUSAGE_NORMAL,
	GXDECLUSAGE_BLENDWEIGHT,
	GXDECLUSAGE_BLENDINDICES,
	GXDECLUSAGE_COLOR,
	GXDECLUSAGE_TANGENT,
	GXDECLUSAGE_BINORMAL,
	GXDECLUSAGE_TESSFACTOR,
	GXDECLUSAGE_TEXCOORD1,
	GXDECLUSAGE_TEXCOORD2,
	GXDECLUSAGE_TEXCOORD3,
	GXDECLUSAGE_TEXCOORD4,
	GXDECLUSAGE_TEXCOORD5,
	GXDECLUSAGE_TEXCOORD6,
	GXDECLUSAGE_TEXCOORD7,
} GXDECLUSAGE;

#define GXDECLLIST { \
	"POSITION",      \
	"TEXCOORD",      \
	"NORMAL",        \
	"BLENDWEIGHT",   \
	"BLENDINDICES",  \
	"COLOR",         \
	"TANGENT",       \
	"BINORMAL",      \
	"TESSFACTOR",    \
	"TEXCOORD1",     \
	"TEXCOORD2",     \
	"TEXCOORD3",     \
	"TEXCOORD4",     \
	"TEXCOORD5",     \
	"TEXCOORD6",     \
	"TEXCOORD7"      \
}

typedef struct _GXVERTEXELEMENT
{
	WORD Stream;     // Stream index
	WORD Offset;     // Offset in the stream in bytes
	GXDECLTYPE Type;       // Data type
	GXDECLUSAGE Usage;      // Semantics
	GXDECLSPEC spec; // attribute instancing spec
} GXVERTEXELEMENT;

#define MAXGXVSTREAM 16

#define GX_TEXUSAGE_RENDERTARGET 0x00000001
#define GX_TEXUSAGE_AUTOGENMIPMAPS 0x00000002
#define GX_TEXUSAGE_AUTORESIZE 0x00000004

enum GXPT
{
	GXPT_UNDEFINED = 0,
	GXPT_POINTLIST = 1,
	GXPT_LINELIST = 2,
	GXPT_LINESTRIP = 3,
	GXPT_TRIANGLELIST = 4,
	GXPT_TRIANGLESTRIP = 5/*,
	GXPT_TRIANGLEFAN = 6*/
};

enum GXINDEXTYPE
{
	GXIT_USHORT,
	GXIT_UINT
};

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif

// maps unsigned 8 bits/channel to D3DCOLOR
#define GXCOLOR_ARGB(a,r,g,b) \
    ((GXCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define GXCOLOR_RGBA(r,g,b,a) GXCOLOR_ARGB(a,r,g,b)
#define GXCOLOR_XRGB(r,g,b)   GXCOLOR_ARGB(0xff,r,g,b)
							  
#define GXCOLOR_XYUV(y,u,v)   GXCOLOR_ARGB(0xff,y,u,v)
#define GXCOLOR_AYUV(a,y,u,v) GXCOLOR_ARGB(a,y,u,v)

// maps floating point channels (0.f to 1.f range) to D3DCOLOR
#define GXCOLOR_COLORVALUE(r,g,b,a) \
    GXCOLOR_RGBA((DWORD)((r)*255.f),(DWORD)((g)*255.f),(DWORD)((b)*255.f),(DWORD)((a)*255.f))

typedef enum _GXFORMAT
{
	GXFMT_UNKNOWN = 0,

	GXFMT_R8G8B8 = 20,
	GXFMT_A8R8G8B8 = 21,
	GXFMT_X8R8G8B8 = 22,
	GXFMT_R5G6B5 = 23,
	//GXFMT_X1R5G5B5 = 24,
	//GXFMT_A1R5G5B5 = 25,
	//GXFMT_A4R4G4B4 = 26,
	//GXFMT_R3G3B2 = 27,
	//GXFMT_A8 = 28,
	//GXFMT_A8R3G3B2 = 29,
	//GXFMT_X4R4G4B4 = 30,
	//GXFMT_A2B10G10R10 = 31,
	//GXFMT_A8B8G8R8 = 32,
	//GXFMT_X8B8G8R8 = 33,
	//GXFMT_G16R16 = 34,
	//GXFMT_A2R10G10B10 = 35,
	GXFMT_A16B16G16R16 = 36,

	//GXFMT_A8P8 = 40,
	//GXFMT_P8 = 41,

	//GXFMT_L8 = 50,
	//GXFMT_A8L8 = 51,
	//GXFMT_A4L4 = 52,

	//GXFMT_V8U8 = 60,
	//GXFMT_L6V5U5 = 61,
	//GXFMT_X8L8V8U8 = 62,
	//GXFMT_Q8W8V8U8 = 63,
	//GXFMT_V16U16 = 64,
	//GXFMT_A2W10V10U10 = 67,

	//GXFMT_UYVY = MAKEFOURCC('U', 'Y', 'V', 'Y'),
	//GXFMT_R8G8_B8G8 = MAKEFOURCC('R', 'G', 'B', 'G'),
	//GXFMT_YUY2 = MAKEFOURCC('Y', 'U', 'Y', '2'),
	//GXFMT_G8R8_G8B8 = MAKEFOURCC('G', 'R', 'G', 'B'),
	GXFMT_DXT1 = MAKEFOURCC('D', 'X', 'T', '1'),
	GXFMT_DXT2 = MAKEFOURCC('D', 'X', 'T', '2'),
	GXFMT_DXT3 = MAKEFOURCC('D', 'X', 'T', '3'),
	GXFMT_DXT4 = MAKEFOURCC('D', 'X', 'T', '4'),
	GXFMT_DXT5 = MAKEFOURCC('D', 'X', 'T', '5'),

	//GXFMT_D16_LOCKABLE = 70,
	GXFMT_D32 = 71,
	//GXFMT_D15S1 = 73,
	GXFMT_D24S8 = 75,
	GXFMT_D24X8 = 77,
	//GXFMT_D24X4S4 = 79,
	GXFMT_D16 = 80,

	//GXFMT_D32F_LOCKABLE = 82,
	//GXFMT_D24FS8 = 83,

	//GXFMT_L16 = 81,

	//GXFMT_VERTEXDATA = 100,
	//GXFMT_INDEX16 = 101,
	//GXFMT_INDEX32 = 102,

	//GXFMT_Q16W16V16U16 = 110,

	//gxFMT_MULTI2_ARGB8 = MAKEFOURCC('M', 'E', 'T', '1'),

	// Floating point surface formats

	// s10e5 formats (16-bits per channel)
	GXFMT_R16F = 111,
	GXFMT_G16R16F = 112,
	GXFMT_A16B16G16R16F = 113,

	// IEEE s23e8 formats (32-bits per channel)
	GXFMT_R32F = 114,
	GXFMT_G32R32F = 115,
	GXFMT_A32B32G32R32F = 116,

	//GXFMT_CxV8U8 = 117,
	
	GXFMT_FORCE_DWORD = 0x7fffffff
} GXFORMAT;

/* Multi-Sample buffer types */
typedef enum _GXMULTISAMPLE_TYPE
{
	GXMULTISAMPLE_NONE = 0,
	GXMULTISAMPLE_NONMASKABLE = 1,
	GXMULTISAMPLE_2_SAMPLES = 2,
	GXMULTISAMPLE_3_SAMPLES = 3,
	GXMULTISAMPLE_4_SAMPLES = 4,
	GXMULTISAMPLE_5_SAMPLES = 5,
	GXMULTISAMPLE_6_SAMPLES = 6,
	GXMULTISAMPLE_7_SAMPLES = 7,
	GXMULTISAMPLE_8_SAMPLES = 8,
	GXMULTISAMPLE_9_SAMPLES = 9,
	GXMULTISAMPLE_10_SAMPLES = 10,
	GXMULTISAMPLE_11_SAMPLES = 11,
	GXMULTISAMPLE_12_SAMPLES = 12,
	GXMULTISAMPLE_13_SAMPLES = 13,
	GXMULTISAMPLE_14_SAMPLES = 14,
	GXMULTISAMPLE_15_SAMPLES = 15,
	GXMULTISAMPLE_16_SAMPLES = 16,

	GXMULTISAMPLE_FORCE_DWORD = 0x7fffffff
} GXMULTISAMPLE_TYPE;

/* CubeMap Face identifiers */
typedef enum _GXCUBEMAP_FACES
{
	GXCUBEMAP_FACE_POSITIVE_X = 0,
	GXCUBEMAP_FACE_NEGATIVE_X = 1,
	GXCUBEMAP_FACE_POSITIVE_Y = 2,
	GXCUBEMAP_FACE_NEGATIVE_Y = 3,
	GXCUBEMAP_FACE_POSITIVE_Z = 4,
	GXCUBEMAP_FACE_NEGATIVE_Z = 5,

	GXCUBEMAP_FACE_FORCE_DWORD = 0x7fffffff
} GXCUBEMAP_FACES;


typedef enum _GXBLEND
{
	GXBLEND_ZERO = 1,
	GXBLEND_ONE = 2,
	GXBLEND_SRC_COLOR = 3,
	GXBLEND_INV_SRC_COLOR = 4,
	GXBLEND_SRC_ALPHA = 5,
	GXBLEND_INV_SRC_ALPHA = 6,
	GXBLEND_DEST_ALPHA = 7,
	GXBLEND_INV_DEST_ALPHA = 8,
	GXBLEND_DEST_COLOR = 9,
	GXBLEND_INV_DEST_COLOR = 10,
	GXBLEND_SRC_ALPHA_SAT = 11,
	GXBLEND_BLEND_FACTOR = 14,
	GXBLEND_INV_BLEND_FACTOR = 15,
	GXBLEND_SRC1_COLOR = 16,
	GXBLEND_INV_SRC1_COLOR = 17,
	GXBLEND_SRC1_ALPHA = 18,
	GXBLEND_INV_SRC1_ALPHA = 19
} 	GXBLEND;

typedef enum _GXBLEND_OP
{
	GXBLEND_OP_ADD = 1,
	GXBLEND_OP_SUBTRACT = 2,
	GXBLEND_OP_REV_SUBTRACT = 3,
	GXBLEND_OP_MIN = 4,
	GXBLEND_OP_MAX = 5
} 	GXBLEND_OP;

typedef enum _GXCOLOR_WRITE_ENABLE
{
	GXCOLOR_WRITE_ENABLE_RED = 1,
	GXCOLOR_WRITE_ENABLE_GREEN = 2,
	GXCOLOR_WRITE_ENABLE_BLUE = 4,
	GXCOLOR_WRITE_ENABLE_ALPHA = 8,
	GXCOLOR_WRITE_ENABLE_ALL = (((GXCOLOR_WRITE_ENABLE_RED | GXCOLOR_WRITE_ENABLE_GREEN) | GXCOLOR_WRITE_ENABLE_BLUE) | GXCOLOR_WRITE_ENABLE_ALPHA)
} 	GXCOLOR_WRITE_ENABLE;

typedef struct _GXBLEND_DESC
{
	BOOL bAlphaToCoverageEnable;
	BOOL bIndependentBlendEnabled;
	struct GXBLEND_RT_DESC
	{
		BOOL bBlendEnable;
		GXBLEND srcBlend;
		GXBLEND destBlend;
		GXBLEND_OP blendOp;
		GXBLEND srcBlendAlpha;
		GXBLEND destBlendAlpha;
		GXBLEND_OP blendOpAlpha;
		byte u8RenderTargetWriteMask;
	} renderTarget[8];
} GXBLEND_DESC;

typedef enum _GXCOMPARISON_FUNC
{
	GXCOMPARISON_NEVER,
	GXCOMPARISON_LESS,
	GXCOMPARISON_EQUAL,
	GXCOMPARISON_LESS_EQUAL,
	GXCOMPARISON_GREATER,
	GXCOMPARISON_NOT_EQUAL,
	GXCOMPARISON_GREATER_EQUAL,
	GXCOMPARISON_ALWAYS
} GXCOMPARISON_FUNC;

typedef enum _GXSTENCIL_OP
{
	GXSTENCIL_OP_KEEP,
	GXSTENCIL_OP_ZERO,
	GXSTENCIL_OP_REPLACE,
	GXSTENCIL_OP_INCR_SAT,
	GXSTENCIL_OP_DECR_SAT,
	GXSTENCIL_OP_INVERT,
	GXSTENCIL_OP_INCR,
	GXSTENCIL_OP_DECR
} GXSTENCIL_OP;

typedef struct _GXDEPTH_STENCILOP_DESC
{
	GXSTENCIL_OP stencilFailOp;
	GXSTENCIL_OP stencilDepthFailOp;
	GXSTENCIL_OP stencilPassOp;
	GXCOMPARISON_FUNC stencilFunc;
} GXDEPTH_STENCILOP_DESC;

typedef struct _GXDEPTH_STENCIL_DESC
{
	BOOL bDepthEnable;
	BOOL bEnableDepthWrite;
	GXCOMPARISON_FUNC depthFunc;
	BOOL bStencilEnable;
	byte u8StencilReadMask;
	byte u8StencilWriteMask;
	GXDEPTH_STENCILOP_DESC frontFace;
	GXDEPTH_STENCILOP_DESC backFace;
} GXDEPTH_STENCIL_DESC;

typedef enum _GXFILL_MODE
{
	GXFILL_WIREFRAME,
	GXFILL_SOLID
} GXFILL_MODE;

typedef enum _GXCULL_MODE
{
	GXCULL_NONE,  // Always draw all triangles.
	GXCULL_FRONT, // Do not draw triangles that are front - facing
	GXCULL_BACK	  // Do not draw triangles that are back - facing.
} GXCULL_MODE;

typedef struct _GXRASTERIZER_DESC
{
	GXFILL_MODE fillMode;
	GXCULL_MODE cullMode;
	BOOL bFrontCounterClockwise;
	int iDepthBias;
	float fDepthBiasClamp;
	float fSlopeScaledDepthBias;
	BOOL bDepthClipEnable;
	BOOL bScissorEnable;
	BOOL bMultisampleEnable;
	BOOL bAntialiasedLineEnable;
} GXRASTERIZER_DESC;

typedef enum _GXFILTER
{
	GXFILTER_MIN_MAG_MIP_POINT,
	GXFILTER_MIN_MAG_POINT_MIP_LINEAR,
	GXFILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
	GXFILTER_MIN_POINT_MAG_MIP_LINEAR,
	GXFILTER_MIN_LINEAR_MAG_MIP_POINT,
	GXFILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	GXFILTER_MIN_MAG_LINEAR_MIP_POINT,
	GXFILTER_MIN_MAG_MIP_LINEAR,
	GXFILTER_ANISOTROPIC
} GXFILTER;

typedef enum _GXTEXTURE_ADDRESS_MODE
{
	GXTEXTURE_ADDRESS_WRAP,
	GXTEXTURE_ADDRESS_MIRROR,
	GXTEXTURE_ADDRESS_CLAMP,
	GXTEXTURE_ADDRESS_BORDER,
	GXTEXTURE_ADDRESS_MIRROR_ONCE
} GXTEXTURE_ADDRESS_MODE;

typedef struct _GXSAMPLER_DESC
{
	GXFILTER filter;
	GXTEXTURE_ADDRESS_MODE addressU;
	GXTEXTURE_ADDRESS_MODE addressV;
	GXTEXTURE_ADDRESS_MODE addressW;
	float fMipLODBias;
	UINT uMaxAnisotropy;
	GXCOMPARISON_FUNC comparisonFunc;
	float fBorderColor[4];
	float fMinLOD;
	float fMaxLOD;
} GXSAMPLER_DESC;

typedef struct _GXMACRO
{
	const char *szName;
	const char *szDefinition;

} GXMACRO;


//##########################################################################

class IGXBaseInterface
{
protected:
	virtual ~IGXBaseInterface(){}
public:
	virtual void Release() = 0;
};

class IGXIndexBuffer: public IGXBaseInterface
{
public:
	virtual bool lock(void **ppData, GXBUFFERLOCK mode) = 0;
	virtual void unlock() = 0;
};

class IGXVertexBuffer: public IGXBaseInterface
{
public:
	virtual bool lock(void **ppData, GXBUFFERLOCK mode) = 0;
	virtual void unlock() = 0;
};

class IGXVertexDeclaration: public IGXBaseInterface
{
};

class IGXRenderBuffer: public IGXBaseInterface
{
};

class IGXVertexShader: public IGXBaseInterface
{
public:
	virtual void setConstantF(UINT uStartRegister, const float *pConstantData, UINT uVector4fCount) = 0;
	virtual void setConstantI(UINT uStartRegister, const int *pConstantData, UINT uVector4iCount) = 0;
	virtual UINT getConstantLocation(const char *szConstName) = 0;
};

class IGXPixelShader: public IGXBaseInterface
{
public:
	virtual void setConstantF(UINT uStartRegister, const float *pConstantData, UINT uVector4fCount) = 0;
	virtual void setConstantI(UINT uStartRegister, const int *pConstantData, UINT uVector4iCount) = 0;
	virtual UINT getConstantLocation(const char *szConstName) = 0;
};

class IGXShader: public IGXBaseInterface
{
	virtual IGXPixelShader *getPixelShader() = 0;
	virtual IGXVertexShader *getVertexShader() = 0;

	virtual void setPixelShader(IGXPixelShader *pShader) = 0;
	virtual void setVertexShader(IGXVertexShader *pShader) = 0;
};

class IGXDepthStencilSurface: public IGXBaseInterface
{
};

class IGXSurface: public IGXBaseInterface
{
public:
	virtual UINT getWidth() = 0;
	virtual UINT getHeight() = 0;
	virtual GXFORMAT getFormat() = 0;
};

class IGXBaseTexture: public IGXBaseInterface
{
public:
	virtual GXFORMAT getFormat() = 0;
};

class IGXTexture2D: public IGXBaseTexture
{
public:
	virtual IGXSurface *getMipmap(UINT i = 0) = 0;

	virtual UINT getWidth() = 0;
	virtual UINT getHeight() = 0;

	virtual bool lock(void **ppData, GXTEXLOCK mode) = 0;
	virtual void unlock() = 0;
};

class IGXTextureCube: public IGXBaseTexture
{
public:
	virtual IGXSurface *getMipmap(GXCUBEMAP_FACES face, UINT i = 0) = 0;

	virtual UINT getSize() = 0;

	virtual bool lock(void **ppData, GXCUBEMAP_FACES face, GXTEXLOCK mode) = 0;
	virtual void unlock() = 0;
};

class IGXBlendState: public IGXBaseInterface
{
};

class IGXDepthStencilState: public IGXBaseInterface
{
};

class IGXRasterizerState: public IGXBaseInterface
{
};

class IGXSamplerState: public IGXBaseInterface
{
};

//class IGXRenderTarget
//{
//public:
//	virtual void Release() = 0;
//
//	virtual IGXDepthStencilSurface *getDepthStencilSurface() = 0;
//	virtual IGXSurface *getColorSurface(UINT i);
//};


//##########################################################################

#define bAllowDiscard bAllowDiscard /* данные могут быть потеряны, например при потере/восстановлении устройства в dx9 */
class IGXContext
{
public:
	virtual BOOL initContext(SXWINDOW wnd, int iWidth, int iHeight, bool isWindowed) = 0;
	virtual ~IGXContext()
	{
	};

	virtual void resize(int iWidth, int iHeight, bool isWindowed) = 0;

	virtual void swapBuffers() = 0;

	virtual void beginFrame() = 0;
	virtual void endFrame() = 0;
	
	virtual bool wasReset() = 0; // могли ли быть утеряны данные в буферах и текстурах, для которых это допустимо?

	virtual void setClearColor(const float4_t & color) = 0;
	virtual void clearTarget() = 0;
	virtual void clearDepth(float val = 1.0f) = 0;
	virtual void clearStencil(UINT val = 0) = 0;

	virtual IGXVertexBuffer * createVertexBuffer(size_t size, UINT flags, void * pInitData = NULL, bool bAllowDiscard = false) = 0;
	virtual IGXIndexBuffer * createIndexBuffer(size_t size, UINT flags, GXINDEXTYPE it, void * pInitData = NULL, bool bAllowDiscard = false) = 0;


	virtual void destroyIndexBuffer(IGXIndexBuffer * pBuff) = 0;
	virtual void destroyVertexBuffer(IGXVertexBuffer * pBuff) = 0;

	virtual void setIndexBuffer(IGXIndexBuffer * pBuff) = 0;
	virtual void setRenderBuffer(IGXRenderBuffer * pBuff) = 0;
	//virtual void setVertexBuffers(UINT startSlot, UINT countSlots, IDSRvertexBuffer ** pBuff) = 0;

	virtual IGXVertexDeclaration * createVertexDeclaration(const GXVERTEXELEMENT * pDecl) = 0;
	virtual void destroyVertexDeclaration(IGXVertexDeclaration * pDecl) = 0;

	//virtual void IASetInputLayout(IGXvertexDeclaration * pDecl) = 0;
	virtual void setPrimitiveTopology(GXPT pt) = 0;

	virtual void drawIndexed(UINT uVertexCount, UINT uPrimitiveCount, UINT uStartIndexLocation, int iBaseVertexLocation) = 0;
	virtual void drawIndexedInstanced(UINT uInstanceCount, UINT uVertexCount, UINT uPrimitiveCount, UINT uStartIndexLocation, int iBaseVertexLocation) = 0;
	virtual void drawPrimitive(UINT uStartVertex, UINT uPrimitiveCount) = 0;
	virtual void drawPrimitiveInstanced(UINT uInstanceCount, UINT uStartVertex, UINT uPrimitiveCount) = 0;
	
	// https://github.com/LukasBanana/XShaderCompiler/releases
	// https://github.com/Thekla/hlslparser/tree/master/src
	virtual IGXVertexShader * createVertexShader(const char * szFile, GXMACRO *pDefs = NULL) = 0;
	virtual IGXVertexShader * createVertexShader(void *pData, UINT uSize) = 0;
	virtual void destroyVertexShader(IGXVertexShader * pSH) = 0;

	virtual IGXPixelShader * createPixelShader(const char * szFile, GXMACRO *pDefs = NULL) = 0;
	virtual IGXPixelShader * createPixelShader(void *pData, UINT uSize) = 0;
	virtual void destroyPixelShader(IGXPixelShader * pSH) = 0;

	// virtual void setVertexShader(IGXVertexShader * pSH) = 0;
	// virtual void setPixelShader(IGXPixelShader * pSH) = 0;

	virtual IGXShader *createShader(IGXVertexShader *pVS = NULL, IGXPixelShader *pPS = NULL) = 0;
	virtual void destroyShader(IGXShader *pSH) = 0;
	virtual void setShader(IGXShader *pSH) = 0;
	virtual IGXShader *getShader() = 0;

	virtual IGXRenderBuffer * createRenderBuffer(UINT countSlots, IGXVertexBuffer ** ppBuff, IGXVertexDeclaration * pDecl) = 0;
	virtual void destroyRenderBuffer(IGXRenderBuffer * pDecl) = 0;

	virtual IGXDepthStencilSurface *createDepthStencilSurface(UINT uWidth, UINT uHeight, GXFORMAT format, GXMULTISAMPLE_TYPE) = 0;
	virtual void destroyDepthStencilSurface(IGXDepthStencilSurface *pSurface) = 0;
	virtual void setDepthStencilSurface(IGXDepthStencilSurface *pSurface) = 0;
	virtual IGXDepthStencilSurface *getDepthStencilSurface() = 0;

	virtual void setColorTarget(IGXSurface *pSurf, UINT idx = 0) = 0;
	virtual IGXSurface *getColorTarget(UINT idx = 0) = 0;

	virtual IGXTexture2D *createTexture2D(UINT uWidth, UINT uHeight, UINT uMipLevels, UINT uTexUsageFlags, GXFORMAT format, void * pInitData = NULL, bool bAllowDiscard = false) = 0;
	virtual IGXTextureCube *createTextureCube(UINT uSize, UINT uMipLevels, UINT uTexUsageFlags, GXFORMAT format, void * pInitData = NULL, bool bAllowDiscard = false) = 0;
	virtual void destroyTexture2D(IGXTexture2D * pTexture) = 0;
	virtual void destroyTextureCube(IGXTextureCube * pTexture) = 0;

	virtual IGXTexture2D *createTexture2DFromFile(const char *szFileName, bool bAllowNonPowerOf2 = false, bool bAllowDiscard = false) = 0;
	virtual IGXTextureCube *createTextureCubeFromFile(const char *szFileName, bool bAllowNonPowerOf2 = false, bool bAllowDiscard = false) = 0;

	virtual void setTexture(IGXBaseTexture *pTexture, UINT uStage = 0) = 0;
	virtual IGXBaseTexture *getTexture(UINT uStage = 0) = 0;

	virtual IGXBlendState *createBlendState(GXBLEND_DESC *pBlendDesc) = 0;
	virtual void destroyBlendState(IGXBlendState *pState) = 0;
	virtual void setBlendState(IGXBlendState *pState) = 0;
	virtual IGXBlendState *getBlendState() = 0;

	virtual IGXDepthStencilState *createDepthStencilState(GXDEPTH_STENCIL_DESC *pDSDesc) = 0;
	virtual void destroyDepthStencilState(IGXDepthStencilState *pState) = 0;
	virtual void setDepthStencilState(IGXDepthStencilState *pState) = 0;
	virtual IGXDepthStencilState *getDepthStencilState() = 0;

	virtual IGXRasterizerState *createRasterizerState(GXRASTERIZER_DESC *pDSDesc) = 0;
	virtual void destroyRasterizerState(IGXRasterizerState *pState) = 0;
	virtual void setRasterizerState(IGXRasterizerState *pState) = 0;
	virtual IGXRasterizerState *getRasterizerState() = 0;

	virtual IGXSamplerState *createSamplerState(GXSAMPLER_DESC *pSamplerDesc) = 0;
	virtual void destroySamplerState(IGXSamplerState *pState) = 0;
	virtual void setSamplerState(IGXSamplerState *pState) = 0;
	virtual IGXSamplerState *getSamplerState() = 0;


	// http://www.gamedev.ru/terms/StencilBuffer
	// virtual void setStencilTest(bool isEnable) = 0;

	//https://docs.microsoft.com/ru-ru/windows/desktop/direct3d10/d3d10-graphics-programming-guide-api-features-deprecated
};

#endif
