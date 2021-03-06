#include "ParticleShader.h"
#include "Camera.h"
#include "TextureManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "Scene.h"

ParticleShader::ParticleShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, VertexShader* pVS, PixelShader* pPS, GeometryShader* pGS )
{
	mDevice			= pDevice;
	mDeviceContext	= pDeviceContext;

	mVS = pVS;
	mPS = pPS;
	mGS = pGS;

	D3D11_INPUT_ELEMENT_DESC PositionNormalTexCoord[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"ANGULARVELOCITY", 0, DXGI_FORMAT_R32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"ROTATION", 0, DXGI_FORMAT_R32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mDevice->CreateInputLayout(PositionNormalTexCoord, 6, mVS->GetCompiledData(), mVS->GetCompiledSize(), &mIL);

	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(ParticleBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	mDevice->CreateBuffer(&BufferDesc, NULL, &mBuffer);
}

ParticleShader::~ParticleShader()
{
	mIL->Release();
}

ID3D11VertexShader* ParticleShader::GetVertexShader()
{
	return mVS->GetShader();
}

ID3D11PixelShader* ParticleShader::GetPixelShader()
{
	return mPS->GetShader();
}

ID3D11GeometryShader* ParticleShader::GetGeometryShader()
{
	return mGS->GetShader();
}

ID3D11InputLayout* ParticleShader::GetInputLayout()
{
	return mIL;
}
void ParticleShader::SetBuffer(ParticleSystem* pPS)
{
	float scale = pPS->getScale();
	AglMatrix w = AglMatrix::createScaleMatrix(AglVector3(scale, scale, scale));

	AglMatrix v = Camera::GetInstance()->GetViewMatrix();
	AglMatrix p = Camera::GetInstance()->GetProjectionMatrix();

	if (pPS->getParticleSystem()->getParticleSpace() == AglParticleSystemHeader::AglSpace_SCREEN)
	{
		v = p = AglMatrix::identityMatrix();
		w = AglMatrix::identityMatrix();
	}

	AglVector3 pos = Camera::GetInstance()->GetPosition();
	v = v.transpose();
	p = p.transpose();

	D3D11_MAPPED_SUBRESOURCE resource;
	ParticleBuffer* buffer;
	mDeviceContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	buffer = (ParticleBuffer*)resource.pData;
	buffer->World = w;
	buffer->View = v;
	buffer->Projection = p;
	buffer->EyePosition = AglVector4(pos.x, pos.y, pos.z, 1);
	buffer->Color = pPS->getColor();
	buffer->FadeIn = pPS->getFadeInStop();
	buffer->FadeOut = pPS->getFadeOutStart();
	buffer->ParticleMaxAge = pPS->GetHeader().particleAge;
	buffer->Opacity = pPS->GetHeader().maxOpacity;
	buffer->scale	= 1.0f;
	buffer->space = pPS->getParticleSystem()->getParticleSpace();

	AglVector3 y = Camera::GetInstance()->LocalYAxis();
	AglVector3 z = Camera::GetInstance()->LocalZAxis();
	buffer->CameraY = AglVector4(y.x, y.y, y.z, 0);
	buffer->CameraZ = AglVector4(z.x, z.y, z.z, 0);

	if (pPS->getAlignment() == AglParticleSystemHeader::OBSERVER)
		buffer->Alignment = 0;
	else if (pPS->getAlignment() == AglParticleSystemHeader::SCREEN)
		buffer->Alignment = 1;
	else if (pPS->getAlignment() == AglParticleSystemHeader::WORLD)
		buffer->Alignment = 2;
	else
		buffer->Alignment = 3;

	mDeviceContext->Unmap(mBuffer, 0);

	unsigned int bufferNumber = 0;
	mDeviceContext->GSSetConstantBuffers(bufferNumber, 1, &mBuffer);

	int textureIndex = pPS->getTextureIndex();
	if (textureIndex >= 0)
	{
		string s = Scene::GetInstance()->GetName(textureIndex);
		TextureData* data = TextureManager::GetInstance()->GetTexture(s);
		if (data)
			mDeviceContext->PSSetShaderResources(0, 1, &data->SRV);
	}
}

