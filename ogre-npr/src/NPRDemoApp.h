/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef _NPRDEMOAPP__H_
#define _NPRDEMOAPP__H_

#include <vector>
#include <list>
#include <utility>
#include "OgreApplication.h"
#include "TextRenderer.h"

//#include <OgreMaxScene.hpp>

using namespace Ogre;


class NPRDemoApp : public OgreApplication/*, public OgreMax::OgreMaxSceneCallback*/
{
protected:
    typedef struct 
    {
        size_t          vertexCount;
        Vector3         *vertices;
        Vector3         *normals;
        size_t          indexCount;
        unsigned long   *indices;
    }MeshData;

    typedef std::pair<ManualObject*, SceneNode*>    EdgeGeometryItem;
    typedef std::list<EdgeGeometryItem>             EdgeGeometryList;


public:
    NPRDemoApp();
    void createScene();
    ManualObject*   _createQuadFinGeometry(Entity*);

    bool frameStarted(const FrameEvent& evt);
    bool keyReleased( const OIS::KeyEvent &e );
protected:
    // Scene generation
    void            _populate();
    void            _createLight();
    SceneNode*      _loadMesh(const String&, const Vector3&);
    void            _setCelShadingMaterial(Entity*);

    void            _getMeshInformation(const MeshPtr mesh,
                                        MeshData &meshData,
                                        const Vector3 &position,
                                        const Quaternion &orient,
                                        const Vector3 &scale);

    void _loadScene(const String&, SceneNode*);

    void _buildEdgeQuad(const Vector3&, const Vector3&
                        , const Vector4&,const Vector4&
                        , const Real, unsigned int
                        , ManualObject*);

    inline bool _isEdgeARidge(const Vector3&, const Vector3&, const Real&);
    inline bool _isEdgeAValley(const Vector3&, const Vector3&, const Real&);

    // overlay
    void _createDebugOverlay();
    void _updateDebugOverlay();

    // OgreMaxSceneCallback
    //void CreatedEntity(const OgreMax::OgreMaxScene*, Ogre::Entity*);

    void _setEdgesVisible(EdgeGeometryItem&, bool);

protected:
    BillboardSet *mBBset;
    Billboard *mLightFlare;
    Light *mLight;
    SceneNode *mLightNode;

    TextRenderer *mDebugText;

    AnimationState *mAnimState;
    EdgeGeometryList mEdges;
    bool mEdgesVisible;
};


#endif _NPRDEMOAPP__H_