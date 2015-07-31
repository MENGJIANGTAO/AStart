#include "AStart.h"

vector<PathSprite*> PathSearchInfo::m_openList;

PathSprite* PathSearchInfo::m_inspectArray[MAP_WIDTH][MAP_HEIGHT] = {NULL};

vector<PathSprite*> PathSearchInfo::m_pathList;

vector<PathSprite*> PathSearchInfo::m_haveInspectList;

Size PathSearchInfo::m_mapSize;

Size PathSearchInfo::m_tileSize;

int PathSearchInfo::m_startX;

int PathSearchInfo::m_startY;

int PathSearchInfo::m_endX;

int PathSearchInfo::m_endY;

Scene* AStart::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    AStart *layer = AStart::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
void AStart::onEnter()
{
    Layer::onEnter();
    //    Director* pDirector = CCDirector::getInstance();
    //    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(AStart::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(AStart::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(AStart::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool AStart::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    
    
    auto* pLabel = LabelTTF::create("A* + tiledMap", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                             origin.y + visibleSize.height - pLabel->getContentSize().height));
    
    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    
    TMXTiledMap* map = TMXTiledMap::create("map2.tmx");
    this->addChild(map);
    map->setPosition(Point());
    TMXLayer* _road = map->getLayer("floor");//行走路径的地图
    Size _mapSize = map->getMapSize();
    for (int j = 0;  j < _mapSize.height; j++) {
        for (int i = 0;  i < _mapSize.width; i++) {
            Sprite* _sp = _road->getTileAt(Point(i, j));
            if (_sp) {
                PathSprite* _pathSprite = new PathSprite(_sp);
                _pathSprite->m_x = i;
                _pathSprite->m_y = j;
                PathSearchInfo::m_inspectArray[i][j] = _pathSprite;//把地图中所有的点一一对应放入检测列表中
            }
        }
    }
    PathSearchInfo::m_mapSize = _mapSize;//获取地图的尺寸
    PathSearchInfo::m_tileSize = map->getTileSize();//获取瓦片的尺寸
    
    //设置起始和终点
    PathSearchInfo::m_startX =0;
    PathSearchInfo::m_startY = 0;
    
    //创建一个人物
    m_player = new PathSprite(Sprite::create("start.png"));
    m_player->m_sprite->setAnchorPoint(Point(0.5,0));
    this->addChild(m_player->m_sprite);
    
    m_player->m_x = PathSearchInfo::m_startX;//设置人物的起始的地图坐标
    m_player->m_y = PathSearchInfo::m_startY;
    
    m_orignPoint = PathSearchInfo::m_inspectArray[PathSearchInfo::m_startX][PathSearchInfo::m_startY]->m_sprite->getPosition();
    m_player->m_sprite->setPosition(m_orignPoint);//设置人物的起始的世界坐标
    
    if(_mapSize.width>visibleSize.width && _mapSize.height>visibleSize.height)
    {
            this->scheduleUpdate();
    }
    
    
    // 定义一个弹出层，传入一张背景图片
    PopLayer* popup = PopLayer::create("HelloWorld.png");
    // ContentSize是可选的设置，可以不设置，如果设置则把它当做9图缩放
    popup->setContentSize(CCSizeMake(250, 160));
    popup->setTitle("Title");
    popup->setContentText("A* TitleMap Example A* TitleMap Example A* TitleMap Example A* TitleMap Example", 20, 50, 150);
    //添加按钮，设置图片、文字，tag信息
//    popup->addButton("start.png", "start.png", "Ok", Vec2(10,150),0);
    popup->addButton("start.png", "start.png", "Cancel", Vec2(160,150),1);
    this->addChild(popup);
    
    return true;
}

void AStart::clearPath()
{
    for (vector<PathSprite*>::iterator iter = PathSearchInfo::m_haveInspectList.begin(); iter !=  PathSearchInfo::m_haveInspectList.end(); iter++)
    {
        (*iter)->m_sprite->setColor(Color3B::WHITE);
        (*iter)->m_costToSource = 0;
        (*iter)->m_FValue = 0;
        (*iter)->m_parent = NULL;
        (*iter)->m_child = NULL;
        
        PathSearchInfo::m_inspectArray[(*iter)->m_x][(*iter)->m_y] = (*iter);
    }
    
    //把移除了障碍物的地图放入检测列表中
    //PathSearchInfo::m_inspectList = PathSearchInfo::m_mapList;
    PathSearchInfo::m_openList.clear();
    PathSearchInfo::m_pathList.clear();
    PathSearchInfo::m_haveInspectList.clear();
    PathSearchInfo::m_startX = m_player->m_x;
    PathSearchInfo::m_startY = m_player->m_y;
    m_player->m_sprite->stopAllActions();
    
    m_playerMoveStep = 0;
}

void AStart::playerMove()
{
    m_playerMoveStep++;
    
    if (m_playerMoveStep >= PathSearchInfo::m_pathList.size()) {
        return;
    }
    
    m_player->m_x = PathSearchInfo::m_pathList[m_playerMoveStep]->m_x;
    m_player->m_y = PathSearchInfo::m_pathList[m_playerMoveStep]->m_y;
    
    //根据路径列表移动人物
    m_player->m_sprite->runAction(CCSequence::create(CCMoveTo::create(1/24.0, PathSearchInfo::m_pathList[m_playerMoveStep]->m_sprite->getPosition()), CallFunc::create(this, SEL_CallFunc(&AStart::playerMove)) , NULL));
    
}
void AStart::update(float dt)
{
    this->setPosition(m_orignPoint - m_player->m_sprite->getPosition());
}

void AStart::calculatePath()
{
    
    //得到开始点的节点
    PathSprite* _startNode = PathSearchInfo::m_inspectArray[PathSearchInfo::m_startX][PathSearchInfo::m_startY];
    //得到结束点的节点
    PathSprite* _endNode = PathSearchInfo::m_inspectArray[PathSearchInfo::m_endX][PathSearchInfo::m_endY];
    
    //因为是开始点 把到起始点的距离设为0, F值也为0
    _startNode->m_costToSource = 0;
    _startNode->m_FValue = 0;
    
    //把已经检测过的点从检测列表中删除
    PathSearchInfo::m_inspectArray[PathSearchInfo::m_startX][PathSearchInfo::m_startY] = NULL;
    //把该点放入已经检测过点的列表中
    PathSearchInfo::m_haveInspectList.push_back(_startNode);
    //然后加入开放列表
    PathSearchInfo::m_openList.push_back(_startNode);
    
    PathSprite* _node = NULL;
    while (true)
    {
        //得到离起始点最近的点(如果是第一次执行, 得到的是起点)
        _node = PathSearchInfo::getMinPathFormOpenList();
        if (!_node)
        {
            //找不到路径
            break;
        }
        //把计算过的点从开放列表中删除
        PathSearchInfo::removeObjFromOpenList( _node);
        int _x = _node->m_x;
        int _y = _node->m_y;
        
        //
        if (_x ==PathSearchInfo::m_endX && _y == PathSearchInfo::m_endY)
        {
            break;
        }
        
        //检测8个方向的相邻节点是否可以放入开放列表中
        log("%d, %d",_x, _y);
        PathSprite* _adjacent = PathSearchInfo::getObjFromInspectArray( _x + 1, _y + 1);
        PathSearchInfo::inspectTheAdjacentNodes(_node, _adjacent, _endNode);
        
        _adjacent = PathSearchInfo::getObjFromInspectArray(  _x +1, _y);
        PathSearchInfo::inspectTheAdjacentNodes(_node, _adjacent, _endNode);
        
        _adjacent = PathSearchInfo::getObjFromInspectArray(  _x +1, _y-1);
        PathSearchInfo::inspectTheAdjacentNodes(_node, _adjacent, _endNode);
        
        _adjacent = PathSearchInfo::getObjFromInspectArray(  _x , _y -1);
        PathSearchInfo::inspectTheAdjacentNodes(_node, _adjacent, _endNode);
        
        _adjacent = PathSearchInfo::getObjFromInspectArray(  _x -1, _y - 1);
        PathSearchInfo::inspectTheAdjacentNodes(_node, _adjacent, _endNode);
        
        _adjacent = PathSearchInfo::getObjFromInspectArray(  _x -1, _y);
        PathSearchInfo::inspectTheAdjacentNodes(_node, _adjacent, _endNode);
        
        _adjacent = PathSearchInfo::getObjFromInspectArray(  _x -1, _y+1);
        PathSearchInfo::inspectTheAdjacentNodes(_node, _adjacent, _endNode);
        
        _adjacent = PathSearchInfo::getObjFromInspectArray(  _x , _y+1);
        PathSearchInfo::inspectTheAdjacentNodes(_node, _adjacent, _endNode);
        
    }
    
    while (_node)
    {
        //把路径点加入到路径列表中
        PathSearchInfo::m_pathList.insert(PathSearchInfo::m_pathList.begin(), _node);
        _node = _node->m_parent;
    }
}


void AStart::drawPath(  )
{
    for (vector<PathSprite*>::iterator iter = PathSearchInfo::m_pathList.begin(); iter !=  PathSearchInfo::m_pathList.end(); iter++)
    {
        (*iter)->m_sprite->setColor(Color3B::GREEN);
    }
    
}

Rect getBoundingBox(float x, float y, float width, float height)
{
    return Rect(x - width/2, y - height/2, width, height);
}
bool AStart::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
    //清除之前的路径
    clearPath();
    auto nodePosition = convertToNodeSpace( touch->getLocation() );
    log("%f, %f", nodePosition.x, nodePosition.y);
    //    for (int i = 0; i < PathSearchInfo::m_inspectList.size(); i++)
    //    {
    //        PathSprite* _sp = PathSearchInfo::m_inspectList[i];
    //
    //        CCRect _rect = getBoundingBox( _sp->m_sprite->getPositionX(), _sp->m_sprite->getPositionY(), _sp->m_sprite->getContentSize().width, _sp->m_sprite->getContentSize().height);
    //
    //        if (_rect.containsPoint(nodePosition))
    //        {
    PathSprite* _sp = PathSearchInfo::m_inspectArray[(int)(nodePosition.x/PathSearchInfo::m_tileSize.width)][(int)(PathSearchInfo::m_mapSize.height - nodePosition.y/PathSearchInfo::m_tileSize.height)];
    if (_sp) {
        log("%f, %f", _sp->m_sprite->getPositionX(), _sp->m_sprite->getPositionY());
        //获取触摸点, 设置为终点
        PathSearchInfo::m_endX = _sp->m_x;
        PathSearchInfo::m_endY = _sp->m_y;
        //计算路径
        calculatePath();
        //绘制路径
        drawPath(  );
        //移动物体
        playerMove();
    }
    
    
    //        }
    //
    //    }
    
    return true;
}

void AStart::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event){
    
}



void AStart::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
}

void AStart::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}