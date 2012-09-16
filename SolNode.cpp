#include "SolNode.h"

#include "Juego.h"
#include "AtmosferaNode.h"

#include <cmath>

#include <stdlib.h>
using namespace std;
using namespace irr;

SolNode::SolNode(scene::ISceneNode *parent, scene::ISceneManager *mgr, s32 id, float radio) 
		: scene::ISceneNode(parent, mgr, id)
	{

		this->setRotation(core::vector3df(0,0,-110));
		this->setPosition(core::vector3df(0,100,0));
		this->setScale(core::vector3df(260,260,260));

		this->addAnimator( mgr->createRotationAnimator(core::vector3df(0,0.3,0)));

		material.AmbientColor = video::SColor::SColor(255,255,255,255);
		material.BackfaceCulling = false;
		material.DiffuseColor = video::SColor(255,0,192,0);
		material.GouraudShading = true;
		material.Lighting = true;
		material.Wireframe = true;
		material.ZBuffer = true;
		material.ZWriteEnable = true;
		material.Shininess = 0;
		material.MaterialType = video::E_MATERIAL_TYPE::EMT_SOLID_2_LAYER;

		material.Texture1 = Juego::GetInstance()->GetVideoDriver()->getTexture("data/Rocas.bmp");
		material.Texture2 = Juego::GetInstance()->GetVideoDriver()->getTexture("data/Hierba.bmp");

		// -------------------------------------------------------------------
		// Generamos los vertices
		// -------------------------------------------------------------------

		// Inicializamos los vertices
		vertices = new video::S3DVertex[W*H];
		for ( int x = 0 ; x < W ; ++x )
		{
			for ( int y = 0 ; y < H ; ++y )
			{
				vertices[y*W+x] = video::S3DVertex((x-W/2)*0.01, 0.0, (y-H/2)*0.01, 0, 0, 0, video::SColor(255,255,255,255), x/10.0, y/10.0);
			}
		}


		// Modificamos el terreno

		// Campo de potencial 
		// [ y += mag / ( dist^(exp/2) ) ]
		/*
		for ( int i = 0 ; i < 1000 ; ++i )
		{
			float mag = (rand() % 100 - 50)/500.0f ;
			float exp = (rand() % 20 +10 ) /100.0f ;

			float x0 = rand() % W + 0.5f ;
			float y0 = rand() % H + 0.5f ;
			
			for ( int x = 0 ; x < W ; ++x )
			{
				for ( int y = 0 ; y < H ; ++y )
				{
					float dist = pow( (x-x0)*(x-x0) + (y-y0)*(y-y0), exp);  

					vertices[y*W+x].Pos.Y += mag/dist;
				}
			}
		}
		*/

		// Paraboloide de revolución 
		// [ y += mag - (dist^2)/radio ] (Siempre y cuando mag > (dist^2)/radio )
		/*
		for ( int i = 0 ; i < 1000 ; ++i )
		{
			float mag = (rand()%100) / 4000.0f ;
			float rad = (rand()%50000000 + 1000000) / 100.0f ;

			float x0 = rand() % W + 0.5f ;
			float y0 = rand() % H + 0.5f ;

			for ( int x = 0 ; x < W ; ++x )
			{
				for ( int y = 0 ; y < H ; ++y )
				{
					float dist = pow( (x-x0)*(x-x0) + (y-y0)*(y-y0), 2 ) / rad ;
					if ( dist < mag )
					{
						vertices[y*W+x].Pos.Y += mag - dist ;
					}
				}
			}
		}
		*/

		// Arcotangente
		// [ y += mag * atan(rad*k - dist*k)/PI + PI/2 ]
		
		float PI = 3.1416;
		for ( int i = 0 ; i < 10 ; ++i )
		{
			float mag = (rand()%200 +300) / 4000.0f ;
			float rad = (rand()%1000 + 1000) / 150.0f ;
			float k = 1/rad ;

			float x0 = rand() % W + 0.5f ;
			float y0 = rand() % H + 0.5f ;

			for ( int x = 0 ; x < W ; ++x )
			{
				for ( int y = 0 ; y < H ; ++y )
				{
					float dist = sqrt( (x-x0)*(x-x0) + (y-y0)*(y-y0) ) ;

					vertices[y*W+x].Pos.Y += mag * atan(1 - dist*k)/PI + PI/2;
				}
			}
		}
		

		

		// Resituamos el terreno
		float mean = 0.0f ;
		for ( int i = 0 ; i < W*H ; ++i )
		{
			mean += vertices[i].Pos.Y;
		}
		mean /= W*H ;

		for ( int i = 0 ; i < W*H ; ++i )
		{
			vertices[i].Pos.Y -= mean ;
		}
		

		// -------------------------------------------------------------------
		// Generamos los triangulos
		// -------------------------------------------------------------------

		indices = new u16[(W-1)*(H-1)*2*3];
		int nTrig = 0 ;
		for ( int x = 0 ; x < W-1 ; ++x )
		{
			for ( int y = 0 ; y < H-1 ; ++y )
			{

				indices[nTrig*3+0] = (y  )*W + (x  );
				indices[nTrig*3+1] = (y+1)*W + (x+1);
				indices[nTrig*3+2] = (y+1)*W + (x  );

				nTrig++;

				indices[nTrig*3+0] = (y  )*W + (x  );
				indices[nTrig*3+1] = (y  )*W + (x+1);
				indices[nTrig*3+2] = (y+1)*W + (x+1);				

				nTrig++;
			}
		}

		// -------------------------------------------------------------------
		// Calculamos las normales
		// -------------------------------------------------------------------
		for ( int i = 0 ; i < nTrig ; ++i )
		{
			core::triangle3df t;
			t.set(
				vertices[ indices[i*3 + 0] ].Pos,
				vertices[ indices[i*3 + 1] ].Pos,
				vertices[ indices[i*3 + 2] ].Pos);

			core::vector3df n = -t.getNormal();

			vertices[indices[i*3 + 0]].Normal += n ;
			vertices[indices[i*3 + 1]].Normal += n ;
			vertices[indices[i*3 + 2]].Normal += n ;
		}

		for ( int i = 0 ; i < W*H ; ++i )
		{
			vertices[i].Normal.normalize();
		}

		// -------------------------------------------------------------------
		// Calculamos el valor alpha (mapea la hierba)
		// -------------------------------------------------------------------
		for ( int i = 0 ; i < W*H ; ++i )
		{
			float ny;
			ny = vertices[i].Normal.Y ;
			//vertices[i].Color = video::SColor(255, 128*(1.0-ny), 255-128*(1.0-ny), 128*(1.0-ny));
			vertices[i].Color = video::SColor(255*ny, 255, 255, 255);
			//vertices[i].Color = video::SColor(255, 255, 255, 255);
		}


		// -------------------------------------------------------------------
		// Calculamos el bounding box
		// -------------------------------------------------------------------
		box.reset(vertices[0].Pos);
		for (s32 i=0; i< W*H ; ++i)
		{
			box.addInternalPoint(vertices[i].Pos);
		}
	}

SolNode::~SolNode(void)
{
}

void 
SolNode::OnPreRender()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);
	}

	ISceneNode::OnPreRender();
}

void 
SolNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	driver->setMaterial(material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawIndexedTriangleList(&vertices[0], W*H, &indices[0], (W-1)*(H-1)*2);	
}
