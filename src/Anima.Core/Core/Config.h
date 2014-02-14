#pragma once

#include "Anima_nsbegin.h"

class AnimaLicense;

class ANIMA_CORE_API Config
{
private:

	static QString itemLibraryRootPath;
	static QString rootPath;
	static QString tmpPath;
	static QString programPath;
	static QString userFolderPath;
	static QString licenseFilePath;
	//static bool isTrial;
	static AnimaLicense *license;

	static bool useNullRender;

	static QString animaVersionDesc;
	static QString animaVersion;

public:
	static FILE_ANI_PATH AniPathMode;
	static QString VideoFolderPath;
	static QString VideoMode;
	static QString VideoPrefixName;
	static int VideoWidth;
	static int VideoHeight;

public:
	Config(void);

	static void Init(bool useNullRender,const QString &programPath="");
	static void Close();

	static QString ProgramPath(){return programPath;}
	static QString DataRootPath(){return rootPath;}
	static QString TmpPath(){return tmpPath;}
	static QString UserFolderPath(){return userFolderPath;}
	static QString GetLicenseFilePath(){return licenseFilePath;}

	static bool IsNullRender(){return useNullRender;}

	static bool IsDemo(); // versione non registrata
//	static bool IsTrial(){return isTrial;} // trial versione

	static QString AnimaVersionDesc(){return animaVersionDesc;}
	static QString GetVersion(){return animaVersion;}

	// license API
	static AnimaLicense *GetLicense(){return license;}
	static void SetLicense_SerialCode(const QString &registerName,const QString &serialCode);
	static void SetLicense_ModelList(const QStringList &modelList);
	static void ClearLicense();

};

class ANIMA_CORE_API Crypto
{
public:
	static QString ToString(QByteArray &arr);
	static QString MD5(const QString &string);
	static QString MD5_File(const QString &filePath);
	static QString RotateHEX(const QString &string,int len);
	static bool CheckSerialCode(const QString &registerName,const QString &serialCodeconst,const QString &licenseType);
	static QString Encode(const QString &string,const QString &licenseType);

};

class ANIMA_CORE_API AnimaLicense
{
public:
	// register
	QDateTime Date;
	QString RegisterName;
	QString SerialCode;
	QString LicenseType;

	// user web info
	bool ModelListAll;
	QStringList ModelList;

	AnimaLicense();
	static AnimaLicense *Load();
	void Save();
	QString GetCheckCode();
	QString Encode(const QString &string);
};

#include "Anima_nsend.h"
