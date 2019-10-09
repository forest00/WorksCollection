//画面全体になみなみエフェクトをかけるポストエフェクト用のシェーダーです

//頂点シェーダーの入力
struct vs_in
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

//頂点シェーダーの出力
struct vs_out
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

//ピクセルシェーダーの入力
struct ps_in
{
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

//蜃気楼テクスチャ
texture mirage_tex;

//現在時間
float now;

//エフェクト発生の瞬間
float origin = -100.0f;

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

//サンプラー
sampler mirage_tex_sampler = sampler_state
{
	Texture = <mirage_tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU = Clamp;
	AddressV = Clamp;
};

vs_out vs_func(vs_in i)
{
	vs_out o = (vs_out)0;
	o.pos = mul(i.pos, wvp);
	o.uv = i.uv;
	return o;
}

//ピクセルシェーダー
ps_out ps_func(ps_in i)
{
	ps_out o = (ps_out)0;
	//円周率
	float pi = 3.14159265;
#if 0
	//丸グネグネ
	//波長
	float L = 1.0f / 4.0f;
	//振幅
	float A = 1.0f / 40.0f;
	//周期
	float T = 2.0f;
	//中心位置から現在位置までのベクトル
	float2 d;
	d.x = i.uv.x - 0.5;
	d.y = i.uv.y - 0.5;
	//その長さを取得
	float dist = sqrt(d.x * d.x + d.y * d.y);
	//正規化する
	float2 norm = normalize(d);
	//距離によってUV座標を変える
	float2 uv = i.uv + norm * sin(2.0f * pi * (now / T - dist / L)) * A;
	//色取得
	o.col = tex2D(tex_sampler, uv);
#endif


#if 0
	//丸グネグネ連続
	float2 uv = i.uv;
	//中心位置
	float2 center = float2(0.5f, 0.5f);
	//振幅
	float A = 1.0f / 20.0f;
	//現在時刻
	float now_time = fmod(now, 1.0f);
	//円の太さ
	float thickness = 0.1f;
	//外側の円の半径
	float outside_r = now_time;
	//内側の円の半径
	float inside_r = now_time - thickness;
	//中心位置からの位置ベクトル
	float2 pos = uv - center;
	//中心位置からの距離
	float pos_length = length(pos);
	//エフェクトがかかる位置か
	if (outside_r > pos_length && pos_length > inside_r)
	{
		//中心位置からの方向
		float2 normalized_pos = normalize(pos);
		//動かす
		uv -= normalized_pos * sin(lerp(0.0f, pi, (pos_length - inside_r) / (outside_r - inside_r))) * A;
	}
	//色取得
	o.col = tex2D(tex_sampler, uv);
#endif


#if 1
	//丸グネグネ連続&だんだん細くなる&イージングを採用
	float2 uv = i.uv;
	//中心位置
	float2 center = float2(0.5f, 0.5f);
	//振幅
	float A = 1.0f / 20.0f;
	//波動一回分の時間
	float single_session_time = 1.5f;
	//現在時刻
	float now_time = min(now - origin, single_session_time);
	//割合(現在/最大)
	float t = now_time / single_session_time;
	t = 1.0f - (1.0f - t) * (1.0f - t);
	//1-割合
	float s = 1.0f - t;
	//円の太さ
	float thickness = 0.2f * s;
	//外側の円の半径
	float outside_r = 1.5f * t;
	//内側の円の半径
	float inside_r = outside_r - thickness;
	//中心位置からの位置ベクトル
	float2 pos = uv - center;
	//中心位置からの距離
	float pos_length = length(pos);
	//エフェクトがかかる位置か
	if (outside_r > pos_length && pos_length > inside_r)
	{
		//中心位置からの方向
		float2 normalized_pos = normalize(pos);
		//動かす
		uv -= normalized_pos * sin(lerp(0.0f, pi, (pos_length - inside_r) / (outside_r - inside_r))) * A;
	}
	////色取得
	//float4 base_col = tex2D(tex_sampler, uv);
	////RGBの平均を取る
	//float base = (base_col.r + base_col.g + base_col.b) / 3.0;
	////RGBの分散を取る
	//float base_r = base_col.r - base;
	//float base_g = base_col.g - base;
	//float base_b = base_col.b - base;
	//float dispersion = base_r * base_r + base_g * base_g + base_b * base_b;
	////UVを変える
	//uv.x += dispersion * pos_length * sin(now * 2) / 64.0;
	//uv.y += dispersion * pos_length * sin(now * 0.4) / 64.0;
	//改めて色取得
	o.col = tex2D(tex_sampler, uv);
#endif
	//返す
	return o;
}

//テクニック
technique tec
{
	pass pass0
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
}
