#pragma once
#include "TextureParser.h"
#include <string>
#include <ResourceManager.h>
#include "Texture.h"

using namespace std;

// =======================================================================================
//                                      TextureFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # TextureFactory
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

class TextureFactory
{
public:
	TextureFactory(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext,
		ResourceManager<Texture>* p_textureManager);
	virtual ~TextureFactory() {}

	unsigned int createTexture(const string& p_name,
		const string& p_path);

	unsigned int createTexture( const byte* p_source, int p_width, int p_height,
		int p_pitch, int p_bitLevel, TextureParser::TEXTURE_TYPE p_type );

protected:
private:
	ResourceManager<Texture>* m_textureManager;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
};