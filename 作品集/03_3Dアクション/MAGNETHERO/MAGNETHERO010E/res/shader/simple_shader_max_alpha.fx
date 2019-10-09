
//型定義
struct vs_in
{
	float4 pos : POSITION;
	float3 norm : NORMAL;
	float2 uv : TEXCOORD;
};

struct vs_out
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
	float4 col : COLOR;
	//float fog : FOG;
};

struct ps_in
{
	float2 uv : TEXCOORD0;
	float4 col : COLOR;
	//float fog : FOG;
};

struct ps_out
{
	float4 col : COLOR;
};

//グローバル
float4x4 wvp; //ワールドから→ビュー→プロジェクションまでの座標変換行列　
float4x4 wit;  //ワールド行列の逆行列の転置行列
float4 light_dir;  //ライトの方向ベクトル
float4 light_intensity; //入射光の強度
float4 diffuse; //拡散反射率
float4 ambient; //環境光
texture tex; //メッシュのテクスチャ
float4 ambient2;//環境光その2
float time;//時間
float near_clip;//近クリップ面
float far_clip;//遠クリップ面

			   //サンプラー
sampler Sampler = sampler_state
{
	Texture = <tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU = Wrap;//Clamp;
	AddressV = Wrap;//Clamp;
};

//バーテックス・シェーダー
vs_out vs_func(vs_in i)
{
	vs_out o = (vs_out)(0);
	//ワールド座標からスクリーン座標へ
	o.pos = mul(i.pos, wvp);
	//UV座標の計算
	o.uv = i.uv;
	//ライトの計算
	float3 WNormal = normalize(mul(i.norm, (float3x3)wit));
	o.col = light_intensity * diffuse * max(dot(WNormal, light_dir), 0.0) + ambient;// +ambient2;
																					//カメラの位置からの距離を計算
																					//o.fog = (o.pos.z - near_clip) / (far_clip - near_clip);
	return o;
}

//ピクセル・シェーダー
ps_out ps_func(ps_in i)
{
	ps_out o = (ps_out)(0);
	float2 uv = i.uv;
	//カメラの位置からの距離に応じて揺らす
	//float z = i.fog;
	//色取得
	o.col = tex2D(Sampler, uv) * i.col;
	//アルファ値を最大まで上げる
	o.col.a = 1.0;
	return o;
}

// テクニック
technique tecMinimum
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
		VertexShader = compile vs_3_0 vs_func();
		PixelShader = compile ps_3_0 ps_func();
	}
}
