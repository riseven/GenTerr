// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_MESH_BUFFER_H_INCLUDED__
#define __I_MESH_BUFFER_H_INCLUDED__

#include "IUnknown.h"
#include "SMaterial.h"
#include "aabbox3d.h"
#include "S3DVertex.h"

namespace irr
{
namespace scene
{
//! Enumeration for all vertex types there are.
enum E_PRIMITIVE_TYPE
{
	//! All vertices are non-connected points.
	EPT_POINTS=0,

	//! All vertices form a single connected line.
	EPT_LINE_STRIP,

	//! Just as LINE_STRIP, but the last and the first vertex is also connected.
	EPT_LINE_LOOP,

	//! Every two vertices are connected creating n/2 lines.
	EPT_LINES,

	//! After the first two vertices each vertex defines a new triangle.
	//! Always the two last and the new one form a new triangle.
	EPT_TRIANGLE_STRIP,

	//! After the first two vertices each vertex defines a new triangle.
	//! All around the common first vertex.
	EPT_TRIANGLE_FAN,

	//! Explicitly set all vertices for each triangle.
	EPT_TRIANGLES,

	//! After the first two vertices each further tw vetices create a quad with the preceding two.
	EPT_QUAD_STRIP,

	//! Every four vertices create a quad.
	EPT_QUADS,

	//! Just as LINE_LOOP, but filled.
	EPT_POLYGON
};

	//! Struct for holding a mesh with a single material
	/** SMeshBuffer is a simple implementation of a MeshBuffer. */
	class IMeshBuffer : public virtual IUnknown
	{
	public:

		//! destructor
		virtual ~IMeshBuffer() {}; 

		//! returns the material of this meshbuffer
		virtual video::SMaterial& getMaterial() = 0;

		//! returns the material of this meshbuffer
		virtual const video::SMaterial& getMaterial() const = 0;

		//! returns which type of vertex data is stored.
		virtual video::E_VERTEX_TYPE getVertexType() const = 0;

		//! returns pointer to vertex data. The data is an array of vertices. Which vertex
		//! type is used can be determined with getVertexType().
		virtual const void* getVertices() const = 0; 

		//! returns pointer to vertex data. The data is an array of vertices. Which vertex
		//! type is used can be determined with getVertexType().
		virtual void* getVertices() = 0; 

		//! returns amount of vertices
		virtual s32 getVertexCount() const = 0;

		//! returns pointer to Indices
		virtual const u16* getIndices() const = 0;

		//! returns pointer to Indices
		virtual u16* getIndices() = 0;

		//! returns amount of indices
		virtual s32 getIndexCount() const = 0;

		//! returns an axis aligned bounding box
		virtual const core::aabbox3df& getBoundingBox() const = 0;

		//! returns an axis aligned bounding box
		virtual core::aabbox3df& getBoundingBox() = 0;
	};

} // end namespace scene
} // end namespace irr

#endif

