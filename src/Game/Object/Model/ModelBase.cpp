#include"ModelBase.h"
#include "../src/System/Vector3.h"
#include"DxLib.h"
#include <vector>

//コンストラクタ
//-----------------------------------------------
ModelBase::ModelBase() :
	model_handle(-1),
	model(-1),
	anim_no(0),
	totaltime(0),
	attachindex(-1),
	playtime(0.0f)
{


}
//デストラクタ
//-----------------------------------------------
ModelBase::~ModelBase()
{


}
//重力ラインの設定
//---------------------------------
void ModelBase::SetGravityLineStart(VECTOR& now_gravity_start, float charatorheight_adjest)
{
    gravity_line_start = VGet(now_gravity_start.x, now_gravity_start.y + charatorheight_adjest, now_gravity_start.z);
}
VECTOR ModelBase::GetGravityLineStart() const
{
    return gravity_line_start;
}
void ModelBase::SetGravityLineEnd(VECTOR& now_gravity_end, float charatorheight_adjest)
{
    gravity_line_end = VGet(now_gravity_end.x, now_gravity_end.y - charatorheight_adjest, now_gravity_end.z);
}
VECTOR ModelBase::GetGravityLineEnd() const
{
    return gravity_line_end;
}
//頭上ラインの設定
//---------------------------------
void ModelBase::SetTopLineStart(VECTOR& now_top_start, float charatorheight_adjest)
{
    top_line_start = VGet(now_top_start.x, now_top_start.y + charatorheight_adjest, now_top_start.z);
}
VECTOR ModelBase::GetTopLineStart() const
{
    return top_line_start;
}
void ModelBase::SetTopLineEnd(VECTOR& now_top_end, float charatorheight_adjest)
{
    top_line_end = VGet(now_top_end.x, now_top_end.y + charatorheight_adjest, now_top_end.z);
}
//重力の当たり判定の設定
//---------------------------------
VECTOR ModelBase::GetTopLineEnd() const
{
    return top_line_end;
}
HITRESULT_LINE ModelBase::GetBlockT1() const
{
    return block_t1;
}
HITRESULT_LINE ModelBase::GetBlockT2() const
{
    return block_t2;
}
HITRESULT_LINE ModelBase::GetBlockB1() const
{
    return block_b1;
}
HITRESULT_LINE ModelBase::GetBlockB2() const
{
    return block_b1;
}

void ModelBase::CheckIfHitWithBlock(const VECTOR& TA,
                                    const VECTOR& TB,
                                    const VECTOR& TC,
                                    const VECTOR& TD,
                                    const VECTOR& BA,
                                    const VECTOR& BB,
                                    const VECTOR& BC,
                                    const VECTOR& BD)
{
    //プレイヤーが下の板をぶつかったら強制出来に落ちます
    block_b1 = HitCheck_Line_Triangle(GetTopLineStart(), GetTopLineEnd(), BA, BB, BC);
    block_b2 = HitCheck_Line_Triangle(GetTopLineStart(), GetTopLineEnd(), BA, BB, BC);

    //ブロックの当たり判定
    //プレイヤーが上板を当たったら上にいます
    block_t1 = HitCheck_Line_Triangle(GetGravityLineStart(), GetGravityLineEnd(), TA, TB, TC);

    block_t2 = HitCheck_Line_Triangle(GetGravityLineStart(), GetGravityLineEnd(), TA, TD, TC);
}
void ModelBase::SetAnimation(const std::vector<int>& handle, int new_anim_no)
{
    if(anim_no != new_anim_no) {
        MV1DetachAnim(model_handle, attachindex);
        anim_no     = new_anim_no;
        attachindex = MV1AttachAnim(model_handle, 0, handle[anim_no], FALSE);
        totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
    }
}
//半径の設定
void ModelBase::SetAttackRadius(float now_radious)
{
	attackradius = now_radious;
}
//半径の取得
float ModelBase::GetAttackRadius() const
{
	return attackradius;
}