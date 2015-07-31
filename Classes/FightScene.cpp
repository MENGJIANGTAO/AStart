//
//  FightScene.cpp
//  AStart
//
//  Created by Mengjiangtao on 7/31/15.
//
//

#include "FightScene.h"

Scene* FightScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = FightScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool FightScene::init()
{
    if(!Layer::init())
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Node* node = CSLoader::createNode("fightScene.csb");
    addChild(node);
    
    return true;
}