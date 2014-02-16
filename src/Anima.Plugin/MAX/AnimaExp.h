
#pragma pack(1)

class AnimaExportererClassDesc;

class AnimaExporter : public UtilityObj {
public:
	AnimaExporter(AnimaExportererClassDesc* pDesc);
	~AnimaExporter();

	void	BeginEditParams(Interface* ip,IUtil* iu);
	void	EndEditParams(Interface* ip,IUtil* iu);
	void	DeleteThis();

private:
	HMODULE m_hModuleHandle;
	typedef UtilityObj* (*fnCreate)();
	typedef void (*fnDestroy)(UtilityObj*);

	fnCreate	m_fnCreate;
	fnDestroy	m_fnDestroy;
	static UtilityObj* m_pLEXIMain;

};

