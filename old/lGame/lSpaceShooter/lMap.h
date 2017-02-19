#ifndef L_GAME_H
#define L_GAME_H

#include <list>

#include "../lCollision.h"
#include "lObject.h"

#include "../../lResourceManager/RM.h"

class lSpaceShooter;

class lMap
{
private:
    class lTerrain
    {
    public:
        class lTile
        {
        private:
            lRect *Rect;

        public:
            lRect *GetRect();

            lTile(lRect *rect);
            ~lTile();
        };

    private:
        unsigned int Width;
        unsigned int Height;

        lTile ***Terrain;

    public:
        lTile **operator [](unsigned int i);
        void Draw();

        lTerrain(unsigned int width,unsigned int height);
        ~lTerrain();
    };

    lSpaceShooter *Game;
    lRect *Background;
    lTerrain *Terrain;

    std::list<lObject *> Objects;
    std::list<lProjectile *> Projectiles;

public:
    const std::list<lObject *> &GetObjects() const;
    const std::list<lProjectile *> &GetProjectiles() const;

    void AddObject(lObject *Object);
    void DeleteObject(lObject *Object);

    void AddProjectile(lProjectile *Projectile);

    void Init();

    void Step(float dt,lRect *Viewport);

    void Draw();

    lMap(lSpaceShooter *game,float Width,float Height,RM &resource_manager,const std::string &content_directory,const std::string &map_name);
    ~lMap();
};

#endif // L_GAME_H
