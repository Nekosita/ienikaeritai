#include "ModelBase.h"
#include "../src/System/Random.h"
#include "../src/System/Vector3.h"
#include "../src/System/Effect.h"
#include "House.h"
#include "DxLib.h"
#include <vector>
#include <array>


//コンストラクタ
//-----------------------------------------------
House::House()
{

    // 普通のモデル設定
    char str[1024];
    // ここはGameフォルダ
    GetCurrentDirectory(1024, str);
    // 強制Bossフォルダセットします
    std::string strM = std::string(str) + "/Data/Character/House/";
    SetCurrentDirectory(strM.c_str());
    model_handle = MV1LoadModel("house.mv1");
    //Gameフォルダに戻します
    SetCurrentDirectory(str);
    MV1SetScale(model_handle, VGet(10.0f, 10.0f, 10.0f));   //モデルの大きさを変えます

    //角度設定
    float Start_rot = TO_RADIAN(90.0f);
    rot             = {0.0f, Start_rot, 0.0f};

}
House::~House()
{
    Exit();
}
//更新処理
//---------------
void House::Update(int now_house)
{
    switch(now_house) {
    case HOUSE_PATTERN::GAME_TITLE:
        pos = VGet(0.0f * 200.0f, -10500.0f, 0.0f * 200.0f);
        break;
    case HOUSE_PATTERN::GAME_STAGR:
        pos = VGet(318.0f * 200.0f, 4000.0f, 10.0f * 200.0f);
        break;
    }

}
//描画処理
//---------------
void House::Render()
{
    SetUseLighting(FALSE);     //ライティングOFF
    //平常状態の描画設定
    MV1SetRotationXYZ(model_handle, VGet(rot.x, rot.y, rot.z));
    MV1SetPosition(model_handle, pos);
    MV1DrawModel(model_handle);
    SetUseLighting(TRUE);   //ライティングON
}

void House::Exit()
{
    MV1DeleteModel(model_handle);
}
