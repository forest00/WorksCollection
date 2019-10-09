#pragma once
//	
//	冬期制作「Magnet Hero」
//	
//	作成者	森	仁志
//	
//	最終更新		2019/01/30
//	

//楽曲のindex
//使用する楽曲が増えたらここも増える
enum sound_index_t
{
	si_none,
	si_bgm_bgm,						//BGM(?)
	si_bgm_title,					//タイトルで流すBGM
	si_se_beam,						//磁力発射の瞬間の効果音
	si_se_footstep,					//自機の地上足音
	si_se_jump,						//ジャンプした瞬間
	si_se_magarrive,				//自機がビームの衝突相手とくっついた瞬間
	si_se_magfly,					//くっつく移動開始時
	si_se_maghit,					//磁力が対象にヒットしたとき
	si_se_maghover,					//浮遊開始時
	si_se_voice_goal,				//ゴール時
	si_se_voice_magjump,			//磁力命中時　どちらかをランダムに再生
	si_se_voice_magjump2,			//磁力命中時　どちらかをランダムに再生
	si_se_voice_miss,				//ステージ外落下時
	si_se_voice_start,				//ステージ開始時
	si_se_voice_title_call,			//タイトル描画時
	si_max,
};

