
#ifndef _MYAPPLICATION__H_
#define _MYAPPLICATION__H_

#include <vector>
#include <list>
#include "OgreApplication.h"
#include "TextRenderer.h"

using namespace Ogre;


class MyApplication : public OgreApplication
{
protected:
	typedef struct 
	{
		size_t			vertexCount;
		Vector3			*vertices;
		Vector3			*normals;
		size_t			indexCount;
		unsigned long	*indices;
	}MeshData;

public:
    void createScene();

protected:
    bool frameStarted(const FrameEvent& evt);
    bool frameEnded(const FrameEvent& evt);
    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );

	void			_populate();
    void			_createLight();
    void			_createSphere(int, Vector3);
	SceneNode*		_loadMesh(const String&, const Vector3&);
	void			_setCelShadingMaterial(Entity*);
	ManualObject*	_createQuadFinGeometry(Entity*);
	void			_getMeshInformation(const MeshPtr mesh,
										MeshData &meshData,
										const Vector3 &position,
										const Quaternion &orient,
										const Vector3 &scale);

	inline bool _isEdgeARidge(const Vector3&, const Vector3&, const Real&);
	inline bool _isEdgeAValley(const Vector3&, const Vector3&, const Real&);

	void _createDebugOverlay();
	void _updateDebugOverlay();

protected:
    BillboardSet *mBBset;
    Billboard *mLightFlare;
    Light *mLight;
    SceneNode *mLightNode;

    TextRenderer *mDebugText;

    AnimationState *mAnimState;

};


#endif _MYAPPLICATION__H_