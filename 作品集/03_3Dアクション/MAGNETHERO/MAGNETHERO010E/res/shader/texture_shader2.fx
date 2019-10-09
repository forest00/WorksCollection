//�v���~�e�B�u���e�N�X�`���t���ŕ`�悵�����Ƃ��Ɏg���V�F�[�_�[�ł�
//�������ɑΉ����Ă��܂�

//���_�V�F�[�_�[�̓���
struct vs_in
{
	float4 col : COLOR;
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

//���_�V�F�[�_�[�̏o��
struct vs_out
{
	float4 col : COLOR;
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

//�s�N�Z���V�F�[�_�[�̓���
struct ps_in
{
	float4 col : COLOR;
	float2 uv : TEXCOORD;
};

//�s�N�Z���V�F�[�_�[�̏o��
struct ps_out
{
	float4 col : COLOR;
};


//���[���h���灨�r���[���v���W�F�N�V�����܂ł̍��W�ϊ��s��@
float4x4 wvp;

//�e�N�X�`��
texture tex;

//�T���v���[
sampler tex_sampler = sampler_state
{
	Texture = <tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU = Wrap;//Clamp;
	AddressV = Wrap;//Clamp;
};

//���_�V�F�[�_�[
vs_out vs_func(vs_in i)
{
	vs_out o;
	o.col = i.col;
	o.pos = mul(i.pos, wvp);
	o.uv = i.uv;
	return o;
}

//�s�N�Z���V�F�[�_�[
ps_out ps_func(ps_in i)
{
	ps_out o;
	o.col = tex2D(tex_sampler, i.uv) * i.col * i.col.a;
	return o;
}

// �e�N�j�b�N
technique tec
{
	//�p�X0�E�t�@�N�^�[�u�����h
	pass P0
	{
		// �����_�����O�X�e�[�g�ݒ�
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