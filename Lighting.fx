cbuffer cbData
{
	float4x4 World; 
	float4x4 View; 
	float4x4 Projection;

	float4 gDiffuseMtrl; 
	float4 gDiffuseLight; 
	float3 gLightVecW;
};

struct VS_IN
{
	float4 posL   : POSITION;
	float3 normalL : NORMAL;
};

struct VS_OUT
{
	float4 Pos    : SV_POSITION;
	float4 Col    : COLOR;
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT output = (VS_OUT)0;

	output.Pos = mul( vIn.posL, World ); 
	output.Pos = mul( output.Pos, View ); 
	output.Pos = mul( output.Pos, Projection );

	// Convert from local to world normal
	float3 normalW = mul(float4(vIn.normalL, 0.0f), World).xyz;
	normalW = normalize(normalW);

	// Compute Colour
	float s = max(dot(gLightVecW, normalW), 0.0f); 
	output.Col.rgb = s*(gDiffuseMtrl*gDiffuseLight).rgb; 
	output.Col.a = gDiffuseMtrl.a;
 
	return output;
}

float4 PS(VS_OUT pIn) : SV_Target
{
	return pIn.Col;
}

technique11 Render
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) ); SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}
