#include "ModelBase.h"
#include "../src/Game/Object/Model/Player.h"
#include <vector>

class MoveBlock;
class BlockBase;
class Player;
class Vector3;

//! キャラクター
//===============================================
class Boss : public ModelBase
{
private:
    static constexpr float BOSS_SPEED = 30.0f;   //!< ボスのスピード
    enum MOVE_PATTERN
    {
        STAY,
        ATTACK_START,
        HP_DYIND,
        HP_NORMAL,
        THINKING,
        BALL_SUMMON,
        ATTACK_DOWNWARD,
        WAVE_,
        JUMP_ATTACK,
        HURT,
        DEAD,
    };

    std::vector<int> anim_handle{
        MV1LoadModel("Data/Character/Boss/Seated Idle.mv1"),
        MV1LoadModel("Data/Character/Boss/Victory.mv1"),
        MV1LoadModel("Data/Character/Boss/Zombie Death.mv1"),
        MV1LoadModel("Data/Character/Boss/Sword And Shield Casting.mv1"),
        MV1LoadModel("Data/Character/Boss/Slow Run.mv1"),
        MV1LoadModel("Data/Character/Boss/Jump Attack.mv1"),
        MV1LoadModel("Data/Character/Boss/Standing Melee Attack Downward.mv1"),
        MV1LoadModel("Data/Character/Boss/Breathing Idle.mv1"),
    };

    //----------------------------------------------------------
    //!	@name	個体番号確認
    //----------------------------------------------------------
    //!@{
    int boss_num;
    //----------------------------------------------------------
    //!	@name	モデルの行動パターン設定
    //----------------------------------------------------------
    //!@{
    bool         is_attatched;   //プレーヤーと接触するがしないが
    MOVE_PATTERN boss_action;    //!ボスの移動パターン
    //!<各位置のあたり判定用座標
    VECTOR framePosL;   //!左手
    VECTOR framePosR;   //!右手
    float  part_radius = 150.f;
    //攻撃用のカウンター
    int start_attack_count;                        //!開幕攻撃用
    int now_is_attacking_start_hp_up200_count;     //!HPが200以上の時用
    int attack_kind_up200_count;                   //!HPが200以上の時の攻撃パターン選択用
    int now_is_attacking_start_hp_down200_count;   //!HPが200以下の時用
    int attack_kind_down200_count;                 //!HPが200以下の時の攻撃パターン選択用
    //!@}
    //----------------------------------------------------------
    //!	@name	ダメージ関係設定
    //----------------------------------------------------------
    //!@{
    float transparency;   //!<ダメージ用透明度
    int   hurt_count;     //!<殴られた時に起動するとカウンター
    float hurt_rot;       //!<殴られた時のロット
    float roty;           //!<角度計算用のロット
    //!@}
    //----------------------------------------------------------
    //!	@name	生死判定
    //----------------------------------------------------------
    //!@{
    float Hp;        //!<モンスターのHP
    bool  is_dead;   //!<死亡しているがしていないが
    //!@}
    //----------------------------------------------------------
    //!	@name	重力関係設定
    //----------------------------------------------------------
    //!@{
    float gravity;   //!<重力
    //!@}
    //----------------------------------------------------------
    //!	@name 攻撃用設定
    //----------------------------------------------------------
    //!@{
    bool is_jump_attack;   //ジャンプする攻撃用
    bool is_standing_melee_attack_downward;
    bool on_wave;
    bool ball_summon;   //他の所でボール召喚
    //!@}

    //!@}
    //----------------------------------------------------------
    //!	蓄積カウンター用設定
    //----------------------------------------------------------
    //!@{
    //蓄積カウンター用
    int   accumlate_count;       //ダメージ蓄積用のカウンター
    float accumlate_damage;      //蓄積ダメージ
    bool  in_big_damage;         //大きなダメージが貰うがどうか
    int   count_in_big_damage;   //大きなダメージが貰う時のカウンター
    //!@}
    //----------------------------------------------------------
    //!	@name	他に使うクラス設定
    //----------------------------------------------------------
    //!@{
    //プレイヤーを宣言して
    const Player* player_ptr = nullptr;
    //!@}

public:

    //! コンストラクタ
    Boss(size_t num);
    //! デストラクタ
    virtual ~Boss();

    //! @name   アニメの設定
    //!@{
    //!アニメーションを再生処理を行う用の関数
    void PlayAnimation();
    //!@}

    //! @name　モデルの行動パターン設定
    //!@{
    //! is_attatchedを設定します。
    //! @return 現在のis_attatched
    void SetIsAttatched(bool now_);
    //! is_attatchedを取得します。
    //! @return 現在のis_attatched
    bool GetIsAttatched() const { return is_attatched; }
    //!@}

    //!@name　ダメージ関係設定
    //!@{
    //! now_countを設定します。
    //! @return 現在のnow_count
    void SetPos_HurtByPlayerBullet(float bullet_rot);
    //! hurt_countを取得します。
    //! @return 現在のhurt_count
    int Gehurtcount() const { return hurt_count; }
    //!カウンターリセット
    void ResetSetHurtCount();
    //! transparencyを設定します。
    //! @return 現在のtransparency
    void Set_Transparency(float now_transparency);
    //! transparencyを取得します。
    //! @return 現在のtransparency
    float Get_Transparency() const { return transparency; }
    void Get_Hurt();//普通のダメージ
    void Get_Hurt_in_bigamage();//大ダメージに居る時の関数
    //!@}

    //!@name　生死判定
    //!@{
    //! is_deadを設定します。
    //! @return 現在のis_dead
    void Set_is_dead(bool now_is_dead);
    //! is_dead取得します。
    //! @return 現在のis_dead
    bool Get_is_dead() const { return is_dead; }
    //! Hpを設定します。
    //! @return 現在のHp
    void Set_Hp(bool now_HP);
    //! Hpを取得します。
    //! @return 現在のHp
    float Get_Hp() const { return Hp; }
    //!@}

    //!@}
    //----------------------------------------------------------
    //!	@name	攻撃用設定
    //----------------------------------------------------------
    //!@{
    void   ResetAllCounterAndKind();                       //攻撃用のカウンターを全部リセット
    void   ResetAllAttackBool();                       //攻撃用のboolーを全部リセット
    void   transparency_in_damage(int damage_count);   //透明度用
    bool Get_ball_summon() const { return ball_summon; }
    bool Get_on_wave() const { return on_wave; }
    bool Getis_jump_attack() const { return is_jump_attack; }
    bool Getis_standing_melee_attack_downward() const { return is_standing_melee_attack_downward; }
    int  Getstart_attack_count() const { return start_attack_count; }
    VECTOR GetframePosLPos() const { return framePosL; }
    VECTOR GetframePosRPos() const { return framePosR; }
    float  Get_part_radius() const { return part_radius; }
    //攻撃パターン決める関数
    void Attacking_When_Hp_up_200();
    void Attacking_When_Hp_down_200();
    //!@}

    //! @name　他に使うクラス設定
    //!@{
    //プレイヤーを宣言して
    void SetPlayer(Player* p) { player_ptr = p; }
    //!@}

    //! @name　他関数
    //!@{
    //!固定座標を設定
    void Fixpos(VECTOR& pos);
    //!@}

    //! @name　他キャラとの当たり判定
    //!@{
    bool CheckHitWithPlayerBullet();
    //!@}
    
    //!角度計算用
    void CalculateAngle_boss_to_player(const VECTOR& pos_1,
                                             const VECTOR& pos_2,
                                             const VECTOR& vector1,
                                             const VECTOR& vector2);

    //!更新処理
    void Update();
    //!描画処理
    void Render();
    //!終了処理
    void Exit();
};
