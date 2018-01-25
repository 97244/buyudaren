#include "Fish.h"
#include "GameLayer.h"

USING_NS_CC;

Fish::~Fish()
{
    CCLOG("destruct fish %d", m_nFishType);
}


Fish *Fish::createWithFishType(int fishType, GameLayer *gameLayer, SpriteBatchNode *pBatchNode)
{
    Fish *fish = new Fish();
	if(fish && fish->initWithFishType(fishType, gameLayer, pBatchNode) )
    {
        fish->autorelease();
        return fish;
    }
    else
    {
        delete fish;
        return NULL;
    }
}


bool Fish::initWithFishType(int fishType, GameLayer *gameLayer, SpriteBatchNode *pBatchNode)
{
    m_bCaught = false; 
    this->setFishType(fishType);
    this->setGameLayer(gameLayer);
    this->setBatchNodeFish(pBatchNode);
    m_bParticleBubble = false;
    
    if(m_nFishType == 11 || m_nFishType == 12)
        m_bParticleBubble = true;
    
    //auto *frames = Array::create();
	Vector<SpriteFrame*> frames;
    for(int i = 1; i <= 16; i++)
    {

        String *frameName = cocos2d::String::createWithFormat("fish%02d_%02d.png", fishType, i);

		SpriteFrame *pFrame = cocos2d::SpriteFrameCache::getInstance()->spriteFrameByName(frameName->getCString());
        if(pFrame) {            
            //frames->addObject(pFrame);
			frames.pushBack(pFrame);
        }
    }

    Animation *animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.2f);
    animation->setRestoreOriginalFrame(false);
    Animate *animate = Animate::create(animation);

    Action *swing = RepeatForever::create(animate);
    
    String *originalFrameName = String::createWithFormat("fish%02d_01.png", fishType);
    m_pSpriteFish = Sprite::createWithSpriteFrameName(originalFrameName->getCString());

	m_pSpriteFish->runAction(swing); 


    m_pSpriteFish->setAnchorPoint(ccp(0.5f, 0.5f));
	
    MoveTo *moveto = NULL;

    this->getPath(moveto);

	FiniteTimeAction *releaseFunc = CCCallFunc::create(this, callfunc_selector(Fish::removeSelf));
    FiniteTimeAction *sequence = Sequence::create(moveto, releaseFunc, NULL);
    m_pSpriteFish->runAction(sequence);
    
    this->getGameLayer()->getFishes()->addObject(this);

    this->getBatchNodeFish()->addChild(m_pSpriteFish);
    
    return true;
}


void Fish::showCaught()
{

    m_bCaught = true;
    m_pSpriteFish->stopAllActions();
    
    if(m_bParticleBubble && m_pParticleBubble){
        m_pParticleBubble->stopAllActions();
        m_pParticleBubble->setVisible(false);
    }
    
    //CCArray *frames = CCArray::createWithCapacity(11);
	Vector<SpriteFrame*> frames;
    for(int i = 1; i <= 4; i++) 
    {

        String *frameName = String::createWithFormat("fish%02d_catch_%02d.png", m_nFishType ,i);
		SpriteFrame *pFrame = SpriteFrameCache::getInstance()->spriteFrameByName(frameName->getCString());
        if(pFrame){
           //frames->addObject(pFrame);
		   frames.pushBack(pFrame);
        }
    }
   
    Animation *animation = cocos2d::Animation::createWithSpriteFrames(frames, 0.3f);
    Animate *animate = Animate::create(animation);

    FiniteTimeAction *callFunc = CCCallFunc::create(this, callfunc_selector(Fish::removeSelf));
    FiniteTimeAction *sequence = Sequence::create(animate, callFunc, NULL);
    m_pSpriteFish->runAction(sequence);
    
}

void Fish::offsetPoint(Point& pt, float offsetX, float offsetY)
{
    pt.x += offsetX;
    pt.y += offsetY;
}

void Fish::getPath(cocos2d::CCMoveTo *&moveto)
{
    Size fishSize = m_pSpriteFish->getContentSize();
	Size winSize = cocos2d::Director::getInstance()->getWinSize();
    
    Point ptStart, ptEnd;
    float radius = MAX(fishSize.width, fishSize.height) / 2;
    switch (rand() % 4) {
          
        case 0:
            ptStart.x = - radius;
            ptStart.y = rand() % (int)winSize.height;
          
            ptEnd.x = winSize.width + radius;
            ptEnd.y = rand() % (int)winSize.height;
            break;
        case 1:

            ptStart.x = winSize.width + radius;
            ptStart.y = rand() % (int)winSize.height;
            ptEnd.x = - radius;
            ptEnd.y = rand() % (int)winSize.height;
            break;
        case 2:
         
            ptStart.x = rand() % (int)winSize.width;
            ptStart.y = - radius;
         
            ptEnd.x = rand() % (int)winSize.width;
            ptEnd.y = winSize.height + radius;
            break;
        case 3:
           
            ptStart.x = rand() % (int)winSize.width;
            ptStart.y = winSize.height + radius;
          
            ptEnd.x = rand() % (int)winSize.width;
            ptEnd.y = - radius;
            break;
        default:
            break;
    }
  
    float angle = atan2f(ptEnd.y - ptStart.y, ptEnd.x - ptStart.x);

    float rotation = 180.0f - angle * 180.0f / M_PI;
    

    float duration = rand() % 10 + 4.0f;
   
    m_pSpriteFish->setPosition(ptStart);
    m_pSpriteFish->setRotation(rotation);
    
    moveto = MoveTo::create(duration, ptEnd);
    
    if(m_bParticleBubble)
    {
        this->setParticleBubble(ParticleSystemQuad::create("bubble.plist"));
        m_pGameLayer->addChild(m_pParticleBubble);
        float w = m_pSpriteFish->getContentSize().width / 2.0f;
        offsetPoint(ptStart, cosf(angle) * w, sinf(angle) * w);
        m_pParticleBubble->setPosition(ptStart);
        offsetPoint(ptEnd, cosf(angle) * w, sinf(angle) * w);
        Action *act = MoveTo::create(moveto->getDuration(), ptEnd);
        m_pParticleBubble->setAutoRemoveOnFinish(false);
        m_pParticleBubble->setPositionType(kCCPositionTypeFree);
        m_pParticleBubble->runAction(act);
    }
}

void Fish::removeSelf()
{
    this->getGameLayer()->getFishes()->removeObject(this); 
    m_pSpriteFish->removeFromParentAndCleanup(true);

    if(m_bParticleBubble && m_pParticleBubble)
       m_pParticleBubble->removeFromParentAndCleanup(true);
}