#include "BlockBase.h"
#include"DxLib.h"


//コンストラクタ
//-----------------------------------------------
BlockBase::BlockBase():
	start_pos{(float)(0,0)},
	end_pos{ (float)(0,0)}
{

}
//デストラクタ
//-----------------------------------------------
BlockBase::~BlockBase()
{

}
void BlockBase::SetTrigger(bool now_is_trigger)
{
    trigger = now_is_trigger;
}
bool BlockBase::GetTrigger() const
{
    return trigger;
}
void BlockBase::SetBlockStartPos(const VECTOR& now_block_pos)
{
	start_pos = now_block_pos;
}
void BlockBase::SetBlockEndPos(const VECTOR& now_block_endpos)
{
	end_pos = now_block_endpos;
}
float BlockBase::GetBlockHight() const
{
	return end_pos.y;
}
//上の当たり判定用当たり判定用のポス取得設定
//-----------------------------
VECTOR BlockBase::GetBlock1TopA() const
{
	return VGet(start_pos.x, end_pos.y, start_pos.z);
}
VECTOR BlockBase::GetBlock1TopB() const
{
	return  VGet(start_pos.x, end_pos.y, end_pos.z);
}
VECTOR BlockBase::GetBlock1TopC() const
{
	return end_pos;
}
VECTOR BlockBase::GetBlock1TopD() const
{
	return VGet(end_pos.x, end_pos.y, start_pos.z);
}
//下の当たり判定用当たり判定用のポス取得設定
//-----------------------------
VECTOR BlockBase::GetBlock1BottomA() const
{
	return start_pos;
}
VECTOR BlockBase::GetBlock1BottomB() const
{
	return VGet(start_pos.x, start_pos.y, end_pos.z);
}
VECTOR BlockBase::GetBlock1BottomC() const
{
	return VGet(end_pos.x, start_pos.y, end_pos.z);
}
VECTOR BlockBase::GetBlock1BottomD() const
{
	return VGet(end_pos.x, start_pos.y, start_pos.z);
}
//上下の当たり判定関数を取得する用判定
//-----------------
HITRESULT_LINE BlockBase::GetBlockT1() const
{
    return block_t1;
}
HITRESULT_LINE BlockBase::GetBlockT2() const
{
    return block_t2;
}
HITRESULT_LINE BlockBase::GetBlockB1() const
{
    return block_b1;
}
HITRESULT_LINE BlockBase::GetBlockB2() const
{
    return block_b2;
}
//上下の当たり判定関数を取得する用判定
//-----------------
HITRESULT_LINE BlockBase::GetBlockNT1() const
{
    return block_tn1;
}
HITRESULT_LINE BlockBase::GetBlockNT2() const
{
    return block_tn2;
}
HITRESULT_LINE BlockBase::GetBlockNB1() const
{
    return block_bn1;
}
HITRESULT_LINE BlockBase::GetBlockNB2() const
{
    return block_bn2;
}