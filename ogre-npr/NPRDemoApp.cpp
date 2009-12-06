/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */

#include "precompiled.h"

#include "NPRDemoApp.h"

#include <sstream>
#include <boost/foreach.hpp>

#define ANIMATE_LIGHT

//-----------------------------------------------------------------------------
NPRDemoApp::NPRDemoApp()
    :OgreApplication("NPR Sandbox")
    ,mBBset(NULL)
    ,mLightFlare(NULL)
    ,mLight(NULL)
    ,mLightNode(NULL)
    ,mDebugText(NULL)
    ,mAnimState(NULL)
    ,mEdgesVisible(true)
{

}
//-----------------------------------------------------------------------------
void NPRDemoApp::createScene()
{
    const RenderSystemCapabilities* caps = Root::getSingleton().getRenderSystem()->getCapabilities();
    if (!caps->hasCapability(RSC_VERTEX_PROGRAM) || !(caps->hasCapability(RSC_FRAGMENT_PROGRAM)))
    {
        OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "Your card does not support vertex and fragment programs, so cannot "
            "run this application. Sorry!", 
            "createScene");
    }


    mSceneMgr->setNormaliseNormalsOnScale(true);
    _createGrid(500);
    _createLight();
    _populate();

    _createDebugOverlay();

}
//-----------------------------------------------------------------------------
bool NPRDemoApp::frameStarted(const FrameEvent& evt)
{
    _updateDebugOverlay();

#ifdef ANIMATE_LIGHT
        mAnimState->addTime(evt.timeSinceLastFrame);
#endif

    return OgreApplication::frameStarted(evt);
}
//-----------------------------------------------------------------------------
bool NPRDemoApp::keyReleased( const OIS::KeyEvent &e )
{
    if(mKeyboard->isKeyDown(OIS::KC_H))
    {
        BOOST_FOREACH(EdgeGeometryItem item, mEdges)
        {
            mEdgesVisible = !mEdgesVisible;
            _setEdgesVisible(item, mEdgesVisible);
        }
    }
    return OgreApplication::keyReleased(e);
}
//-----------------------------------------------------------------------------
void NPRDemoApp::_createDebugOverlay()
{
    new TextRenderer();

    mDebugText = TextRenderer::getSingletonPtr();

    int x_offset=100, y_offset=18, w=100, h=18;

    mDebugText->addTextBox("Batches_", "#Batches : "
                            , 10, 10, w, h
                            , ColourValue(0.7,0.7,0.7));
    mDebugText->addTextBox("Batches", "0"
                            , x_offset, 10, w, h
                            , ColourValue(1.0,1.0,1.0));
    mDebugText->addTextBox("FPS_", "#FPS : "
                            , 10, 10+y_offset, w, h
                            , ColourValue(0.7,0.7,0.7));
    mDebugText->addTextBox("FPS", "0"
                            , x_offset, 10+y_offset, w, h
                            , ColourValue(1.0,1.0,1.0));

    mDebugText->addTextBox("Triangles_", "#tris : "
                            , 10, 10+y_offset*2, w, h
                            , ColourValue(0.7,0.7,0.7));
    mDebugText->addTextBox("Triangles", "0"
                            , x_offset, 10+y_offset*2, w, h
                            , ColourValue(1.0,1.0,1.0));



}
//-----------------------------------------------------------------------------
void NPRDemoApp::_updateDebugOverlay()
{
    mDebugText->setText("Batches", StringConverter::toString(mWindow->getBatchCount()));
    mDebugText->setText("FPS", StringConverter::toString(mWindow->getLastFPS()));
    mDebugText->setText("Triangles", StringConverter::toString(mWindow->getTriangleCount()));
}

//-----------------------------------------------------------------------------
void NPRDemoApp::_populate()
{
    _loadMesh("TorusKnot01", Vector3(-50, 0, 0));
    _loadMesh("Teapot01",    Vector3(50, 0, 0));
    _loadMesh("Gengon01",   Vector3(-50, 0, 50));
    _loadMesh("Cone01",     Vector3(50, 0, 50));
    _loadMesh("Box01",      Vector3(-50, 0, -50));
    _loadMesh("Cylinder01", Vector3(50, 0, -50));
    _loadMesh("Rectangle01", Vector3(0, 0, 0));

    
    // stanford models
    /*SceneNode *bunny = _loadMesh("bunny", Vector3(-50, 0, 100), 20.0f);

    bunny->scale(20, 20, 20);
    bunny->pitch(Degree(-90));
    bunny->translate(0, 20, 0);


    SceneNode *dragon = _loadMesh("dragon", Vector3(50, 0, 100), 20.0f);

    dragon->scale(20, 20, 20);
    dragon->pitch(Degree(180));
    dragon->translate(0, 15, 0);*/




    // loading a .scene scene
    //SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("titanic_root");
    //_loadScene("titanic", node);

    ////node->translate(0, 0, 1000);
    //node->scale(0.1, 0.1, 0.1);
}
//-----------------------------------------------------------------------------
void NPRDemoApp::_loadScene(const String &_name, SceneNode* _node)
{
    OgreMax::OgreMaxScene sceneLoader;

    sceneLoader.Load("../media/"+_name+"/"+_name+".scene", mWindow
        , OgreMax::OgreMaxScene::SKIP_ENVIRONMENT |
        OgreMax::OgreMaxScene::SKIP_SKY |
        OgreMax::OgreMaxScene::SKIP_TERRAIN|
        OgreMax::OgreMaxScene::SKIP_EXTERNALS
                    ,mSceneMgr,_node, this);
}
//-----------------------------------------------------------------------------
SceneNode* NPRDemoApp::_loadMesh(const String &_name, const Vector3 &_pos)
{
    Entity *ent = mSceneMgr->createEntity(_name, _name+".mesh");
    SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode(_name+"Node", _pos);

    ent->setMaterialName("NPR/Face");
    node->attachObject(ent);

    ManualObject *edges = _createQuadFinGeometry(ent);
    edges->setVisible(true);
    node->attachObject(edges);
    

    mEdges.push_back(EdgeGeometryItem(edges, node));

    return node;
}
//-----------------------------------------------------------------------------
ManualObject* NPRDemoApp::_createQuadFinGeometry(Ogre::Entity *_ent)
{
    MeshPtr mesh = _ent->getMesh();
    mesh->buildEdgeList();
    
    EdgeData *edgeData = mesh->getEdgeList();
    
    ManualObject *edgeGeometry = mSceneMgr->createManualObject(_ent->getName() + "edges");

    MeshData meshData;
    _getMeshInformation(mesh, meshData, Vector3::ZERO, Quaternion::IDENTITY, Vector3(1,1,1));


    int edgeCount = edgeData->edgeGroups[0].edges.size();
    int idx=0;
    edgeGeometry->begin("NPR/EdgeOutliner", RenderOperation::OT_TRIANGLE_LIST);

    for(int i=0; i<edgeCount ; i++)
    {
        EdgeData::Edge e = edgeData->edgeGroups[0].edges[i];

        Vector3 v0, v1;
        Vector4 nA, nB ;
        Real markedEdge;

        if(e.degenerate)
        {
            v0 =  meshData.vertices[e.vertIndex[0]];
            v1 =  meshData.vertices[e.vertIndex[1]];

            nA = edgeData->triangleFaceNormals[e.triIndex[0]];
            nB = -nA;
            markedEdge = 2.0f;
        }
        else
        {
            v0 =  meshData.vertices[e.vertIndex[0]];
            v1 =  meshData.vertices[e.vertIndex[1]];

            nA = edgeData->triangleFaceNormals[e.triIndex[0]];
            nB = edgeData->triangleFaceNormals[e.triIndex[1]];
            
            Real ridgeThreshold = Degree(35.0f).valueRadians();
            Real valleyThreshold = Degree(35.0f).valueRadians();


            bool isRidge = _isEdgeARidge(Vector3(nA.x, nA.y, nA.z)
                                        	,Vector3(nB.x, nB.y, nB.z)
                                        ,ridgeThreshold);

            bool isValley = _isEdgeAValley(Vector3(nA.x, nA.y, nA.z)
                                          ,Vector3(nB.x, nB.y, nB.z)
                                          ,valleyThreshold);

            markedEdge = (isRidge || isValley) ? 1.0f : 0.0f;
        }

        _buildEdgeQuad(v0, v1, nA, nB, markedEdge, idx, edgeGeometry);
        idx+=6;
    }

    edgeGeometry->end();
    return edgeGeometry;
}
//-----------------------------------------------------------------------------
void NPRDemoApp::_buildEdgeQuad(  const Vector3 &_v0, const Vector3&_v1
                                   , const Vector4 &_nA,const Vector4&_nB
                                   , const Real _markedEdge,unsigned int _idx
                                   , ManualObject *_edgeGeometry)
{
    Vector3 nA = Vector3(_nA.x, _nA.y, _nA.z).normalisedCopy();
    Vector3 nB = Vector3(_nB.x, _nB.y, _nB.z).normalisedCopy();
        
    /* 1st tri
       1 __ 2
        | /
        |/
        3
    */
    _edgeGeometry->position(_v0);
    _edgeGeometry->normal(nA);
    _edgeGeometry->textureCoord(nB);
    _edgeGeometry->textureCoord(_markedEdge);

    _edgeGeometry->index(_idx++);

    _edgeGeometry->position(_v0);
    _edgeGeometry->normal(nB);
    _edgeGeometry->textureCoord(nA);
    _edgeGeometry->textureCoord(_markedEdge);

    _edgeGeometry->index(_idx++);

    _edgeGeometry->position(_v1);
    _edgeGeometry->normal(nA);
    _edgeGeometry->textureCoord(nB);
    _edgeGeometry->textureCoord(_markedEdge);

    _edgeGeometry->index(_idx++);

    /* 2nd tri
          6
         /|
        /_|
       4   5
    */
    _edgeGeometry->position(_v1);
    _edgeGeometry->normal(nA);
    _edgeGeometry->textureCoord(nB);
    _edgeGeometry->textureCoord(_markedEdge);

    _edgeGeometry->index(_idx++);

    _edgeGeometry->position(_v0);
    _edgeGeometry->normal(nB);
    _edgeGeometry->textureCoord(nA);
    _edgeGeometry->textureCoord(_markedEdge);

    _edgeGeometry->index(_idx++);

    _edgeGeometry->position(_v1);
    _edgeGeometry->normal(nB);
    _edgeGeometry->textureCoord(nA);
    _edgeGeometry->textureCoord(_markedEdge);

    _edgeGeometry->index(_idx++);
}
//-----------------------------------------------------------------------------
void NPRDemoApp::_setCelShadingMaterial(Entity *_ent)
{
    _ent->getSubEntity(0)->setMaterialName("Examples/CelShading");

    _ent->getSubEntity(0)->setCustomParameter(0, Vector4(10.0f, 0.0f, 0.0f, 0.0f));
    _ent->getSubEntity(0)->setCustomParameter(1, Vector4(0.0f, 0.5f, 0.0f, 1.0f));
    _ent->getSubEntity(0)->setCustomParameter(2, Vector4(0.3f, 0.5f, 0.3f, 1.0f));
}
//-----------------------------------------------------------------------------
void NPRDemoApp::_createLight()
{
    mBBset = mSceneMgr->createBillboardSet("Light BB");
    mBBset->setMaterialName("Objects/Flare");
    mLightFlare = mBBset->createBillboard(Vector3::ZERO);

    mLight = mSceneMgr->createLight("main light");
    mLight->setType(Light::LT_POINT);
    mLight->setDiffuseColour(ColourValue::White);
    mLight->setSpecularColour(ColourValue::White);

    mLightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("light node");
    mLightNode->attachObject(mLight);
    mLightNode->attachObject(mBBset);


#ifdef ANIMATE_LIGHT
    Real totalTime = 10;                
    
    Animation *anim = mSceneMgr->createAnimation("Light Track", totalTime);
    anim->setInterpolationMode(Animation::IM_SPLINE);

    NodeAnimationTrack *track = anim->createNodeTrack(0, mLightNode);
    TransformKeyFrame *key;      

    Real precision = 36, amplitude = 300;
    int keyframeIndex = 0;
    for(float phi=0.0; phi <= 2*Math::PI; phi+= Math::PI / precision)
    {
        key = track->createNodeKeyFrame(phi * (totalTime/ (2*Math::PI)));
        key->setTranslate(Vector3(amplitude*Math::Cos(phi)
                                 ,amplitude
                                 ,amplitude*Math::Sin(phi)));
    }

    mAnimState = mSceneMgr->createAnimationState("Light Track");
    mAnimState->setEnabled(true);
#else
    mLightNode->setPosition(0, 500, 500);
#endif
}
//-----------------------------------------------------------------------------
void NPRDemoApp::_getMeshInformation(const MeshPtr              _mesh
                                    ,NPRDemoApp::MeshData       &_meshData
                                    ,const Vector3              &_position
                                    ,const Quaternion           &_orient
                                    ,const Vector3              &_scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

    _meshData.vertexCount = _meshData.indexCount = 0;

    // Calculate how many vertices and indices we're going to need
    for (unsigned short i = 0; i < _mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = _mesh->getSubMesh( i );

        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                _meshData.vertexCount += _mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            _meshData.vertexCount += submesh->vertexData->vertexCount;
        }

        // Add the indices
        _meshData.indexCount += submesh->indexData->indexCount;
    }


    // Allocate space for the vertices and indices
    _meshData.vertices = new Vector3[_meshData.vertexCount];
    _meshData.normals = new Vector3[_meshData.vertexCount];
    _meshData.indices = new unsigned long[_meshData.indexCount];

    

    added_shared = false;

    // Run through the submeshes again, adding the data into the arrays
    for ( unsigned short i = 0; i < _mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = _mesh->getSubMesh(i);

        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? _mesh->sharedVertexData : submesh->vertexData;

        if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }
    
            // get position
            {
                const Ogre::VertexElement* posElem =
                    vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
                
                Ogre::HardwareVertexBufferSharedPtr vbuf =
                    vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

                unsigned char* vertex =
                    static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

                // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
                //  as second argument. So make it float, to avoid trouble when Ogre::Real will
                //  be comiled/typedefed as double:
                //      Ogre::Real* pReal;
                float* pReal;

                for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
                {
                    posElem->baseVertexPointerToElement(vertex, &pReal);

                    Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

                    _meshData.vertices[current_offset + j] = (_orient * (pt * _scale)) + _position;
                }
                vbuf->unlock();
            }
            // get normals
            {
                const Ogre::VertexElement* normalElem =
                    vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_NORMAL);
                
                Ogre::HardwareVertexBufferSharedPtr vbuf =
                    vertex_data->vertexBufferBinding->getBuffer(normalElem->getSource());

                unsigned char* normal =
                    static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

                float* pReal;
                for( size_t j = 0; j < vertex_data->vertexCount; ++j, normal += vbuf->getVertexSize())
                {
                    normalElem->baseVertexPointerToElement(normal, &pReal);

                    Ogre::Vector3 normal(pReal[0], pReal[1], pReal[2]);

                    _meshData.normals[current_offset + j] = (_orient * (normal));
                }
                vbuf->unlock();
            }



            next_offset += vertex_data->vertexCount;
        }


        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

        // Ogre 1.6 patch (commenting the static_cast...) - index offsets start from 0 for each submesh
        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                _meshData.indices[index_offset++] = pLong[k] /*+ static_cast<unsigned long>(offset)*/;
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                _meshData.indices[index_offset++] = static_cast<unsigned long>(pShort[k]) /*+
                                                                                static_cast<unsigned long>(offset)*/;
            }
        }

        ibuf->unlock();
        current_offset = next_offset;
    }
} 
//-----------------------------------------------------------------------------
bool NPRDemoApp::_isEdgeARidge(const Vector3 &_nA, const Vector3 &_nB, const Real &_threshold)
{
    Vector3 nA_norm = Vector3(_nA.x, _nA.y, _nA.z).normalisedCopy();
    Vector3 nB_norm = Vector3(_nB.x, _nB.y, _nB.z).normalisedCopy();

    return nA_norm.dotProduct(nB_norm) < Math::Cos(_threshold);
}
//-----------------------------------------------------------------------------
bool NPRDemoApp::_isEdgeAValley(const Vector3 &_nA, const Vector3 &_nB, const Real &_threshold)
{
    Vector3 nA_norm = Vector3(_nA.x, _nA.y, _nA.z).normalisedCopy();
    Vector3 nB_norm = Vector3(_nB.x, _nB.y, _nB.z).normalisedCopy();

    return nA_norm.dotProduct(nB_norm) < Math::Cos(_threshold);
}
//-----------------------------------------------------------------------------
void NPRDemoApp::CreatedEntity(const OgreMax::OgreMaxScene* scene
                                 ,Ogre::Entity* entity)
{
    entity->setMaterialName("NPR/Face");
    ManualObject *edges = _createQuadFinGeometry(entity);
    edges->setVisible(true);

    SceneNode *node = entity->getParentSceneNode();

    if (node)
    {
        node->attachObject(edges);
        node->setVisible(true); 
    }
}
//-----------------------------------------------------------------------------
void NPRDemoApp::_setEdgesVisible(EdgeGeometryItem &_item, bool _visible)
{
    ManualObject *edges = _item.first;
    SceneNode    *node  = _item.second;
    
    node->setVisible(_visible);
    edges->setVisible(_visible);
}
//-----------------------------------------------------------------------------