#include "RollNum.h"
#include "cocos2d.h"
#include "GameLayer.h"

USING_NS_CC;


RollNumGroup *RollNumGroup::createWithGameLayer(GameLayer *pGameLayer, int nDigit)
{
    RollNumGroup *pRollNumGroup = new RollNumGroup();
    if(pRollNumGroup && pRollNumGroup->initWithGameLayer(pGameLayer, nDigit))
    {
        //将自己设置为内存自动管理的对象
        //pRollNumGroup->autorelease();
		pRollNumGroup->retain();
        return pRollNumGroup;
    }
    else
    {
        CC_SAFE_DELETE(pRollNumGroup);
        return NULL;
    }
}

RollNumGroup::RollNumGroup():m_pRollNumArray(NULL), m_nValue(0){}

RollNumGroup::~RollNumGroup()
{
    CC_SAFE_RELEASE(m_pRollNumArray);
	//CC_SAFE_RETAIN(m_pRollNumArray);
}

bool RollNumGroup::initWithGameLayer(GameLayer *pGameLayer, int nDigit)
{
    m_pGameLayer = pGameLayer;
    //装载数字纹理
    CCTexture2D *pTex = CCTextureCache::sharedTextureCache()->addImage("number.png");
    m_pBatchNode = CCSpriteBatchNode::createWithTexture(pTex);
    //将积分榜添加到主游戏界面上
    m_pGameLayer->addChild(m_pBatchNode, 100);
    this->setRollNumArray(CCArray::createWithCapacity(nDigit));
    for(int i = 0; i < nDigit; i++)
    {
        //创建积分榜中的每一个数字位
        RollNum *pRollNum = RollNum::create();
        m_pRollNumArray->addObject(pRollNum);
        //将数字位显示到主游戏界面上
        m_pBatchNode->addChild(pRollNum);
    }
    return true;
}

void RollNumGroup::setPosition(cocos2d::CCPoint pt)
{
    m_ptPosition = pt;//积分榜的个位的位置坐标
    for(int i = 0; i < m_pRollNumArray->count(); ++i)
    {
        RollNum *pRollNum = (RollNum *)m_pRollNumArray->objectAtIndex(i);
        pRollNum->setPosition(pt);
        //每个位的 坐标从右往左移动，间隔20.7px
        pt.x -= 20.7f;
    }
}

void RollNumGroup::setValue(int nValue)
{
    //设置的数字与显示的一直时，直接返回
    if(m_nValue == nValue)
        return;
    //设置积分榜中得数字滚动的方向
    bool bUp = m_nValue < nValue;
    m_nValue = nValue;
    //解析每个位上的数，循环调度对应位上的数的滚动显示
    for(int i = 0; i < m_pRollNumArray->count(); ++i)
    {
        RollNum *pRollNum = (RollNum *)m_pRollNumArray->objectAtIndex(i);
        int num = nValue % 10;//取得与显示的积分榜中的末位上的数
        //将m_pRollNumArray 中的第一个RollNum定义为个位，第二位为十位，依次类推
        if(pRollNum->getNumber() != num)
            //调度相应位上数字滚动
            pRollNum->setNumber(num, bUp);
        //去掉预显示的积分数的末位，进入下一位数字显示调度
        nValue = nValue / 10;
    }
}