// =======================================================================================
//                                      ShaderVariableContainer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains the needed information to create a basic shader
///        
/// # ShaderInitStruct
/// Detailed description.....
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include "ShaderStageData.h"

struct ShaderVariableContainer
{
	ID3D11DeviceContext* deviceContext;

	VSData* vertexShader;
	GSData* geometryShader;
	DSData* domainShader;
	HSData* hullShader;
	PSData* pixelShader; 

	ID3D11InputLayout* inputLayout;
};