// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_GPU_PROGRAMMING_SERVICES_H_INCLUDED__
#define __I_GPU_PROGRAMMING_SERVICES_H_INCLUDED__

#include "IUnknown.h"
#include "SMaterial.h"

namespace irr
{

namespace io
{
	class IReadFile;
}

namespace video  
{

class IVideoDriver;
class IShaderConstantSetCallBack;

//! Compile target enumeration for the addHighLevelShaderMaterial() method. 
enum E_VERTEX_SHADER_TYPE
{
	EVST_VS_1_1 = 0,
	EVST_VS_2_0,
	EVST_VS_2_a,
	EVST_VS_3_0,

	//! This is not a type, but a value indicating how much types there are.
	EVST_COUNT
};

//! Names for all vertex shader types, each entry corresponds to a E_VERTEX_SHADER_TYPE entry.
const c8* const VERTEX_SHADER_TYPE_NAMES[] = {
	"vs_1_1",
	"vs_2_0",
	"vs_2_a",
	"vs_3_0",
	0 };

//! Compile target enumeration for the addHighLevelShaderMaterial() method. 
enum E_PIXEL_SHADER_TYPE
{
	EPST_PS_1_1 = 0,
	EPST_PS_1_2,
	EPST_PS_1_3,	
	EPST_PS_1_4,
	EPST_PS_2_0,
	EPST_PS_2_a,
	EPST_PS_2_b,
	EPST_PS_3_0,

	//! This is not a type, but a value indicating how much types there are.
	EPST_COUNT
};

//! Names for all pixel shader types, each entry corresponds to a E_PIXEL_SHADER_TYPE entry.
const c8* const PIXEL_SHADER_TYPE_NAMES[] = {
	"ps_1_1", 
	"ps_1_2",
	"ps_1_3",
	"ps_1_4",
	"ps_2_0",
	"ps_2_a",
	"ps_2_b",
	"ps_3_0",
	0 };

//!	Interface making it possible to create and use programs running on the GPU.
class IGPUProgrammingServices
{
public:

	//! destructor
	virtual ~IGPUProgrammingServices() {}

	//! Adds a new material renderer to the VideoDriver, based on a high level shading 
	//! language. Currently only HLSL/D3D9 and GLSL/OpenGL is supported. 
	//! \param vertexShaderProgram: String containing the source of the vertex shader program.
	//! This can be 0 if no vertex program should be used. 
	//! \param Entry name of the function of the vertexShaderProgram
	//! \param vsCompileTarget: Vertex shader version where the high level shader should be compiled to.
	//! \param pixelShaderProgram: String containing the source of the pixel shader program.
	//! This can be 0 if no pixel shader should be used. 
	//! \param pixelShaderEntryPointName: Entry name of the function of the pixelShaderEntryPointName
	//! \param psCompileTarget: Pixel shader version where the high level shader should be compiled to.
	//! \param callback: Pointer to an implementation of IShaderConstantSetCallBack in which you 
	//! can set the needed vertex and pixel shader program constants. Set this to 0 if you don't need this.
	//! \param baseMaterial: Base material which renderstates will be used to shade the
	//! material.
	//! \param userData: a user data int. This int can be set to any value and will be set as parameter
	//! in the callback method when calling OnSetConstants(). In this way it is easily possible to 
	//! use the same callback method for multiple materials and distinguish  between them during the call.
	//! \return Returns the number of the
	//! material type which can be set in SMaterial::MaterialType to use the renderer.
	//! -1 is returned if an error occured. -1 is returned for example if a vertex or pixel shader
	//! program could not be compiled or a compile target is not reachable.
	//! The error strings are then printed out into the error log, and
	//! can be catched with a custom event receiver.
    virtual s32 addHighLevelShaderMaterial(
		const c8* vertexShaderProgram,
		const c8* vertexShaderEntryPointName = "main",
		E_VERTEX_SHADER_TYPE vsCompileTarget = EVST_VS_1_1,
		const c8* pixelShaderProgram = 0, 
		const c8* pixelShaderEntryPointName = "main",
		E_PIXEL_SHADER_TYPE psCompileTarget = EPST_PS_1_1,
		IShaderConstantSetCallBack* callback = 0,
		E_MATERIAL_TYPE baseMaterial = video::EMT_SOLID,
		s32 userData = 0 ) = 0;    

	//! Like IGPUProgrammingServices::addShaderMaterial() (look there for a detailed description),
	//! but tries to load the programs from files. 
	//! \param vertexShaderProgram: Text file containing the source of the vertex shader program.
	//! Set to null if no shader should be created.
	//! \param pixelShaderProgram: Text file containing the source of the pixel shader program. Set to 
	//! 0 if no shader should be created.
	virtual s32 addHighLevelShaderMaterialFromFiles(
		const c8* vertexShaderProgram,
		const c8* vertexShaderEntryPointName = "main",
		E_VERTEX_SHADER_TYPE vsCompileTarget = EVST_VS_1_1,
		const c8* pixelShaderProgram = 0, 
		const c8* pixelShaderEntryPointName = "main",
		E_PIXEL_SHADER_TYPE psCompileTarget = EPST_PS_1_1,
		IShaderConstantSetCallBack* callback = 0,
		E_MATERIAL_TYPE baseMaterial = video::EMT_SOLID,
		s32 userData = 0) = 0; 


	//! Like IGPUProgrammingServices::addShaderMaterial() (look there for a detailed description),
	//! but tries to load the programs from files. 
	//! \param vertexShaderProgram: Text file containing the source of the vertex shader program.
	//! Set to null if no shader should be created.
	//! \param pixelShaderProgram: Text file containing the source of the pixel shader program. Set to 
	//! 0 if no shader should be created.
	virtual s32 addHighLevelShaderMaterialFromFiles(
		io::IReadFile* vertexShaderProgram,
		const c8* vertexShaderEntryPointName = "main",
		E_VERTEX_SHADER_TYPE vsCompileTarget = EVST_VS_1_1,
		io::IReadFile* pixelShaderProgram = 0, 
		const c8* pixelShaderEntryPointName = "main",
		E_PIXEL_SHADER_TYPE psCompileTarget = EPST_PS_1_1,
		IShaderConstantSetCallBack* callback = 0,
		E_MATERIAL_TYPE baseMaterial = video::EMT_SOLID,
		s32 userData = 0) = 0; 

	//! Adds a new material renderer to the VideoDriver, using pixel and/or 
	//! vertex shaders to render geometry.
	//! Note that it is a good idea to call IVideoDriver::queryFeature() before to check 
	//! if the IVideoDriver supports the vertex and/or pixel shader version your are using.  
	//! The material is added to the VideoDriver like with IVideoDriver::addMaterialRenderer()
	//! and can be used like it had been added with that method.
	//! \param vertexShaderProgram: String containing the source of the vertex shader program. This can be
	//! 0 if no vertex program should be used. 
	//! For DX8 programs, the will always input registers look like this:
	//! v0: position, v1: normal, 
	//! v2: color, v3: texture cooridnates, v4: texture coordinates 2 if available.
	//! For DX9 programs, you can manually set the registers using the dcl_ statements.
	//! \param pixelShaderProgram: String containing the source of the pixel shader program.
	//! This can be 0 if you don't want to use a pixel shader. 
	//! \param callback: Pointer to an implementation of IShaderConstantSetCallBack in which you 
	//! can set the needed vertex and pixel shader program constants. Set this to 0 if you don't need this.
	//! \param baseMaterial: Base material which renderstates will be used to shade the
	//! material.
	//! \param userData: a user data int. This int can be set to any value and will be set as parameter
	//! in the callback method when calling OnSetConstants(). In this way it is easily possible to 
	//! use the same callback method for multiple materials and distinguish  between them during the call.
	//! \return Returns the number of the
	//! material type which can be set in SMaterial::MaterialType to use the renderer.
	//! -1 is returned if an error occured. -1 is returned for example if a vertex or pixel shader
	//! program could not be compiled, the error strings are then printed out into the error log, and
	//! can be catched with a custom event receiver.
	virtual s32 addShaderMaterial(const c8* vertexShaderProgram = 0,
		const c8* pixelShaderProgram = 0, 
		IShaderConstantSetCallBack* callback = 0,
		E_MATERIAL_TYPE baseMaterial = video::EMT_SOLID,
		s32 userData = 0) = 0;

	//! Like IGPUProgrammingServices::addShaderMaterial(), but tries to load the 
	//! programs from files.
	//! \param vertexShaderProgram: Text file containing the source of the vertex shader program.
	//! Set to null if no shader should be created.
	//! \param pixelShaderProgram: Text file containing the source of the pixel shader program. Set to 
	//! 0 if no shader should be created.
 	//! \param callback: Pointer to an IShaderConstantSetCallback object to which the
 	//! OnSetConstants function is called.
 	//! \param baseMaterial: baseMaterial
	virtual s32 addShaderMaterialFromFiles(io::IReadFile* vertexShaderProgram,
		io::IReadFile* pixelShaderProgram,
		IShaderConstantSetCallBack* callback = 0,
		E_MATERIAL_TYPE baseMaterial = video::EMT_SOLID,
		s32 userData = 0) = 0;

	//! Like IGPUProgrammingServices::addShaderMaterial(), but tries to load the 
	//! programs from files.
	//! \param vertexShaderProgramFileName: Text file name containing the source of the 
	//! vertex shader program. 
	//! Set to null if no shader should be created.
	//! \param pixelShaderProgramFileName: Text file name containing the source of the
	//! pixel shader program. Set to 0 if no shader should be created.
 	//! \param callback: Pointer to an IShaderConstantSetCallback object to which the
 	//! OnSetConstants function is called.
 	//! \param baseMaterial: baseMaterial
	virtual s32 addShaderMaterialFromFiles(const c8* vertexShaderProgramFileName,
		const c8* pixelShaderProgramFileName,
		IShaderConstantSetCallBack* callback = 0,
		E_MATERIAL_TYPE baseMaterial = video::EMT_SOLID,
		s32 userData = 0) = 0;
};


} // end namespace video
} // end namespace irr

#endif

