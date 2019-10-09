//�e�w�b�_�[
#include "sprite_integration.h"
//���̑��̃w�b�_�[
#include "aiming_cursor.h"
#include "bibibi.h"
#include "qte.h"

//������
extern void sprite_integration_init()
{
	aiming_cursor_init();
	bibibi_init();
	qte_init();
}

//�I��
extern void sprite_integration_end()
{
	aiming_cursor_end();
	bibibi_end();
	qte_end();
}

//�X�V
extern void sprite_integration_update()
{
	aiming_cursor_update();
	bibibi_update();
	qte_update();
}

//�`��
extern void sprite_integration_draw()
{
	//�Ə��J�[�\���`��
	aiming_cursor_draw();
	//�т�т�effect�`��
	bibibi_draw();
	//QTE�K�C�h�`��
	qte_draw();
}

//�т�т�
extern void sprite_integration_bibibi_fire(int base, int random_range)
{
	bibibi_fire(base, random_range);
}

//QTE�J�n
extern void sprite_integration_qte_start()
{
	qte_start();
}

//QTE��������
extern bool sprite_integration_qte_judge()
{
	return qte_judge();
}

//QTE�^�C���A�E�g�`�F�b�N
extern bool sprite_integration_qte_timeout()
{
	return qte_timeout();
}
