//プリミティブを単色で描画したいときに使うシェーダーです

//ワールドから→ビュー→プロジェクションまでの3D変換行列　
float4x4 wvp;

//描画したい色
float4 diffuse;

//頂点シェーダー
float4 vs_func(float4 f4Position : POSITION) : POSITION
{
	//行列によって変換
	return mul(f4Position, wvp);
}

//ピクセルシェーダー
float4 ps_func() : COLOR
{
	//単色を出力
	return diffuse;
}

//テクニック
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