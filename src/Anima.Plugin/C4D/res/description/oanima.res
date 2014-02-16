CONTAINER Oanima
{
	NAME Oanima;
	INCLUDE Obase;

	GROUP ID_OBJECTPROPERTIES
	{
		GROUP ANIMAOBJECT_GROUP_TEXTURES
		{
			DEFAULT 1;
			REAL ANIMAOBJECT_DARKNESS { CUSTOMGUI REALSLIDER; MIN -5; MAX 5; STEP 0.1; }
		}
		GROUP ANIMAOBJECT_GROUP_DISPLAY
		{
			DEFAULT 1;
			LONG ANIMAOBJECT_BBOX_MODE { CUSTOMGUI LONGSLIDER; MIN 0; MAX 999; }
		}
		GROUP ANIMAOBJECT_GROUP_RENDERING
		{
			DEFAULT 1;
		}
		GROUP ANIMAOBJECT_GROUP_MATERIAL
		{
			LINK ANIMAOBJECT_MATERIAL_OVERRIDE { ACCEPT { Mbase; }; }
		}
		GROUP ANIMAOBJECT_GROUP_UPDATE
		{
			DEFAULT 1;
			FILENAME ANIMAOBJECT_PATH { ANIM OFF; }
			GROUP
			{
				COLUMNS 2;
				BUTTON ANIMAOBJECT_RELOAD {}
				BOOL ANIMAOBJECT_EMBEDDED { ANIM OFF; }
			}
		}
		GROUP ANIMAOBJECT_GROUP_PLUGIN_INFO
		{
			DEFAULT 1;
			GROUP
			{
				BITMAPBUTTON ANIMAOBJECT_INFO_BMP { ANIM OFF; }
			}
			STATICTEXT ANIMAOBJECT_INFO_ANIMA1 { ANIM OFF;}
			STATICTEXT ANIMAOBJECT_INFO_ANIMA2 { ANIM OFF;}
			STATICTEXT ANIMAOBJECT_INFO_ANIMA3 { ANIM OFF;}
		}
		GROUP ANIMAOBJECT_GROUP_SCENE_INFO
		{
			DEFAULT 1;
			STATICTEXT ANIMAOBJECT_SCENEINFO_PRESET { ANIM OFF;}
			STATICTEXT ANIMAOBJECT_SCENEINFO_ACTORS { ANIM OFF;}
			STATICTEXT ANIMAOBJECT_SCENEINFO_POLYS { ANIM OFF;}
			STATICTEXT ANIMAOBJECT_SCENEINFO_POINTS { ANIM OFF;}
			STATICTEXT ANIMAOBJECT_SCENEINFO_RENDERER { ANIM OFF;}
		}
	}
}
