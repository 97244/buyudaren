#ifndef __FishingJoy__RollNum__
#define __FishingJoy__RollNum__

#include <iostream>
#include "cocos2d.h"

class GameLayer;

class RollNum : public cocos2d::Sprite
{
public:
    RollNum();
    virtual ~RollNum();
    CREATE_FUNC(RollNum);
    CC_SYNTHESIZE(bool, m_bUp, UpDirection);
    
    void setNumber(int var, bool bUp);
    int getNumber();
private:
    bool init();
    int m_nNumber;
    int m_nCurTexH;
    int m_nEndTexH;
    bool m_bRolling;
    cocos2d::Texture2D *m_pTexture;
    void updateNumber(float dt);
};

class RollNumGroup : public cocos2d::Object
{
public:
    RollNumGroup();
    virtual ~RollNumGroup();
    static RollNumGroup *createWithGameLayer(GameLayer *pGameLayer, int nDigit);
    CC_SYNTHESIZE(cocos2d::SpriteBatchNode *, m_pBatchNode, BatchNode);
    CC_SYNTHESIZE(GameLayer *, m_pGameLayer, GameLayer);
    CC_SYNTHESIZE_RETAIN(cocos2d::Array *, m_pRollNumArray, RollNumArray);
    void setValue(int nValue);
    void setPosition(cocos2d::Point pt);
private:
    cocos2d::Point m_ptPosition;
    int m_nDigit;
    int m_nValue;
    bool initWithGameLayer(GameLayer *pGameLayer, int nDigit);
};

#endif 
