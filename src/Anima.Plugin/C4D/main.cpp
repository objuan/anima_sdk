#include "AnimaIncludes.h"
#include "c4d.h"
#include "AnimaObject.h"
#include "AnimaMeshObject.h"
#include "ColorWeightShader.h"
#include "BulkGUI.h"

Bool PluginStart(void)
{
	int argc=0;
	char **argv=NULL;
	QApplication app(argc, argv);
#ifdef __C4D_64BIT
	Filename xmlPath = GeGetPluginPath() + "Anima64x.xml";
#else
	Filename xmlPath = GeGetPluginPath() + "Anima.xml";
#endif

	CHAR* configPath = xmlPath.GetString().GetCStringCopy();
	FILE* configFp = fopen(configPath, "r" );
	char *l;
	char line[1000];
	CHAR animaPath[255];
	Bool bFound = FALSE;
	if(!configFp)
	{
		GePrint("Anima.xml File not found!");
		return TRUE;
	}
	do
	{
		l = fgets(line,1000,configFp);
		if (strncmp(line,"PATH",4) == 0)
		{
			strcpy(animaPath, line+5);
			bFound=TRUE;
			break;
		}
	}while(l!=NULL);
	fclose(configFp);
	gDelete(configPath);
	if(bFound)
	{
		if (!AnimaObject::RegisterAnimaObject()) return FALSE;
		if (!AnimaMeshObject::RegisterAnimaMeshObject()) return FALSE;
		if (!ColorWeightShader::RegisterColorWeightShader()) return FALSE;

		Anima::Config::Init(TRUE,animaPath);
		Anima::GfxContext::GfxContext_New(NULL);
		Anima::Context::Singleton.Init(TRUE);

		BulkGUIManager *designerGUIManager = new BulkGUIManager();

#ifdef __MAC
		Ogre::NameValuePairList params;
		params["macAPI"] = "cocoa";
		params["FSAA"] = "0";
		params["macAPICocoaUseNSView"] = "true";
		params["parentWindowHandle"] = Ogre::StringConverter::toString(0);//(unsigned long) (winId()));
		Ogre::RenderWindow *renderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("AnimaWindow", 1,1, false, &params);
#else
		Ogre::RenderWindow *renderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("AnimaWindow", 1,1, false);
#endif



		Anima::Context::Singleton.InitUI(NULL,NULL,designerGUIManager,renderWindow,NULL);
		GePrint("----------------------------------------");
		GePrint("an(i)ma for C4D");
		GePrint("Copyright aXYZ design 2013");
		GePrint("Plugin developed by at2 GmbH (Blackstar)");
		GePrint("----------------------------------------");
		return TRUE;
	}
	return FALSE;
}

void PluginEnd(void)
{
	Anima::Context::Singleton.End();
//	Ogre::Root::getSingletonPtr()->shutdown();
//	gqapp->quit();
}

Bool PluginMessage(Int32 id, void *data)
{

	switch (id)
	{
		case C4DPL_INIT_SYS:
			if (!resource.Init()) return FALSE; // don't start plugin without resource
			return TRUE;

		case C4DMSG_PRIORITY: 
			return TRUE;
	}

	return FALSE;
}
