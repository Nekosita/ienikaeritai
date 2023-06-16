#include"GoalArea.h"
#include"../src/Game/Object/Model/Player.h"
#include"../src/Game/Object/Model/Npc.h"
#include"../src/Game/Object/Model/Boss.h"

//ゲームクリアエリアの位置を取る為の関数
//---------------------------------
float GoalArea::GetGoalAreaHight() const
{
	return end_pos.y;
}
//コンストラクタ
//---------------------------------
GoalArea::GoalArea()
{
    //追加分
    char arr[1024];
    // ここはGameフォルダ
    GetCurrentDirectory(1024, arr);
    // 強制mouseフォルダセットします
    std::string strA = std::string(arr) + "/Data/Arrow/";
    SetCurrentDirectory(strA.c_str());
    clear_arrow = MV1LoadModel("Arrow5.mv1");
    //Gameフォルダに戻します
    SetCurrentDirectory(arr);
    clear_arrow_vector = {0.0f, 0.0f, 0.0f};
    clear_arrow_count  = 0;
    clear_area_start   = {280.0f * 200.f, 4000.0f + stage_hight, 5.0f * 200.f};
    clear_area_end     = {290.0f * 200.f, 4000.0f + stage_hight, 15.0f * 200.f};

}
//デストラクタ
//---------------------------------
GoalArea::~GoalArea()
{

}
void GoalArea::clear_arror_update()
{
   if(boss_ptr->Get_is_dead())
    {
        clear_arrow_count++;
        if(clear_arrow_count > 120) {
            clear_arrow_count = 0;
        }
        if(clear_arrow_count < 60) {
            clear_arrow_vector.y -= 2.0f;
        }
        if((clear_arrow_count > 60) && (clear_arrow_count < 120)) {
            clear_arrow_vector.y += 2.0f;
        }
    }
   
}
void GoalArea::Render_clear_arror()
{
    SetUseLighting(TRUE);
    if(boss_ptr->Get_is_dead()) {
        //立方体描画終了後にライティングを再びON
        MV1SetScale(clear_arrow, VGet(1200.f, 1200.f, 1200.f));   //モデルの大きさを変えます
        MV1SetRotationXYZ(clear_arrow, VGet(TO_RADIAN(90.0f), 0.0f, TO_RADIAN(270.0f)));
        MV1SetPosition(clear_arrow,
                       VGet((clear_area_start.x + clear_area_end.x) / 2,
                            4500.f + clear_arrow_vector.y,
                            (clear_area_start.z + clear_area_end.z) / 2));
        MV1DrawModel(clear_arrow);
    }
    SetUseLighting(TRUE);
}
void GoalArea::Exit()
{

}
