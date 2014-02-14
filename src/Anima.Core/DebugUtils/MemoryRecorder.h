#pragma once

//#include "Context.h"
#include "Anima_nsbegin.h"

class MemoryRecorder;
class SceneObject;

class MemoryRecorderEntry
{
public:
    QString filePath;
	void *pointer;
	bool isArray;
public:
	MemoryRecorderEntry( QString file,int line,void *pointer,bool isArray);
};

class MemoryRecorderList
{
private:
	QMap<void *,MemoryRecorderEntry *> recList;
	QMutex mutex;
	QMap<void *,MemoryRecorderEntry *> diffList;
public:
	MemoryRecorderList();
	~MemoryRecorderList();

	void Add(const MemoryRecorder &rec,bool isArray);
	void Del(const MemoryRecorder &rec,bool isArray);
	void Del(const char* file, int line,void *pointer,bool isArray);
	QString GetInfo(void *pointer);

	void ClearDiff();
	QString Dump();
	void DumpLog();
};

class MemoryRecorder
{
public:
	int mode;
    QString file;
    const int   line;
	void *pointer;

    void process(void* pointer);
	
public:

	MemoryRecorder(int mode,const char* _file, int line);

    /**
     * Operator to write the context information to memory.
     * <code>operator->*</code> is chosen because it has the right
     * precedence, it is rarely used, and it looks good: so people can
     * tell the special usage more quickly.
     */
	/*template <class _Tp> _Tp* operator->*(_Tp* pointer)
    { process(pointer); return pointer; }*/

	template <class _Tp> _Tp* operator->*(_Tp* pointer)
    { process(pointer); return pointer; }

private:
	MemoryRecorder(const MemoryRecorder&);
    MemoryRecorder& operator=(const MemoryRecorder&);
};


class NewRecorder : public MemoryRecorder
{
public:
	NewRecorder(const char* file, int line) : MemoryRecorder(0,file,line){}
};

class DelRecorder : public MemoryRecorder
{
public:
	DelRecorder(const char* file, int line) : MemoryRecorder(2,file,line){}
	DelRecorder() : MemoryRecorder(2,"",0){}
};

void __Delete(const char* file, int line,void *pointer);
//void __NotifyDelete(BObject *pointer);

#define DEBUG_NEW NewRecorder(__FILE__,__LINE__) ->* new 
//#define ANIMA_NEW_ARR NewRecorderArr(__FILE__,__LINE__) ->* New 
#define DEBUG_DEL(O) {__NotifyDelete(O);DelRecorder(__FILE__,__LINE__)  ->* delete (O);}
//#define ANIMA_DEL_ARR DelRecorderArr(__FILE__,__LINE__) ->* delete[] 

template <typename ForwardIterator>
Q_OUTOFLINE_TEMPLATE void DEBUG_DEL_ALL(ForwardIterator begin, ForwardIterator end)
{
    while (begin != end) {
		//DEBUG_DEL(*begin);
        ++begin;
    }
}

template <typename Container>
inline void DEBUG_DEL_ALL(const Container &c)
{
    DEBUG_DEL_ALL(c.begin(), c.end());
}

#ifdef ANIMA_FULL_DEBUG
#define New NewRecorder(__FILE__,__LINE__) ->* new 
#define ANIMA_DEL DelRecorder(__FILE__,__LINE__) ->* delete 
#endif

extern MemoryRecorderList *memoryManager;

#include "Anima_nsend.h"

