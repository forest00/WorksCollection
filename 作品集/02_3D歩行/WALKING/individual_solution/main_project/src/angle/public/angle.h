#pragma once

//角度に関する計算をする関数を提供します


//angle_t型です
//double型と同じですが、角度を扱うときはこっちを使うことにしてください
typedef double angle_t;


//マイナスπからプラスπの数値にしたものを返します
extern angle_t angle_pi_to_pi(angle_t angle);
//目標の角度に近づけたものを返します
extern angle_t angle_follow(angle_t angle, angle_t target, angle_t turning_force);
