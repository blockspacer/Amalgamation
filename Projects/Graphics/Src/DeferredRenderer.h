#pragma once

#include "Buffer.h"
#include "D3DException.h"
#include "PTVertex.h"
#include "RendererSceneInfo.h"
#include "ResourceManager.h"
#include "RenderStateHelper.h"
#include <InstanceData.h>
#include <d3d11.h>

class BufferFactory;
class ShaderBase;
class DeferredBaseShader;
class DeferredComposeShader;
class Mesh;
class GUIShader;
class ShaderFactory;
struct Texture;
class RenderStateHelper;
struct RasterizerFillMode;
struct RasterizerCullMode;
struct RasterizerFaceVertexOrder;
struct BlendState;
struct LightInstanceData;

// =======================================================================================
//                                      DeferredRenderer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Deferred Pipeline
///        
/// # DeferredRenderer
/// The Geometry buffers (gBuffers) order is, depth, diffuse, normal.
/// Created on: 29-11-2012 
///---------------------------------------------------------------------------------------

class DeferredRenderer
{
public:
	/************************************************************************/
	/* See wiki for more details.											*/
	/* https://github.com/BiceMaster/PA2505-Stort-Spelprojekt-Kod/wiki/GBuffers */
	/************************************************************************/
	const static int BASESHADERS = 3;
	const static int RT0 = 0;
	const static int RT1 = 1;
	const static int RT2 = 2;
	const static int RT3 = 3;
	const static int RT4 = 4;
	enum RenderTargets{
		DIFFUSE, NORMAL, SPECULAR, LIGHT, DEPTH, NUMTARGETS,
	};
public:
	// ===================================================================================
	// Setup
	// ===================================================================================
	DeferredRenderer(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
		int p_width, int p_height);
	virtual ~DeferredRenderer();

	///-----------------------------------------------------------------------------------
	/// Clear the buffers used by the deferred renderer.
	/// \return void
	///-----------------------------------------------------------------------------------
	void clearBuffers();

	///-----------------------------------------------------------------------------------
	/// Sets the scene info, which can be regarded as "global" information to be used 
	/// when rendering. For example a world-view-projection matrix.
	/// \param p_sceneInfo
	/// \return void
	///-----------------------------------------------------------------------------------
	void setSceneInfo(const RendererSceneInfo& p_sceneInfo);

	void setBasePassRenderTargets();


	// ===================================================================================
	// Mesh Render
	// ===================================================================================

	///-----------------------------------------------------------------------------------
	/// Render instanced mesh data
	/// \param p_mesh
	/// \param p_textureArray
	/// \param p_textureArraySize
	/// \param p_instanceBuffer
	/// \return void
	///-----------------------------------------------------------------------------------
	void renderMeshInstanced(Mesh* p_mesh,
							 Texture** p_textureArray,
							 unsigned int p_textureArraySize,
							 Buffer<InstanceData>* p_instanceBuffer );

	void renderInstanced( Mesh* p_mesh, ShaderBase* p_shader,
		Buffer<InstanceData>* p_instanceBuffer );

	// HACK: DUPLICATE of above but with LightMesh instead of Mesh
	void renderInstanced( LightMesh* p_mesh, ShaderBase* p_shader,
		Buffer<LightInstanceData>* p_instanceBuffer );

	///-----------------------------------------------------------------------------------
	/// Render a full screen quad textured with the gbuffer.
	/// \return void
	///-----------------------------------------------------------------------------------
	void mapDeferredBaseRTSToShader();

	void mapVariousPassesToComposeStage();
	void unmapVariousPassesFromComposeStage();

	void unmapDepthFromShaderVariables();
	void renderLights( LightMesh* p_mesh, Buffer<LightInstanceData>* p_instanceBuffer );

	// ===================================================================================
	// GUI Render
	// ===================================================================================

	// ===================================================================================
	// Blend states
	// ===================================================================================

	///-----------------------------------------------------------------------------------
	/// Set blend state to draw with
	/// \return void
	///-----------------------------------------------------------------------------------
	void setBlendState(BlendState::Mode p_state);
	
	void setBlendFactors(float p_red, float p_green, float p_blue, float p_alpha);
	void setBlendFactors(float p_oneValue);

	void setBlendMask(UINT p_mask);

	void setLightRenderTarget();

	BlendState::Mode getCurrentBlendStateType() {return m_currentBlendStateType;}

	// ===================================================================================
	// Rasterizer States
	// ===================================================================================

	///-----------------------------------------------------------------------------------
	/// Set settings for rasterizer states
	/// \return void
	///-----------------------------------------------------------------------------------
	void setRasterizerStateSettings(RasterizerState::Mode p_state);

	RasterizerState::Mode getCurrentRasterizerStateType() {return m_currentRasterizerStateType;}

	void releaseRenderTargetsAndDepthStencil();
	void initRendertargetsAndDepthStencil( int p_width, int p_height );

	ID3D11DepthStencilView* getDepthStencil();

	void renderComposeStage();

	// ===================================================================================
	// Debug
	// ===================================================================================
	void hookUpAntTweakBar();

private:
	void initDepthStencil();
	void initGeometryBuffers();
	void buildBlendStates();
	void buildRasterizerStates();
	void unMapGBuffers();
	void initShaders();
	void initFullScreenQuad();
private:
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;

	ShaderFactory*			m_shaderFactory;
	BufferFactory*			m_bufferFactory;

	RendererSceneInfo		m_sceneInfo;

	ID3D11RenderTargetView*		m_gBuffers[NUMTARGETS];
	ID3D11ShaderResourceView*	m_gBuffersShaderResource[NUMTARGETS];
	ID3D11DepthStencilView*		m_depthStencilView;

	DeferredBaseShader*		m_baseShader;
	DeferredBaseShader*		m_lightShader;
	DeferredComposeShader*	m_composeShader;

	Buffer<PTVertex>* m_fullscreenQuad;

	// blend states
	vector<ID3D11BlendState*> m_blendStates;
	BlendState::Mode m_currentBlendStateType;
	float m_blendFactors[4];
	UINT m_blendMask;

	// rasterizer states
	vector<ID3D11RasterizerState*> m_rasterizerStates;
	RasterizerState::Mode m_currentRasterizerStateType;

	int m_width;
	int m_height;
};