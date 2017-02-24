-- STATES
APPROACH = "approach"
WAIT = "wait"
REPEL = "repel"

-- GLOBAL VAR
avoiding = false
current_state = APPROACH
avoiding_turns = 0
inventory = {}
robot_close = false

-- CONST
P_leave = 0.001
P_return = 0.01
MAX_AVOID_TURNS = 20
SENSE_THRESH = 0.5

-- the main loop
function step()
    robot_close = false
    
    if avoiding then
        avoiding_turns = avoiding_turns - 1
        robot.wheels.set_velocity(10,0)
        if avoiding_turns == 0 then
            avoiding = false
        end
    
    elseif current_state == APPROACH then
        Approach()
        if robot_close then
            Wait()
        elseif SenseObstacle() then
            Avoid()
        end

    elseif current_state == REPEL then
        Repel()
        if SenseObstacle() then
            Avoid()
        elseif robot.random.uniform() < P_return then
            Approach()
        end
        
    elseif current_state == WAIT then
        robot.wheels.set_velocity(0,0)
        
        if robot.random.uniform() < P_leave then
            Repel()
        end
    end
end

function Avoid()
    avoiding = true
    avoiding_turns = math.random(MAX_AVOID_TURNS)
end

function Wait()
    current_state = WAIT
    
    local data = math.random(255) --create a new word
    if #inventory > 0 then --or choose a word if we have some
        data = inventory[math.random(#inventory)]
    end
        robot.range_and_bearing.set_data(1,data) -- send the word to the other robots
end
    
function Approach()
    current_state = APPROACH
    Aim(true)
end

function Repel()
    current_state = REPEL
    robot.range_and_bearing.set_data(1,0) --stop sending
    Aim(false)
end

function Aim(front)
    dir = -1
    closest = -1
    dir_closest = -1
	for i = 1, #robot.range_and_bearing do -- for each robot seen                                              
		if robot.range_and_bearing[i].data[1] > 0 then
            if UpdateLexicon(robot.range_and_bearing[i].data[1]) then
                dir = robot.range_and_bearing[i].horizontal_bearing
            else
                dir = -1
            end
		end
        if closest == -1 or robot.range_and_bearing[i].range < closest then
            closest = robot.range_and_bearing[i].range
            dir_closest = robot.range_and_bearing[i].horizontal_bearing
        end
	end
    
    if closest >= 0 then
        if closest < 20 then
            robot_close = true
        else
            if dir==-1 then
                dir = dir_closest
            end
            
            if(front) then
                if(math.abs(dir) < 0.5) then
                    robot.wheels.set_velocity(10,10)
                else
                    robot.wheels.set_velocity(-dir*10,dir*10)
                end
            else
                if(math.pi-math.abs(dir) < 0.5) then
                    robot.wheels.set_velocity(10,10)
                else
                    robot.wheels.set_velocity((math.pi-dir)*10,(dir-math.pi)*10)
                end
            end        
        end
    else
        robot.wheels.set_velocity(10,10)
    end
end

function SenseObstacle()
    local p = robot.proximity
	max = 0
	max_id = 0
	for i = 1,#p do
		if math.abs(p[i].angle) < math.pi/2 and p[i].value > SENSE_THRESH then
			return true
		end
	end
	return false
end

function UpdateLexicon(w)
        local inside = false
        if w > 0 then
            for i = 1,#inventory do
                if inventory[i]==w then
                    inside = true
                end
            end

            
            if inside then
                inventory = {w}
            else
                inventory[#inventory+1] = w
            end
        end
        return inside
end

function getWord()
    if #inventory==1 then
        return inventory[1]
    else
        return "_"
    end
end

-- init/reset/destroy
function init()
   current_state = APPROACH
   robot_close = false
    avoiding = false
	avoiding_turns = 0
    inventory = {}
   robot.range_and_bearing.set_data(1,0)
end
function reset()
	current_state = APPROACH
    robot_close = false
    avoiding = false
	avoiding_turns = 0
    inventory = {}
    robot.range_and_bearing.set_data(1,0)
end
function destroy()
end
