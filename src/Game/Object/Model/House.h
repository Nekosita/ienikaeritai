#include "ModelBase.h"
#include <vector>


//! キャラクター
//===============================================
class House : public ModelBase
{
private:

    //!カメラのパターンの設定
    enum HOUSE_PATTERN
    {
        GAME_TITLE,
        GAME_STAGR,
        NOTHING
    };

    HOUSE_PATTERN house;

public:

    //! コンストラクタ
    House();
    //! デストラクタ
    virtual ~House();

    //!更新処理
    void Update(int now_house);
    //!描画処理
    void Render();
    //!終了処理
    void Exit();
};
