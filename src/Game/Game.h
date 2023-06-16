#pragma once
#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

//!全てのゲーム画面
enum class GAME_SCREEN
{
    GAME_TITLE,
    GAME_RULE,
    GAME_NOWLOADING,
    GAME_START_ANIME,
    GAME_PAUSE,
    GAME_STAGE,
    GAME_OVER,
    GAME_CLEAR_ANIME,
    GAME_CLEAR,
    GAME_PARFECT_CLEAR,
};

//!クラスの前方宣言
class RedGoblin;
class Goblin;
class Base;
class Camera;
class Player;
class Npc;
class Boss;
class Model;
class Model_2;
class MapChip;
class UserInterface;
class SoundEffect;
class GoalArea;
class StageBlock;
class TriggerArea;
class UpArea;
class MoveBlock;
class House;
class TrapBall;
class ShokeWave;
class HealItem;
class Zonbie;
class CkeckArea;
class Npc_Sheltter;

//! Gameクラス
//===============================================
class Game
{
private:
    // メンバ定数
    //===============================================
    static constexpr int RED_NUM        = 9;
    static constexpr int GREEN_NUM      = 3;
    static constexpr int BOSS_NUM       = 1;
    static constexpr int BLOCK_NUM      = 7;
    static constexpr int MOVE_BLOCK_NUM = 18;
    static constexpr int TRIGGER_AREA   = 1;
    static constexpr int UP_AREA        = 1;
    static constexpr int TOTAL_ITEM     = 10;
    static constexpr int RANKING_NUM    = 5;
    static constexpr int TRAP_BALL      = 4;
    static constexpr int SHOKE_WAVE     = 1;
    static constexpr int HEAL_ITEM_NUM  = 3;
    static constexpr int ZONBIE_NUM     = 7;

    GAME_SCREEN    current_screen;         //!<今ゲーム画面はどこにいたのかの設定
    Camera*        camera     = nullptr;   //!<カメラ
    MapChip*       mapchip    = nullptr;   //!<マップ設定
    Player*        player     = nullptr;   //!<playerの設定
    Npc*           npc        = nullptr;   //!<Npcの設定
    House*         house      = nullptr;   //!<ハウス設計
    Model*         model      = nullptr;   //!<model実装
    Model_2*       model_2    = nullptr;   //!<model実装
    UserInterface* ui         = nullptr;   //!<UIを設定
    SoundEffect*   sound      = nullptr;   //!<音関係
    GoalArea*      goal_area  = nullptr;   //!<ゴールドエリアー実装
    CkeckArea*     check_area = nullptr;   //!<チェックエリア実装
    Npc_Sheltter*  sheltter   = nullptr;   //!<NPCの隠れ家を作ります

    std::vector<std::shared_ptr<RedGoblin>>   redgoblin;      //!<レードゴブリンの設定
    std::vector<std::shared_ptr<Goblin>>      goblin;         //!<ゴブリンの設定
    std::vector<std::shared_ptr<StageBlock>>  block_1;        //!<ステージ1の踏み台のブロック実装
    std::vector<std::shared_ptr<TriggerArea>> trigger_area;   //!<トラップエリア実装
    std::vector<std::shared_ptr<UpArea>>      up_area;        //!<スローエリア実装
    std::vector<std::shared_ptr<MoveBlock>>   move_block;     //!<移動するブロック実装
    std::vector<std::shared_ptr<Boss>>        boss;           //!<ボス実装
    std::vector<std::shared_ptr<TrapBall>>    trap_ball;      //!<ボス用の攻撃実装（ボール
    std::vector<std::shared_ptr<ShokeWave>>   shoke_wave;      //!<ボス用の攻撃実装（衝撃波
    std::vector<std::shared_ptr<HealItem>>    heal_item;      //!<回復アイテム実装
    std::vector<std::shared_ptr<Zonbie>>      zonbie;         //!<ゾンビの実装


    //----------------------------------------------------------
    //!	@name	ゲームクリア演出用
    //----------------------------------------------------------
    //!@{
    int  dead_count;         //!今死んだモンスターをカウンターします。
    bool all_zonbi_dead;     //!ゾンビと赤いゴブリンが消えるがどうがを確認用
    bool is_clear = false;   //!<クリアは最初からフォルス
    bool show_button;        //!<設定出すが出さないが用
    bool music_on;           //!<音楽切り替え用
    bool CheckTrigger;       //!<上に行くトリガー起動用
    int  clear_count = 0;    //!<クリアした回数
    //!@}

    // !当たり判定用関数
    bool GoblinBulletCheckHitWithNpc(const Base* obj1, const Goblin* obj2);
    bool GoblinBulletHitWithPlayer(const Base* obj1, const Goblin* obj2);
    bool RedGoblinRarmHitWithPlayer(const Base* obj1, const RedGoblin* obj2);
    //! @name その他の設定
    //!@{
    //	!設定見る用の関数
    bool        IsPause();
    bool        GetMusicOn();
    GAME_SCREEN UpdatePause();
    //!@}
    
    //　!ステージのBGMを再生する関数
    void PlayStageBGM();
    //	!被った演出用のブロック、罠エリア、速度ダーウィンエリア描画
    void StageShow_Render(bool shadow);

public:
    //	!初期化処理
    void Init();
    //	!ステーシ用のデータの入れる
    void StageInit();
    //	!更新処理
    void Update();
    //	!描画処理
    void Render(bool shadow_);
    //!ステーシ用のデータの消す
    void ResetStageData();
    //	!終了処理
    void Exit();
};
