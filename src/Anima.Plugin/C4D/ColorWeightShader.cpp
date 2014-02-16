#include "ColorWeightShader.h"
#include "xcolorweight.h"
#include "oldversions.h"

Bool ColorWeightShader::Init(GeListNode *node)
{
	BaseContainer *data = ((BaseShader*)node)->GetDataInstance();
	data->SetVector(COLORWEIGHT_COLOR, Vector(1.0,1.0,1.0));
	return TRUE;
}


SHADERINFO ColorWeightShader::GetRenderInfo(BaseShader *sh)
{
	if(m_shader)
		return m_shader->GetRenderInfo();
	return SHADERINFO_0;
}

#if API_VERSION > 11600
INITRENDERRESULT ColorWeightShader::InitRender	(BaseShader *chn, const InitRenderStruct &irs)
{
	const InitRenderStruct *mirs = &irs;
#else
Int32 ColorWeightShader::InitRender	(BaseShader *chn, InitRenderStruct *irs)
{
	const InitRenderStruct *mirs = irs;
#endif

	BaseContainer *data = chn->GetDataInstance();
	m_shader  = (BaseShader*)data->GetLink(COLORWEIGHT_TEXTURE,mirs->doc,Xbase);
	m_color = data->GetVector(COLORWEIGHT_COLOR);
	if (m_shader) 
		return m_shader->InitRender(irs);
	return INITRENDERRESULT_OK;
}

void ColorWeightShader::FreeRender(BaseShader *sh)
{
	if (m_shader)
		m_shader->FreeRender();
	m_shader=NULL;
}

Vector ColorWeightShader::Output(BaseShader *sh, ChannelData *cd)
{
	if(m_shader)
	{
		Vector v = m_shader->Sample(cd);
		return Vector(v.x*m_color.x, v.y*m_color.y, v.z*m_color.z);
	}
	return m_color;
}
#ifdef C4D_R13
Bool ColorWeightShader::Message(GeListNode *node, LONG type, void *msgdat)
#else
Bool ColorWeightShader::Message(GeListNode *node, Int32 type, void *msgdat)
#endif
{
	BaseContainer *data = ((BaseShader*)node)->GetDataInstance();

	HandleInitialChannel(node,COLORWEIGHT_TEXTURE,type,msgdat);
	HandleShaderMessage(node,(BaseShader*)data->GetLink(COLORWEIGHT_TEXTURE,node->GetDocument(),Xbase),type,msgdat);

	return TRUE;
}

Bool ColorWeightShader::RegisterColorWeightShader()
{
	return RegisterShaderPlugin(ID_COLORWEIGHTSHADER, "Color Weight Shader", PLUGINFLAG_HIDE, ColorWeightShader::Alloc, "Xcolorweight", 0, NULL);
}