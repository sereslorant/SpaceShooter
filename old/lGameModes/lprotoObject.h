#ifndef L_JRPG_OBJECT
#define L_JRPG_OBJECT

#include <list>

#include "../lGame/lRect.h"

class lprotoCollisionCallback;

class lprotoObject
{
public:
	enum COLLISION_RESPONSE
	{
		IMMOVABLE,
		MOVABLE,
		GHOST,
	};

protected:
	lRect *Rect;

	bool Removable;

	lmVector2D Position;
	lmVector2D Center;
	lmVector2D Velocity;
	float Radius;

	COLLISION_RESPONSE CollisionResponse;

	lprotoCollisionCallback *CollisionCallback;

	void *UserData;

public:

	/*int GetSpecies();
	bool GetAlive()
	{
		return IsAlive;
	}

	bool GetTargetable()
	{
		return IsTargetable;
	}*/

	lRect *GetRect()
	{
		return Rect;
	}

	void SetRemovable(bool removable)
	{
		Removable = removable;
	}

	bool IsRemovable()
	{
		return Removable;
	}

	void *GetUserData()
	{
		return UserData;
	}

	void SetUserData(void *user_data)
	{
		UserData = user_data;
	}

	const lmVector2D &GetPosition();
	const lmVector2D &GetCenter();
	const lmVector2D &GetVelocity();
	float GetRadius();
	COLLISION_RESPONSE GetCollisionResponse();
	lprotoCollisionCallback *GetCollisionCallback();

	void SetPosition(const lmVector2D &position);
	void SetCenter(const lmVector2D &center);
	void SetVelocity(const lmVector2D &velocity);
	void AddVelocity(const lmVector2D &dv);
	void SetRadius(float radius);

	void GetCollisionResponse(COLLISION_RESPONSE collision_response);
	void SetCollisionCallback(lprotoCollisionCallback *collision_callback);
	void DeleteCollisionCallback();

	lprotoObject(const lmVector2D &position,const lmVector2D &center,float width,float height,const lColor &color,GLuint texture,float radius,/*int species,bool is_alive,bool is_targetable,*/COLLISION_RESPONSE collision_reaction,void *user_data = nullptr);
	virtual ~lprotoObject();

	//virtual void TakeDamage(float damage) = 0;
	//virtual void ExecuteBehavior(float dt,std::list<lprotoObject *> &Objects) = 0;

	void Step(float dt);
	virtual void Draw();
};

class lprotoAnimObject : public lprotoObject
{
private:
	unsigned int nTextures;
	GLuint *Textures;

	unsigned int TextureIndex;

public:

	void SetTextureIndex(unsigned int texture_index)
	{
		if(texture_index < nTextures)
		{
			TextureIndex = texture_index;
		}
		else
		{
			TextureIndex = nTextures - 1;
		}
	}

	unsigned int GetTextureIndex()
	{
		return TextureIndex;
	}

	GLuint GetTexture(unsigned int i)
	{
		if(i < nTextures)
		{
			return Textures[i];
		}
		else
		{
			return Textures[nTextures - 1];
		}
	}

	lprotoAnimObject(const lmVector2D &position,const lmVector2D &center,float width,float height,const lColor &color,GLuint *textures,unsigned int n_textures,float radius,COLLISION_RESPONSE collision_reaction,void *user_data = nullptr)
		:lprotoObject(position,center,width,height,color,0,radius,collision_reaction,user_data),nTextures(n_textures),TextureIndex(0)
	{
		Textures = new GLuint[nTextures];

		for(int i=0;i < nTextures;i++)
		{
			Textures[i] = textures[i];
		}
	}

	virtual ~lprotoAnimObject() override
	{
		delete Textures;
	}

	virtual void Draw() override
	{
		Rect->SetTexture(Textures[TextureIndex]);

		lprotoObject::Draw();
	}
};

#endif
