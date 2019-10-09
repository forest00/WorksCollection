//�v���~�e�B�u��P�F�ŕ`�悵�����Ƃ��Ɏg���V�F�[�_�[�ł�

//���[���h���灨�r���[���v���W�F�N�V�����܂ł�3D�ϊ��s��@
float4x4 wvp;

//�`�悵�����F
float4 diffuse;

//���_�V�F�[�_�[
float4 vs_func(float4 f4Position : POSITION) : POSITION
{
	//�s��ɂ���ĕϊ�
	return mul(f4Position, wvp);
}

//�s�N�Z���V�F�[�_�[
float4 ps_func() : COLOR
{
	//�P�F���o��
	return diffuse;
}

//�e�N�j�b�N
technique tec
{
	//�p�X0�E�u�����h����
	pass P0
	{
		// �����_�����O�X�e�[�g�ݒ�
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
	//�p�X1�E�A���t�@�u�����h
	pass P1
	{
		// �����_�����O�X�e�[�g�ݒ�
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
	//�p�X2�E���Z�u�����h
	pass P2
	{
		// �����_�����O�X�e�[�g�ݒ�
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