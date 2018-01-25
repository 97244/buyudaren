
#ifndef FishingJoyStep1_Cannon_h
#define FishingJoyStep1_Cannon_h

#include "cocos2d.h"

class GameLayer;

class Cannon : public cocos2d::Object{
public:

    static Cannon* createWithCannonType(int cannonType,GameLayer* pGameLayer,cocos2d::SpriteBatchNode* pBatchNode);

    void rotateToPoint(cocos2d::Point pt);
	
    void fire();
    int getConnonType();
    
    virtual ~Cannon();
    
private: 
    Cannon();
    Cannon(const Cannon &obj);

    Cannon(int cannonType,GameLayer *gameLayer);
    

    int m_nCannonType;
    CC_SYNTHESIZE(GameLayer *, m_pGameLayer, GameLayer);
    //GameLayer *m_pGameLayer;
    float m_fRotation;
    cocos2d::Sprite *m_pSprite;
    cocos2d::Point m_pDirection;

    void setRotation(float rotation);

    bool initWithCannonType(cocos2d::SpriteBatchNode* pBatchNode);
};

#endif

