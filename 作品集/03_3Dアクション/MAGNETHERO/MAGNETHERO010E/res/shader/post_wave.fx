//��ʑS�̂ɂȂ݂Ȃ݃G�t�F�N�g��������|�X�g�G�t�F�N�g�p�̃V�F�[�_�[�ł�

//���_�V�F�[�_�[�̓���
struct vs_in
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

//���_�V�F�[�_�[�̏o��
struct vs_out
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

//�s�N�Z���V�F�[�_�[�̓���
struct ps_in
{
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

//凋C�O�e�N�X�`��
texture mirage_tex;

//���ݎ���
float now;

//�G�t�F�N�g�����̏u��
float origin = -100.0f;

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

//�T���v���[
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

//�s�N�Z���V�F�[�_�[
ps_out ps_func(ps_in i)
{
	ps_out o = (ps_out)0;
	//�~����
	float pi = 3.14159265;
#if 0
	//�ۃO�l�O�l
	//�g��
	float L = 1.0f / 4.0f;
	//�U��
	float A = 1.0f / 40.0f;
	//����
	float T = 2.0f;
	//���S�ʒu���猻�݈ʒu�܂ł̃x�N�g��
	float2 d;
	d.x = i.uv.x - 0.5;
	d.y = i.uv.y - 0.5;
	//���̒������擾
	float dist = sqrt(d.x * d.x + d.y * d.y);
	//���K������
	float2 norm = normalize(d);
	//�����ɂ����UV���W��ς���
	float2 uv = i.uv + norm * sin(2.0f * pi * (now / T - dist / L)) * A;
	//�F�擾
	o.col = tex2D(tex_sampler, uv);
#endif


#if 0
	//�ۃO�l�O�l�A��
	float2 uv = i.uv;
	//���S�ʒu
	float2 center = float2(0.5f, 0.5f);
	//�U��
	float A = 1.0f / 20.0f;
	//���ݎ���
	float now_time = fmod(now, 1.0f);
	//�~�̑���
	float thickness = 0.1f;
	//�O���̉~�̔��a
	float outside_r = now_time;
	//�����̉~�̔��a
	float inside_r = now_time - thickness;
	//���S�ʒu����̈ʒu�x�N�g��
	float2 pos = uv - center;
	//���S�ʒu����̋���
	float pos_length = length(pos);
	//�G�t�F�N�g��������ʒu��
	if (outside_r > pos_length && pos_length > inside_r)
	{
		//���S�ʒu����̕���
		float2 normalized_pos = normalize(pos);
		//������
		uv -= normalized_pos * sin(lerp(0.0f, pi, (pos_length - inside_r) / (outside_r - inside_r))) * A;
	}
	//�F�擾
	o.col = tex2D(tex_sampler, uv);
#endif


#if 1
	//�ۃO�l�O�l�A��&���񂾂�ׂ��Ȃ�&�C�[�W���O���̗p
	float2 uv = i.uv;
	//���S�ʒu
	float2 center = float2(0.5f, 0.5f);
	//�U��
	float A = 1.0f / 20.0f;
	//�g����񕪂̎���
	float single_session_time = 1.5f;
	//���ݎ���
	float now_time = min(now - origin, single_session_time);
	//����(����/�ő�)
	float t = now_time / single_session_time;
	t = 1.0f - (1.0f - t) * (1.0f - t);
	//1-����
	float s = 1.0f - t;
	//�~�̑���
	float thickness = 0.2f * s;
	//�O���̉~�̔��a
	float outside_r = 1.5f * t;
	//�����̉~�̔��a
	float inside_r = outside_r - thickness;
	//���S�ʒu����̈ʒu�x�N�g��
	float2 pos = uv - center;
	//���S�ʒu����̋���
	float pos_length = length(pos);
	//�G�t�F�N�g��������ʒu��
	if (outside_r > pos_length && pos_length > inside_r)
	{
		//���S�ʒu����̕���
		float2 normalized_pos = normalize(pos);
		//������
		uv -= normalized_pos * sin(lerp(0.0f, pi, (pos_length - inside_r) / (outside_r - inside_r))) * A;
	}
	////�F�擾
	//float4 base_col = tex2D(tex_sampler, uv);
	////RGB�̕��ς����
	//float base = (base_col.r + base_col.g + base_col.b) / 3.0;
	////RGB�̕��U�����
	//float base_r = base_col.r - base;
	//float base_g = base_col.g - base;
	//float base_b = base_col.b - base;
	//float dispersion = base_r * base_r + base_g * base_g + base_b * base_b;
	////UV��ς���
	//uv.x += dispersion * pos_length * sin(now * 2) / 64.0;
	//uv.y += dispersion * pos_length * sin(now * 0.4) / 64.0;
	//���߂ĐF�擾
	o.col = tex2D(tex_sampler, uv);
#endif
	//�Ԃ�
	return o;
}

//�e�N�j�b�N
technique tec
{
	pass pass0
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
}
