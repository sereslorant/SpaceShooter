#ifndef L_JRPG_MAP
#define L_JRPG_MAP

#include "../lResourceManager/RM.h"

#include <list>

#include "lprotoObject.h"
#include "lprotoCollisionCallback.h"

class lprotoMap
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

	lRect *Background;
	lTerrain *Terrain;

	std::list<lprotoObject *> Objects;

public:

	const std::list<lprotoObject *> &GetObjects() const;

	void AddObject(lprotoObject *Object);
	void DeleteObject(lprotoObject *Object);

	void Step(float dt/*,const lmVector2D &vp_pos,float width,float height*/);

	void Draw(const lmVector2D &vp_pos);

	lprotoMap(RM &resource_manager,const std::string &map_name);
	~lprotoMap();
};

#endif
