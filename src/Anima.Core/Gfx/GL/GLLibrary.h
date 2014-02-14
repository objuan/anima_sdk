#pragma once

class GLObject;

class ANIMA_CORE_API GLLibrary
{
private:

	QMap<QString,QString> registerList;
	QMap<QString,GLObject *> objectLibrary;

	void Register(const QString &name,const QString &path);
	void Register(const QString &name,GLObject *obj);

public:

	static GLLibrary Singleton;

private:

	GLLibrary();
	~GLLibrary();

public:

	void Init();

	GLObject *GetObject(const QString &name);

};
