
#ifndef __FishingJoyStep1__Fish__
#define __FishingJoyStep1__Fish__

#include "cocos2d.h"

USING_NS_CC;

class GameLayer;

class Fish : public cocos2d::Object{
public:

    virtual ~Fish();
    static Fish * createWithFishType(int fishType,GameLayer *gameLayer,cocos2d::SpriteBatchNode *pBatchNodeFish);
    void showCaught();

public:
    CC_SYNTHESIZE_READONLY(bool, m_bCaught, Caught);
    CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, m_pSpriteFish, SpriteFish);

	void offsetPoint(Point& pt, float offsetX, float offsetY);
private:

    bool initWithFishType(int fishType,GameLayer *gameLayer,cocos2d::SpriteBatchNode *pBatchNodeFish);

    void removeSelf();

    void getPath(cocos2d::MoveTo *&moveto);


private:
    CC_SYNTHESIZE(GameLayer *, m_pGameLayer, GameLayer);
    CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNodeFish, BatchNodeFish);
    CC_SYNTHESIZE(int, m_nFishType, FishType);
    CC_SYNTHESIZE(cocos2d::ParticleSystem *, m_pParticleBubble, ParticleBubble);
    bool m_bParticleBubble;
};

#endif