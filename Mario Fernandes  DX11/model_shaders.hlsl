cbuffer CB0
{
    matrix WVPMatrix;   //64 bytes
    float4 directional_light_vector; //16 bytes
    float4 directional_light_colour; //16 bytes
    float4 ambient_light_colour; //16 bytes
};//Total Size = 112 bytes

Texture2D texture0;
SamplerState sampler0;

struct VOut
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;    //Note the spelling of this and all instances below
    float2 texcoord : TEXCOORD;
    float3 normal   : NORMAL;
};


VOut ModelVS(float4 position : POSITION, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut output;
      
    float4 default_color = { 1.0f, 1.0f, 1.0f, 1.0f };
    output.position = mul(WVPMatrix, position);
    
    float diffusse_amount = dot(directional_light_vector, normal);
    diffusse_amount = saturate(diffusse_amount);
    
    output.texcoord = texcoord; 
    
    output.color = ambient_light_colour + (directional_light_colour * diffusse_amount);
    
    return output;
}

float4 ModelPS(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
    return color * texture0.Sample(sampler0, texcoord);
}