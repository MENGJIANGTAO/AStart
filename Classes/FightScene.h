//
//  FightScene.h
//  AStart
//
//  Created by Mengjiangtao on 7/31/15.
//
//

#ifndef __AStart__FightScene__
#define __AStart__FightScene__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include"cocostudio/CocoStudio.h"

USING_NS_CC;

class FightScene : public Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(FightScene);
private:
};

#endif /* defined(__AStart__FightScene__) */
