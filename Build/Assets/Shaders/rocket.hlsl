cbuffer VertexProgramCBuffer
{
    float4 color;
	float4x4 vp;
};

Texture2D diffuseTexture : register(t0);

SamplerState pointSampler : register(s0);

struct VertexIn
{
	float3 position : POSITION;	
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD; 
	float3 tangent : TANGENT;	
	float3 binormal : BINORMAL;
	float4x4 instanceTransform : INSTANCETRANSFORM;
};

struct VertexOut
{
    float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
	float3 normal : NORMAL;
};

VertexOut VS(VertexIn p_input)
{
	VertexOut vout;

	vout.position = mul(float4(p_input.position,1.0f), p_input.instanceTransform);
	vout.texCoord = p_input.texCoord;
    
	return vout;
}

float4 PS(VertexOut p_input) : SV_TARGET
{
	float2 uv = p_input.texCoord;
	float4 Col = diffuseTexture.Sample( pointSampler, uv );
	return Col;
}

