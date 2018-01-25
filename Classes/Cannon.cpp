#include "Cannon.h"
#include "Bullet.h"
#include "GameLayer.h"

USING_NS_CC;

Cannon* Cannon::createWithCannonType(int cannonType, GameLayer *pGameLayer, cocos2d::SpriteBatchNode *pBatchNode){
    Cannon* pCannon = new Cannon(cannonType,pGameLayer);
    
	if(pCannon && pCannon->initWithCannonType(pBatchNode))
	{
        pCannon->autorelease();
        return pCannon;
    }else
	{
        CC_SAFE_DELETE(pCannon);
        return NULL;
    }
}


Cannon::Cannon(int cannonType,GameLayer *gameLayer):
    m_nCannonType(cannonType),m_pGameLayer(gameLayer),
    m_fRotation(0.0f)
{

}

Cannon::~Cannon()
{
	this->m_pSprite->removeFromParentAndCleanup(true);
}

int Cannon::getConnonType()
{
    return this->m_nCannonType;
}


bool Cannon::initWithCannonType( cocos2d::SpriteBatchNode *pBatchNode)
{
    
    String* cannonName=String::createWithFormat("actor_cannon1_%d1.png",m_nCannonType);
    this->m_pSprite=CCSprite::createWithSpriteFrameName(cannonName->getCString()); 

    this->m_pSprite->setPosition(ccp(520, 50));

    pBatchNode->addChild(m_pSprite);
    return true;
}

void Cannon::rotateToPoint(cocos2d::Point ptTo)
{
    Point ptFrom=m_pSprite->getPosition();
    float angle=atan2f(ptTo.y-ptFrom.y,ptTo.x-ptFrom.x)/M_PI * 180.0f;
    this->setRotation(90.0f-angle);
    this->m_pDirection= ptTo;
}

void Cannon::setRotation(float rotation)
{
	m_fRotation=rotation;
	float absf_rotation=fabsf(m_fRotation-this->m_pSprite->getRotation());
    float duration=absf_rotation/180.0f*0.2f;

    FiniteTimeAction *pAction = RotateTo::create(duration, m_fRotation);
    this->m_pSprite->runAction(pAction);
}

void Cannon::fire()
{

    String *pFireStartFrameName=String::createWithFormat("actor_cannon1_%d1.png",this->m_nCannonType);
    String *pFireEndFrameName=String::createWithFormat("actor_cannon1_%d2.png",this->m_nCannonType);
	SpriteFrame *pFireStartFrame=SpriteFrameCache::getInstance()->spriteFrameByName(pFireStartFrameName->getCString());
    SpriteFrame *pFireEndFrame=SpriteFrameCache::getInstance()->spriteFrameByName(pFireEndFrameName->getCString());
	Vector<SpriteFrame*> frames;
	frames.pushBack(pFireStartFrame);
	frames.pushBack(pFireEndFrame);
	Animation *pAnimationFire=Animation::createWithSpriteFrames(
		frames
		,0.1f);
    pAnimationFire->setRestoreOriginalFrame(true);
    FiniteTimeAction *pAction=CCAnimate::create(pAnimationFire);
    this->m_pSprite->runAction(pAction);
    
    Bullet *pBullet = Bullet::createWithBulletType(m_nCannonType, this->m_pGameLayer, this->m_pGameLayer->getBatchNodeFish2AndBullets(), this->m_pGameLayer->getBatchNodeFish3AndNets());
    pBullet->shootTo(this->m_pDirection);
}