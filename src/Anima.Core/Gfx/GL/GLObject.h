#pragma once

class ModelInstance;

class ANIMA_CORE_API GLObject
{
private:


public:

	void Load(const QString &path);
	virtual void Draw(KFbxXMatrix &worldTrx);
};
