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
	o.col = tex2D(tex_sampler, i.uv) * i.col * i.col.a;
	return o;
}

// テクニック
technique tec
{
	//パス0・ファクターブレンド
	pass P0
	{
		// レンダリングステート設定
		AlphaTestEnable = TRUE;
		AlphaBlendEnable = TRUE;
		SrcBlend = BLENDFACTOR;
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