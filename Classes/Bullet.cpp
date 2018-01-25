#include <iostream>
#include "Bullet.h"
#include "GameLayer.h"

using namespace cocos2d;

Bullet *Bullet::createWithBulletType(int bulletType, GameLayer *gameLayer, SpriteBatchNode *pBatchNodeBullet, SpriteBatchNode *pBatchNodeNet)
{
    Bullet *pBullet = new Bullet();
    if(pBullet && pBullet->initWithBulletType(bulletType, gameLayer, pBatchNodeBullet, pBatchNodeNet))
    {
        pBullet->autorelease();
        return pBullet;
    }
    else
    {
        CC_SAFE_DELETE(pBullet);
        return NULL;
    }
}

bool Bullet::initWithBulletType(int bulletType, GameLayer *gameLayer, SpriteBatchNode *pBatchNodeBullet, SpriteBatchNode *pBatchNodeNet)
{
        m_bCaught = false; 
        this->setBulletType(bulletType);
        this->setGameLayer(gameLayer);
        String *bulletName = String::createWithFormat("bullet0%d.png", m_nBulletType);
        this->setSpriteBullet(Sprite::createWithSpriteFrameName(bulletName->getCString())); 
        m_pSpriteBullet->setPosition(ccp(520, 50));
        pBatchNodeBullet->addChild(m_pSpriteBullet);

        String *nodenetName = String::createWithFormat("net0%d.png", m_nBulletType); 
        this->setSpriteNet(Sprite::createWithSpriteFrameName(nodenetName->getCString()));
        m_pSpriteNet->setVisible(false);
        pBatchNodeNet->addChild(m_pSpriteNet);
    
        m_pGameLayer->getBullets()->addObject(this);    
        return true;
     
}

void Bullet::shootTo(CCPoint targetDirection)
{
    Point ptFrom = m_pSpriteBullet->getPosition();
    Point ptTo = targetDirection;
    float angle = atan2f(ptTo.y - ptFrom.y, ptTo.x - ptFrom.x);
    float rotation = angle / M_PI * 180.0f;
    m_pSpriteBullet->setRotation(90.0f - rotation);
    
	Size size = Director::getInstance()->getWinSize(); 
    
    float distance =MAX(size.width, size.height); 
    Point targetPt = ccp(ptFrom.x + distance * cosf(angle), ptFrom.y + distance * sinf(angle));
    FiniteTimeAction *moveto = MoveTo::create(1.0f, targetPt);
    FiniteTimeAction *callFunc = CCCallFunc::create(this, callfunc_selector(Bullet::removeSelf));
    FiniteTimeAction *sequence = Sequence::create(moveto, callFunc, NULL);
    m_pSpriteBullet->runAction(sequence);
}

void Bullet::showNet()
{
    m_bCaught = true;
    m_pSpriteBullet->stopAllActions();
    m_pSpriteBullet->setVisible(false);
    m_pSpriteNet->setVisible(true);
     
    ScaleTo *scale = ScaleTo::create(0.3f, 1.25f);
    FiniteTimeAction *callFunc = CCCallFunc::create(this, callfunc_selector(Bullet::removeSelf));
    FiniteTimeAction *sequence = Sequence::create(scale, callFunc, NULL);
    m_pSpriteNet->runAction(sequence);
    m_pSpriteNet->setPosition(m_pSpriteBullet->getPosition());
    
    ParticleSystem *particle = ParticleSystemQuad::create("netparticle.plist");
    particle->setPosition(m_pSpriteNet->getPosition());
    particle->setPositionType(kCCPositionTypeGrouped);
    particle->setAutoRemoveOnFinish(true);
    m_pGameLayer->addChild(particle, 100);

}

void Bullet::removeSelf()
{
    CCLOG("remove bullet");
    this->getGameLayer()->getBullets()->removeObject(this);
    m_pSpriteBullet->removeFromParentAndCleanup(true);
    m_pSpriteNet->removeFromParentAndCleanup(true);
}