-- Lua test script


i = 0;

boss = false;


function MainLoop()

    if(i < 20) then
        if (lGame.GetCameraPosition() < i*-40) then
            lGame.Agents.SpawnAgent("Ellenseg "..i,600,lGame.GetCameraPosition() - 60,lGame.Agents.Species.Enemy,"Normal",0.0,lGame.GetDefPlayerSpeedY(),lGame.Agents.Aggression.Aggressive);

            lGame.Agents.MoveDown("Ellenseg "..i);


            i = i+1;
        end

    elseif (i == 20) then
        lGame.Agents.SpawnAgent("Boss",600,lGame.GetCameraPosition() - 120,lGame.Agents.Species.Enemy,"Boss",0.0,lGame.GetDefPlayerSpeedY(),lGame.Agents.Aggression.Aggressive);

        lGame.Agents.MoveDown("Boss");
        lGame.Agents["Boss"].Pos = 0;

        lGame.Agents["Boss"].Logic = function()
            if(lGame.Agents["Boss"].Pos < 300) then
                lGame.Agents["Boss"].Pos = lGame.Agents["Boss"].Pos + 120/60;
            else
                lGame.Agents.StopMoving("Boss");
            end

        end
        i = i+1;

        boss = true;
    end

    if(boss) then
        lGame.Agents["Boss"].Logic();
        if (lGame.Agents.IsAlive("Boss") == false) then
            lGame.SendMsg.MissionCompleted();
        end
    end

end

