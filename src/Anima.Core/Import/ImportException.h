#pragma once

//#include "stdafx.h"
#include "Anima_nsbegin.h"
class ANIMA_CORE_API ImportException: public BObject
{
private:
	
	QString message;

public:

	ImportException(const QString &_message) 
	{
		message = _message;
	}
	~ImportException(void){}

	QString Message(){return message;}
};
#include "Anima_nsend.h"