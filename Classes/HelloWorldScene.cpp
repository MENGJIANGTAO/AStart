#include "HelloWorldScene.h"

USING_NS_CC;


Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    
    auto layer = HelloWorld::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //载入TileMap地图
    auto map = TMXTiledMap::create("map2.tmx");
    addChild(map);
    
    //获取TileMap的层
    layer = map->getLayer("floor");
    
    //确定起点瓦片坐标和初始化二维数组
    beginPoint = Point(0,0);
    arr[0][0] = 1;
    
    //点击事件监听,记录终点瓦片坐标
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch *t, Event *e)
    {
        //设置终点瓦片坐标
        int dx = (int)(t->getLocation().x)/32;
        int dy = 14 - (int)(t->getLocation().y)/32;
        this->destination = Point(dx,dy);
        CCLOG("%d,%d",dx,dy);
        
        //自己设置起点到终点的最短格子数估值，即G值
        total = 1000;
        
        //寻找最短路径，坐标存放在closeList里面
        aStart();
        
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
    return true;
}

bool HelloWorld::aStart()
{
    //每调用一次aStart相当于移动一步，每移动一步total减30,作为G值
    total = total - 30;
    
    if ((int)beginPoint.x == (int)destination.x && (int)beginPoint.y == (int)destination.y)
    {
        return true;
    }
    
    //探索这个点的4周(上，右，下，左，左下，右下，左上，右上),若存在，则放进优先队列openList中
    //判断上方
    if ((int)beginPoint.x >= 0 && ((int)beginPoint.y-1) >= 0 && (int)beginPoint.x <= 9 && ((int)beginPoint.y-1) <= 14 ){
        Sprite *testUp = layer->getTileAt(Vec2((int)beginPoint.x,(int)beginPoint.y-1));
        if (testUp != NULL && (arr[(int)beginPoint.x][(int)beginPoint.y-1] != 1) )
        {
            CCLOG("up");
            //初始化探索的点，将它放进优先队列openList中，并设置二维数组中的值为1，防止重复探索
            auto temp = new pointValue(Point(beginPoint.x,beginPoint.y-1),destination,total);
            p_quene.push(*temp);
            arr[(int)beginPoint.x][(int)beginPoint.y-1] = 1;
        }
    }
    
    // 左上方
    if ((int)beginPoint.x-1 >= 0 && ((int)beginPoint.y-1) >= 0 && (int)beginPoint.x-1 <= 9 && ((int)beginPoint.y-1) <= 14 ){
        Sprite *testUp = layer->getTileAt(Vec2((int)beginPoint.x-1,(int)beginPoint.y-1));
        if (testUp != NULL && (arr[(int)beginPoint.x-1][(int)beginPoint.y-1] != 1) )
        {
            CCLOG("upLeft");
            //初始化探索的点，将它放进优先队列openList中，并设置二维数组中的值为1，防止重复探索
            auto temp = new pointValue(Point(beginPoint.x-1,beginPoint.y-1),destination,total);
            p_quene.push(*temp);
            arr[(int)beginPoint.x-1][(int)beginPoint.y-1] = 1;
        }
    }
    
    // 左下方
    if ((int)beginPoint.x-1 >= 0 && ((int)beginPoint.y+1) >= 0 && (int)beginPoint.x-1 <= 9 && ((int)beginPoint.y+1) <= 14 ){
        Sprite *testUp = layer->getTileAt(Vec2((int)beginPoint.x-1,(int)beginPoint.y+1));
        if (testUp != NULL && (arr[(int)beginPoint.x-1][(int)beginPoint.y+1] != 1) )
        {
            CCLOG("upDown");
            //初始化探索的点，将它放进优先队列openList中，并设置二维数组中的值为1，防止重复探索
            auto temp = new pointValue(Point(beginPoint.x-1,beginPoint.y+1),destination,total);
            p_quene.push(*temp);
            arr[(int)beginPoint.x-1][(int)beginPoint.y+1] = 1;
        }
    }
    
    //判断左方
    if (((int)beginPoint.x-1) >= 0 && (int)beginPoint.y >= 0 && ((int)beginPoint.x-1) <= 9 && (int)beginPoint.y <= 14) {
        Sprite *testLeft = layer->getTileAt(Vec2((int)beginPoint.x-1,(int)beginPoint.y));
        if (testLeft != NULL && (arr[(int)beginPoint.x-1][(int)beginPoint.y] != 1) )
        {
            CCLOG("left");
            //初始化探索的点，将它放进优先队列openList中，并设置二维数组中的值为1，防止重复探索
            auto temp2 = new pointValue(Point(beginPoint.x-1,beginPoint.y),destination,total);
            p_quene.push(*temp2);
            arr[(int)beginPoint.x-1][(int)beginPoint.y] = 1;
            
        }
    }
    
    // 右下方
    if ((int)beginPoint.x+1 >= 0 && ((int)beginPoint.y+1) >= 0 && (int)beginPoint.x+1 <= 9 && ((int)beginPoint.y+1) <= 14 ){
        Sprite *testUp = layer->getTileAt(Vec2((int)beginPoint.x+1,(int)beginPoint.y+1));
        if (testUp != NULL && (arr[(int)beginPoint.x+1][(int)beginPoint.y+1] != 1) )
        {
            CCLOG("upDown");
            //初始化探索的点，将它放进优先队列openList中，并设置二维数组中的值为1，防止重复探索
            auto temp = new pointValue(Point(beginPoint.x+1,beginPoint.y+1),destination,total);
            p_quene.push(*temp);
            arr[(int)beginPoint.x+1][(int)beginPoint.y+1] = 1;
        }
    }
    
    // 右上方
    if ((int)beginPoint.x+1 >= 0 && ((int)beginPoint.y-1) >= 0 && (int)beginPoint.x+1 <= 9 && ((int)beginPoint.y-1) <= 14 ){
        Sprite *testUp = layer->getTileAt(Vec2((int)beginPoint.x+1,(int)beginPoint.y-1));
        if (testUp != NULL && (arr[(int)beginPoint.x+1][(int)beginPoint.y-1] != 1) )
        {
            CCLOG("upDown");
            //初始化探索的点，将它放进优先队列openList中，并设置二维数组中的值为1，防止重复探索
            auto temp = new pointValue(Point(beginPoint.x+1,beginPoint.y-1),destination,total);
            p_quene.push(*temp);
            arr[(int)beginPoint.x+1][(int)beginPoint.y-1] = 1;
        }
    }
    
    //判断下方
    if ((int)beginPoint.x >= 0 && ((int)beginPoint.y+1) >= 0 && (int)beginPoint.x <= 9 && ((int)beginPoint.y+1) <= 14) {
        Sprite *testDown = layer->getTileAt(Vec2((int)beginPoint.x,(int)beginPoint.y+1));
        if (testDown != NULL && (arr[(int)beginPoint.x][(int)beginPoint.y+1] != 1) )
        {
            CCLOG("down");
            //初始化探索的点，将它放进优先队列openList中，并设置二维数组中的值为1，防止重复探索
            auto temp3 = new pointValue(Point(beginPoint.x,beginPoint.y+1),destination,total);
            p_quene.push(*temp3);
            arr[(int)beginPoint.x][(int)beginPoint.y+1] = 1;
            
        }
    }
    
    //判断右方
    if (((int)beginPoint.x+1) >= 0 && (int)beginPoint.y >= 0 && ((int)beginPoint.x+1) <= 9 && (int)beginPoint.y <= 14){
        Sprite *testRight = layer->getTileAt(Vec2((int)beginPoint.x+1,(int)beginPoint.y));
        if (testRight != NULL && (arr[(int)beginPoint.x+1][(int)beginPoint.y] != 1))
        {
            CCLOG("right");
            //初始化探索的点，将它放进优先队列openList中，并设置二维数组中的值为1，防止重复探索
            auto temp4 = new pointValue(Point(beginPoint.x+1,beginPoint.y),destination,total);
            p_quene.push(*temp4);
            arr[(int)beginPoint.x+1][(int)beginPoint.y] = 1;
            
        }
    }
    
    //重置当前位置
    beginPoint.x = p_quene.top().tempPoint.x;
    beginPoint.y = p_quene.top().tempPoint.y;
    CCLOG("%d,%d",(int)beginPoint.x,(int)beginPoint.y);
    
    //把F值最小的点，放进closeList中,并从优先队列中pop掉
    closeList.push_back(p_quene.top());
    p_quene.pop();
    
    //在路径中添加精灵
    auto star = Sprite::create("start.png");
    star->setPosition(Point(beginPoint.x*32+10,480-beginPoint.y*32-10));
    addChild(star);
    
    //继续递归	    
    aStart();
    
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
