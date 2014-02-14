######################################################################
# Automatically generated by qmake (2.01a) Thu May 19 17:05:32 2011
######################################################################

QT       += xml

TARGET = AnimaCore
TEMPLATE = lib

DEPENDPATH += . \
              Core \
              DebugUtils \
              Export \
              Gfx \
              Import \
              Ogre \
              Physic \
              Task \
              UI \
              Core/Action \
              Core/AnimParts \
              Core/Command \
              Core/Edit \
              Core/IA \
              Core/Modders \
              Core/PathGfxBuilders \
              Gfx/GL \
              Gfx/Render \
              Import/IO \
              Core/Action/ActionModes \
              Core/IA/OpenSteer
INCLUDEPATH += . \
               Core \
               DebugUtils \
               Core/Edit \
               Core/Action \
               Core/Action/ActionModes \
               Core/AnimParts \
               Gfx \
               Gfx/Render \
               Physic \
               Task \
               Import/IO \
               Core/Modders \
               UI \
               Core/IA \
               Core/IA/OpenSteer \
               Core/PathGfxBuilders \
               Export \
               Core/Command \
               Import \
               Ogre \
               Gfx/GL \
				../Anima.Import \
               ../../lib/Ogre.framework/Headers \
               ../../lib/boost_1_46_1

# Input
HEADERS += Anima.Core.h \
           Anima_nsbegin.h \
           Anima_nsend.h \
           Anima_nsuse.h \
           GeneralParams.h \
           resource.h \
           stdafx.h \
           Core/Action.h \
           Core/ActionLibrary.h \
           Core/Animation.h \
           Core/AnimationPart.h \
           Core/AnimClip.h \
           Core/AnimClipCollection.h \
           Core/AnimClipModel.h \
           Core/BaseModel.h \
           Core/BuildDispatcher.h \
           Core/Clip.h \
           Core/ClipBlend.h \
           Core/ClipBrain.h \
           Core/ClipLibrary.h \
           Core/CollisionManager.h \
           Core/Config.h \
           Core/Context.h \
           Core/CoreException.h \
           Core/DAG.h \
           Core/Film.h \
           Core/FilmLayer.h \
           Core/IModelRenderer.h \
           Core/IMovePath.h \
           Core/Item.h \
           Core/Mesh.h \
           Core/MeshLibrary.h \
           Core/MeshOgre.h \
           Core/Modder.h \
           Core/Model.h \
           Core/ModelClip.h \
           Core/ModelEdit.h \
           Core/ModelInstance.h \
           Core/ModelLibrary.h \
           Core/ModelOverlayFunction.h \
           Core/MovePath.h \
           Core/MovePathTrx.h \
           Core/PathOffsetFunction.h \
           Core/PathOffsetFunctionNew.h \
           Core/Property.h \
           Core/resource.h \
           Core/ResourceManager.h \
           Core/SceneObject.h \
           Core/SceneObjectGfx.h \
           Core/SceneObjectGfxMesh.h \
           Core/SceneObjectMaterial.h \
           Core/SpeedFunction.h \
           DebugUtils/MemoryRecorder.h \
		   Export/MeshExporter.h \
           Export/MeshExt.h \
           Export/ParamsExporter.h \
           Export/PresetsExporter.h \
           Export/VideoManager.h \
           Gfx/GfxBaseClip.h \
           Gfx/GfxBaseModel.h \
           Gfx/GfxClipAnim.h \
           Gfx/GfxClipAnimCache.h \
           Gfx/GfxContext.h \
           Gfx/GfxLabelManager.h \
           Gfx/GfxModel.h \
           Gfx/GfxModelGroup.h \
           Gfx/GfxModelInstance.h \
           Gfx/GfxModelInstanceRender.h \
           Gfx/GfxOverlayClip.h \
           Gfx/GfxPathFinder.h \
           Gfx/GfxPerformanceManager.h \
           Gfx/GfxRenderRecord.h \
           Gfx/GfxScene.h \
           Gfx/GfxUtil.h \
           Gfx/GfxZoomManager.h \
           Import/AddOnManager.h \
           Import/ImportException.h \
           Import/ImportGeometry.h \
           Import/ImportModel.h \
           Import/OgreBaseImporter.h \
           Import/OgreModelImporter.h \
           Ogre/DynamicLines.h \
           Ogre/DynamicRenderable.h \
           Ogre/EntityMesh.h \
           Ogre/EscalatorMovePathMesh.h \
           Ogre/ModelMesh.h \
           Ogre/MovableText.h \
		   Ogre/OgreGridPlane.h \
           Ogre/OgreModelRenderer.h \
           Ogre/OgreSpline.h \
           Ogre/OgreUtil.h \
           Ogre/PointMovePathMesh.h \
           Ogre/SplineMovePathMesh.h \
           Ogre/StairMovePathMesh.h \
           Physic/Body.h \
           Physic/BodyCoord.h \
           Physic/BodyModder.h \
           Physic/BodyModderStair.h \
           Physic/BodyPose.h \
           Physic/Bone.h \
           Physic/IKSkeleton.h \
           Physic/Marker.h \
           Physic/MarkerFootTranform.h \
           Physic/MarkerModder.h \
           Physic/TransformFun.h \
           Task/DeleteTask.h \
           Task/GfxClipAnimTask.h \
           Task/Task.h \
           Task/TaskObject.h \
           Task/TaskObjectPtr.h \
           Task/TaskQueue.h \
           UI/GUIManager.h \
           UI/ZoomManager.h \
           Core/Action/ActionActorModder.h \
           Core/Action/ActionAnimation.h \
           Core/Action/ActionClip.h \
           Core/AnimParts/AnimationPartClipPoint.h \
           Core/AnimParts/AnimationPartClipSpline.h \
           Core/AnimParts/AnimationPartEscalator.h \
           Core/AnimParts/AnimationPartStairs.h \
           Core/AnimParts/EscalatorMovePath.h \
           Core/AnimParts/LinkMovePath.h \
           Core/AnimParts/OgreSpline.h \
           Core/AnimParts/PointMovePath.h \
           Core/AnimParts/SplineMovePath.h \
           Core/AnimParts/StairMovePath.h \
           Core/Command/ActionCommands.h \
           Core/Command/ActorCommands.h \
           Core/Command/AnimationCommands.h \
           Core/Command/AnimationPartCommands.h \
           Core/Command/ClipCommands.h \
           Core/Command/CmdLine.h \
           Core/Command/Command.h \
           Core/Command/CommandManager.h \
           Core/Command/FilmCommands.h \
           Core/Command/GeomCommands.h \
           Core/Command/InCommand.h \
           Core/Command/PathCommands.h \
           Core/Command/StdCommands.h \
           Core/Edit/EditorManager.h \
           Core/Edit/EventManager.h \
           Core/Edit/PathManipolator.h \
           Core/Edit/SelectionManager.h \
           Core/Edit/SelectionManagerGfx.h \
           Core/Edit/SelectionManagerGfxMesh.h \
           Core/Edit/SelectionManagerGfxMeshSplineHelper.h \
           Core/IA/ModelIA.h \
           Core/IA/ModelIAActor.h \
           Core/IA/ModelIAActorCollision.h \
           Core/IA/ModelIAActorDecision.h \
           Core/IA/ModelIAActorDecisionHistory.h \
           Core/IA/ModelIAActorSpeed.h \
           Core/IA/ModelIABuilder.h \
           Core/IA/ModelIABuilderPath.h \
           Core/IA/ModelIABuilderScanner.h \
           Core/IA/ModelIACollision.h \
           Core/IA/ModelIAQualityManager.h \
           Core/IA/ModelIAThread.h \
           Core/IA/ModelInstanceIAGfx.h \
           Core/Modders/ClonerModder.h \
           Core/Modders/ClonerModder_Area_Mask.h \
           Core/Modders/PhaserModder.h \
           Core/Modders/randomc.h \
           Core/PathGfxBuilders/PathGfxBuilder.h \
           Gfx/Render/DebugGraphics.h \
           Gfx/Render/Line3D.h \
           Gfx/Render/OgreRenderer.h \
           Gfx/Render/ogreutility.h \
           Gfx/Render/TextureRenderer.h \
           Gfx/Render/View.h \
           Import/IO/ContextIO.h \
           Import/IO/LibraryLoader.h \
           Core/Action/ActionModes/PathDesignMode.h \
           Core/Action/ActionModes/SplineDesignMode.h \
           ./Import/IO/ContextIO.h \
           Core/IA/ModelIAActorDecisionBrain.h \
	Gfx/GfxClip.h
SOURCES += Anima.Core.cpp \
           GeneralParams.cpp \
		   moc_list.cpp \
		   stdafx.cpp \
           Core/Action.cpp \
           Core/ActionLibrary.cpp \
           Core/Animation.cpp \
           Core/AnimationPart.cpp \
           Core/AnimClip.cpp \
           Core/AnimClipCollection.cpp \
           Core/AnimClipModel.cpp \
           Core/BaseModel.cpp \
           Core/BuildDispatcher.cpp \
           Core/Clip.cpp \
           Core/ClipBlend.cpp \
           Core/ClipBrain.cpp \
           Core/ClipLibrary.cpp \
           Core/CollisionManager.cpp \
           Core/CollisionManagerIce.cpp \
           Core/Config.cpp \
           Core/Context.cpp \
           Core/DAG.cpp \
           Core/Film.cpp \
           Core/FilmLayer.cpp \
           Core/Item.cpp \
		   Core/Mesh.cpp \
           Core/MeshLibrary.cpp \
           Core/Modder.cpp \
           Core/Model.cpp \
           Core/ModelClip.cpp \
           Core/ModelEdit.cpp \
           Core/ModelInstance.cpp \
           Core/ModelLibrary.cpp \
           Core/ModelOverlayFunction.cpp \
           Core/MovePath.cpp \
           Core/MovePathTrx.cpp \
           Core/Property.cpp \
           Core/ResourceManager.cpp \
           Core/SceneObject.cpp \
           Core/SceneObjectGfx.cpp \
           Core/SceneObjectMaterial.cpp \
           Core/SpeedFunction.cpp \
           DebugUtils/MemoryRecorder.cpp \
           Export/MeshExporter.cpp \
           Export/MeshExt.cpp \
           Export/MeshExtGeomExt.cpp \
           Export/ParamsExporter.cpp \
           Export/PresetsExporter.cpp \
           Export/VideoManager.cpp \
           Gfx/GfxBaseClip.cpp \
           Gfx/GfxBaseModel.cpp \
           Gfx/GfxClip.cpp \
           Gfx/GfxClipAnim.cpp \
           Gfx/GfxClipAnimCache.cpp \
           Gfx/GfxContext.cpp \
           Gfx/GfxLabelManager.cpp \
           Gfx/GfxModel.cpp \
           Gfx/GfxModelInstanceRender.cpp \
           Gfx/GfxOverlayClip.cpp \
           Gfx/GfxPerformanceManager.cpp \
           "Gfx/GfxRenderRecord .cpp" \
           Gfx/GfxScene.cpp \
           Gfx/GfxUtil.cpp \
           Gfx/GfxZoomManager.cpp \
           Import/AddOnManager.cpp \
           Import/ImportGeometry.cpp \
           Import/ImportModel.cpp \
           Import/OgreBaseImporter.cpp \
           Import/OgreModelImporter.cpp \
           Ogre/DynamicLines.cpp \
           Ogre/DynamicRenderable.cpp \
           Ogre/EntityMesh.cpp \
           Ogre/EscalatorMovePathMesh.cpp \
           Ogre/ModelMesh.cpp \
           Ogre/MovableText.cpp \
           Ogre/OgreGridPlane.cpp \
           Ogre/OgreModelRenderer.cpp \
           Ogre/OgreUtil.cpp \
           Ogre/PointMovePathMesh.cpp \
           Ogre/SplineMovePathMesh.cpp \
           Ogre/StairMovePathMesh.cpp \
           Physic/Body.cpp \
           Physic/BodyCoord.cpp \
           Physic/BodyModder.cpp \
           Physic/BodyPose.cpp \
           Physic/Bone.cpp \
           Physic/IKBrain.cpp \
           Physic/IKSkeletonBrain.cpp \
           Physic/Marker.cpp \
           Physic/MarkerModder.cpp \
           Physic/MarkerFootTranform.cpp \
           Physic/TransformFun.cpp \
           Task/Task.cpp \
           Task/TaskQueue.cpp \
           UI/GUIManager.cpp \
           UI/ZoomManager.cpp \
           Core/Action/ActionActorModder.cpp \
           Core/Action/ActionAnimation.cpp \
           Core/Action/ActionClip.cpp \
           Core/AnimParts/AnimationPartClipPoint.cpp \
           Core/AnimParts/AnimationPartClipSpline.cpp \
           Core/AnimParts/AnimationPartEscalator.cpp \
           Core/AnimParts/AnimationPartStairs.cpp \
           Core/AnimParts/EscalatorMovePath.cpp \
           Core/AnimParts/LinkMovePath.cpp \
           Core/AnimParts/OgreSpline.cpp \
           Core/AnimParts/PointMovePath.cpp \
           Core/AnimParts/SplineMovePath.cpp \
           Core/AnimParts/StairMovePath.cpp \
           Core/Command/ActionCommands.cpp \
           Core/Command/ActorCommands.cpp \
           Core/Command/AnimationCommands.cpp \
           Core/Command/AnimationPartCommands.cpp \
           Core/Command/ClipCommands.cpp \
           Core/Command/CmdLine.cpp \
           Core/Command/Command.cpp \
           Core/Command/CommandManager.cpp \
           Core/Command/FilmCommands.cpp \
           Core/Command/GeomCommands.cpp \
           Core/Command/PathCommands.cpp \
           Core/Command/StdCommands.cpp \
           Core/Edit/EditorManager.cpp \
           Core/Edit/EventManager.cpp \
           Core/Edit/PathManipolator.cpp \
           Core/Edit/SelectionManager.cpp \
           Core/IA/ModelIA.cpp \
           Core/IA/ModelIAActor.cpp \
           Core/IA/ModelIAActorCollision.cpp \
           Core/IA/ModelIAActorDecision.cpp \
           Core/IA/ModelIAActorDecisionBrain.cpp \
           Core/IA/ModelIAActorDecisionHistory.cpp \
           Core/IA/ModelIAActorSpeed.cpp \
           Core/IA/ModelIABuilder.cpp \
           Core/IA/ModelIABuilderPath.cpp \
           Core/IA/ModelIABuilderPathInfo.cpp \
           Core/IA/ModelIABuilderPathIO.cpp \
           Core/IA/ModelIABuilderPathOperator.cpp \
           Core/IA/ModelIABuilderScanner.cpp \
           Core/IA/ModelIACollision.cpp \
           Core/IA/ModelIAQualityManager.cpp \
           Core/IA/ModelIAThread.cpp \
           Core/IA/ModelInstanceIAGfx.cpp \
           Core/Modders/ClonerModder.cpp \
           Core/Modders/ClonerModder_Area_Mask.cpp \
           Core/Modders/mersenne.cpp \
           Core/Modders/PhaserModder.cpp \
           Core/PathGfxBuilders/PathGfxBuilder.cpp \
           Gfx/Render/DebugGraphics.cpp \
           Gfx/Render/Line3D.cpp \
           Gfx/Render/OgreRenderer.cpp \
           Gfx/Render/ogreutility.cpp \
           Gfx/Render/ogreutility1.cpp \
           Gfx/Render/TextureRenderer.cpp \
           Gfx/Render/View.cpp \
		   Import/IO/ContextIO.cpp
