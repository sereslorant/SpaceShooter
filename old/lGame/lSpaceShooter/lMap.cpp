
#include "lMap.h"

#include "../../lMath/lmGeometry2D.h"

#include "lSpaceShooter.h"

using namespace std;

lRect *lMap::lTerrain::lTile::GetRect()
{
    return Rect;
}
/*
void lMap::lTerrain::lTile::MoveY(float dy)
{
    Rect->SetY(Rect->GetPosition().Y + dy);
}
*/
lMap::lTerrain::lTile::lTile(lRect *rect)
    :Rect(rect)
{

}

lMap::lTerrain::lTile::~lTile()
{
    delete Rect;
}

lMap::lTerrain::lTile **lMap::lTerrain::operator [](unsigned int i)
{
    return Terrain[i];
}

void lMap::lTerrain::Draw()
{
    for(unsigned int i=0;i < Height;i++)
    {
        for(unsigned int j=0;j < Width;j++)
        {
            if(Terrain[i][j] != nullptr)
            {
                Terrain[i][j]->GetRect()->Draw();
            }
        }
    }
}

lMap::lTerrain::lTerrain(unsigned int width,unsigned int height)
    :Width(width),Height(height)
{
    Terrain = new lTile **[Height];

    for(unsigned int i=0;i < Height;i++)
    {
        Terrain[i] = new lTile *[Width];
    }
}

lMap::lTerrain::~lTerrain()
{
    for(unsigned int i=0;i < Height;i++)
    {
        for(unsigned int j=0;j < Width;j++)
        {
            if(Terrain[i][j] != nullptr)
            {
                delete Terrain[i][j];
            }
        }

        delete Terrain[i];
    }

    delete Terrain;
}

const std::list<lObject *> &lMap::GetObjects() const
{
    return Objects;
}
const std::list<lProjectile *> &lMap::GetProjectiles() const
{
    return Projectiles;
}

void lMap::AddObject(lObject *Object)
{
    Objects.push_back(Object);
}

void lMap::DeleteObject(lObject *Object)
{
    for(auto I = Objects.begin();I != Objects.end();I++)
    {
        if(*I == Object)
        {
            Objects.erase(I);
            break;
        }
    }
}

void lMap::AddProjectile(lProjectile *Projectile)
{
    Projectiles.push_back(Projectile);
}

void lMap::Step(float dt,lRect *Viewport)
{
    Background->SetPosition(Viewport->GetPosition());

    //CHECK_COLLISION
    for(auto I = Objects.begin();I != Objects.end();I++)
    {
        for(auto J = next(I);J != Objects.end();J++)
        {
            if((*J)->IsAlive() == false)
            {
                continue;
            }

            const lmVector2D &I_Min = (*I)->GetRect()->GetPosition();
            lmVector2D I_Max = I_Min + lmVector2D((*I)->GetRect()->GetWidth(),(*I)->GetRect()->GetHeight());

            const lmVector2D &J_Min = (*J)->GetRect()->GetPosition();
            lmVector2D J_Max = J_Min + lmVector2D((*J)->GetRect()->GetWidth(),(*J)->GetRect()->GetHeight());

            if(lmAABBAABBIntersection(I_Min,I_Max,J_Min,J_Max))
            {
                if((*I)->GetSpecies() != (*J)->GetSpecies())
                {
                    float I_ColDmg = (*I)->GetCollisionDamage();
                    float J_ColDmg = (*J)->GetCollisionDamage();

                    (*I)->TakeDamage(J_ColDmg);
                    (*J)->TakeDamage(I_ColDmg);
                }
            }
        }
    }

    for(auto I = Projectiles.begin();I != Projectiles.end();  )
    {
        bool Collides = false;
        float MinMlt;
        lmVector2D MinPoI;
        list<lObject *>::iterator K;

        for(auto J = Objects.begin();J != Objects.end();J++)
        {
            if((*J)->IsAlive() == false)
            {
                continue;
            }

            float Mlt;
            lmVector2D PofImpact;

            if((*I)->GetSpecies() != (*J)->GetSpecies())
            {
                if(CheckCollision((*J)->GetRect(),(*I)->GetTrajectory(),&Mlt,&PofImpact))
                {
                    if(Mlt < ((*I)->GetSpeed() * dt) && Mlt > 0)
                    {
                        if(Collides != true)
                        {
                            Collides = true;

                            MinMlt = Mlt;
                            MinPoI = PofImpact;
                            K = J;
                        }
                        else
                        {
                            if(Mlt < MinMlt)
                            {
                                MinMlt = Mlt;
                                MinPoI = PofImpact;
                                K = J;
                            }
                        }
                    }
                }
            }
        }

        const lmVector2D &ViewportMin = Viewport->GetPosition();
        lmVector2D ViewportMax = ViewportMin + lmVector2D(Viewport->GetWidth(),Viewport->GetHeight());

        if(Collides || !lmPointAABBIntersection((*I)->GetTrajectory()->GetPosition(),ViewportMin,ViewportMax))
        {
            if(Collides)
            {
                (*K)->TakeDamage((*I)->GetDamage());
                cout << "Talalat " << (*I)->GetDamage() << " PofImpact: " << MinPoI.X << ";" << MinPoI.Y << endl;
            }
            else
            {
                cout << "Megpusztult " << (*I)->GetDamage() << endl;
            }

            auto L = I;
            I++;
            Projectiles.erase(L);
        }
        else
        {
            I++;
        }
    }

    //UPDATE
    for(auto I = Objects.begin();I != Objects.end(); )
    {
        if((*I)->IsAlive() == false)
        {
            auto K = I;
            I++;

            cout << "Points earned " << (*K)->GetPoints() << endl;

            Game->AddPoints(((*K)->GetPoints()));

            Objects.erase(K);
            cout << "Egy objektum megpusztult" << endl;
        }
        else
        {
            (*I)->Update(dt);
            (*I)->UpdateRect();
            I++;
        }
    }

    for(auto I = Projectiles.begin();I != Projectiles.end();I++)
    {
        (*I)->Update(dt);
    }
}

void lMap::Draw()
{
    glMatrixMode(GL_MODELVIEW);

    Background->Draw();
    Terrain->Draw();

    for(auto I = Objects.begin();I != Objects.end();I++)
    {
        (*I)->Draw();
    }

    for(auto I = Projectiles.begin();I != Projectiles.end();I++)
    {
        (*I)->Draw();
    }
}

lMap::lMap(lSpaceShooter *game,float Width,float Height,RM &resource_manager,const std::string &content_directory,const std::string &map_name)
    :Game(game)
{
    fstream fin;
    fin.open("Map/" + map_name + "/Map.map");

    if(!fin.is_open())
    {
        cerr << "Error! Couldn't open map\n";
        return;
    }

    string MapName;
    //Background parameters
    float BgWidth;
    float BgHeight;

    string BgTexture;
    //Terrain parameters
    unsigned int TerrainWidth;
    unsigned int TerrainHeight;
    float TileSize;

    //Minden egyéb
    float lTilePos;//a legszélső baloldali tile pozíciója

    while(!fin.eof())
    {
        string Tmp;
        fin >> Tmp;

        if(Tmp == "MapName")
        {
            LoadQuotedString(fin,MapName);
        }
        if(Tmp == "BgWidth")
        {
            fin >> BgWidth;
        }
        if(Tmp == "BgHeight")
        {
            fin >> BgHeight;
        }
        if(Tmp == "BgTexture")
        {
            LoadQuotedString(fin,BgTexture);
            Background = new lRect(lmVector2D::NULL_VECTOR,BgWidth,BgHeight,lRect::UP,lColor(1.0,1.0,1.0,1.0),resource_manager.GetImage(content_directory + BgTexture));
        }
        if(Tmp == "TerrainWidth")
        {
            fin >> TerrainWidth;
        }
        if(Tmp == "TerrainHeight")
        {
            fin >> TerrainHeight;
            Terrain = new lTerrain(TerrainWidth,TerrainHeight);
        }
        if(Tmp == "TileSize")
        {
            fin >> TileSize;
            lTilePos = (Width - TileSize*TerrainWidth)/2.0;
        }

        if(Tmp == "{")
        {
            for(unsigned int i=0;i < TerrainHeight;i++)
            {
                for(unsigned int j=0;j < TerrainWidth;j++)
                {
                    bool IsAvailable;
                    string Texture;

                    fin >> Tmp;
                    fin >> IsAvailable;
                    fin >> Tmp;
                    if(IsAvailable)
                        {LoadQuotedString(fin,Texture);}
                    fin >> Tmp;

                    lmVector2D tPosition(lTilePos + j*TileSize,Height - i*TileSize);

                    if(IsAvailable)
                    {
                        (*Terrain)[i][j] = new lTerrain::lTile(new lRect(tPosition,TileSize,TileSize,lRect::UP,lColor(1.0,1.0,1.0,1.0),resource_manager.GetImage(content_directory + Texture)));
                    }
                    else
                    {
                        (*Terrain)[i][j] = nullptr;
                    }
                }
            }

            fin >> Tmp;
        }
    }

}

lMap::~lMap()
{
    delete Background;
    delete Terrain;

    for(auto I = Objects.begin();I != Objects.end();I++)
    {
        delete *I;
    }

    for(auto I = Projectiles.begin();I != Projectiles.end();I++)
    {
        delete *I;
    }
}
