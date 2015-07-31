#ifndef __ASTART_H__
#define __ASTART_H__

#include "cocos2d.h"
#include "vector"
#include "PopLayer.h"

using namespace std;
USING_NS_CC;

#define MAP_WIDTH 321//要比tmx中的map大
#define MAP_HEIGHT 481

class PathSprite
{
public:
    PathSprite(Sprite* sprite)
    {
        m_parent = NULL;
        m_child = NULL;
        m_costToSource = 0;
        m_FValue = 0;
        m_sprite = sprite;
    };
public:
    Sprite* m_sprite;//包含的瓦片精灵
    PathSprite* m_parent;//父节点
    PathSprite* m_child;//子节点
    float m_costToSource;//到起始点的距离
    int m_x;//地图坐标
    int m_y;
    float m_FValue;
};
class PathSearchInfo//寻路类(主要负责寻路的参数和逻辑)
{
public:
    
    static int m_startX;//开始点
    static int m_startY;
    
    static int m_endX;//结束点
    static int m_endY;
    
    static Size m_mapSize;//地图大小
    static Size m_tileSize;//地图的块大小
    static vector<PathSprite*> m_openList;//开放列表(里面存放相邻节点)
    static PathSprite* m_inspectArray[MAP_WIDTH][MAP_HEIGHT];//全部需要检测的点
    static vector<PathSprite*> m_pathList;//路径列表
    static vector<PathSprite*> m_haveInspectList;//检测过的列表
    
    static float calculateTwoObjDistance(PathSprite* obj1, PathSprite* obj2)//计算两个物体间的距离
    {
        //        float _offsetX = obj1->m_x - obj2->m_x;
        //        float _offsetY = obj1->m_y - obj2->m_y;
        //        return sqrt( _offsetX * _offsetX + _offsetY * _offsetY);
        
        float _x = abs(obj2->m_x - obj1->m_x);
        float _y = abs(obj2->m_y - obj1->m_y);
        
        return _x + _y;
    }
    static void inspectTheAdjacentNodes(PathSprite* node, PathSprite* adjacent, PathSprite* endNode)//把相邻的节点放入开放节点中
    {
        if (adjacent)
        {
            float _x = abs(endNode->m_x - adjacent->m_x);
            float _y = abs(endNode->m_y - adjacent->m_y);
            
            float F , G, H1, H2, H3;
            adjacent->m_costToSource = node->m_costToSource + calculateTwoObjDistance(node, adjacent);//获得累计的路程
            G = adjacent->m_costToSource;
            
            //三种算法, 感觉H2不错
            H1 = _x + _y;
            // hypot 根据两边长求斜边长度
            H2 = hypot(_x, _y);
            H3 = max(_x, _y);
            
#if 1 //A*算法 = Dijkstra算法 + 最佳优先搜索
            F = G + H2;
#endif
#if 0//Dijkstra算法
            F = G;
#endif
#if 0//最佳优先搜索
            F = H2;
#endif
            adjacent->m_FValue = F;
            
            adjacent->m_parent = node;//设置父节点
            adjacent->m_sprite->setColor(Color3B::ORANGE);//搜寻过的节点设为橘色(测试用)
            m_haveInspectList.push_back(adjacent);
            node->m_child = adjacent;//设置子节点
            
            PathSearchInfo::m_inspectArray[adjacent->m_x][adjacent->m_y] = NULL;//把检测过的点从检测列表中删除
            PathSearchInfo::m_openList.push_back(adjacent);//加入开放列表
        }
    }
    static PathSprite* getMinPathFormOpenList()//从开放节点中获取F值最小值的点
    {
        if (m_openList.size()>0) {
            PathSprite* _sp =* m_openList.begin();
            for (vector<PathSprite*>::iterator iter = m_openList.begin(); iter !=  m_openList.end(); iter++)
            {
                if ((*iter)->m_FValue < _sp->m_FValue)
                {
                    _sp = *iter;
                }
            }
            return _sp;
        }
        else
        {
            return NULL;
        }
        
    }
    static PathSprite* getObjFromInspectArray(int x, int y)//根据横纵坐标从检测数组中获取点
    {
        if (x >=0 && y >=0 && x < m_mapSize.width && y < m_mapSize.height) {
            return m_inspectArray[x][y];
        }
        return  NULL;
    }
    static bool removeObjFromOpenList( PathSprite* sprite)//从开放列表中移除对象
    {
        if (!sprite) {
            return  false;
        }
        for (vector<PathSprite*>::iterator iter = m_openList.begin(); iter !=  m_openList.end(); iter++)
        {
            if (*iter == sprite)
            {
                m_openList.erase(iter);
                return true;
            }
        }
        return false;
        
    }
};
class AStart : public cocos2d::Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // a selector callback
    void menuCloseCallback(Ref* pSender);

    virtual void onEnter();
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void calculatePath();//计算路径
    
    void drawPath();//绘制路径(测试用)
    
    void clearPath();//清理路径
    
    void playerMove();//人物走动
    
    void update(float dt);//跟新大地图(行走时, 人不动, 地图跟着人动);
    
    // implement the "static node()" method manually
    CREATE_FUNC(AStart);
    
public:
    Point m_orignPoint;//人物的起始点
    
    PathSprite* m_player;//人物点
    
    int m_playerMoveStep;//人物当前的行程的索引
    
};

#endif // __ASTART_H__
