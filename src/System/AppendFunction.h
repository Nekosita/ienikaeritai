#pragma once
#include <math.h>

//クラスの前方宣言
class Base;
class BlockBase;
class ModelBase;
class Player;
class Npc;
class Zonbie;
class Boss;
class RedGoblin;

//!距離取る用
float GetDistance(const VECTOR& pos1, const VECTOR& pos2);

//!ステージのギミックとの接触確認
bool AttachCheck(const VECTOR &pos1, float r1, const VECTOR &pos2, float r2);
bool AttachCheck2D(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2);
bool CheckInTrigger(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3);

//!接触確認
bool AttachCheck_PlayertoBoss(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2);
bool CheckHitStageObject(const Base* obj1, const Base* obj2, float addjust_y = 0.0f);
bool IsHitBall(const ModelBase* ball, const ModelBase* player);
bool AttachCheck_BalltoChractor(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2, float adjest_y);
bool CheckHitWithEnemy(const Base* obj1, const ModelBase* obj2);
//!ゴブリン用
bool CheckCircleHit_WithGoblin(const Player* obj1, const ModelBase* obj2);
//!ボス用
bool CheckCircleHit_WithBoss(const Player* obj1, const ModelBase* obj2);
//!ボス用
bool CheckCircleHit_WithZonbie(const Player* obj1, const ModelBase* obj2);
bool CheckCircleHit_WithZonbieHand(const Player* obj1, const Zonbie* obj2);

//!最後ぼブロック　
bool CheckSquareHit_WithPlayerandNPC(const Player* obj1, const Npc* obj2, const BlockBase* obj);

//ゴブリンの弾丸ととプレイヤーとの当たり判定
bool BulletAttachCheckToNpc(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2);
bool BulletAttachCheckToplayer(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2);

//ゴブリンの右手とプレイヤーとの当たり判定
//bool RedGoblinAttachCheckToplayer(const Player& pos1,const RedGoblin& pos2);

bool AttachCheckWithPlayer(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2);
