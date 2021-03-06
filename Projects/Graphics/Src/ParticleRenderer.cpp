#include "ParticleRenderer.h"
#include "ShaderFactory.h"
#include <d3d11.h>
#include <AglMatrix.h>
#include <AglParticleSystem.h>
#include "ParticleShader.h"
#include <AglStandardParticle.h>
#include "Texture.h"
#include "TextureParser.h"
#include "ParticleCBuffer.h"
#include <AglVector4.h>
#include <InstanceData.h>

ParticleRenderer::ParticleRenderer( ID3D11Device* p_device, 
								   ID3D11DeviceContext* p_deviceContext ){
	m_device = p_device;
	m_deviceContext = p_deviceContext;
	m_vertexBuffer = NULL;

	m_shaderFactory = new ShaderFactory(m_device,m_deviceContext,
		m_device->GetFeatureLevel());

	initShaders();
	initDepthStencil();
	initTexture();
}

ParticleRenderer::~ParticleRenderer(){
	SAFE_RELEASE(m_depthStencil);
	SAFE_RELEASE(m_vertexBuffer);
	delete m_shaderFactory;
	delete m_shader;
	delete m_stdTexture;
}

void ParticleRenderer::renderParticleSystem( AglParticleSystem* p_system,
											const InstanceData& p_transform, 
											Texture* p_texture, 
											const AglVector4& p_uvRect )
{
	vector<AglStandardParticle>* particles = p_system->getParticlesPtr();
	if (particles->size() > 0){
		SAFE_RELEASE(m_vertexBuffer);
		D3D11_BUFFER_DESC bD;
		ZeroMemory(&bD, sizeof(bD));
		bD.Usage			= D3D11_USAGE_DYNAMIC;
		bD.ByteWidth		= sizeof(AglStandardParticle)* particles->size();
		bD.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		bD.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA vD;
		vD.pSysMem			= &(*particles)[0];
		vD.SysMemPitch		= 0;
		vD.SysMemSlicePitch	= 0;

		m_device->CreateBuffer(&bD, &vD, &m_vertexBuffer);
		
		Buffer<ParticleCBuffer>* data = m_shader->getPerSystemBuffer();
		data->accessBuffer.setParticleData(p_system->getHeader(), p_transform.worldTransform);
		data->accessBuffer.setRect(p_uvRect);

		renderParticles(p_system, particles->size(), p_texture );
	}
}

void ParticleRenderer::renderParticles( AglParticleSystem* p_system, 
									  const int numOfParticles, Texture* p_texture ){
	ID3D11DepthStencilState* old;
	UINT stencil;
	m_deviceContext->OMGetDepthStencilState(&old, &stencil);
	m_deviceContext->OMSetDepthStencilState(m_depthStencil, 1);

	m_shader->apply();

	UINT stride = sizeof(AglStandardParticle);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	if( p_texture == NULL )
		p_texture = m_stdTexture;
	m_deviceContext->PSSetShaderResources(0, 1, &p_texture->data);

	m_deviceContext->Draw(numOfParticles, 0);

	m_shader->unApply();
	m_deviceContext->OMSetDepthStencilState(old, 1);

}

void ParticleRenderer::initShaders(){
	m_shader = m_shaderFactory->createParticleShader(
		L"Shaders/ParticleEffects/particleShader.hlsl");
}

void ParticleRenderer::initDepthStencil(){
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencil);
}

void ParticleRenderer::initTexture()
{
	m_stdTexture = new Texture(TextureParser::loadTexture(
		m_device,m_deviceContext,"Assets/Textures/Test/smoke.png"));
}