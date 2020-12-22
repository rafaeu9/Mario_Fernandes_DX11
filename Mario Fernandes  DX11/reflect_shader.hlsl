cbuffer CB0
{
    matrix WVPMatrix;   //64 bytes
    matrix worldViewMatrix; //64 bytes

}

TextureCube cube0;
SamplerState sampler0;

struct VOut
{
	float4 position : SV_POSITION;
    float3 texcoord : TEXCOORD;
    float3 normal   : NORMAL;
};


VOut ModelVS(float4 position : POSITION, float3 texcoord : TEXCOORD, float3 normal: NORMAL)
{
	VOut output;
      
    float4 default_color = { 1.0f, 1.0f, 1.0f, 1.0f };
    output.position = mul(WVPMatrix, position);

    float3 wvpos = mul(worldViewMatrix, position);
    
    float3 wvnormal = mul(worldViewMatrix, normal);
    wvnormal = normalize(wvnormal);
    
    float3 eyer = -normalize(wvpos);
    
    output.texcoord = 2.0 * dot(eyer, wvnormal) * wvnormal - eyer;
    
    return output;
}

float4 ModelPS(float4 position : SV_POSITION, float4 color : COLOR, float3 texcoord : TEXCOORD) : SV_TARGET
{
    return cube0.Sample(sampler0, texcoord);
}