//
//  PopLayer.cpp
//  AStart
//
//  Created by Mengjiangtao on 7/30/15.
//
//

#include "PopLayer.h"

PopLayer::PopLayer()
{
    m_contentPadding = 0;
    m_contentPaddingTop = 0;
    m_pMenu = NULL;
    m_callbackListener = NULL;
    m_callback = NULL;
    m_sfBackGround = NULL;
    m_s9BackGround = NULL;
    m_ContentText = NULL;
    m_title = NULL;
}

PopLayer::~PopLayer()
{
    CC_SAFE_RELEASE(m_pMenu);
    CC_SAFE_RELEASE(m_sfBackGround);
    CC_SAFE_RELEASE(m_s9BackGround);
    CC_SAFE_RELEASE(m_ContentText);
    CC_SAFE_RELEASE(m_title);
}

bool PopLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    this->setContentSize(Size(0,0));
    Menu* menu = Menu::create();
    menu->setPosition(Vec2(0,0));
    setMenuButton(menu);
    return true;
}

bool PopLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

PopLayer* PopLayer::create(const char* bacgroundImage)
{
    PopLayer* popup = PopLayer::create();
    popup->setSpriteBackGround(Sprite::create(bacgroundImage));
    popup->setSprite9BackGround(Scale9Sprite::create(bacgroundImage));
    return popup;
}

void PopLayer::setTitle(const char* title,int fontsize)
{
    Label* title1 = Label::createWithSystemFont(title, "Arial", fontsize);
    title1->setColor(Color3B(0,0,0));
    setLabelTitle(title1);
}

void PopLayer::setContentText(const char* title,int fontsize,int padding,int paddingTop)
{
    Label* content = Label::createWithSystemFont(title, "Arial", fontsize);
    content->setColor(Color3B(0,0,0));
    setLabelContentText(content);
    m_contentPadding = padding;
    m_contentPaddingTop = paddingTop;
}

//销毁弹出框，传递参数node给下层
void PopLayer::buttonCallBack( Ref* pSender ){
    Node* node = dynamic_cast<Node*>(pSender);
    log("touch tag: %d", node->getTag());
    if (m_callback && m_callbackListener)
    {
        //执行HelloWorld层的回调函数，传递node参数
        (m_callbackListener->*m_callback)(node);
    }
    this->removeFromParentAndCleanup(true);
}

bool PopLayer::addButton(const char *normalImage, const char *selectImage, const char *title,Vec2 position,int tag)
{
    Size winSize = Director::getInstance()->getWinSize();
    Point center = Vec2(winSize.width/2,winSize.height/2);
    
    MenuItemImage* menuImage = MenuItemImage::create(normalImage, selectImage, this,menu_selector(PopLayer::buttonCallBack));
    menuImage->setTag(tag);
//    menuImage->setPosition(Vec2(50, 100));
    
    Size menuSize = menuImage->getContentSize();
    Label* ttf = Label::create(title, "Arial", 15);
    ttf->setColor(Color3B(0, 0, 0));
//    ttf->setPosition(position);
    menuImage->addChild(ttf);
    getMenuButton()->setPosition(Vec2(50, 50));
    getMenuButton()->addChild(menuImage,tag);
    return true;
}

//全部参数都设定好后，在运行时动态加载
void PopLayer::onEnter(){
    Layer::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(PopLayer::onTouchBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    Size winSize = Director::getInstance()->getWinSize();
    Point center = Vec2(winSize.width/2, winSize.height/2);
    Size contentSize;
    // 设定好参数，在运行时加载
    if (getContentSize().equals(CCSizeZero)){
        getSpriteBackGround()->setPosition(center);
        this->addChild(getSpriteBackGround(),0,0);
        contentSize = getSpriteBackGround()->getTexture()->getContentSize();
    }
    else{
        Scale9Sprite* background = getSprite9BackGround();
        background->setContentSize(getContentSize());
        background->setPosition(center);
        this->addChild(background, 0);
        contentSize = getContentSize();
    }
    
    //添加按钮，并根据Item的个数设置其位置
    this->addChild(getMenuButton());
    float btnWidth = contentSize.width / (getMenuButton()->getChildrenCount()+1);
//    CCArray* array = getMenuButton()->getChildren();
    Vector<Node*> array = getMenuButton()->getChildren();
//    Ref* pObj = NULL;
    int i = 0;
    
//    CCARRAY_FOREACH(array, pObj){
    for(Vector<Node*>::iterator it = array.begin();it!=array.end();it++){

        (*it)->setPosition(Vec2(winSize.width/2 - contentSize.width/2 + btnWidth*(i+1),
                                winSize.height/2 - contentSize.height/3));
//        Node* node = dynamic_cast(pObj);
//            node->setPosition(Vec2(winSize.width/2 - contentSize.width/2 + btnWidth*(i+1),
//                              winSize.height/2 - contentSize.height/3));
//        i++;
    }
    
    // 显示对话框标题
    if (getLabelTitle()){
        getLabelTitle()->setPosition(center + Vec2(0, contentSize.height/2 - 25.0f));
        this->addChild(getLabelTitle());
    }
    
    //显示文本内容
    if (getLabelContentText()){
        Label* ltf = getLabelContentText();
        ltf->setPosition(center);
//        ltf->setDimensions(CCSize(contentSize.width - m_contentPadding*2 , contentSize.height - m_contentPaddingTop));
        ltf->setDimensions(contentSize.width-10,contentSize.height-30);
        ltf->setHorizontalAlignment(TextHAlignment::LEFT);
        this->addChild(ltf);
    }
    
    //弹出效果
    CCSequence *popupActions = CCSequence::create(
                                                  CCScaleTo::create(0.0, 0.0), 
                                                  CCScaleTo::create(0.06, 1.05),
                                                  CCScaleTo::create(0.08, 0.95),
                                                  CCScaleTo::create(0.08, 1.0), NULL);
    this->runAction(popupActions);
}

//退出
void PopLayer::onExit(){
    CCLayer::onExit();
}
