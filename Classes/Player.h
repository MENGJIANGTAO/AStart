//
//  Player.h
//  AStart
//
//  Created by Mengjiangtao on 7/31/15.
//
//

#ifndef __AStart__Player__
#define __AStart__Player__

#include "cocos2d.h"
USING_NS_CC;
class Player
{
public:
    Player();
    ~Player();
    static Player* createPlayer(const char* filename);
    
private:
    bool isDie;
    int attack;
    int defen;
    int blood;
    Sprite* player_sprite;
};

#endif /* defined(__AStart__Player__) */
