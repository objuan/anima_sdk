#pragma once
#include "AnimaIncludes.h"
#include "c4d.h"
#include "oldversions.h"

#define VRAY_VP 1019782
#define VRAY_MAT 1020295
#define VRAYMATERIAL_COLOR_SHADER 4100

#define SELECTIONTAG_PRE "Selection"
#define IMPORTS_SCALE_FACTOR 100.0
#define ANIMA_MAT_LAYER "AnimaMatLayer"
#define ANIMA_MAT_PRE "AnimaMat"
#define ANIMA_MAT_SINGLE "AnimaMatSingle"

struct Utils
{
	static QString toQString(const String& st)
	{
		CHAR* c = st.GetCStringCopy();
		QString s = c;
		gDelete(c);
		return s;
	}

	static String toC4DString(const QString& st)
	{
		QByteArray b = st.toAscii();
		String s = b.data();
		return s;
	}

	static Vector toC4DVector(const Ogre::Vector3& v)
	{
		return Vector(v.x, v.y, v.z);
	}

	static BaseObject* findFirstObjectOfType(BaseObject* obj, LONG type)
	{
		while(obj)
		{
			if(obj->GetType()==type)
				return obj;
			BaseObject* found = Utils::findFirstObjectOfType(obj->GetDown(), type);
			if(found)
				return found;
			obj = obj->GetNext();
		}
		return NULL;
	}

	static String replaceAll(const String& st, CHAR find, CHAR repl)
	{
		String s = st;
		for(LONG i=0; i < s.GetLength(); i++)
			if(s[i] == find)
				s[i] = repl;
		return s;
	}

	static LayerObject* getAnimaMatLayer(BaseDocument* doc)
	{
		LayerObject* matLayer = NULL;
		GeListHead* lh = doc->GetLayerObjectRoot();
		if(lh)
		{
			LayerObject* lo = (LayerObject*) lh->GetFirst();
			while(lo)
			{
				if(lo->GetName().Compare(ANIMA_MAT_LAYER) == 0)
					matLayer = lo;
				lo=lo->GetNext();
			}
		}
		if(!matLayer)
		{
			matLayer = LayerObject::Alloc();
			matLayer->SetName(ANIMA_MAT_LAYER);
			lh->InsertFirst(matLayer);
			const LayerData *data = matLayer->GetLayerData(doc,TRUE);
			LayerData newdata(*data);
			newdata.locked = TRUE;
			newdata.manager = FALSE;
			matLayer->SetLayerData(doc, newdata);
		}
		return matLayer;
	}

	static void getAllSubShaderOfType(BaseDocument* doc, BaseShader* shader, AtomArray& arr, LONG type)
	{
		if(shader)
		{
			if(shader->GetType() == type)
				arr.Append(shader);

			BaseShader* child = shader->GetDown();
			while(child)
			{
				Utils::getAllSubShaderOfType(doc, child, arr, type);
				child = child->GetNext();
			}
		}
	}

	static void getAllAnimaMaterials(BaseDocument* doc, AtomArray& arr)
	{
		if(doc)
		{
			arr.Flush();
			BaseMaterial* mat = doc->GetFirstMaterial();
			LayerObject* matLayer = Utils::getAnimaMatLayer(doc);
			while(mat)
			{
				if(mat->GetLayerObject(doc) == matLayer)
					arr.Append(mat);
				mat = mat->GetNext();
			}
		}
	}

	static void adjustBitmapShaderDarkness(BaseShader* shader, Real val)
	{
		if(shader)
		{
			GeData d;
			shader->GetParameter(BITMAPSHADER_FILENAME, d, DESCFLAGS_GET_0);
			Filename f = d.GetFilename();
			f.ClearSuffix();
			String st = f.GetFileString();
			//bit of a hack but the MAX plugin does it the same way:
			//change only darkness of tex files that end with "-D"
			if(st.Content() && st.GetLength() > 2 && st[st.GetLength()-1] == 'D' && st[st.GetLength()-2] == '-')
			{
				GeData data(val / 5.0f);
				shader->SetParameter(DescLevel(BITMAPSHADER_EXPOSURE), data, DESCFLAGS_SET_0);
				shader->Message(MSG_CHANGE);
			}
		}
	}
	static void adjustAllMaterialsDarkness(BaseDocument* doc, Real val)
	{
		if(!doc)
			return;

		AutoAlloc<AtomArray> animaMats;
		Utils::getAllAnimaMaterials(doc, *animaMats);
		for(LONG i=0; i < animaMats->GetCount(); i++)
		{
			BaseMaterial* mat = (BaseMaterial*)animaMats->GetIndex(i);
					BaseContainer* bc = mat->GetDataInstance();
			BaseShader* colorShader = NULL;
			switch(mat->GetType())
			{
				case Mmaterial:
					colorShader = (BaseShader*) bc->GetLink(MATERIAL_COLOR_SHADER, doc);
					break;
				case VRAY_MAT:
					colorShader = (BaseShader*) bc->GetLink(VRAYMATERIAL_COLOR_SHADER, doc);
					break;
			}
			if(colorShader)
			{
				if(colorShader->GetType() == Xbitmap)
					adjustBitmapShaderDarkness(colorShader, val);
				else
				{
					AutoAlloc<AtomArray> bmpShader;
					Utils::getAllSubShaderOfType(doc, colorShader, *bmpShader, Xbitmap);
					for(LONG j=0; j < bmpShader->GetCount(); j++)
						adjustBitmapShaderDarkness((BaseShader*) bmpShader->GetIndex(j), val);
				}
			}

		}
	}

	static Vector rgbToHsl(Vector rgb)
	{
		Real max = rgb.x; if (rgb.y>max) max = rgb.y; if (rgb.z>max) max = rgb.z;
		Real min = rgb.x; if (rgb.y<min) min = rgb.y; if (rgb.z<min) min = rgb.z;

		Vector hsl;
		hsl.z = (max+min)/2.0;
		if(max==min)
			hsl.x=hsl.y=0.0;
		else
		{
			Real d = max-min;
			hsl.y = hsl.z > 0.5 ? d / (2.0 - max - min) : d / (max + min);
			if(max == rgb.x)
				hsl.x = (rgb.y - rgb.z) / d + (rgb.y < rgb.z ? 6.0 : 0.0);
			else if(max == rgb.y)
				hsl.x = (rgb.z - rgb.x) / d + 2.0;
			else
				hsl.x = (rgb.x - rgb.y) / d + 4.0;
			hsl.x /= 6.0;

		}
		return hsl;
	}

	static Real getUnitScaleFactor()
	{
		BaseContainer bc = GetWorldContainer();

		LONG unit = bc.GetInt32(WPREF_UNITS_BASIC);
		
		
		switch(unit)
		{
		case UNIT_KM:	return 0.00001;
		case UNIT_M:	return 0.01;	
		case UNIT_CM:	return 1.0;
		case UNIT_MM:	return 10.0;
		case UNIT_UM:	return 10000.0;
		case UNIT_NM:	return 10000000.0; // ...
		case UNIT_MILE:	return 10.0 / 1609344.0;
		case UNIT_YARD:	return 0.010936;
		case UNIT_FEET:	return 0.0328;
		case UNIT_INCH:	return 0.3937;
		case UNIT_NONE:	
		default:
			return 1.0;
		}
	}

	static void addLog(const String& st)
	{
		GePrint(st);
		return;


		AutoAlloc<BaseFile> logfile;
		String path = "c:\\anima.log";
		Filename logpath = Filename(path);
		if(logfile->Open(logpath, FILEOPEN_APPEND, FILEDIALOG_NONE, BYTEORDER_INTEL, MACTYPE_CINEMA, 'ttxt'))
		{
			String stt = st + "\r\n";
			char* ch = new char[stt.GetCStringLen()+1];
			stt.GetCString(ch, stt.GetCStringLen());
			logfile->WriteBytes(ch, stt.GetCStringLen()-1);
			logfile->Close();
			delete[] ch;
			GePrint(st);
		}
		else
			GePrint("Could not write " + path);
	}
};