#include "RollNum.h"
#include "cocos2d.h"
#include "GameLayer.h"

USING_NS_CC;


RollNumGroup *RollNumGroup::createWithGameLayer(GameLayer *pGameLayer, int nDigit)
{
    RollNumGroup *pRollNumGroup = new RollNumGroup();
    if(pRollNumGroup && pRollNumGroup->initWithGameLayer(pGameLayer, nDigit))
    {
        //���Լ�����Ϊ�ڴ��Զ�����Ķ���
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
    //װ����������
    CCTexture2D *pTex = CCTextureCache::sharedTextureCache()->addImage("number.png");
    m_pBatchNode = CCSpriteBatchNode::createWithTexture(pTex);
    //�����ְ���ӵ�����Ϸ������
    m_pGameLayer->addChild(m_pBatchNode, 100);
    this->setRollNumArray(CCArray::createWithCapacity(nDigit));
    for(int i = 0; i < nDigit; i++)
    {
        //�������ְ��е�ÿһ������λ
        RollNum *pRollNum = RollNum::create();
        m_pRollNumArray->addObject(pRollNum);
        //������λ��ʾ������Ϸ������
        m_pBatchNode->addChild(pRollNum);
    }
    return true;
}

void RollNumGroup::setPosition(cocos2d::CCPoint pt)
{
    m_ptPosition = pt;//���ְ�ĸ�λ��λ������
    for(int i = 0; i < m_pRollNumArray->count(); ++i)
    {
        RollNum *pRollNum = (RollNum *)m_pRollNumArray->objectAtIndex(i);
        pRollNum->setPosition(pt);
        //ÿ��λ�� ������������ƶ������20.7px
        pt.x -= 20.7f;
    }
}

void RollNumGroup::setValue(int nValue)
{
    //���õ���������ʾ��һֱʱ��ֱ�ӷ���
    if(m_nValue == nValue)
        return;
    //���û��ְ��е����ֹ����ķ���
    bool bUp = m_nValue < nValue;
    m_nValue = nValue;
    //����ÿ��λ�ϵ�����ѭ�����ȶ�Ӧλ�ϵ����Ĺ�����ʾ
    for(int i = 0; i < m_pRollNumArray->count(); ++i)
    {
        RollNum *pRollNum = (RollNum *)m_pRollNumArray->objectAtIndex(i);
        int num = nValue % 10;//ȡ������ʾ�Ļ��ְ��е�ĩλ�ϵ���
        //��m_pRollNumArray �еĵ�һ��RollNum����Ϊ��λ���ڶ�λΪʮλ����������
        if(pRollNum->getNumber() != num)
            //������Ӧλ�����ֹ���
            pRollNum->setNumber(num, bUp);
        //ȥ��Ԥ��ʾ�Ļ�������ĩλ��������һλ������ʾ����
        nValue = nValue / 10;
    }
}