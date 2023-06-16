#include "ModelBase.h"
#include <vector>

class MoveBlock;
class BlockBase;
class Player;
class Npc;
class Vector3;
class Camera;

//! キャラクター
//===============================================
class Zonbie : public ModelBase
{
private:
    enum MOVE_PATTERN
    {
        STAY,
        THINKING,
        MOVE,
        CATCH,
        HURT,
        PLAYER_ATTACK,
        DEAD,
    };

    std::vector<int> anim_handle{
        MV1LoadModel("Data/Character/Zonbi/Idle.mv1"),
        MV1LoadModel("Data/Character/Zonbi/Goalkeeper Catch.mv1"),
        MV1LoadModel("Data/Character/Zonbi/Standing Run Right.mv1"),
        MV1LoadModel("Data/Character/Zonbi/Zombie Attack.mv1"),
        MV1LoadModel("Data/Character/Zonbi/Zombie Death.mv1"),
        MV1LoadModel("Data/Character/Zonbi/Walker Walk.mv1"),
        MV1LoadModel("Data/Character/Zonbi/Fast Run.mv1"),
    };


     //----------------------------------------------------------
    //!	@name	個体番号確認
    //----------------------------------------------------------
    //!@{
     int zonbie_num;   //!<作成されるムーヴブロックのナンバー
    //!@} 
    //----------------------------------------------------------
    //!	@name	モデル移動関係設定
    //----------------------------------------------------------
    //!@{

    MOVE_PATTERN zonbie_action;       //!<ゴブリンの移動パターン
    int          move_count;          //!<固定の秒数の後で移動;
    float        fix_move_distance;   //!<固定で移動する距離;
    VECTOR       pos_before_move;     //!<逃げる前の位置
    bool         is_move;             //!<動いでいがどうが
    bool         is_live;             //!<生きるがどうかの確認
    bool         now_move;            //!<個体番号8から10用の1回性トリガー
    //!@}

    //----------------------------------------------------------
    //!	@name	重力関係設定
    //----------------------------------------------------------
    //!@{
    float gravity;      //!<重力
    bool  is_stepped;   //!<踏み台の上に踏んでいるのか
    bool  on_move_block;
    //!@}
    //----------------------------------------------------------
    //!	@name	生死判定
    //----------------------------------------------------------
    //!@{

    float Hp;        //!<モンスターのHP
    bool  is_dead;   //!<死亡しているがしていないが
    //!@}
    //----------------------------------------------------------
    //!	@name	ダメージ関係設定
    //----------------------------------------------------------
    //!@{
   // bool  is_hurt;
    float hurt_rot;
    float transparency;   //!<ダメージ用透明度
    int   hurt_count;     //!<殴られた時に起動するとカウンター
    //!@}
    //----------------------------------------------------------
    //!	@name	他に使うクラス設定
    //----------------------------------------------------------
    //!@{
    const Player* player_ptr = nullptr;   //!<プレイヤーを宣言して
    const Camera* camera_ptr = nullptr;   //!<カメラーを宣言して
    const Npc*    npc_ptr    = nullptr;   //!<NPCーを宣言して
    //!@}
    //----------------------------------------------------------
    //!	@name  攻撃設定
    //----------------------------------------------------------
    //!@{
    VECTOR framePosL;                     //!左手
    VECTOR framePosR;                     //!右手
    float  part_radius = 80.f;
    bool   catch_npc;        //!NPCを捕まえるがどうが
    bool   now_Iscatching;   //!今捕まえの途中
    bool   now_is_attacking_player;   //プレーヤー攻撃中
    float attack_rot;
    int   now_is_attacking;

    //!@}
    
    //セリフの座標を決めます
    VECTOR screen_position;   //!セリフ用のポジション
    //追加未整理
    float roty;   //!移動処理用暫定rot

    //計算用
    //暫定用
    VECTOR GOAL_AREA = {0.0f,0.0f,0.0f};

public:

    //! コンストラクタ
    Zonbie(size_t num);
    //! デストラクタ
    virtual ~Zonbie();


    //! @name   アニメの設定
    //!@{
    //!アニメーションを再生処理を行う用の関数
    void PlayAnimation();
    //!@}

    //! @name 生死判定
    //!@{
    //!殺された時の座標設定
    bool Check_Is_Dead();
    //! is_dead設定します。
    void Set_is_dead(bool now_is_dead);
    //! is_dead取得します。
    //! @return 現在のis_dead
    bool Get_is_dead() const { return is_dead; }
    //!@}
    
    //! @name ブロックとの当たり判定
    //!@{
    void ResetBeforeCheck();                          //!チェック前に他の変数をリセットして直す
    void GravityLine();                               //!<上下の当たりラインの処理
    bool HitWithStageBlock(const BlockBase* block);   //!<ステージブロックの当たり判定
    bool HitWithMoveBlock(const MoveBlock* block);    //!<ムーヴブロックの当たり判定
    void SetDownWhenHit(float block_hight);   //!<ステージブロックと当たった時ブロックの高さに合わせての設定
    void SetPosY(float fix_y);   //!<高さの設定
    void SetPosOnMoveBlock(const Vector3& dir);   //!<ムーヴブロックと当たった時ブロックの高さに合わせての設定
    //! is_stepped設定します。
    void SetIsStepped(bool now_is_stepped);
    //! is_steppedを取得します。
    //! @return 現在のis_stepped
    bool GetIsStepped() const { return is_stepped; }
    //! on_move_blockを取得します。
    //! @return 現在のon_move_block
    bool GetOnMoveBlock() const { return on_move_block; }
    void Gravity();   //!<重力設定
    //!@}
    
    //!@name　ダメージ関係設定
    //!@{
    //! transparency設定します。
    void Set_Transparency(float now_transparency);
    //! is_hurt設定します。
    //void Set_is_hurt(bool now_is_hurt);
    //! Hp設定します。
    void Set_Hp(bool now_HP);
    void Reset_tomove();                                //!<攻撃終わった後STAYに戻る
    void SetPos_HurtByPlayerBullet(float bullet_rot);   //!< プレーヤー弾丸に攻撃された時の設定
    //! transparencyを取得します。
    //! @return 現在のtransparency
    float Get_Transparency() const { return transparency; }
    //! is_hurtを取得します。
    //! @return 現在のis_hurt
    //bool Get_is_hurt() const { return is_hurt; }
    //! Hpを取得します。
    //! @return 現在のHp
    bool Get_Hp() const { return Hp; }
    //!@}  
    
    //! @name　他に使うクラス設定
    //!@{
    //プレイヤーを宣言して
    void SetPlayer(Player* p) { player_ptr = p; }
    void SetCamera(Camera* c) { camera_ptr = c; }
    void SetNpc(Npc* n) { npc_ptr = n; }
    //!@}

    //! @name　他関数
    //!@{
    //!固定座標を設定
    void Fixpos(VECTOR& pos);
    //!座標設定
    void SetPosBeforeMove(VECTOR& pos, float rot);
    //!@}
 

    
    //! @name  殺された時設定
    //!@{
    //! now_liveを設定します。
    //! @param  [in]    now_live  現在のnow_live
    void SetIsLive(bool now_live);
    //! is_liveを取得します。
    //! @return 現在のis_live
    bool GetIsLive() const;
    //!@}
    
    //! @name　その他設定
    //!@{
    //ステージに合わせて正常稼働中
    bool MovieORDead();
    //!@}

    //追加未整理
    bool CheckNowIsKitching();
    void Setcatch_npc(bool now_catch);
    bool Getcatch_npc() { return catch_npc; }
    VECTOR GetPosBeCatch() const { return framePosR; }
    void  ReleaseNpc();
    VECTOR GetPosWhenAttachPlayer() const { return framePosR; }
    float  Getpart_radius() const { return part_radius; }
    bool   Getnow_is_attacking_player() const { return now_is_attacking_player; }

    void reload_now_is_attacking();

    float Get_attack_rot() const { return attack_rot; }

    //!追加未整理
    void CalculateAngle_zonbi_to_target(const VECTOR& pos_1,
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
