
#include "../lMath/lmGeometry2D.h"
#include "../lInterface/lInput.h"

#include "lprotoMap.h"

lRect *lprotoMap::lTerrain::lTile::GetRect()
{
	return Rect;
}

lprotoMap::lTerrain::lTile::lTile(lRect *rect)
	:Rect(rect)
{

}

lprotoMap::lTerrain::lTile::~lTile()
{
	delete Rect;
}

lprotoMap::lTerrain::lTile **lprotoMap::lTerrain::operator [](unsigned int i)
{
	return Terrain[i];
}

void lprotoMap::lTerrain::Draw()
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

lprotoMap::lTerrain::lTerrain(unsigned int width,unsigned int height)
	:Width(width),Height(height)
{
	Terrain = new lTile **[Height];

	for(unsigned int i=0;i < Height;i++)
	{
		Terrain[i] = new lTile *[Width];
	}
}

lprotoMap::lTerrain::~lTerrain()
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

const std::list<lprotoObject *> &lprotoMap::GetObjects() const
{
	return Objects;
}

void lprotoMap::AddObject(lprotoObject *Object)
{
	Objects.push_back(Object);
}

void lprotoMap::DeleteObject(lprotoObject *Object)
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

//void Init();

void lprotoMap::Step(float dt/*,const lmVector2D &vp_pos,float width,float height*/)
{
	//CHECK_COLLISION
	for(auto I = Objects.begin();I != Objects.end();)
	{
		for(auto J = next(I);J != Objects.end();J++)
		{
			if(lmCircleCircleIntersection((*J)->GetPosition(),(*J)->GetRadius(),(*I)->GetPosition(),(*I)->GetRadius()))
			{
				lprotoCollisionCallback *Callback_I = (*I)->GetCollisionCallback();
				lprotoCollisionCallback *Callback_J = (*J)->GetCollisionCallback();

				if(Callback_I != nullptr)
				{
					Callback_I->Callback(*J);
				}

				if(Callback_J != nullptr)
				{
					Callback_J->Callback(*I);
				}

				if((*J)->GetCollisionResponse() == lprotoObject::MOVABLE && (*I)->GetCollisionResponse() == lprotoObject::MOVABLE)
				{
					lmVector2D D = (*J)->GetPosition() + ((*I)->GetPosition() * -1);
					float Dist = sqrt(D.LengthSquared());
					float SumRadius = (*J)->GetRadius() + (*I)->GetRadius();

					float Overlap = SumRadius - Dist;
					float Speed = Overlap/(2.0*dt);

					D = D * (1.0/Dist);

					(*J)->AddVelocity(D * (Speed));
					(*I)->AddVelocity(D * (-Speed));
				}
				else if((*J)->GetCollisionResponse() == lprotoObject::MOVABLE && (*I)->GetCollisionResponse() == lprotoObject::IMMOVABLE)
				{
					lmVector2D D = (*J)->GetPosition() + (*I)->GetPosition() * -1;
					float Dist = sqrt(D.LengthSquared());
					float SumRadius = (*J)->GetRadius() + (*I)->GetRadius();

					float Overlap = SumRadius - Dist;
					float Speed = Overlap/dt;

					D = D * (1.0/Dist);

					(*J)->AddVelocity(D * Speed);
				}
				else if((*J)->GetCollisionResponse() == lprotoObject::IMMOVABLE && (*I)->GetCollisionResponse() == lprotoObject::MOVABLE)
				{
					lmVector2D D = (*J)->GetPosition() + (*I)->GetPosition() * -1;
					float Dist = sqrt(D.LengthSquared());
					float SumRadius = (*J)->GetRadius() + (*I)->GetRadius();

					float Overlap = SumRadius - Dist;
					float Speed = Overlap/dt;

					D = D * (1.0/Dist);

					(*I)->AddVelocity(D * (-Speed));
				}
			}
		}

		if((*I)->IsRemovable())
		{
			auto K = I;
			I++;

			Objects.erase(K);
			continue;
		}
		else
		{
			I++;
		}
	}

	// Y sorting
	for(auto I = Objects.begin();I != Objects.end();I++)
	{
		for(auto J = next(I);J != Objects.end();J++)
		{
			if((*I)->GetPosition().Y > (*J)->GetPosition().Y)
			{
				auto K = *I;

				*I = *J;
				*J = K;
			}
		}
	}

	//Step
	for(auto I : Objects)
	{
		I->Step(dt);
		//I->ExecuteBehavior(dt,Objects);
	}
}

void lprotoMap::Draw(const lmVector2D &vp_pos)
{
	Background->SetPosition(vp_pos);

	if(Background != nullptr)
	{
		Background->Draw();
	}

	Terrain->Draw();

	for(auto I = Objects.begin();I != Objects.end();I++)
	{
		(*I)->Draw();
	}
}

lprotoMap::lprotoMap(RM &resource_manager,const std::string &map_name)
{
	std::fstream fin;
	fin.open("Map/" + map_name + "/Map.map");

	if(!fin.is_open())
	{
		std::cerr << "Error! Couldn't open map\n";
		return;
	}

	std::string MapName;
	//Background parameters
	float BgWidth;
	float BgHeight;

	std::string BgTexture;
	//Terrain parameters
	unsigned int TerrainWidth;
	unsigned int TerrainHeight;
	float TileSize;

	//Minden egyéb
	float lTilePos;//a legszélső baloldali tile pozíciója

	while(!fin.eof())
	{
		std::string Tmp;
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
			Background = new lRect(lmVector2D::NULL_VECTOR,BgWidth,BgHeight,lRect::UP,lColor(1.0,1.0,1.0,1.0),resource_manager.GetImage(BgTexture));
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
			//lTilePos = (Width - TileSize*TerrainWidth)/2.0;
		}

		if(Tmp == "{")
		{
			for(unsigned int i=0;i < TerrainHeight;i++)
			{
				for(unsigned int j=0;j < TerrainWidth;j++)
				{
					bool IsAvailable;
					std::string Texture;

					fin >> Tmp;
					fin >> IsAvailable;
					fin >> Tmp;
					if(IsAvailable)
						{LoadQuotedString(fin,Texture);}
					fin >> Tmp;

					lmVector2D tPosition(j*TileSize,i*TileSize);

					if(IsAvailable)
					{
						(*Terrain)[i][j] = new lTerrain::lTile(new lRect(tPosition,TileSize,TileSize,lRect::UP,lColor(1.0,1.0,1.0,1.0),resource_manager.GetImage(Texture)));
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

lprotoMap::~lprotoMap()
{
	delete Background;
	delete Terrain;

	for(auto I = Objects.begin();I != Objects.end();I++)
	{
		delete *I;
	}
}
