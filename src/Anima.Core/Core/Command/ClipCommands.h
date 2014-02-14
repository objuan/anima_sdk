#pragma once

#include "Command.h"

#include "Anima_nsbegin.h"
class AnimClip;
class AnimClipCollection;
////class ClipMoveCommand : public Command
////{
////	class ClipMoveCommandEvent : public CommandEvent
////	{
////	public:
////		ClipMoveCommandEvent(Command *cmd):CommandEvent(cmd){}
////		virtual void OnMouseUp(MouseCollisionEvent &coll);
////		virtual void OnMouseDrag(MouseCollisionEvent &coll);
////	};
////
////private:
////	AnimClip *animClip;
////	//AnimClipCollection *clips_old;
////	QList<double> clipSizeList;
////public:
////	ClipMoveCommand();
////	~ClipMoveCommand();
////	virtual VoidP Exec();
////	virtual void Undo();
////};

//
//class ClipMovePosCommand : public Command
//{
//private:
//	AnimClip *animClip;
//	int oldIdx;
//
//public:
//	virtual VoidP Exec();
//	virtual void Undo();
//};
#include "Anima_nsend.h"




