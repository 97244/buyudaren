
#ifndef FishingJoy_Bullet_h
#define FishingJoy_Bullet_h

#include "cocos2d.h"

class GameLayer;

class Bullet : public cocos2d::Object
{
public:
    static Bullet *createWithBulletType(
		int bulletType, GameLayer *gameLayer, 
		cocos2d::SpriteBatchNode *pBatchNodeBullet, 
		cocos2d::SpriteBatchNode *pBatchNodeNet);
    
    CC_SYNTHESIZE_READONLY(bool, m_bCaught, Caught);
    void shootTo(cocos2d::Point targetDirection);
    void showNet();
private:
    CC_SYNTHESIZE(GameLayer *, m_pGameLayer, GameLayer);
    CC_SYNTHESIZE(int, m_nBulletType, BulletType);
    CC_SYNTHESIZE(cocos2d::Sprite *, m_pSpriteBullet, SpriteBullet);
    CC_SYNTHESIZE(cocos2d::Sprite *, m_pSpriteNet, SpriteNet);
    bool initWithBulletType(int bulletType, GameLayer *gameLayer, cocos2d::SpriteBatchNode *pBatchNodeBullet, cocos2d::SpriteBatchNode *pBatchNodeNet);
    void removeSelf();
};

#endif
