#include "Game.h"
#include "Main.h"
#include "../src/Game/Object/Model/Model.h"
#include "../src/Game/Object/Model/Model_2.h"
#include "../src/Game/Object/Model/Player.h"
#include "../src/Game/Object/Model/TrapBall.h"
#include "../src/Game/Object/Model/ShokeWave.h"
#include "../src/Game/Object/Model/Npc.h"
#include "../src/Game/Object/Model/Boss.h"
#include "../src/Game/Object/Model/House.h"
#include "../src/Game/Object/Model/Zonbie.h"
#include "../src/Game/Object/Model/HealItem.h"
#include "../src/Game/Object/Camera.h"
#include "../src/Game/Object/MapChip.h"
#include "../src/Game/Object/Model/RedGoblin.h"
#include "../src/Game/Object/Model/Goblin.h"
#include "../src/Game/Object/Stage/TriggerArea/TriggerArea.h"
#include "../src/Game/Object/Stage/UpArea/UpArea.h"
#include "../src/Game/Object/Stage/StageBlock/StageBlock.h"
#include "../src/Game/Object/Stage/MoveBlock/MoveBlock.h"
#include "../src/Game/Object/Stage/CheckArea/CheckArea.h"
#include "../src/Game/Object/Stage/Npc_Sheltter/Npc_Sheltter.h"
#include "../src/Game/Object/SoundEffect.h"
#include "../src/Game/Object/GoalArea.h"
#include "DxLib.h"
#include "../src/System/KeyInput.h"
#include "../src/System/Effect.h"
#include "../src/System/Vector3.h"
#include "../src/System/AppendFunction.h"
#include "../src/Game/Object/UserInterface.h"
#include <iostream>
#include <array>
#include <fstream>
#include <string>
#include <sstream>
// Scene関連
#include "../src/Game/Scene/SceneTitle.h"
#include "../src/Game/Scene/SceneRule.h"
#include "../src/Game/Scene/SceneNowLoading.h"
#include "../src/Game/Scene/SceneStage.h"

//当たり判定用の関数
// ==================================================
bool Game::GoblinBulletCheckHitWithNpc(const Base* obj1, const Goblin* obj2)
{
    return BulletAttachCheckToNpc(obj1->GetPos(), obj1->GetRadius(), obj2->GetBulletPos(), obj2->GetBulletRadius());
}
bool Game::GoblinBulletHitWithPlayer(const Base* obj1, const Goblin* obj2)
{
    return BulletAttachCheckToplayer(obj1->GetPos(), obj1->GetRadius(), obj2->GetBulletPos(), obj2->GetBulletRadius());
}
bool Game::RedGoblinRarmHitWithPlayer(const Base* obj1, const RedGoblin* obj2)
{
    if(obj2->Get_is_attacking()) {
        return AttachCheckWithPlayer(obj1->GetPos(),
                                     obj1->GetRadius(),
                                     obj2->GetframePos_R(),
                                     obj2->Getright_hand_radius());
    }
    return false;
}
// ==================================================
//設定見る用の関数
// ==================================================
bool Game::IsPause()
{
    //もしもEnterキーを押したらset画面が出ます
    if(IsKeyOn(KEY_INPUT_RETURN))
        return show_button ? false : true;
    //もしもボタンが押されていない場合は、現在のset_buttonの値が返る
    return show_button;
}
bool Game::GetMusicOn()
{
    if(IsKeyOn(KEY_INPUT_1))
        return music_on ? false : true;
    //もしもボタンが押されていない場合は、現在のmusic_onの値が返る
    return music_on;
}
GAME_SCREEN Game::UpdatePause()
{
    show_button = IsPause();
    //セットボタン押したらGAME_PAUSE画面に入ります
    if(show_button) {
        return GAME_SCREEN::GAME_PAUSE;
    }
    return GAME_SCREEN::GAME_STAGE;
}
// ==================================================
//ステージのBGMを再生する関数
// ==================================================
void Game::PlayStageBGM()
{
    if(CheckMusic() == 0) {
        // 音楽が再生していない状態
        if(music_on) {
            PlayMusic("Data/music/魔王魂  村07.mp3", DX_PLAYTYPE_LOOP);
        }
        return;
    }
    if(!music_on) {
        sound->ResetVolume();
        StopMusic();   //音楽再生の停止
    }
}
// --------------------------------------------------
//UI設定以外なステージ物件を描画
// --------------------------------------------------
void Game::StageShow_Render(bool shadow)
{
    if(shadow == false) {
        mapchip->DrawMap();
    }
    //影を描画したくない所
    if(shadow == false) {
        for(size_t i = 0; i < redgoblin.size(); i++) {
            if(!redgoblin[i]->GetIsLive())
                continue;
            redgoblin[i]->Render();
        }
        house->Render();
        for(size_t i = 0; i < goblin.size(); i++) {
            if(goblin[i]->Get_is_dead())
                continue;
            goblin[i]->Render();
        }
        for(size_t i = 0; i < zonbie.size(); i++) {
            if(!zonbie[i])
                continue;
            zonbie[i]->Render();
        }
        for(size_t i = 0; i < heal_item.size(); i++) {
            heal_item[i]->Update();
        }

        for(size_t i = 0; i < block_1.size(); i++) {
            block_1[i]->Render();
        }
        for(size_t i = 0; i < move_block.size(); i++) {
            move_block[i]->Render();
        }
        for(size_t i = 0; i < trigger_area.size(); i++) {
            trigger_area[i]->Render();   //トラップエリア実装
        }
        for(size_t i = 0; i < up_area.size(); i++) {
            up_area[i]->Render();   //スローエリア実装
        }
    }

    for(size_t i = 0; i < boss.size(); i++) {
        if(boss[i]->Get_is_dead())
            continue;
        boss[i]->Render();
    }

    player->Render();
    npc->Render(shadow);

    for(size_t i = 0; i < trap_ball.size(); i++) {
        trap_ball[i]->Render();
    }
    for(size_t a = 0; a < shoke_wave.size(); a++) {
        shoke_wave[a]->Render();
    }

    for(size_t i = 0; i < heal_item.size(); i++) {
        if(heal_item[i]->GetIsPicked())
            continue;
        heal_item[i]->Render();
    }

    sheltter->Render();
}
//----------------------------
//初期化処理
//----------------------------
void Game::Init()
{
    // --------------------------------------------------
    // GameTitleから使われたオブジェクトの初期化
    // --------------------------------------------------
    current_screen = GAME_SCREEN::GAME_TITLE;   //初期はタイトル
    model          = new Model();               //model実装
    model_2        = new Model_2();             //model実装
    camera         = new Camera;                //カメラ実装
    player         = new Player();              //プレイヤーとカメラと打つ弾丸を実装
    npc            = new Npc();                 //移動するNPC
    house          = new House();
    sound          = new SoundEffect();

    camera->SetPlayer(player);

    sound = new SoundEffect();

    //UI設定
    ui = new UserInterface;
    ui->Init();
    ui->SetCamera(camera);
    ui->SetPlayer(player);
    ui->SetNpc(npc);

    //マっプの設定
    mapchip = new MapChip;
    mapchip->BuildMap();

    GameStartAnime();
    GameStageInit();
    InitGameTitle();
    InitGameRule();
    sound->InitEffectSound();
    //セットボタン
    music_on    = true;
    show_button = false;
}
//----------------------------
//STAGEに入る前の処理
//----------------------------
void Game::StageInit()
{
    //全体一旦解放
    ResetStageData();

    //レッドゴブリンの数設定します
    for(size_t i = 0; i < RED_NUM; i++) {
        auto g = std::make_shared<RedGoblin>(i);
        g->SetNpc(npc);
        g->SetCamera(camera);
        g->SetPlayer(player);
        redgoblin.push_back(std::move(g));
    }

    //ゴブリンの数設定します
    for(size_t i = 0; i < GREEN_NUM; i++) {
        auto p = std::make_shared<Goblin>(i);
        p->SetPlayer(player);
        p->SetCamera(camera);
        p->SetNpc(npc);
        goblin.push_back(std::move(p));
    }

    //レッドゴブリンの数設定します
    for(size_t i = 0; i < ZONBIE_NUM; i++) {
        auto z = std::make_shared<Zonbie>(i);
        z->SetNpc(npc);
        z->SetCamera(camera);
        z->SetPlayer(player);
        zonbie.push_back(std::move(z));
    }
    for(size_t i = 0; i < BOSS_NUM; i++) {
        auto g = std::make_shared<Boss>(i);
        g->SetPlayer(player);
        boss.push_back(std::move(g));
    }

    //ブロックの設定
    for(size_t i = 0; i < BLOCK_NUM; i++) {
        auto p = std::make_shared<StageBlock>(i);
        p->SetPlayer(player);
        block_1.push_back(std::move(p));
    }

    //ブロックの設定
    for(size_t i = 0; i < MOVE_BLOCK_NUM; i++) {
        auto m = std::make_shared<MoveBlock>(i);
        m->SetPlayer(player);
        move_block.push_back(std::move(m));
    }
    //トラップArea設定
    for(size_t i = 0; i < TRIGGER_AREA; i++) {
        auto t = std::make_shared<TriggerArea>(i);
        t->SetPlayer(player);
        trigger_area.push_back(std::move(t));
    }

    //上に行くエリアを設定します
    for(size_t i = 0; i < UP_AREA; i++) {
        auto s = std::make_shared<UpArea>(i);
        up_area.push_back(std::move(s));
    }

    //ボスのボール攻撃を設定します
    for(size_t i = 0; i < TRAP_BALL; i++) {
        auto t = std::make_shared<TrapBall>(i);
        t->SetPlayer(player);
        t->SetBoss(boss[0].get());
        trap_ball.push_back(std::move(t));
    }

    //ボスのウェーブ攻撃を設定します
    for(size_t i = 0; i < SHOKE_WAVE; i++) {
        auto w = std::make_shared<ShokeWave>();
        w->SetPlayer(player);
        w->SetBoss(boss[0].get());
        shoke_wave.push_back(std::move(w));
    }

    //治療アイテムを設定します
    for(size_t i = 0; i < HEAL_ITEM_NUM; i++) {
        auto h = std::make_shared<HealItem>(i);
        h->SetPlayer(player);
        heal_item.push_back(std::move(h));
    }

    //プレイヤーのチェック用
    check_area = new CkeckArea();
    check_area->SetPlayer(player);

    //シェルター
    sheltter = new Npc_Sheltter();

    //ゴールエリア実装
    goal_area = new GoalArea();
    goal_area->SetPlayer(player);
    goal_area->SetNpc(npc);
    goal_area->SetBoss(boss[0].get());

    //プレーヤ
    player->PlayerReset();
    player->SetCamera(camera);
    player->SetGoalArea(goal_area);
    player->SetBoss(boss[0].get());
    player->SetShokeWave(shoke_wave[0].get());
    //npc
    npc->NpcReset();
    npc->SetGoal(goal_area);
    npc->SetPlayer(player);
    npc->SetBoss(boss[0].get());
    npc->SetCamera(camera);
    npc->SetCkeckArea(check_area);
    npc->Set_Sheltter(sheltter);

    camera->Reset_count();
    camera->SetPlayer(player);
    camera->SetBoss(boss[0].get());

    //UI設定
    ui->Init();
    ui->SetCamera(camera);
    ui->SetBoss(boss[0].get());

    //モデル
    model_2->Reset();
    //サウンド
    sound->InitEffectSound();



    //毎回使うトリガーを
    all_zonbi_dead = false;
    dead_count     = 0;
}
//更新処理
//-----------------------------------
void Game::Update()
{
    //1回でもクリアしたら、クリアのまま
    if(is_clear) {
        is_clear = true;
    }

    switch(current_screen) {
    case GAME_SCREEN::GAME_TITLE:
        camera->SetNowCamera(1);
        camera->Update();
        model->Update(0, is_clear);
        house->Update(0);
        //タイトル画面の更新処理
        current_screen = UpdateGameTitle();
        break;
    case GAME_SCREEN::GAME_RULE:
        current_screen = UpdateGameRule();
        break;
    case GAME_SCREEN::GAME_NOWLOADING:
        StageInit();
        house->Update(1);   //ヘアの番号更新
        camera->Update();   //カメラの更新
        //Boss
        for(size_t i = 0; i < boss.size(); i++) {
            if(boss[i]->Get_is_dead())
                break;
            boss[i]->Update();
        }
        player->Update();
        npc->Update();
        current_screen = UpdateGameNowLoading();
        break;
    case GAME_SCREEN::GAME_STAGE:
        PlayStageBGM();                   //BGMを流す
        current_screen = UpdatePause();   //Pause処理
        //モンスターを倒したら
        if(boss[0]->Get_is_dead()) {
            //その後プレイヤーが矢印の所に行ったらゲームクリア
            if(CheckInTrigger(player->GetPos(), goal_area->GetClearAreaStart(), goal_area->GetClearAreaEnd())) {
                clear_count++;   //クリア回数増える
                current_screen = GAME_SCREEN::GAME_CLEAR_ANIME;
            }
        }

        //トラップArea設定
        for(size_t i = 0; i < trigger_area.size(); i++) {
            trigger_area[i]->Update();
            if(!trigger_area[i]->CheckHitStageFloor())
                continue;
            trigger_area[i]->SetTrigger(true);
        }

        //ゾンビの死ぬ数
        if(dead_count == ZONBIE_NUM) {
            all_zonbi_dead = true;
        }

        CheckTrigger = CheckSquareHit_WithPlayerandNPC(player, npc, up_area[0].get());   //□のトリガーのチェック

        //トリガー起動のアップデート
        for(size_t i = 0; i < up_area.size(); i++) {
            up_area[i]->Update(CheckTrigger);
        }
        //ギミック起動の確認
        for(size_t i = 0; i < move_block.size(); i++) {
            move_block[i]->Move(all_zonbi_dead, CheckTrigger, trigger_area[0]->GetTrigger(), boss[0]->Get_is_dead());
        }

        //キャラクターとブロックとの当たり判定
        //player
        player->ResetBeforeCheck();   //判定が被らないように判定前に先に上と下の判定を消します
        for(size_t i = 0; i < move_block.size(); i++) {
            move_block[i]->Update_player();
            if(player->HitWithMoveBlock(move_block[i].get())) {
                break;
            }
        }
        //ムーヴブロックとゴールバーの上に立ってないならこちの処理に入ります
        if(!player->GetOnMoveBlock()) {
            //ブロック普通のブロックの上に立つ判定が発動する
            player->ResetBeforeCheck();   //判定が被らないように判定前に先に上と下の判定を消します
            for(size_t i = 0; i < block_1.size(); i++) {
                block_1[i]->Update_player();
                player->HitWithStageBlock(block_1[i].get());
            }
        }
        //npc
        for(size_t i = 0; i < move_block.size(); i++) {
            if(npc->HitWithMoveBlock(move_block[i].get())) {
                break;
            }
        }
        if(!npc->GetOnMoveBlock()) {
            for(size_t i = 0; i < block_1.size(); i++) {
                npc->HitWithStageBlock(block_1[i].get());
            }
        }
        //redgoblin
        //!<レッドゴブリンとブロックの判定
        for(size_t i = 0; i < redgoblin.size(); i++) {
            for(size_t x = 0; x < move_block.size(); x++) {
                if(redgoblin[i]->HitWithMoveBlock(move_block[x].get())) {
                    break;
                }
            }
            if(!redgoblin[i]->GetOnMoveBlock()) {
                //ブロック普通のブロックの上に立つ判定が発動する
                for(size_t z = 0; z < block_1.size(); z++) {
                    redgoblin[i]->HitWithStageBlock(block_1[z].get());
                }
            }
        }
        //goblin
        for(size_t i = 0; i < goblin.size(); i++) {
            for(size_t x = 0; x < move_block.size(); x++) {
                if(goblin[i]->HitWithMoveBlock(move_block[x].get())) {
                    break;
                }
            }
            if(!goblin[i]->GetOnMoveBlock()) {
                for(size_t z = 0; z < block_1.size(); z++) {
                    goblin[i]->HitWithStageBlock(block_1[z].get());
                }
            }
        }
        //zonbie
        for(size_t i = 0; i < zonbie.size(); i++) {
            if(zonbie[i]) {
                for(size_t x = 0; x < move_block.size(); x++) {
                    if(zonbie[i]->HitWithMoveBlock(move_block[x].get())) {
                        break;
                    }
                }
                if(!zonbie[i]->GetOnMoveBlock()) {
                    for(size_t z = 0; z < block_1.size(); z++) {
                        zonbie[i]->HitWithStageBlock(block_1[z].get());
                    }
                }
            }
        }

        //弾丸と当たり判定
        //弾丸との当たり判定
        //モンスターとPlayerの弾丸
        for(size_t i = 0; i < redgoblin.size(); i++) {
            if(!CheckCircleHit_WithGoblin(player, redgoblin[i].get()))
                continue;
            redgoblin[i]->SetPos_HurtByPlayerBullet(player->GetBulletRot());
        }

        //playerの弾丸とゴブリン
        for(size_t i = 0; i < goblin.size(); i++) {
            if(!CheckCircleHit_WithGoblin(player, goblin[i].get()))
                continue;

            goblin[i]->SetPos_HurtByPlayerBullet(player->GetBulletRot());
        }

        //playerの弾丸とBoss
        for(size_t i = 0; i < boss.size(); i++) {
            //Playerが地下つく前には起動しない
            if(!boss[i]->GetIsAttatched())
                break;
            if(!CheckCircleHit_WithBoss(player, boss[i].get()))
                continue;
            boss[i]->SetPos_HurtByPlayerBullet(player->GetBulletRot());
        }

        for(size_t i = 0; i < redgoblin.size(); i++) {
            if(!RedGoblinRarmHitWithPlayer(player, redgoblin[i].get()))
                continue;
            player->ResetCount(redgoblin[i]->GetAttachRot());
        }

        for(size_t i = 0; i < goblin.size(); i++) {
            if(!GoblinBulletCheckHitWithNpc(npc, goblin[i].get()))
                continue;
            npc->ResetCount();
        }
        for(size_t i = 0; i < goblin.size(); i++) {
            if(!GoblinBulletHitWithPlayer(player, goblin[i].get()))
                continue;
            player->ResetCount(goblin[i]->GetBulletRot());
        }

        //Update管理
        for(size_t i = 0; i < zonbie.size(); i++) {
            if(!zonbie[i])
                continue;
            if(!CheckCircleHit_WithZonbieHand(player, zonbie[i].get()))
                continue;
            player->ResetCount(zonbie[i]->Get_attack_rot());
        }

        //ボストンのトラップボールとプレーヤーの当たり判定
        for(size_t i = 0; i < trap_ball.size(); i++) {
            if(!AttachCheck_BalltoChractor(trap_ball[i]->GetPos(),
                                           trap_ball[i]->GetRadius(),
                                           player->GetPos(),
                                           player->GetRadius(),
                                           150.0f))
                continue;
            player->SetHurtByBall();
        }

        //ゾンビとプレーヤー
        for(size_t i = 0; i < zonbie.size(); i++) {
            if(!zonbie[i])
                continue;
            if(!CheckCircleHit_WithZonbie(player, zonbie[i].get()))
                continue;
            zonbie[i]->SetPos_HurtByPlayerBullet(player->GetBulletRot());
            //もしもNPCを捕まっているならZonbieにもNpcにも解放
            if(zonbie[i]->Getcatch_npc())
                zonbie[i]->ReleaseNpc();
            npc->ReleaseFromZonbie();
        }

        //拾うアイテムの当たり判定
        for(size_t i = 0; i < heal_item.size(); i++) {
            if(!CheckHitStageObject(heal_item[i].get(), player))
                continue;
            player->Heal();
            npc->Heal();
            sound->PlayGetSound();
            heal_item[i]->SetIsPicked(true);
        }

        //NPCがゾンビにつかまれるがどうがの確認
        for(size_t i = 0; i < zonbie.size(); i++) {
            if(!zonbie[i])
                continue;
            //もしも捕まっていないなら
            if(!zonbie[i]->Getcatch_npc())
                continue;
            //もしも捕まっていたらゾンビの座標をNPCに送りつつける
            npc->SetPosBeCatch(zonbie[i]->GetPosBeCatch());
        }

        //モンスターの全てのアップデート
        //RedGoblin
        for(size_t i = 0; i < redgoblin.size(); i++) {
            if(!redgoblin[i]->GetIsLive())
                continue;
            redgoblin[i]->Update();
        }

        //Goblin
        for(size_t i = 0; i < goblin.size(); i++) {
            if(goblin[i]->Get_is_dead())
                continue;
            goblin[i]->Update();
        }

        //Boss
        for(size_t i = 0; i < boss.size(); i++) {
            if(boss[i]->Get_is_dead())
                break;
            boss[i]->Update();
        }
        //Bossの攻撃
        for(size_t x = 0; x < trap_ball.size(); x++) {
            trap_ball[x]->Update();
        }
        for(size_t a = 0; a < shoke_wave.size(); a++) {
            shoke_wave[a]->Update();
        }

        //Zonbie
        for(size_t i = 0; i < zonbie.size(); i++) {
            if(!zonbie[i])
                continue;
            zonbie[i]->Update();
        }

        //ゾンビが死ぬ時確認用の判定
        for(size_t i = 0; i < zonbie.size(); i++) {
            if(!zonbie[i])
                continue;
            if(zonbie[i]->GetIsLive() == false) {
                //ここで死亡フラグ判定
                dead_count++;
                zonbie[i].reset();
            }
        }
        for(size_t i = 0; i < heal_item.size(); i++) {
            heal_item[i]->Update();
        }

        camera->Update();   //カメラの更新
        player->Update();
        npc->Update();
        check_area->Update();
        house->Update(1);
        sound->Update();
        goal_area->clear_arror_update();
        ui->TimeCountFlame();
        //ゲームオーヴァー処理
        if((ui->GetTimeCount() < 0) || (player->Get_is_dead()) || (npc->Get_is_dead())) {
            current_screen = GAME_SCREEN::GAME_OVER;
        }
        break;
    case GAME_SCREEN::GAME_PAUSE:
        PlayStageBGM();
        // Pause処理
        current_screen = UpdatePause();
        music_on       = GetMusicOn();
        //音量維持
        SetVolumeMusic(sound->SetVolume());
        sound->Update();
        break;
    case GAME_SCREEN::GAME_CLEAR_ANIME:
        //ムーヴィースキップ
        if(camera->GetEndAnimeCount() < 0 || ((GetMouseInput() & MOUSE_INPUT_LEFT) == 1)) {
            current_screen = GAME_SCREEN::GAME_CLEAR;
        }
        //ギミック起動の確認
        for(size_t i = 0; i < move_block.size(); i++) {
            move_block[i]->Move(all_zonbi_dead, CheckTrigger, trigger_area[0]->GetTrigger(), boss[0]->Get_is_dead());
        }
        //カメラアップデート
        camera->SetNowCamera(4);
        camera->Update();
        //NPCとプレーヤーのアップデート
        player->Update();
        npc->Update();
        break;
    case GAME_SCREEN::GAME_OVER:
        StopMusic();
        camera->SetNowCamera(5);
        camera->Update();
        model->Update(3, is_clear);
        ui->Update();
        if(ui->Getcheck()) {
            if((GetMouseInput() & MOUSE_INPUT_LEFT) == 1){
                    if(CheckMusic() == 0) {
                        sound->PlayEffectSound();
                    }
                    current_screen = GAME_SCREEN::GAME_TITLE;
                }
        }
        break;
    case GAME_SCREEN::GAME_CLEAR:
        is_clear = true;
        StopMusic();
        camera->SetNowCamera(3);
        camera->Update();
        model->Update(2, is_clear);
        model_2->Update(2);
        ui->Update();
        if(ui->Getcheck()) {
                if((GetMouseInput() & MOUSE_INPUT_LEFT) == 1) {
                    if(CheckMusic() == 0) {
                        sound->PlayEffectSound();
                    }
                    current_screen = GAME_SCREEN::GAME_TITLE;
                }
        }
        break;
    }
}
//描画処理
//---------------------------------------------
void Game::Render(bool shadow_)
{
    if(shadow_ == false) {
        camera->Render();
    }
    switch(current_screen) {
    case GAME_SCREEN::GAME_TITLE:
        //フォント設定
        ChangeFont("ＭＳ 明朝");
        SetBackgroundColor(100, 200, 255);
        //モデルとNPC設定
        if(shadow_ == false) {
            house->Render();
            model->Render(clear_count);
        }
        RenderGameTitle();
        break;
    case GAME_SCREEN::GAME_RULE:
        RenderGameRule();
        break;
    case GAME_SCREEN::GAME_NOWLOADING:
        DrawGameNowLoadingText();
        break;
    case GAME_SCREEN::GAME_STAGE:
        StageShow_Render(shadow_);
        //クリアした時の矢印
        goal_area->Render_clear_arror();
        if(shadow_ == false) {
            ui->DrawUi(all_zonbi_dead);
        }
        break;
    case GAME_SCREEN::GAME_PAUSE:
        StageShow_Render(shadow_);
        goal_area->Render_clear_arror();
        ui->DrawGamepauseText(music_on, sound->GetVolume());   //設定画面の描画
        break;
    case GAME_SCREEN::GAME_CLEAR_ANIME:
        StageShow_Render(shadow_);
        if(shadow_ == false) {
            ui->DrawUi(all_zonbi_dead);
        }
        break;
    case GAME_SCREEN::GAME_CLEAR:
        camera->Render();
        model->Render(clear_count);
        model_2->Render(clear_count);
        ui->DrawGameClearText();
        break;
    case GAME_SCREEN::GAME_OVER:
        camera->Render();
        model->Render(clear_count);
        ui->DrawGameoverText();
        SetBackgroundColor(200, 200, 200);
        break;
    }
}
//ステーシ始まる前に倒していないものを全部解放します
void Game::ResetStageData()
{
    if(goal_area) {
        delete goal_area;
        goal_area = nullptr;
    }
    trap_ball.clear();
    goblin.clear();
    redgoblin.clear();
    boss.clear();
    move_block.clear();
    block_1.clear();
    trigger_area.clear();
    up_area.clear();
    heal_item.clear();
    zonbie.clear();
    shoke_wave.clear();

    if(check_area) {
        delete check_area;
        check_area = nullptr;
    }
    if(sheltter) {
        delete sheltter;
        sheltter = nullptr;
    }
}
void Game::Exit()
{
    //ゲーム終わっていた時全ての物は解放される
    if(model) {
        delete model;
        model = nullptr;
    }
    if(model_2) {
        delete model_2;
        model_2 = nullptr;
    }
    if(player) {
        delete player;
        player = nullptr;
    }
    if(npc) {
        delete npc;
        npc = nullptr;
    }
    if(house) {
        delete house;
        house = nullptr;
    }
    if(camera) {
        delete camera;
        camera = nullptr;
    }
    if(mapchip) {
        delete mapchip;
        mapchip = nullptr;
    }
    if(ui) {
        delete ui;
        ui = nullptr;
    }
    if(sound) {
        delete sound;
        sound = nullptr;
    }
    trap_ball.clear();
    goblin.clear();
    redgoblin.clear();
    boss.clear();
    move_block.clear();
    block_1.clear();
    trigger_area.clear();
    up_area.clear();
    heal_item.clear();
    zonbie.clear();
    shoke_wave.clear();
    if(goal_area) {
        delete goal_area;
        goal_area = nullptr;
    }
    if(check_area) {
        delete check_area;
        check_area = nullptr;
    }
    if(sheltter) {
        delete sheltter;
        sheltter = nullptr;
    }
}
