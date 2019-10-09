
//�^��`
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

//�O���[�o��
float4x4 wvp; //���[���h���灨�r���[���v���W�F�N�V�����܂ł̍��W�ϊ��s��@
float4x4 wit;  //���[���h�s��̋t�s��̓]�u�s��
float4 light_dir;  //���C�g�̕����x�N�g��
float4 light_intensity; //���ˌ��̋��x
float4 diffuse; //�g�U���˗�
float4 ambient; //����
texture tex; //���b�V���̃e�N�X�`��
float4 ambient2;//��������2
float time;//����
float near_clip;//�߃N���b�v��
float far_clip;//���N���b�v��

			   //�T���v���[
sampler Sampler = sampler_state
{
	Texture = <tex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU = Wrap;//Clamp;
	AddressV = Wrap;//Clamp;
};

//�o�[�e�b�N�X�E�V�F�[�_�[
vs_out vs_func(vs_in i)
{
	vs_out o = (vs_out)(0);
	//���[���h���W����X�N���[�����W��
	o.pos = mul(i.pos, wvp);
	//UV���W�̌v�Z
	o.uv = i.uv;
	//���C�g�̌v�Z
	float3 WNormal = normalize(mul(i.norm, (float3x3)wit));
	o.col = light_intensity * diffuse * max(dot(WNormal, light_dir), 0.0) + ambient;// +ambient2;
																					//�J�����̈ʒu����̋������v�Z
																					//o.fog = (o.pos.z - near_clip) / (far_clip - near_clip);
	return o;
}

//�s�N�Z���E�V�F�[�_�[
ps_out ps_func(ps_in i)
{
	ps_out o = (ps_out)(0);
	float2 uv = i.uv;
	//�J�����̈ʒu����̋����ɉ����ėh�炷
	//float z = i.fog;
	//�F�擾
	o.col = tex2D(Sampler, uv) * i.col;
	//�A���t�@�l���ő�܂ŏグ��
	o.col.a = 1.0;
	return o;
}

// �e�N�j�b�N
technique tecMinimum
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
		VertexShader = compile vs_3_0 vs_func();
		PixelShader = compile ps_3_0 ps_func();
	}
}
