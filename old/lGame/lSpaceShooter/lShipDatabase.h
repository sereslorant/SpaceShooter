#ifndef L_SHIP_DATABASE
#define L_SHIP_DATABASE

#include <vector>
#include <map>

#include <fstream>

#include "../../lResourceManager/RM.h"

#include "../../lMath/lmVector2D.h"
#include "../../lColor.h"

class lShipDatabase
{
public:
    struct ShipData
    {
        lmVector2D Speed;
        float MaxHp;
        float MaxShield;
        //TMP
        float ShieldRegenRate;
        //TMP
        float Width;
        float Height;
        lColor Color;
        std::string Texture;
        unsigned int Points;
        unsigned int Price;
        unsigned int nWeapons;
        std::vector<lmVector2D> WPos;
        std::vector<lmVector2D> WDir;
    };

    struct WeaponData
    {
        float Speed;
        float Damage;
        float RechargeSpeed;
        unsigned int Price;
    };

	std::map<std::string,ShipData> Ships;
	std::map<std::string,WeaponData> Weapons;

    lShipDatabase(const std::string &ship_filename,const std::string &wpn_filename)
    {
        std::ifstream fin;
        fin.open(ship_filename.c_str());

        while(!fin.eof())
        {
            std::string Tmp;

            lmVector2D Speed;
            float MaxHealth;
            float MaxShield;
            float ShieldRegenRate;
            float Width;
            float Height;
            lColor Color;
            std::string Texture;
            unsigned int Points;
            unsigned int Price;
            unsigned int nWeapons;
            std::vector<lmVector2D> wPos;
            std::vector<lmVector2D> wDir;

            fin >> Tmp;

            if(Tmp == "Ship")
            {
                std::string ShipName;

                do
                {
                    fin >> Tmp;
                    if(Tmp == "Name")
                    {
                        LoadQuotedString(fin,ShipName);
                    }
                    else if(Tmp == "Speed")
                    {
                        fin >> Tmp;
                        fin >> Speed.X;
                        fin >> Tmp;
                        fin >> Speed.Y;
                        fin >> Tmp;
                    }
                    else if(Tmp == "MaxHealth")
                    {
                        fin >> MaxHealth;
                    }
                    else if(Tmp == "MaxShield")
                    {
                        fin >> MaxShield;
                    }
                    else if(Tmp == "ShieldRegenRate")
                    {
                        fin >> ShieldRegenRate;
                    }
                    else if(Tmp == "Width")
                    {
                        fin >> Width;
                    }
                    else if(Tmp == "Height")
                    {
                        fin >> Height;
                    }
                    else if(Tmp == "Color")
                    {
                        fin >> Tmp;
                        fin >> Color.Red;
                        fin >> Tmp;
                        fin >> Color.Green;
                        fin >> Tmp;
                        fin >> Color.Blue;
                        fin >> Tmp;
                        fin >> Color.Alpha;
                        fin >> Tmp;
                    }
                    else if(Tmp == "Texture")
                    {
                        LoadQuotedString(fin,Texture);
                    }
                    else if(Tmp == "Points")
                    {
                        fin >> Points;
                    }
                    else if(Tmp == "Price")
                    {
                        fin >> Price;
                    }
                    else if(Tmp == "nWeapons")
                    {
                        fin >> nWeapons;
                        wPos.resize(nWeapons);
                        wDir.resize(nWeapons);

                        fin >> Tmp;
                        for(unsigned int i=0;i < nWeapons;i++)
                        {
                            fin >> Tmp;
                            fin >> Tmp;

                            fin >> wPos[i].X;
                            fin >> Tmp;
                            fin >> wPos[i].Y;

                            fin >> Tmp;
                            fin >> Tmp;
                            fin >> Tmp;

                            fin >> wDir[i].X;
                            fin >> Tmp;
                            fin >> wDir[i].Y;

                            fin >> Tmp;
                            fin >> Tmp;
                        }
                        fin >> Tmp;
                    }

                }while(Tmp != "End");

                Ships[ShipName] = {Speed,MaxHealth,MaxShield,ShieldRegenRate,Width,Height,Color,Texture,Points,Price,nWeapons,wPos,wDir};
            }
        }

        fin.close();

        fin.open(wpn_filename.c_str());

        while(!fin.eof())
        {
            std::string WeaponName;
            float Speed;
            float Damage;
            float RechargeSpeed;
            unsigned int Price;

            std::string Tmp;

            fin >> Tmp;

            if(Tmp == "Weapon")
            {
                do
                {
                    fin >> Tmp;

                    if(Tmp == "Name")
                    {
                        LoadQuotedString(fin,WeaponName);
                    }
                    else if(Tmp == "Speed")
                    {
                        fin >> Speed;
                    }
                    else if(Tmp == "Damage")
                    {
                        fin >> Damage;
                    }
                    else if(Tmp == "RechargeSpeed")
                    {
                        fin >> RechargeSpeed;
                    }
                    else if(Tmp == "Price")
                    {
                        fin >> Price;
                    }
                } while(Tmp != "End");

                Weapons[WeaponName] = {Speed,Damage,RechargeSpeed,Price};
            }
        }

        fin.close();
    }

};

#endif
