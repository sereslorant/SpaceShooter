
lGame = {};
lGame.pGame = pGame;

lGame.SendMsg = {};
lGame.SendMsg.MissionCompleted = function()
    lSS_SendMsgMissionCompleted(pGame);
end

lGame.SendMsg.MissionFailed = function()
    lSS_SendMsgMissionFailed(pGame);
end

--Setting up getter functions
lGame.GetCameraPosition = function()
    return lSS_GetCameraPos(pGame);
end

lGame.GetDefPlayerSpeedX = function()
    return lSS_GetDefPlayerSpeedX(pGame);
end

lGame.GetDefPlayerSpeedY = function()
    return lSS_GetDefPlayerSpeedY(pGame);
end

lGame.Agents = {};
lGame.Agents.Species = {};
lGame.Agents.Species.Friend = 0;
lGame.Agents.Species.Enemy = 1;

lGame.Agents.Aggression = {};
lGame.Agents.Aggression.Aggressive = 1;
lGame.Agents.Aggression.Evasive = 2;
lGame.Agents.Aggression.Controllable = 0;

lGame.Agents.Movement = {};
lGame.Agents.Movement.Up = 0;
lGame.Agents.Movement.Down = 1;
lGame.Agents.Movement.Left = 0;
lGame.Agents.Movement.Right = 1;
lGame.Agents.Movement.NotMoving = 2;

--setting up getter functions
lGame.Agents.IsAlive = function(Name)
    return lSS_AvatarIsAlive(lGame.Agents[Name].pAgent);
end

--setting up controls
lGame.Agents.MoveUp = function(Name)
    lSS_SetVerticalMovement(lGame.Agents[Name].pAgent,lGame.Agents.Movement.Up)
end

lGame.Agents.MoveDown = function(Name)
    lSS_SetVerticalMovement(lGame.Agents[Name].pAgent,lGame.Agents.Movement.Down);
end

lGame.Agents.MoveLeft = function(Name)
    if(lGame.Agents[Name].Aggression == lGame.Agents.Aggression.Controllable) then
        lSS_SetHorizontalMovement(lGame.Agents[Name].pAgent,lGame.Agents.Movement.Left);
    end
end

lGame.Agents.MoveRight = function(Name)
    if(lGame.Agents[Name].Aggression == lGame.Agents.Aggression.Controllable) then
        lSS_SetHorizontalMovement(lGame.Agents[Name].pAgent,lGame.Agents.Movement.Right);
    end
end

lGame.Agents.StopMoving = function(Name)
    if(lGame.Agents[Name].Aggression == lGame.Agents.Aggression.Controllable) then
        lSS_SetHorizontalMovement(lGame.Agents[Name].pAgent,lGame.Agents.Movement.NotMoving);
    end

    lSS_SetVerticalMovement(lGame.Agents[Name].pAgent,lGame.Agents.Movement.NotMoving);
end

lGame.Agents.Shoot = function(Name)
    lSS_SetShoot(lGame.Agents[Name].pAgent,true);
end

lGame.Agents.CeaseFire = function(Name)
    lSS_SetShoot(lGame.Agents[Name].pAgent,false);
end

lGame.Agents.SpawnAgent = function (Name,PosX,PosY,Species,ShipType,DefSpeedX,DefSpeedY,Aggression)
    lGame.Agents[Name] = {}
    lGame.Agents[Name].pAgent = lSS_SpawnAgent(lGame.pGame,PosX,PosY,Species,ShipType,DefSpeedX,DefSpeedY,Aggression);

    lGame.Agents[Name].Aggression = Aggression;
end

require(MapScript)
