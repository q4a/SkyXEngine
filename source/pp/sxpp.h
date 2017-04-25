
/*!
\file
Заголовочный файл sxpp - библиотеки постпроцесса
*/

/*! \defgroup sxpp sxpp - библиотека постпроцесса
@{
*/
#ifndef __sxpostprocess
#define __sxpostprocess

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif
#include <gcore\\sxgcore.h>

/*! \name Значения цветов для создания эффекта sepia
@{*/

#define PP_SEPIA_LIGHT_COLOR float3_t(1.0,0.9,0.5)
#define PP_SEPIA_DARK_COLOR float3_t(0.2,0.05,0.0)
#define PP_SEPIA_WEIGHTS_COLOR float3_t(0.27, 0.67, 0.06)

//!@}

#define PP_SUN_RADIUS 30 /*!< радиус солнца в пикселях для lens flare */
#define PP_MAX_ANGLE_VISIBLE_SUN 1.57 /*!< максимальный угол между взглядом наблюдателя и позицией источнкиа света при котором еще видно солнце, больше этого солнце уже не видно */

/*! \name Базовые функции библиотеки
@{*/
SX_LIB_API long SPP_0GetVersion();				//!< возвращает версию подсистемы
SX_LIB_API void SPP_Dbg_Set(report_func rf);	//!< установка функции обработки сообщений

//! инициализация подсистемы
SX_LIB_API void SPP_0Create(
	const char* name,			//имя подсистемы
	IDirect3DDevice9* device,	//dx устройство
	float2_t* winsize,			//размеры backbuffer (окна рендера)
	bool is_unic				//должно ли быть имя подсистемы уникальным
	);

SX_LIB_API void SPP_0Kill();	//!< уничтожение подсистемы

//!@}

/*! \name Установка основных render targets
 \warning Глубина должна быть линейная, от 0 до 1, где 0 это возле камеры, а 1 это максимальная удаленность от камеры. 
Причем линейность в пространстве камеры. 
Однако допускается и такая глубина: \n 
depth = (z + near)/far \n
где near и far ближняя и дальняя плоскости отсечения соответственно (к примеру 0.25 и 400), 
z - z компонента трехмерного вектора отрансформированного wordview projection матрицей и интерпалированного в пиксельный шейдер
@{*/
SX_LIB_API void SPP_RTSetInput(ID rt);	//!< входное изображение которое будет модифицироваться
SX_LIB_API void SPP_RTSetOutput(ID rt);	//!< дополнительное изображение для внутренних нужд

SX_LIB_API void SPP_RTSetDepth0(ID rt);	//!< глубина (линейная 0-1), абсолютно непрозрачные пиксели
/*! глубина (линейная 0-1), непрозрачные и прозрачные пиксели, однако прозрачные (если есть) будут перекрывать непрозрачные,
и в этом случае их глубина будет 1 то есть максимальной (для идентификации)*/
SX_LIB_API void SPP_RTSetDepth1(ID rt);
SX_LIB_API void SPP_RTSetNormal(ID rt);	//!< нормали (в том числе и микрорейльеф)

//!@}

/*! \name Переключение очереди и получением текущих rt
@{*/

SX_LIB_API ID SPP_RTGetCurrRender();	//!< поулчить текущий rt для рендера в него
SX_LIB_API ID SPP_RTGetCurrSend();		//!< получить текущий rt для отправки в шейдер
SX_LIB_API void SPP_RTIncr();			//!< переключить очередь между ке в который рисуем и который отправляем
SX_LIB_API void SPP_RTNull();			//!< обнулить очередь rt, то есть после вызова rt рендера станет input, а дополнительной output

//!@}

/*! обновление данных постпроцессорной подсистемы, необходимо вызывать перед началом рендера постпроцесса*/
SX_LIB_API void SPP_Update(
	float3_t* viewpos,	//!< позиция наблюдателя
	float3_t* viewdir,	//!< направление взгляда наблюдателя
	float4x4* view,		//!< матрица вида наблюдателя
	float4x4* proj,		//!< матрица проекции наблюдателя
	float2_t* winsize,	//!< текущие размеры окна рендера
	float2_t* nearfar,	//!< ближняя и дальняя плоскости отсечения
	float projfov		//!< угол обзора в радианах
	);
SX_LIB_API void SPP_UpdateSun(float3* sunpos);		//!< обвновление позиции солнца
SX_LIB_API void SPP_ChangeTexSun(const char* str);	//!< сменить текстуру солнца на str


/*! \defgroup sxpp_eff Эффекты
 \ingroup sxpp
@{*/

/*! рендер солнца, использует depth0, 
используется текстура загруженная функцией #SPP_ChangeTexSun \n 
sun_color - rgb цвет (0-1), w - на сколько цвет применится к текстуре (0-1)
*/
SX_LIB_API void SPP_RenderSun(float4_t* sun_color);

/*! bloom \n 
param.x - во сколько раз увеличиваем цвет пикселя при анализе \n 
param.y - минимальное значение цвета для прохождение теста bright pass \n 
param.z - во сколько раз увеличиваем цвет пикселя если он прошел тест \n 
примерные значения: 1, 0.8, 1
*/
SX_LIB_API void SPP_RenderBloom(float3_t* param);

/*! lens flare, эффект восприятия яркого света линзами, использует depth0 \n 
param.x - дисперсия (разброс) \n 
param.y - ширина свечения \n 
param.z - множитель интенсивности \n 
use_bloom - использовать ли в дополнение данные от bloom прохода, если true то этот эффект должен идти поле bloom \n 
примерные данные для 8 (NumSamples) выборок: 0.25f, 0.3f, 0.9f, use_bloom - на любителя*/
SX_LIB_API void SPP_RenderLensFlare(float3_t* param, bool use_bloom);

/*! depth of field, глубина резкости, использует depth0 \n 
все компоненты param указываются в метрах \n 
param.x - откуда начинается нормальное видиние \n 
param.y - где оно заканчивается \n 
param.z - постепенный переход на ближней плоскости \n 
param.w - постепенный переход на дальней плоскости \n 
sky_blur - на сколько % размывать небо (0-1) \n 
для примеры: param:0, 200, 0, 50, sky_blur: 0*/
SX_LIB_API void SPP_RenderDOF(float4_t* param, float sky_blur);

/*! linear fog, линейный туман, использует depth0 \n 
color - цвет тумана (0-1) \n 
param.x - интенсивность тумана \n 
param.y - на сколько небо в тумане \n 
param.z - минимальное значение тумана \n 
param.w - максимальное значение тумана \n 
примерные значения: 0.8, 1, 0.1, 0.9*/
SX_LIB_API void SPP_RenderFog(float3_t* color, float4_t* param);

/*! space screen ambient occulusion, глобальное освещение (точнее затенение) в пространстве экрана, использует depth1 \n 
param.x - радиус \n 
param.y - минимально допустимая разница в глубине между пикселя для просчета затенения \n 
param.z - освещенность пикселя по дефолту (если разница меньше минимальной) \n 
param.w - допустимая разница для сравнения \n 
примерные значения: 0.3f, 0.1f, 0.8f, 0.3f / GData::NearFar.y*/
SX_LIB_API void SPP_RenderSSAO(float4_t* param);

/*! motion blur, размытие в движении, использует depth0 \n 
coef - коэфициент размытия, 0 - 1 \n 
timeDelta - время рендера текущего кадра в млсек*/
SX_LIB_API void SPP_RenderMotionBlur(float coef, DWORD timeDelta);


/*! \name Стандартные эффекты
@{*/

SX_LIB_API void SPP_RenderWhiteBlack(float coef);	//!< черно-белое изображение, coef - коэфициент перехода от цветного к черное-белому (0-1)
SX_LIB_API void SPP_RenderSepia(float coef);		//!< эффект сепия, , coef - коэфициент перехода от цветного к сепии (0-1)

/*! коррекция изображения \n 
param.x - contrast/контраст \n 
param.y - brightness/яркость \n 
param.z - gamma/гамма-коррекция \n 
нейтральные параметры: 1,1,0*/
SX_LIB_API void SPP_RenderCBG(float3_t* param);

//!@}


/*! \name Anti aliasing
@{*/

/*! nfaa \n 
param.x - strength, множитель нормали, усиляет влияние нормали \n 
param.y - scale, охватываемый масштаб \n 
param.z - notmal map, вывести сформированные нормали \n 
рекомендуемые параметры: 2, 1, 0*/
SX_LIB_API void SPP_RenderNFAA(float3_t* param);

SX_LIB_API void SPP_RenderDLAA();	//!< dlaa

//!@}

//!@} sxpp_eff

#endif

//!@} sxpp