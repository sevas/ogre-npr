#include "precompiled.h"

#include "MyApplication.h"

#include <sstream>
#include <boost/foreach.hpp>

//#define ANIMATE_LIGHT

void MyApplication::createScene()
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
bool MyApplication::frameStarted(const FrameEvent& evt)
{
    _updateDebugOverlay();

#ifdef ANIMATE_LIGHT
		mAnimState->addTime(evt.timeSinceLastFrame);
#endif

    return OgreApplication::frameStarted(evt);
}
//-----------------------------------------------------------------------------
bool MyApplication::frameEnded(const FrameEvent& evt)
{
    return OgreApplication::frameEnded(evt);
}
//-----------------------------------------------------------------------------
bool MyApplication::mouseMoved(const OIS::MouseEvent &e)
{
    bool ret = OgreApplication::mouseMoved(e);
    return ret;
}
//-----------------------------------------------------------------------------
bool MyApplication::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
    return OgreApplication::mousePressed(e, id);
}
//-----------------------------------------------------------------------------
bool MyApplication::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
    return OgreApplication::mouseReleased(e, id);
}
//-----------------------------------------------------------------------------
bool MyApplication::keyPressed( const OIS::KeyEvent &e )
{
    return OgreApplication::keyPressed(e);
}
//-----------------------------------------------------------------------------
bool MyApplication::keyReleased( const OIS::KeyEvent &e )
{
    return OgreApplication::keyReleased(e);
}
//-----------------------------------------------------------------------------
void MyApplication::_createDebugOverlay()
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
void MyApplication::_updateDebugOverlay()
{
    mDebugText->setText("Batches", StringConverter::toString(mWindow->getBatchCount()));
    mDebugText->setText("FPS", StringConverter::toString(mWindow->getLastFPS()));
    mDebugText->setText("Triangles", StringConverter::toString(mWindow->getTriangleCount()));
}

//-----------------------------------------------------------------------------
void MyApplication::_populate()
{
	_loadMesh("TorusKnot01", Vector3(50, 0, 0));
	_loadMesh("Teapot01", Vector3(-50, 0, 0));
	_loadMesh("Gengon01", Vector3(50, 0, 50));
	_loadMesh("Cone01", Vector3(-50, 0, 50));
	_loadMesh("Box01", Vector3(50, 0, -50));
	_loadMesh("Cylinder01", Vector3(-50, 0, -50));
	//
	
	//SceneNode *bunny = _loadMesh("bunny", Vector3(-50, 0, 100));

	//bunny->scale(20, 20, 20);
	//bunny->pitch(Degree(-90));
	//bunny->translate(0, 20, 0);


	//SceneNode *dragon = _loadMesh("dragon", Vector3(50, 0, 100));

	//dragon->scale(20, 20, 20);
	//dragon->pitch(Degree(180));
	//dragon->translate(0, 15, 0);


	//_loadMesh("Rectangle01", Vector3(0, 0, 0));

}
//-----------------------------------------------------------------------------
SceneNode* MyApplication::_loadMesh(const String &_name, const Vector3 &_pos)
{
	Entity *ent = mSceneMgr->createEntity(_name, _name+".mesh");
	SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode(_name+"Node", _pos);
	
	//_setCelShadingMaterial(ent);
	ent->setMaterialName("Objects/Cube");
	ManualObject *edges = _createQuadFinGeometry(ent);


	node->attachObject(ent);
	node->attachObject(edges);
	return node;
}
//-----------------------------------------------------------------------------
ManualObject* MyApplication::_createQuadFinGeometry(Ogre::Entity *_ent)
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
		Vector3 ns0, ns1;
		Vector4 nA, nB ;
		Real tc;

		if(e.degenerate)
		{
			v0 =  meshData.vertices[e.vertIndex[0]];
			v1 =  meshData.vertices[e.vertIndex[1]];

			ns0 = meshData.normals[e.vertIndex[0]].normalisedCopy();
			ns1 = meshData.normals[e.vertIndex[1]].normalisedCopy();

			nA = edgeData->triangleFaceNormals[e.triIndex[0]];
			nB = -nA;
			tc = 1.0f;
		}
		else
		{
			v0 =  meshData.vertices[e.vertIndex[0]];
			v1 =  meshData.vertices[e.vertIndex[1]];

			ns0 = meshData.normals[e.vertIndex[0]].normalisedCopy();
			ns1 = meshData.normals[e.vertIndex[1]].normalisedCopy();

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

			tc = (isRidge  || isValley) ? 1.0f : 0.0f;
		}

		//build degenerate triangles for this edge
		// 1st tri
		Vector3 nA_ = Vector3(nA.x, nA.y, nA.z).normalisedCopy();
		Vector3 nB_ = Vector3(nB.x, nB.y, nB.z).normalisedCopy();

		Real offset = 0.5f;

		edgeGeometry->position(v0);
		edgeGeometry->normal(ns0);
		edgeGeometry->textureCoord(nA_);
		edgeGeometry->textureCoord(tc);
		edgeGeometry->index(idx++);

		edgeGeometry->position(v0);
		edgeGeometry->normal(ns0);
		edgeGeometry->textureCoord(nB_);
		edgeGeometry->textureCoord(tc);
		edgeGeometry->index(idx++);

		edgeGeometry->position(v1);
		edgeGeometry->normal(ns1);
		edgeGeometry->textureCoord(nA_);
		edgeGeometry->textureCoord(tc);
		edgeGeometry->index(idx++);


		
		//2nd tri
		edgeGeometry->position(v1);
		edgeGeometry->normal(ns1);
		edgeGeometry->textureCoord(nA_);
		edgeGeometry->textureCoord(tc);
		edgeGeometry->index(idx++);

		edgeGeometry->position(v0);
		edgeGeometry->normal(ns0);
		edgeGeometry->textureCoord(nB_);
		edgeGeometry->textureCoord(tc);
		edgeGeometry->index(idx++);


		edgeGeometry->position(v1);
		edgeGeometry->normal(ns1);
		edgeGeometry->textureCoord(nB_);
		edgeGeometry->textureCoord(tc);
		edgeGeometry->index(idx++);
		
	}

	edgeGeometry->end();
	
	//MeshPtr newmesh = edgeGeometry->convertToMesh(_ent->getName()+"edge mesh");
	//Entity *ent = mSceneMgr->createEntity(_ent->getName()+"edge entity", *newmesh);

	return edgeGeometry;
}
//-----------------------------------------------------------------------------
void MyApplication::_setCelShadingMaterial(Entity *_ent)
{
	_ent->getSubEntity(0)->setMaterialName("Examples/CelShading");

	_ent->getSubEntity(0)->setCustomParameter(0, Vector4(10.0f, 0.0f, 0.0f, 0.0f));
	_ent->getSubEntity(0)->setCustomParameter(1, Vector4(0.0f, 0.5f, 0.0f, 1.0f));
	_ent->getSubEntity(0)->setCustomParameter(2, Vector4(0.3f, 0.5f, 0.3f, 1.0f));
}
//-----------------------------------------------------------------------------
void MyApplication::_createSphere(int id, Vector3 _pos)
{


}
//-----------------------------------------------------------------------------
void MyApplication::_createLight()
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
void MyApplication::_getMeshInformation(const MeshPtr				_mesh
										,MyApplication::MeshData	&_meshData
										,const Vector3				&_position
										,const Quaternion			&_orient
										,const Vector3				&_scale)
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
bool MyApplication::_isEdgeARidge(const Vector3 &_nA, const Vector3 &_nB, const Real &_threshold)
{
	Vector3 nA_norm = Vector3(_nA.x, _nA.y, _nA.z).normalisedCopy();
	Vector3 nB_norm = Vector3(_nB.x, _nB.y, _nB.z).normalisedCopy();

	return nA_norm.dotProduct(nB_norm) < Math::Cos(_threshold);
}
bool MyApplication::_isEdgeAValley(const Vector3 &_nA, const Vector3 &_nB, const Real &_threshold)
{
	Vector3 nA_norm = Vector3(_nA.x, _nA.y, _nA.z).normalisedCopy();
	Vector3 nB_norm = Vector3(_nB.x, _nB.y, _nB.z).normalisedCopy();

	return nA_norm.dotProduct(nB_norm) < Math::Cos(_threshold);
}
//-----------------------------------------------------------------------------