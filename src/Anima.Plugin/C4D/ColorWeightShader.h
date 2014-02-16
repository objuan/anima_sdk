#pragma once
#include "c4d.h"
#include "oldversions.h"

#define ID_COLORWEIGHTSHADER 1026315 //colorWeightShader

class ColorWeightShader : public ShaderData
{
		BaseShader* m_shader;
		Vector m_color;
	public:

		virtual Bool Init(GeListNode *node);
		virtual	SHADERINFO GetRenderInfo(BaseShader *sh);

#if API_VERSION > 11600
		INITRENDERRESULT InitRender(BaseShader *chn, const InitRenderStruct &irs);
#else
		Int32 InitRender(BaseShader *chn, InitRenderStruct *irs);
#endif
		virtual	void FreeRender(BaseShader *sh);

		virtual	Vector Output(BaseShader *sh, ChannelData *cd);

#ifdef C4D_R13
		virtual Bool Message(GeListNode *node, LONG type, void *msgdat);
#else
		virtual Bool Message(GeListNode *node, Int32 type, void *msgdat);
#endif
		static NodeData *Alloc(void) { return new ColorWeightShader; }

		static Bool RegisterColorWeightShader();
};