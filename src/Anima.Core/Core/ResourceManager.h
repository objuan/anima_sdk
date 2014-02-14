#pragma once
#include "Anima_nsbegin.h"

class ANIMA_CORE_API ResourceImage
{
public:

	QImage normal;
	QImage over;
	QImage selected;
    QImage disabled;

	QString path;
	QString path_o;
	QString path_s;
	QString path_d;
	//QPixmap normalPix;
	//QPixmap overPix;
	//QPixmap selectedPix;
};

class ANIMA_CORE_API ResourceManager 
{
public:
    struct ColorRecord
    {
        QColor color;
        QIcon icon;
    };

	QImage item_color;
	QImage item_group;
	QImage item_model;
	QImage item_action;
	QImage item_path;
	QImage item_link;
	QImage item_stairs;
	QImage item_escalator;
	QImage item_open;
	QImage item_close;
    QImage item_scene;
    QImage item_animation;
    QImage item_spline;
    QImage item_fixed;
    QImage item_man;
	QImage item_man_off;
	QImage item_off;
	QImage item_lock;
	QImage item_unlock;
	QImage item_cloner;
	QImage item_phaser;
	QImage item_clip_fixed;
	QImage item_clip_spline;


    QCursor cursor_addPoint;
//	QCursor cursor_addPoint_replace;
    QCursor cursor_delPoint;
	QCursor cursor_insertPoint;
    QCursor cursor_normal;
	QCursor cursor_move_path;
	QCursor cursor_rotate_path;
	QCursor cursor_move_path_point;
	QCursor cursor_move_path_offset;
	QCursor cursor_move_clip_bound;
	QCursor cursor_move_ui_axis;
	QCursor cursor_move_point_bad;
	QCursor cursor_move_actor;

	QCursor cursor_add_fixed;
	QCursor cursor_add_escalator_1;
	QCursor cursor_add_escalator_2;
	QCursor cursor_add_stair_1;
	QCursor cursor_add_stair_2;
	
    QCursor cursor_pan;
    QCursor cursor_zoom;
    QCursor cursor_rotate;
		
public:
	static ResourceManager Singleton;
private:
	
	QMap<QString,ResourceImage> imageMap;
    QList<ColorRecord *> standartColors;

	void Add(const QString &id,const QString &normalName,const QString &overName,const QString &selectedName, const QString &disabledName);
public:
    //return count of currently registered standart colors
    int standartColorsCount() {return standartColors.size();}
    //returns standart color record by index
    ColorRecord * getStandartColor(int index);
    //finds standart color, returns -1 if this color doesn't exists
    int findStandartColor(const QColor &col);
    //adds New standar color and returns it's record

	static ColorRecord * getStandartColor(const QColor &col);
    ColorRecord * addStandartColor(const QColor &col);
    //create material for path
  //  Ogre::MaterialPtr createPathMaterial(const QString &name, const QString & group, const Ogre::Vector4 &color);

	ResourceManager();
	~ResourceManager();

	void Init();
	void End();

	ResourceImage *GetImage(const QString &id);
};

ANIMA_CORE_API QPixmap *_ResourceManager_LoadImage(const QString &name);
ANIMA_CORE_API QImage ResourceManager_LoadImage(const QString &name);

#include "Anima_nsend.h"
