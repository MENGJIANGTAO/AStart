#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <queue>
#include <vector>
USING_NS_CC;

class pointValue
{
public:
    pointValue(Point start,Point end,int total)
    {
        tempPoint = start;
        FValue = (abs((int)end.x-(int)start.x +(int)end.y-(int)start.y) + total);
    }
    Point tempPoint;
    int FValue;
};

struct Compare
{
    bool operator()  (pointValue a,pointValue b)
    {
        return a.FValue > b.FValue;
    }
};
class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void menuCloseCallback(Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    int arr[30][20];
    
    TMXLayer *layer;
    std::priority_queue<pointValue,std::vector<pointValue>,Compare> p_quene;
    
    std::vector<pointValue> closeList;
    std::vector<pointValue>::iterator closeListStart;
    
    Point beginPoint;
    
    Point destination;
    
    int total;
    
    bool aStart();
};

#endif // __HELLOWORLD_SCENE_H__
