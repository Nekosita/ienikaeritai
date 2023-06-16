#include "DxLib.h"
#include "../src/Game/Object/Base.h"
#include "../src/Game/Object/BlockBase.h"
#include "../src/Game/Object/Model/Model.h"
#include "../src/Game/Object/Model/Player.h"
#include "../src/Game/Object/Model/Redgoblin.h"
#include "../src/Game/Object/Model/ShokeWave.h"
#include "../src/Game/Object/Model/Npc.h"
#include "../src/Game/Object/Model/Boss.h"
#include "../src/Game/Object/Model/Zonbie.h"
#include "Vector3.h"
#include "AppendFunction.h"

//接触関数3D
//-------------------------------------
bool AttachCheck(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2)
{
    float X = pos1.x - pos2.x;
    float Y = pos1.y - pos2.y;
    float Z = pos1.z - pos2.z;

    float length = sqrtf(X * X + Y * Y + Z * Z);

    if(length < r1 + r2) {
        return true;
    }
    return false;
}
//接触関数 2D
//-------------------------------------
bool AttachCheckWithAdjest(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2, float radious)
{
    float X = pos1.x - pos2.x;
    float Y = pos1.y - (pos2.y + radious);
    float Z = pos1.z - pos2.z;

    float length = sqrtf(X * X + Y * Y + Z * Z);

    if(length < r1 + r2) {
        return true;
    }
    return false;
}
//接触関数 2D
//-------------------------------------
bool AttachCheckWithPlayer(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2)
{
    float X = pos1.x - pos2.x;
    float Y = (pos1.y + 150.f) - pos2.y;
    float Z = pos1.z - pos2.z;

    float length = sqrtf(X * X + Y * Y + Z * Z);

    if(length < r1 + r2) {
        return true;
    }
    return false;
}
//接触関数3D
//-------------------------------------
bool AttachCheck_PlayertoGoblin(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2)
{
    float X = pos1.x - pos2.x;
    float Y = pos1.y - (pos2.y + 150.f);
    float Z = pos1.z - pos2.z;

    float length = sqrtf(X * X + Y * Y + Z * Z);

    if(length < r1 + r2) {
        return true;
    }
    return false;
}
//接触関数3D
//-------------------------------------
bool AttachCheck_PlayertoBoss(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2)
{
    float X = pos1.x - pos2.x;
    float Y = pos1.y - (pos2.y + 250.f);
    float Z = pos1.z - pos2.z;

    float length = sqrtf(X * X + Y * Y + Z * Z);

    if(length < r1 + r2) {
        return true;
    }
    return false;
}
//接触関数3D
//-------------------------------------
bool AttachCheck_PlayerBullettoZonbie(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2)
{
    float X = pos1.x - pos2.x;
    float Y = pos1.y - (pos2.y);
    float Z = pos1.z - pos2.z;

    float length = sqrtf(X * X + Y * Y + Z * Z);

    if(length < r1 + r2) {
        return true;
    }
    return false;
}
bool AttachCheck_BalltoChractor(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2, float adjest_y)
{
    float X = pos1.x - pos2.x;
    float Y = pos1.y - (pos2.y + adjest_y);
    float Z = pos1.z - pos2.z;

    float length = sqrtf(X * X + Y * Y + Z * Z);

    if(length < r1 + r2) {
        return true;
    }
    return false;
}
//接触関数2D
//-------------------------------------
bool AttachCheck2D(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2)
{
    float X = pos1.x - pos2.x;
    float Y = pos1.y - pos2.y;
    float Z = pos1.z - pos2.z;

    float length = sqrtf(X * X + Z * Z);

    if(length < r1 + r2) {
        return true;
    }

    return false;
}
bool BulletAttachCheckToNpc(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2)
{
    float X = pos1.x - pos2.x;
    float Y = (pos1.y + 150.f) - (pos2.y);
    float Z = pos1.z - pos2.z;

    float length = sqrtf(X * X + Z * Z);

    if(length < r1 + r2) {
        return true;
    }

    return false;
}
bool BulletAttachCheckToplayer(const VECTOR& pos1, float r1, const VECTOR& pos2, float r2)
{
    float X = pos1.x - pos2.x;
    float Y = (pos1.y + 200.f) - (pos2.y);
    float Z = pos1.z - pos2.z;

    float length = sqrtf(X * X + Z * Z);

    if(length < r1 + r2) {
        return true;
    }

    return false;
}
//bool RedGoblinAttachCheckToplayer(const Player* obj_1, const RedGoblin* obj_2)
//{
//    //ゴブリンが攻撃した時　右手の当たり判定がプレイヤーに当たったら　ダメージ判定に入る
//    if(obj_2.Get_is_attacking() && AttachCheckWithAdjest_WithPos1(obj_1.GetPos(),
//                                                                  obj_1.GetRadius(),
//                                                                  obj_2.GetframePos_R(),
//                                                                  obj_2.Getright_hand_radius(),
//                                                                  obj_1.Getplayer_hight())) {
//        return true;
//    }
//    return false;
//}
//距離取る用の関数
//-----------------------------------------
float GetDistance(const VECTOR& pos1, const VECTOR& pos2)
{
    float distance;

    float X = pos1.x - pos2.x;
    float Y = pos1.y - pos2.y;
    float Z = pos1.z - pos2.z;

    distance = sqrtf(X * X + Y * Y + Z * Z);

    return distance;
}
// ==================================================
//当たり判定関数
// ==================================================
bool CheckHitStageObject(const Base* obj1, const Base* obj2, float addjust_y)
{
    return AttachCheck(obj1->GetPos(),
                       obj1->GetRadius(),
                       VGet(obj2->GetPos().x, obj2->GetPos().y + addjust_y, obj2->GetPos().z),
                       obj2->GetRadius());
}
bool CheckInTrigger(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3)
{
    if((pos1.x > pos2.x) && (pos1.x < pos3.x) && (pos1.z > pos2.z) && (pos1.z < pos3.z)) {
        return true;
    }
    else {
        return false;
    }
}
//指定されたボールとプレイヤーの当たり判定
//--------------------------------------------
bool IsHitBall(const ModelBase* ball, const ModelBase* player)
{
    return CheckHitStageObject(ball, player, 50.0f);
}
//ゴブリン攻撃モードに切り替え用
//--------------------------------------------
bool CheckHitWithEnemy(const Base* obj1, const ModelBase* obj2)
{
    return AttachCheck2D(obj1->GetPos(), obj1->GetRadius(), obj2->GetPos(), obj2->GetAttackRadius());
}
//プレーヤーとゴブリン
bool CheckCircleHit_WithGoblin(const Player* obj1, const ModelBase* obj2)
{
    return AttachCheck_PlayertoGoblin(obj1->GetBulletPos(), obj1->GetRadius(), obj2->GetPos(), obj2->GetRadius());
}
//プレーヤーとBossキャラ
bool CheckCircleHit_WithBoss(const Player* obj1, const ModelBase* obj2)
{
    return AttachCheck_PlayertoBoss(obj1->GetBulletPos(), obj1->GetBulletRadius(), obj2->GetPos(), obj2->GetRadius());
}
//ゾンビとの
bool CheckCircleHit_WithZonbie(const Player* obj1, const ModelBase* obj2)
{
    return AttachCheck_PlayerBullettoZonbie(obj1->GetBulletPos(),
                                            obj1->GetBulletRadius(),
                                            obj2->GetPos(),
                                            obj2->GetRadius());
}

bool CheckCircleHit_WithZonbieHand(const Player* obj1, const Zonbie* obj2)
{
    //ゾンビの右手が攻撃入ったら
    if(obj2->Getnow_is_attacking_player() && (AttachCheckWithAdjest(obj2->GetPosWhenAttachPlayer(),
                                                                    obj2->Getpart_radius(),
                                                                    obj1->GetPos(),
                                                                    obj1->GetRadius(),
                                                                    125.f))) {
        return true;
    }
    return false;
}

bool CheckSquareHit_WithPlayerandNPC(const Player* obj1, const Npc* obj_N, const BlockBase* obj_B)
{

    if((obj1->GetPos().x < obj_B->GetBlock1TopC().x)&&(obj_B->GetBlock1BottomA().x < obj1->GetPos().x)&&
       (obj1->GetPos().z < obj_B->GetBlock1TopC().z)&&(obj_B->GetBlock1BottomA().z < obj1->GetPos().z)&&
       (obj_N->GetPos().x < obj_B->GetBlock1TopC().x) && (obj_B->GetBlock1BottomA().x<obj_N->GetPos().x) &&
       (obj_N->GetPos().z < obj_B->GetBlock1TopC().z) && (obj_B->GetBlock1BottomA().z<obj_N->GetPos().z)) {
        return true;
    }

    return false;
}

