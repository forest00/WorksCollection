//プリミティブをテクスチャ付きで描画したいときに使うシェーダーです
//半透明に対応しています

//頂点シェーダーの入力
struct vs_in
{
	float4 col : COLOR;
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

//頂点シェーダーの出力
struct vs_out
{
	float4 col : COLOR;
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

//ピクセルシェーダーの入力
struct ps_in
{
	float4 col : COLOR;
	float2 uv : TEXCOORD;
};

//ピクセルシェーダーの出力
struct ps_out
{
	float4 col : COLOR;
};


//ワールドから→ビュー→プロジェクションまでの座標変換行列　
float4x4 wvp;

//テクスチャ
texture tex;

//サンプラー
sampler tex_sampler = sampler_state
{
	Texture = <tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU = Wrap;//Clamp;
	AddressV = Wrap;//Clamp;
};

//頂点シェーダー
vs_out vs_func(vs_in i)
{
	vs_out o;
	o.col = i.col;
	o.pos = mul(i.pos, wvp);
	o.uv = i.uv;
	return o;
}

//ピクセルシェーダー
ps_out ps_func(ps_in i)
{
	ps_out o;
	o.col = tex2D(tex_sampler, i.uv) * i.col;
	return o;
}

// テクニック
technique tec
{
	//パス0・ブレンド無し
	pass P0
	{
		// レンダリングステート設定
		AlphaTestEnable = TRUE;
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCALPHA;
		DestBlend = ZERO;
		ColorOp[0] = SELECTARG1;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = DIFFUSE;
		AlphaOp[0] = SELECTARG1;
		AlphaArg1[0] = TEXTURE;
		AlphaArg2[0] = DIFFUSE;
		ColorOp[1] = DISABLE;
		AlphaOp[1] = DISABLE;
		ZEnable = true;
		ZWriteEnable = true;
		VertexShader = compile vs_2_0 vs_func();
		PixelShader = compile ps_2_0 ps_func();
	}
	//パス1・アルファブレンド
	pass P1
	{
		// レンダリングステート設定
		AlphaTestEnable = TRUE;
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		ColorOp[0] = MODULATE;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = CURRENT;
		AlphaOp[0] = MODULATE;
		AlphaArg1[0] = TEXTURE;
		AlphaArg2[0] = CURRENT;
		ColorOp[1] = DISABLE;
		AlphaOp[1] = DISABLE;
		ZEnable = true;
		ZWriteEnable = false;
		VertexShader = compile vs_2_0 vs_func();
		PixelShader = compile ps_2_0 ps_func();
	}
	//パス2・加算ブレンド
	pass P2
	{
		// レンダリングステート設定
		AlphaTestEnable = TRUE;
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCALPHA;
		DestBlend = ONE;
		ColorOp[0] = SELECTARG1;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = DIFFUSE;
		AlphaOp[0] = SELECTARG1;
		AlphaArg1[0] = TEXTURE;
		AlphaArg2[0] = DIFFUSE;
		ColorOp[1] = DISABLE;
		AlphaOp[1] = DISABLE;
		ZEnable = true;
		ZWriteEnable = false;
		VertexShader = compile vs_2_0 vs_func();
		PixelShader = compile ps_2_0 ps_func();
	}
}