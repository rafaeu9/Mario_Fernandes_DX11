TextureCube cube0;
SamplerState sampler0;

struct VOut
{
	float4 position : SV_POSITION;
    float3 texcoord : TEXCOORD;
};

cbuffer CBuffer0
{
    matrix WVPMatrix;	                    //64 bytes

}//Total Size = 64 bytes

VOut VShader(float4 position : POSITION, float3 texcoord : TEXCOORD)
{
	VOut output;
        
    output.texcoord = position.xyz;
    output.position = mul(WVPMatrix, position);        
    
    return output;
}

float4 PShader(float4 position : SV_POSITION, float3 texcoord : TEXCOORD) : SV_TARGET
{
    return cube0.Sample(sampler0, texcoord);
}