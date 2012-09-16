#pragma once
#include <irrlicht.h>

class SolNode :
	public irr::scene::ISceneNode
{
private:
	irr::core::aabbox3d<irr::f32> box;
	irr::video::S3DVertex *vertices;
	irr::video::SMaterial material;
	irr::u16 *indices ;
	static const int W = 100;
	static const int H = 100;

public:
	SolNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id, float radio);
	virtual ~SolNode(void);

	virtual void OnPreRender();
	virtual void render();

	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const
	{
		return box;
	}

	virtual irr::s32 getMaterialCount()
	{
		return 1;
	}

	virtual irr::video::SMaterial& getMaterial(irr::s32 i)
	{
		return material;
	}
};
