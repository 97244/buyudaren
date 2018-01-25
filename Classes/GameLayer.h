#ifndef FishingJoyStep1_GameLayer_h
#define FishingJoyStep1_GameLayer_h

#include "cocos2d.h"
#include "Cannon.h"
#include "RollNum.h"

  
class GameLayer : public cocos2d::Layer{
public:
    GameLayer();

    virtual ~GameLayer();
    virtual bool init();
    static cocos2d::Scene* scene();

	virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

    CC_SYNTHESIZE_RETAIN(cocos2d::Array *, m_pBullets, Bullets);
    CC_SYNTHESIZE_RETAIN(cocos2d::Array *, m_pFishes, Fishes);
    
	//CC_SYNTHESIZE_RETAIN(RollNumGroup *, m_pRollNumGroup, RollNumGroup);
	CC_SYNTHESIZE(RollNumGroup *, m_pRollNumGroup, RollNumGroup);

    CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNodeFish1, BatchNodeFish1);
    CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNodeFish2AndBullets, BatchNodeFish2AndBullets);
    CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNodeFish3AndNets, BatchNodeFish3AndNets);
    CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNodeFish4, BatchNodeFish4);
    
    CREATE_FUNC(GameLayer);
private:
     
    int m_nScore;
    GameLayer(const GameLayer &other);
    CC_SYNTHESIZE_RETAIN(Cannon *, m_pCannon, Cannon);
    cocos2d::Sprite *m_pSpriteAdd;
    cocos2d::Sprite *m_pSpriteReduce;

	void initListeners();
    void initFrames();
    void initBackground();
    void initCannon();
    void initFishes();
    void updateGame(float dt);
    void updateFish(float dt);
    void addFish();
    void addConnonSize();
    void reduceConnonSize();
};

#endif
