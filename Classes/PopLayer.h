//
//  PopLayer.h
//  AStart
//
//  Created by Mengjiangtao on 7/30/15.
//
//

#ifndef __AStart__PopLayer__
#define __AStart__PopLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class PopLayer:public Layer
{
public:
    PopLayer();
    ~PopLayer();
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    static PopLayer* create(const char* backgroundImage);
    void setTitle(const char* title,int fontsize = 20);
    void setContentText(const char* title,int fontsize=20,int padding=50,int paddingTop=100);
    bool addButton(const char* normalImage,const char* selectImage,const char*title,Vec2 position,int tag=0);
    CREATE_FUNC(PopLayer);
private:
    int m_contentPadding;
    int m_contentPaddingTop;
    Ref* m_callbackListener;
    SEL_CallFuncN m_callback;
    
    CC_SYNTHESIZE_RETAIN(Menu* , m_pMenu , MenuButton);
    CC_SYNTHESIZE_RETAIN(Sprite*, m_sfBackGround, SpriteBackGround);
    CC_SYNTHESIZE_RETAIN(Scale9Sprite*, m_s9BackGround, Sprite9BackGround);
    CC_SYNTHESIZE_RETAIN(Label*, m_title, LabelTitle);
    CC_SYNTHESIZE_RETAIN(Label*, m_ContentText, LabelContentText);
    void buttonCallBack(Ref* pSender);
    
};

#endif /* defined(__AStart__PopLayer__) */
