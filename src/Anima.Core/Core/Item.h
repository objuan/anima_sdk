#pragma once


#include "Anima.Core.h"
#include "Anima_nsbegin.h"
typedef enum ItemType
{
	ItemType_Model =0,
	ItemType_Action = 1,
};

class ANIMA_CORE_API ItemCategoryCollection : public BObject
{
public:
	QList<QString> Items;

	bool Contains(const QString &cat);
	void Set(QStringList &stringList);
};

class ANIMA_CORE_API AnimatedImage: public BObject
{
	QString filePath;
	QMovie *movie;
	QImage image;
	bool started;

public:
	AnimatedImage();
	virtual ~AnimatedImage(void);

	void Load(const QString &filePath);

	void Show(bool show);
	QImage CurrentImage();
};

class ANIMA_CORE_API Item : public BObject
{
public:
	Item(void);
	virtual ~Item(void);

	virtual AnimatedImage &GetImage()=0;

	virtual QString &GetName()=0;
	virtual ItemType GetItemType()=0;
	virtual QColor &GetColor()=0;
	virtual ItemCategoryCollection &GetCategory()=0;
	virtual QString &GetDesc()=0;
};

//class ANIMA_CORE_API ItemLibrary
//{
//public:
//	virtual QList<Item *> &ItemList()=0;
//	//virtual int Count()=0;
//};
#include "Anima_nsend.h"