#include "Model_2.h"
#include "../Base.h"
#include "DxLib.h"
#include <string>

#pragma once
//コンストラクタ
//-----------------------------------------------
Model_2::Model_2()
{
    // モデル設定
    char str[1024];
    // ここはGameフォルダ
    GetCurrentDirectory(1024, str);
    // 強制mouseフォルダからセットします
    std::string strM = std::string(str) + "/Data/Character/Mousey/";
    SetCurrentDirectory(strM.c_str());
    model_handle = MV1LoadModel("Sitting Talking.mv1");

    // Gameフォルダに戻します
    SetCurrentDirectory(str);
    attachindex = MV1AttachAnim(model_handle, 1, anim_handle[anim_no], FALSE);
    totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);

    MV1SetScale(model_handle, VGet(2.0f, 2.0f, 2.0f));   //モデルの大きさを変えます
    pos = {5.0f * 200.0f, 19000.0f, 5.0f * 200.0f};      // オープニングのモデルの生成位置

    // モデル設定
    char tab[1024];
    // ここはGameフォルダ
    GetCurrentDirectory(1024, tab);
    // 強制mouseフォルダからセットします
    std::string strTAB = std::string(tab) + "/Data/Character/Clear_Obj/";
    SetCurrentDirectory(strTAB.c_str());
    table     = MV1LoadModel("Wood_Table_74.mv1");
    chair[0]  = MV1LoadModel("uploads_files_3603362_Simple+Wooden+chair.mv1");
    chair[1]  = MV1LoadModel("uploads_files_3603362_Simple+Wooden+chair.mv1");
    coffee[0] = MV1LoadModel("Paper coffee cup _FBX.mv1");
    coffee[1] = MV1LoadModel("Paper coffee cup _FBX.mv1");
    // Gameフォルダに戻します
    SetCurrentDirectory(tab);
    MV1SetScale(table, VGet(4.5f, 3.0f, 4.0f));       //モデルの大きさを変えます
    MV1SetScale(chair[0], VGet(1.0f, 1.0f, 1.0f));    //モデルの大きさを変えます
    MV1SetScale(chair[1], VGet(1.0f, 1.0f, 1.0f));    //モデルの大きさを変えます
    MV1SetScale(coffee[0], VGet(1.0f, 1.0f, 1.0f));   //モデルの大きさを変えます
    MV1SetScale(coffee[1], VGet(1.0f, 1.0f, 1.0f));   //モデルの大きさを変えます

    Clear_count = 300;

    root_node = MV1SearchFrame(model_handle, "mixamorig1:Spine");
    root_node = 1;   // 描画用の関数設定

    CreateIdentityMatrix(&identity);
    playtime = 0.0f;   // 再生時間の初期化
}
//デストラクタ
//---------------------------------
Model_2::~Model_2()
{
    Exit();
}
//アニメーション設定
//---------------------
void Model_2::SetAnimation(const std::vector<int>& handle, int new_anim_no)
{
    if(anim_no != new_anim_no) {
        MV1DetachAnim(model_handle, attachindex);
        anim_no     = new_anim_no;
        attachindex = MV1AttachAnim(model_handle, 1, handle[anim_no], FALSE);
        totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
    }
}
void Model_2::PlayAnimation()
{
    playtime += 0.5f;   //描画時間を更新

    if(totaltime <= playtime) {
        playtime = 0.0f;
        if(anim_no == 0) {
            MV1DetachAnim(model_handle, attachindex);
            anim_no     = 1;
            attachindex = MV1AttachAnim(model_handle, 1, anim_handle[anim_no], FALSE);
            totaltime   = MV1GetAttachAnimTotalTime(model_handle, attachindex);
        }
    }
}
void Model_2::Reset()
{
    Clear_count = 300;
}
//アップデート
//---------------------
void Model_2::Update(int now_model)
{
    switch(now_model) {
    case Model_PATTERN::GAME_CLEAR:
        Clear_count--;
        SetAnimation(anim_handle, 0);
        MV1SetScale(model_handle, VGet(1.5f, 1.5f, 1.5f));   //モデルの大きさを変えます
        player_model_rot.y = TO_RADIAN(270.0f);
        pos                = {-180.0f, SCREEN_H / 2 - 50.0f, 0.0f};
        table_pos          = {0.0f, SCREEN_H / 2 - 200.0f, 0.0f};
        chair_pos[0]       = {-220.0f, SCREEN_H / 2 - 100.0f, 0.0f};
        chair_pos[1]       = {220.0f, SCREEN_H / 2 - 100.0f, 0.0f};
        coffee_pos[0]      = {100.0f, SCREEN_H / 2 - 50.0f, 0.0f};
        coffee_pos[1]      = {-100.0f, SCREEN_H / 2 - 50.0f, 0.0f};
        break;
    case Model_PATTERN::NOTHING:
        break;
    }

    PlayAnimation();
    MV1SetAttachAnimTime(model_handle, attachindex, playtime);   //再生時間をセットする
    MV1SetFrameUserLocalMatrix(model_handle, root_node, identity);
}
//モデルの描画
//--------------------
void Model_2::Render(int count)
{
    ChangeFont("ＭＳ 明朝");
    SetFontSize(60);
    if(Clear_count < 290) {
        if(count > 1) {
            DrawString(SCREEN_W / 2 + 350, 200, "周回したんだ！", GetColor(19, 46, 225));
        }
        DrawString(SCREEN_W / 2 + 400, 275, "お疲れ！", GetColor(19, 46, 225));
    }
    if(Clear_count < 170) {
        if(count > 1) {
            DrawString(SCREEN_W / 2 - 400, 200, "凄く", GetColor(255, 137, 49));
            DrawString(SCREEN_W / 2 - 250, 300, "頑張ったぞ！", GetColor(255, 137, 49));
        }
        else {
            DrawString(SCREEN_W / 2 - 400, 200, "コーヒー", GetColor(255, 137, 49));
            DrawString(SCREEN_W / 2 - 250, 300, "ありがとう！", GetColor(255, 137, 49));
        }
    }
    SetFontSize(40);
    SetUseLighting(FALSE);   //ライティングOFF
    MV1SetRotationXYZ(model_handle, player_model_rot);
    MV1SetPosition(model_handle, pos);
    MV1DrawModel(model_handle);
    //他モデル描画
    MV1SetRotationXYZ(table, VGet(0.0f, TO_RADIAN(270.0f), 0.0f));
    MV1SetPosition(table, table_pos);
    MV1DrawModel(table);
    MV1SetRotationXYZ(chair[0], VGet(0.0f, TO_RADIAN(270.0f), 0.0f));
    MV1SetPosition(chair[0], chair_pos[0]);
    MV1DrawModel(chair[0]);
    MV1SetRotationXYZ(chair[1], VGet(0.0f, TO_RADIAN(90.0f), 0.0f));
    MV1SetPosition(chair[1], chair_pos[1]);
    MV1DrawModel(chair[1]);
    MV1SetRotationXYZ(coffee[0], VGet(0.0f, 0.0f, 0.0f));
    MV1SetPosition(coffee[0], coffee_pos[0]);
    MV1DrawModel(coffee[0]);
    MV1SetRotationXYZ(coffee[1], VGet(0.0f, 0.0f, 0.0f));
    MV1SetPosition(coffee[1], coffee_pos[1]);
    MV1DrawModel(coffee[1]);
    SetUseLighting(TRUE);   //ライティングON
}
//消去処理
//-----------------
void Model_2::Exit()
{
    MV1DeleteModel(model_handle);
}
