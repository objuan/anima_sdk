#pragma once

#include "Core/Context.h"

#include "Anima_nsbegin.h"

class ImportMeshPreset: public BObject
{
public:
	QString name;
	QString ext;
	double rx;
	double ry;
	double rz;
	int unit;
};


class ANIMA_CORE_API GeneralParams   : public BObject
{
private:

	QList<ImportMeshPreset *> importPresetList;

public:

	float MIN_ADD_DISTANCE;
	float MAX_PICK_DIST;
	float MAX_AXE_ANGLE;
	float MIN_VALID_ANGLE;
	float MIN_VALID_POINT_DISTANCE;
	float MAX_VALID_POINT_DISTANCE;
	bool SHOW_FULL;

	float SPLINE_ADDPOINT_IN_STEP;
	float SPLINE_ADDPOINT_OUT_STEP;

	// clip

	int PATH_STEP;
	bool CLIP_BLEND;
	bool OVERLAY_BLEND;

	// STAIR

	float STEP_W_MIN;
	float STEP_W_MAX;
	float STEP_H_MIN;
	float STEP_H_MAX;

	// PATH

	bool PATH_BLEND;
	double morph_x_path_time_prev;
	double morph_x_path_time_next;

	double MIN_NEAR_POINT_DISTANCE;

	// BUILD

	int IK_CLIP_THRESHOLD;
	bool UsePrebuildBrainTable;

	bool COLLISION_ENABLE;
	float COLLISION_EVAL_PASS_RAY;
	float COLLISION_EVAL_INVALIDATE_MIN_DISTANCE;
	float COLLISION_CUT_INITIAL_RAY;
	float COLLISION_CUT_PASS_RAY;
	//float COLLISION_SPEED_BACK_TIME;
	float COLLISION_SPEED_LINE_TIME;
	float COLLISION_SPEED_RAMPA_UP_FACTOR;
	float COLLISION_SPEED_RAMPA_DOWN_FACTOR;
	float COLLISION_SPEED_RAMPA_MIN_FACTOR;
	float COLLISION_SPEED_RAMPA_MAX_FACTOR;
	float COLLISION_SPEED_RAMPA_FOLLOW_DOWN_FACTOR;
	float COLLISION_SPEED_BLEND_UP_FACTOR;
	float COLLISION_SPEED_BLEND_DOWN_FACTOR;
	//int   COLLISION_SPEED_NULL_COUNT;
	bool  COLLISION_ADMIN_MODE;

	float COLLISION_CUT_FRAMES_FACTOR;
	float COLLISION_CUT_ACTOR_FACTOR;

	// MODEL
	float speedMinPercent;
	float speedMaxPercent;

	// ADMIN

	bool showAdminMenu;
	FILE_ANI_PATH AniPath;

	// GFX

	double gizmosZoom;
	double maxModelCount;
	int ShadowMode; // 0 = NO, 1 = STENCIL

	float camera_moveFactorLinear;
	float camera_moveFactorQuadric;
	float camera_moveFactorInvBound;

public:
	GeneralParams(void);
	~GeneralParams(void);

	void Load();
	QList<ImportMeshPreset *> &ImportPresetList(){return importPresetList;}
	int FindByExt(const QString &ext);

	void SetPathMorph(double morph_x_time_prev,double morph_x_time_next);

	bool IsAdminUser(){return showAdminMenu;}
};
#include "Anima_nsend.h"

