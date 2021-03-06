#pragma once

#include "Buffer.h"
#include "BufferConfig.h"
#include "CBuffers.h"
#include "ShaderVariableContainer.h"
#include "ShaderStageConfig.h"
#include "ShaderStageData.h"
#include <D3DCompiler.h>
#include <comdef.h>
#include <d3d11.h>
#include <vector>

class BufferFactory;
class DeferredBaseShader;
class DeferredAnimatedBaseShader;
class DeferredTessBaseShader;
class DeferredTessAnimatedBaseShader;
class DeferredComposeShader;
class DeferredPostProcessing;
class LightShader;
class GUIShader;
class ParticleShader;
class ShadowShader;

// =======================================================================================
//                                      ShaderFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the creation of all the different shaders
///        
/// # ShaderFactory
/// Detailed description.....
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------
class ShaderFactory
{
public:
	ShaderFactory(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext,
		D3D_FEATURE_LEVEL p_featureLevel);
	virtual ~ShaderFactory();
	
	DeferredBaseShader* createDeferredBaseShader(const LPCWSTR& p_vertexPath, const LPCWSTR& p_pixelPath);

	DeferredAnimatedBaseShader* createDeferredAnimatedShader(const LPCWSTR& p_vertexPath, const LPCWSTR& p_pixelPath);

	DeferredTessBaseShader* createDeferredTessBaseShader(const LPCWSTR& p_filePath);

	DeferredTessBaseShader* createDeferredTesselationBaseShader(const LPCWSTR& p_vertexPath, 
		const LPCWSTR& p_hullPath, const LPCWSTR& p_domainPath, const LPCWSTR& p_pixelPath);

	DeferredTessAnimatedBaseShader* createDeferredTessAnimatedShader(const LPCWSTR& p_filePath);

	DeferredTessAnimatedBaseShader* createDeferredTessAnimatedShader(const LPCWSTR& p_vertexPath,
		const LPCWSTR& p_hullPath, const LPCWSTR& p_dominPath, const LPCWSTR& p_pixelPath);

	DeferredComposeShader* createDeferredComposeShader(const LPCWSTR& p_filePath);

	DeferredPostProcessing* createDeferredPostProcessingShader(const LPCWSTR& p_filePath);

	LightShader* createLightShader(const LPCWSTR& p_filePath);

	///-----------------------------------------------------------------------------------
	/// This method returns the GUI shader used for menus and HUDs.
	/// \param p_filePath
	/// \return DeferredComposeShader*
	///-----------------------------------------------------------------------------------
	GUIShader* createGUIShader(const LPCWSTR& p_filePath);

	ParticleShader* createParticleShader(const LPCWSTR& p_filePath);

	ShadowShader* createShadowShader(const LPCWSTR& p_filePath);
private:

	///-----------------------------------------------------------------------------------
	/// A helper function that takes arguments and then compiles the given shader file 
	/// and it's given entry point. Entry point is shader stage, e.g. PixelShaderFunc
	/// \param p_sourceFile
	/// \param p_entryPoint
	/// \param p_profile
	/// \param p_blob
	/// \return void
	///-----------------------------------------------------------------------------------
	void compileShaderStage(const LPCWSTR &p_sourceFile, const string &p_entryPoint, 
		const string &p_profile, ID3DBlob** p_blob);

	///-----------------------------------------------------------------------------------
	/// A helper function that creates and compiles all the shader stages specified
	/// \param p_filePath
	/// \param p_vs
	/// \param p_ps
	/// \return void
	///-----------------------------------------------------------------------------------
	void createAllShaderStages( VSData* p_vs=NULL, PSData* p_ps=NULL, GSData* p_gs=NULL, 
		HSData* p_hs=NULL, DSData* p_ds=NULL);

	///-----------------------------------------------------------------------------------
	/// A helper function that creates and configures the shader from the specified input
	/// \param p_shaderInitData
	/// \param p_inputLayout
	/// \param p_vsd
	/// \param p_psd
	/// \param p_samplerState
	/// \param p_gsd
	/// \param p_hsd
	/// \param p_dsd
	/// \return void
	///-----------------------------------------------------------------------------------
	void createShaderInitData(ShaderVariableContainer* p_shaderInitData,
		ID3D11InputLayout* p_inputLayout, VSData* p_vsd, PSData* p_psd=NULL,
		GSData* p_gsd=NULL, HSData* p_hsd=NULL, DSData* p_dsd=NULL);

	///-----------------------------------------------------------------------------------
	/// Creates a inputlayout for PTVertex
	/// \param p_vs
	/// \param p_inputLayout
	/// \return void
	///-----------------------------------------------------------------------------------
	void createPTVertexInputLayout(VSData* p_vs, ID3D11InputLayout** p_inputLayout);

	///-----------------------------------------------------------------------------------
	/// Creates a inputlayout for PNTVertex
	/// \param p_vs
	/// \param p_inputLayout
	/// \return void
	///-----------------------------------------------------------------------------------
	void createPNTVertexInputLayout(VSData* p_vs, ID3D11InputLayout** p_inputLayout);

	///-----------------------------------------------------------------------------------
	/// Creates a inputlayout for Instanced PNTVertex
	/// \param p_vs
	/// \param p_inputLayout
	/// \return void
	///-----------------------------------------------------------------------------------
	void createInstancedPNTVertexInputLayout(VSData* p_vs, 
											 ID3D11InputLayout** p_inputLayout);

	///-----------------------------------------------------------------------------------
	/// Creates a inputlayout for PNTTBVertex
	/// \param p_vs
	/// \param p_inputLayout
	/// \return void
	///-----------------------------------------------------------------------------------
	void createPNTTBVertexInputLayout(VSData* p_vs, ID3D11InputLayout** p_inputLayout);

	void createParticleInputLayout(VSData* p_vs, ID3D11InputLayout** p_inpuyLayout);

	///-----------------------------------------------------------------------------------
	/// Creates a inputlayout for Instanced PNTVertex
	/// \param p_vs
	/// \param p_inputLayout
	/// \return void
	///-----------------------------------------------------------------------------------
	void createInstancedPNTTBVertexInputLayout(VSData* p_vs, 
		ID3D11InputLayout** p_inputLayout);

	void createInstancedAnimatedPNTTBVertexInputLayout( VSData* p_vs, 
		ID3D11InputLayout** p_inputLayout );

	void createInstancedLightInputLayout( VSData* p_vertexShader,
		ID3D11InputLayout** p_inputLayout);

private:
	void constructInputLayout(const D3D11_INPUT_ELEMENT_DESC* p_inputDesc,
							  UINT p_numberOfElements,
							  VSData* p_vs, ID3D11InputLayout** p_inputLayout);

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	string m_shaderModelVersion;
	BufferFactory* m_bufferFactory;
};