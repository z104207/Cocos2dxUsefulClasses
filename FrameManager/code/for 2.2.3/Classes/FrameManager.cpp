#include "FrameManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

FrameManager::FrameManager():
	m_bFrameVisible(true),
	m_frameLayer(0),	
	m_frameBg(0),
	m_eventTarget(0),
	m_eventSelector(0)
{
	
}

bool FrameManager::setFrame(Widget* frameLayer,Widget* frameBg)
{
	m_frameLayer    = frameLayer;
	m_frameBg       = frameBg;
	m_frameSize     = m_frameBg->getContentSize();
	m_anchorPoint.x = m_frameSize.width  * m_frameBg->getAnchorPoint().x;
	m_anchorPoint.y = m_frameSize.height * m_frameBg->getAnchorPoint().y;
	m_bFrameVisible = true;

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,kCCPriorityNonSystemMin, false);

	return true;
}

void FrameManager::setFrameVisible(bool bVisible)
{
	CCAssert(m_frameLayer!=0 && m_frameBg!=0,"m_frameLayer or m_frameBg is null");

	if(m_frameLayer && m_frameBg)
	{
		m_bFrameVisible = bVisible;
		m_frameLayer->setVisible(m_bFrameVisible);
		m_frameLayer->setTouchEnabled(m_bFrameVisible);
		setChildrenTouchEnabled(m_frameLayer,m_bFrameVisible);
	}
}

bool FrameManager::isOutsideFrame(CCTouch *pTouch)
{
	CCAssert(m_frameBg!=0,"m_frameBg is null");

	if(m_frameBg == 0)
		return false;

	CCPoint point = m_frameBg->convertTouchToNodeSpaceAR(pTouch);
	point = point + m_anchorPoint;

	if(point.x<0.0f || point.y<0.0f ||
	   point.x>m_frameSize.width || point.y>m_frameSize.height)
	{
		return true;
	}
	
	return false;
}

bool FrameManager::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_bFrameVisible)
		return true;

	return false;
}

void FrameManager::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if(isOutsideFrame(pTouch))
	{
		if(m_eventSelector && m_eventTarget)
			(m_eventTarget->*m_eventSelector)(m_frameLayer);
	}
}

void FrameManager::setChildrenTouchEnabled(CCNode* pNode, bool bTouchEnabled)
{
	CCArray* pChildren =  pNode->getChildren();
	CCObject* pObject;
	CCARRAY_FOREACH(pChildren,pObject)
	{
		((UIWidget*)pObject)->setTouchEnabled(bTouchEnabled);
		setChildrenTouchEnabled((CCNode*)pObject,bTouchEnabled);
	}
}