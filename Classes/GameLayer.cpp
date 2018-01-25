#include "GameLayer.h"
#include "Cannon.h"

#include "GameConfig.h"
#include "Fish.h"

#include "Bullet.h"

USING_NS_CC;

const int FishInBatchNode1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 13, 14};

const int FishInBatchNode2[] = {10, 18};

// 这两种鱼的被捕获到后的帧图与游动帧图被分割到了两个不同的大图中分别fish2.png与fish3.png，
// 导致在鱼儿被捕时播放Fish::showCaught()时在其m_pBatchNodeFish3AndNets中找不到对应的被捕帧图而抛出异常。

//const int FishInBatchNode3[] = {16, 17};

const int FishInBatchNode4[] = {11, 12};

Scene* GameLayer::scene(){
    Scene *scene = Scene::create();
    Layer *layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}

GameLayer::GameLayer():m_pBullets(NULL),m_pFishes(NULL),/*m_pRollNumGroup(NULL),*/
	m_nScore(0),m_pCannon(NULL),m_pSpriteAdd(NULL){
    
}

GameLayer::~GameLayer(){
    CC_SAFE_RELEASE(m_pCannon);
    CC_SAFE_RELEASE(m_pBullets);
    CC_SAFE_RELEASE(m_pFishes);
	CC_SAFE_RELEASE(m_pRollNumGroup);
}

bool GameLayer::init(){
    if(!Layer::init()){
        return false;
    }
    this->setTouchEnabled(true);
    this->initFrames();
    this->initBackground();
	this->initListeners();
    this->initFishes();
    this->initCannon();

    this->schedule(schedule_selector(GameLayer::updateFish), 1.0f);
    this->schedule(schedule_selector(GameLayer::updateGame), 0.05f);
    return true;
}

void GameLayer::initListeners()
{
	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->setSwallowTouches(false);

	touchListener->onTouchBegan = [=](Touch *pTouch,Event *pEvent)
	{
		return this->onTouchBegan(pTouch, pEvent);
	};

	touchListener->onTouchMoved = [=](Touch *touch,Event *pEvent)
	{
		this->onTouchMoved(touch, pEvent);
	};

	touchListener->onTouchEnded = [=](Touch *pTouch,Event *pEvent)
	{
		this->onTouchEnded(pTouch, pEvent);
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}


void GameLayer::initFrames(){
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fish.plist");
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fish2.plist");
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fish3.plist");
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fish4.plist"); 
    SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("cannon.plist");
}

void GameLayer::initBackground(){
    Texture2D *texture=CCTextureCache::getInstance()->addImage("bj01.jpg");
    Sprite *pBackGround =Sprite::createWithTexture(texture);

    pBackGround->setAnchorPoint(Point(0.5f, 0.5f));
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    pBackGround->setPosition(Point(winSize.width/2, winSize.height/2));
    this->addChild(pBackGround);
    
    texture=CCTextureCache::getInstance()->addImage("ui_box_01.png");
    Sprite *pTopBar=Sprite::createWithTexture(texture);
    pTopBar->setPosition(Point(winSize.width / 2, winSize.height- pTopBar->getContentSize().height/2));
	CCLOG("dd");
    this->addChild(pTopBar,100);
    
    texture=TextureCache::getInstance()->addImage("ui_box_02.png");
    Sprite *pBottomBar=Sprite::createWithTexture(texture);
	pBottomBar->setAnchorPoint(Vec2::ZERO);
    pBottomBar->setPosition(Point(0,0));
    this->addChild(pBottomBar,100);

	this->setRollNumGroup(RollNumGroup::createWithGameLayer(this, 6));
	
    m_pRollNumGroup->setPosition(Point(pBottomBar->getContentSize().width/2.5, pBottomBar->getContentSize().height / 7.8));
}

void GameLayer::initFishes(){

	Texture2D *texture = TextureCache::getInstance()->addImage("fish.png");
    this->setBatchNodeFish1(SpriteBatchNode::createWithTexture(texture));
    this->addChild(this->m_pBatchNodeFish1); 
    
    texture = TextureCache::getInstance()->addImage("fish2.png");
    this->m_pBatchNodeFish2AndBullets=SpriteBatchNode::createWithTexture(texture);
    this->addChild(this->m_pBatchNodeFish2AndBullets);
    
    texture = TextureCache::getInstance()->addImage("fish3.png");
    this->m_pBatchNodeFish3AndNets=SpriteBatchNode::createWithTexture(texture);
    this->addChild(this->m_pBatchNodeFish3AndNets);
    
	/*
    texture = TextureCache::getInstance()->addImage("fish4.png");
    this->setBatchNodeFish4(SpriteBatchNode::createWithTexture(texture));
    this->addChild(this->m_pBatchNodeFish4);
	*/
     
    this->setFishes(Array::createWithCapacity(MAX_FISH_COUNT));
    m_pFishes->removeAllObjects();
}

void GameLayer::initCannon(){

    this->setBullets(Array::create());
    Texture2D *ptexture=TextureCache::getInstance()->addImage("cannon.png");
    SpriteBatchNode *pBatchNode=SpriteBatchNode::createWithTexture(ptexture);
    this->addChild(pBatchNode,101,7);
    this->setCannon(Cannon::createWithCannonType(6, this, pBatchNode));
    
    Texture2D *texture2dAdd= TextureCache::getInstance()->addImage("button_add.png");
    SpriteFrame *spriteFrameAddNormal=SpriteFrame::createWithTexture(
		texture2dAdd, 
		Rect(0, 0, texture2dAdd->getContentSize().width/2, texture2dAdd->getContentSize().height)); 
    this->m_pSpriteAdd=Sprite::createWithSpriteFrame(spriteFrameAddNormal);
    this->m_pSpriteAdd->setPosition(Point(585,28));
    this->m_pSpriteAdd->setScale(1.5f);
    this->addChild(this->m_pSpriteAdd,101);
    
    Texture2D *texture2dReduce= TextureCache::getInstance()->addImage("button_reduce.png");
    SpriteFrame *spriteFrameReduceNormal=SpriteFrame::createWithTexture(texture2dReduce, Rect(0, 0, texture2dReduce->getContentSize().width/2, texture2dReduce->getContentSize().height));
    this->m_pSpriteReduce=Sprite::createWithSpriteFrame(spriteFrameReduceNormal);
    this->m_pSpriteReduce->setPosition(Point(450,28));
    this->m_pSpriteReduce->setScale(1.5f);
    this->addChild(this->m_pSpriteReduce,101);
}

void GameLayer::addConnonSize(){ 
    int curConnonType=this->m_pCannon->getConnonType();
    if(++curConnonType>7){
        curConnonType=1;
    }
    CC_SAFE_DELETE(this->m_pCannon);
    this->setCannon(Cannon::createWithCannonType(curConnonType, this, (SpriteBatchNode *)this->getChildByTag(7)));
}

void GameLayer::reduceConnonSize(){
    int curConnonType=this->m_pCannon->getConnonType();
    if(--curConnonType<1){
        curConnonType=7;
    }
    CC_SAFE_DELETE(this->m_pCannon);
    this->setCannon(Cannon::createWithCannonType(curConnonType, this, (SpriteBatchNode *)this->getChildByTag(7)));
}

bool GameLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    Point pt = CCDirector::getInstance()->convertToGL(pTouch->getLocationInView());
	
    if(this->m_pSpriteAdd->boundingBox().containsPoint(pt)){
        this->addConnonSize();
		return false;
    }
    if(this->m_pSpriteReduce->boundingBox().containsPoint(pt)){
        this->reduceConnonSize();
		return false;
    }
    this->m_pCannon->rotateToPoint(pt);
	return true;
}

void GameLayer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) 
{
	Point pt = CCDirector::getInstance()->convertToGL(pTouch->getLocationInView());
	this->m_pCannon->rotateToPoint(pt);
}		
void GameLayer::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	this->m_pCannon->fire();
}


void GameLayer::addFish(){
    SpriteBatchNode *loadFishSpriteBatchNode=NULL;
    while (1) {
        int type = rand() % 18 + 1;
        
        if(NULL==loadFishSpriteBatchNode){
            for(int i=0;i<GET_ARRAY_LEN(FishInBatchNode1);++i){
                if (type==FishInBatchNode1[i]) {
                    loadFishSpriteBatchNode=m_pBatchNodeFish1;
                    break;
                }
            }
        }
        if(NULL==loadFishSpriteBatchNode){
            for(int i=0;i<GET_ARRAY_LEN(FishInBatchNode2);++i){
                if (type==FishInBatchNode2[i]) {
                    loadFishSpriteBatchNode=m_pBatchNodeFish2AndBullets;
                    break;
                }
            }
        }
		/*
        if(NULL==loadFishSpriteBatchNode){
            for(int i=0;i<GET_ARRAY_LEN(FishInBatchNode3);++i){
                if (type==FishInBatchNode3[i]) {
                    loadFishSpriteBatchNode=m_pBatchNodeFish3AndNets;
                    break;
                }
            }
        }
		*/

		/*
        if(NULL==loadFishSpriteBatchNode){
            for(int i=0;i<GET_ARRAY_LEN(FishInBatchNode4);++i){
                if (type==FishInBatchNode4[i]) {
                    loadFishSpriteBatchNode=m_pBatchNodeFish4;
                    break;
                }
            }
        }
		*/

        if(loadFishSpriteBatchNode){
            Fish::createWithFishType(type, this, loadFishSpriteBatchNode);
            return;
        } 
    }
    loadFishSpriteBatchNode=NULL;
     
}

void GameLayer::updateFish(float dt){
    if(m_pFishes->count() < MAX_FISH_COUNT)
    {
        int n = MAX_FISH_COUNT - m_pFishes->count();
        int nAdd = rand() % n + 1;

        for(int i = 0; i < nAdd; i++)
        {
            this->addFish();
        }
    }
}

Rect shrinkRect(Rect rc, float xr, float yr)
{
    float w = rc.size.width * xr;
    float h = rc.size.height * yr;
    Point pt = Point(rc.origin.x + rc.size.width * (1.0f - xr) / 2,
                     rc.origin.y + rc.size.height * (1.0f - yr) / 2);
    return Rect(pt.x, pt.y, w, h);
}

void GameLayer::updateGame(float dt)
{
    Object *pFishObj = NULL;
    Object *pBulletObj = NULL;
    CCARRAY_FOREACH(m_pBullets, pBulletObj)
    {
        Bullet *pBullet = (Bullet *)pBulletObj;
        if(pBullet->getCaught())
            continue;
        bool caught = false;
        CCARRAY_FOREACH(m_pFishes, pFishObj)
        {
            Fish *pFish = (Fish *)pFishObj;
            if(pFish->getCaught())
                continue;
            
			
            Rect hittestRect = shrinkRect(pFish->getSpriteFish()->boundingBox(), 0.8f, 0.5f);
            
            if(hittestRect.containsPoint(pBullet->getSpriteBullet()->getPosition()))
            {
                caught = true;
                pFish->showCaught();
                m_nScore += 125;
                m_pRollNumGroup->setValue(m_nScore);
            }
			
        }
        
        if(caught)
        {
            pBullet->showNet();
        }
    }
}