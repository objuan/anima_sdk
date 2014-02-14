#pragma once

#include "Anima_nsbegin.h"
class Command;

class ANIMA_CORE_API CmdLine 
{
public:

	enum CmdLineType
	{
		TypeString =0,
		TypeInt ,
		TypeFloat ,
		TypeBool,
		TypeVector,
		TypeDAG,
		TypeDAGLIST,
	};

	enum CmdLineFormat
	{
		Labeled,
		Unlabeled,
		Switch
	};

	class CmdLineEntry
	{
	public:
		QString name;
		QString desc;
		bool req;
		CmdLineFormat format;
		CmdLineType type;
		QString defValue;

		QString outValue;
		QStringList outMultiValue;

		CmdLineEntry()
		{
			outValue="";
		}

		void SetOutput(const QString &val);
		QString GetDescription();
	};

private:

	Command *command;
	QString cmdName;
	QString usageMessage;
	QList<CmdLineEntry *> entryList;
	QHash<QString,CmdLineEntry *> entryMap;

public:

	CmdLine(Command *command,const QString &usageMessage);
	~CmdLine();

	void AddUnlabeled(  const QString& name, 
			               const QString& desc, 
						   bool req,
				           QString value,
						   CmdLineType type,
						   bool ignoreable = false);

	void Add(  const QString& name, 
			               const QString& desc, 
						   bool req,
				           QString value,
						   CmdLineType type,
						   bool ignoreable = false);

	void AddSwitch( const QString& name, 
			               const QString& desc);

	QString GetValueStr(const QString &name);
	Ogre::Vector3 GetValueVector(const QString &name);
	int GetValueInt(const QString &name);
	float GetValueFloat(const QString &name);
	bool GetValueBool(const QString &name);
	DAG GetValueDAG(const QString &name);
	void GetValueDAGList(const QString &name,QList<DAG> &list);

	void Parse(const QString &name,QStringList args);

	void Error(const QString &desc);
	void Usage();
};

#include "Anima_nsend.h"