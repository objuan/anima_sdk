#pragma once
#include "Anima_nsbegin.h"
class GfxModel;

class GfxTexture
{
private:

	QString txtPath;
	GLuint textureIdx[1];

public:
	GfxTexture(const QString &txtPath);
	~GfxTexture(void);

	void Bind();
};
#include "Anima_nsend.h"