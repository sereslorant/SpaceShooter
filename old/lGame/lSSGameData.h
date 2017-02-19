#ifndef LSS_GAME_DATA
#define LSS_GAME_DATA

#include "lSpaceShooter/lShipDatabase.h"

#include <string>
#include <list>

#include <fstream>

class lSSGameData
{
public:
    class lPlayerShip
    {
    public:
        struct lWeapon
        {
            bool Active;
            std::string Type;

            lWeapon():Active(false){}
        };
    private:
        std::string ShipType;
        std::vector<lWeapon> WeaponSlots;
    public:
        lPlayerShip(const std::string &ship_type,unsigned int n_weapons)
            :ShipType(ship_type),WeaponSlots(n_weapons)
        {

        }

        const std::string &GetShipType()
        {
            return ShipType;
        }

        unsigned int WpnSlotSize()
        {
            return WeaponSlots.size();
        }

        lWeapon &operator[](unsigned int i)
        {
            return WeaponSlots[i];
        }
    };

    struct lMission
    {
        enum lMissionState
        {
            IN_PROGRESS,
            COMPLETED,
            FAILED,
        };
        std::string Name;
        std::string Description;
        //lmVector2D Coordinates;

        std::string MapName;
        lMissionState MissionState;
    };
    static const unsigned int N_PLAYERS = 2;

private:
    lShipDatabase &ShipDatabase;

    lPlayerShip *PlayerShip[N_PLAYERS];
    unsigned int Cash;

    std::list<lMission> Missions;

    void Reset()
    {
        delete PlayerShip[0];
        delete PlayerShip[1];

        Missions.clear();

        Cash = 0;
    }

public:
    void SetDefault()
    {
        Reset();

        PlayerShip[0] = new lPlayerShip("Player",ShipDatabase.Ships["Player"].nWeapons);
        PlayerShip[1] = new lPlayerShip("Player",ShipDatabase.Ships["Player"].nWeapons);

        for(int i=0;i < ShipDatabase.Ships["Player"].nWeapons;i++)
        {
            (*PlayerShip[0])[i].Active = true;
            (*PlayerShip[0])[i].Type = "Normal";

            (*PlayerShip[1])[i].Active = true;
            (*PlayerShip[1])[i].Type = "Normal";
        }

        Cash = 1000;

        Missions.push_back({"Anyad","Aki egy kurva","TestMap",lMission::IN_PROGRESS});
        Missions.push_back({"Apad","Aki nem egy kurva","JRPG",lMission::IN_PROGRESS});
        Missions.push_back({"Hugom","Akivel kozos vibratorunk van","TowerDefense",lMission::IN_PROGRESS});
    }

    bool Load(const std::string &filename)
    {
        std::ifstream fin;
        fin.open(filename.c_str());

        if(!fin.is_open())
        {
            return false;
        }

        Reset();

        while(!fin.eof())
        {
            std::string Tmp;

            fin >> Tmp;

            if(Tmp == "PlayerShip")
            {
                unsigned int Id;
                std::string Name;
                unsigned int nWpn;

                fin >> Id;

                while(Tmp != "End")
                {
                    fin >> Tmp;

                    if(Tmp == "ShipType")
                    {
                        LoadQuotedString(fin,Name);
                    }
                    if(Tmp == "WeaponSlots")
                    {
                        fin >> nWpn;

                        PlayerShip[Id] = new lPlayerShip(Name,nWpn);

                        for(unsigned int i=0;i < PlayerShip[Id]->WpnSlotSize();i++)
                        {
                            bool Active;
                            std::string WpnName;

                            fin >> Tmp;
                            fin >> Active;

                            if(Active)
                            {
                                LoadQuotedString(fin,WpnName);

                                (*PlayerShip[Id])[i].Active = true;
                                (*PlayerShip[Id])[i].Type = WpnName;
                            }

                        }
                    }
                }
            }
            if(Tmp == "Cash")
            {
                fin >> Cash;
            }
            if(Tmp == "Missions")
            {
                while(Tmp != "End")
                {
                    fin >> Tmp;

                    if(Tmp == "Mission")
                    {
                        std::string Tmp;

                        std::string Name;
                        std::string Description;

                        std::string MapName;
                        lMission::lMissionState MissionState;

                        while(Tmp != "End")
                        {
                            fin >> Tmp;

                            if(Tmp == "Name")
                            {
                                LoadQuotedString(fin,Name);
                            }
                            if(Tmp == "Description")
                            {
                                LoadQuotedString(fin,Description);
                            }
                            if(Tmp == "MapName")
                            {
                                LoadQuotedString(fin,MapName);
                            }
                            if(Tmp == "MissionState")
                            {
                                unsigned int TmpMissionState;
                                fin >> TmpMissionState;

                                switch(TmpMissionState)
                                {
                                case lMission::IN_PROGRESS:
                                    MissionState = lMission::IN_PROGRESS;
                                    break;
                                case lMission::COMPLETED:
                                    MissionState = lMission::COMPLETED;
                                    break;
                                case lMission::FAILED:
                                    MissionState = lMission::FAILED;
                                    break;
                                default:
                                    MissionState = lMission::IN_PROGRESS;
                                }
                            }
                        }

                        Missions.push_back({Name,Description,MapName,MissionState});
                    }
                }
            }
        }

        fin.close();

        return true;
    }

    bool Save(const std::string &filename)
    {
        std::ofstream fout;
        fout.open(filename.c_str());

        if(!fout.is_open())
        {
            return false;
        }

        for(unsigned int i=0;i < N_PLAYERS;i++)
        {
            fout << "PlayerShip " << i << "\n";
            fout << "\tShipType \"" << PlayerShip[i]->GetShipType() << "\"\n";
            fout << "\tWeaponSlots " << PlayerShip[i]->WpnSlotSize() << "\n";

            for(unsigned int j=0;j < PlayerShip[i]->WpnSlotSize();j++)
            {
                fout << "\tWeapon " << (*PlayerShip[i])[j].Active;
                if((*PlayerShip[i])[j].Active)
                {
                    fout << "\t\t\"" << (*PlayerShip[i])[j].Type << "\"\n";
                }
                else
                {
                    fout << "\n";
                }
            }
            fout << "End\n\n";
        }

        fout << "Cash " << Cash << "\n\n";

        fout << "Missions\n";

        for(lMission &I : Missions)
        {
            fout << "\tMission\n";

            fout << "\t\tName \"" << I.Name << "\"\n";
            fout << "\t\tDescription \"" << I.Description << "\"\n";

            fout << "\t\tMapName \"" << I.MapName << "\"\n";
            fout << "\t\tMissionState " << I.MissionState << "\n";

            fout << "\tEnd\n\n";
        }

        fout << "End\n";

        fout.close();

        return true;
    }

    lPlayerShip *&GetPlayerShip(unsigned int i)
    {
        return PlayerShip[i];
    }

    void AddCash(unsigned int cash)
    {
        Cash += cash;
    }

    bool TakeCash(unsigned int cash)
    {
        if(Cash >=cash)
        {
            Cash -= cash;
            return true;
        }
        else
        {
            return false;
        }
    }

    unsigned int GetCash()
    {
        return Cash;
    }

    unsigned int GetMissionsSize()
    {
        return Missions.size();
    }

    std::list<lMission>::iterator MissionsBegin()
    {
        return Missions.begin();
    }

    std::list<lMission>::iterator MissionsEnd()
    {
        return Missions.end();
    }

    lSSGameData(lShipDatabase &ship_database)
        :ShipDatabase(ship_database)
    {
        PlayerShip[0] = nullptr;
        PlayerShip[1] = nullptr;
    }

    lSSGameData(lShipDatabase &ship_database,const std::string &filename)
        :ShipDatabase(ship_database)
    {
        Reset();
    }
    ~lSSGameData()
    {
        delete PlayerShip[0];
        delete PlayerShip[1];
    }
};

#endif // LSS_GAME_DATA
