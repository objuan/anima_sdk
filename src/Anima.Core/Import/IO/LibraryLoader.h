#pragma once

#include <QString>

class ContextIO
{
public:
	ContextIO(void);
	~ContextIO(void);

	void Load(const QString &filePath);
	bool Save(const QString &filePath);
};
