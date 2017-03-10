-- STATES
WALK = "walk"
STAY = "stay"
LEAVE = "leave"

-- GLOBAL VAR
current_state = WALK
leaving_turns = 0
staying_turns = 0
avoiding_turns = 0
number_robot_sensed = 0
inventory = {}

-- CONST
STAYING_TURNS = 200
LEAVING_TURNS = 50
MAX_AVOID_TURNS = 20
BASE_STAY_PROBA = 0.18
SENSE_THRESH = 0.5

-- the main loop
function step()
    if avoiding_turns > 0 then
        avoiding_turns = avoiding_turns - 1
        robot.wheels.set_velocity(10,0)
        
    elseif current_state == WALK then
        robot.wheels.set_velocity(10,10)
        CountRAB()
        if robot.random.uniform() < stopProba(number_robot_sensed) then -- stay
            Stay()
        elseif SenseObstacle() then
            Avoid()
        end
            
    elseif current_state == STAY then
        staying_turns = staying_turns - 1
        robot.wheels.set_velocity(0,0)
        
        if staying_turns == 0 then
            CountRAB()
            if robot.random.uniform() < stopProba(number_robot_sensed) then -- stay
                Stay()
            else
                Leave()
            end
        end
        
    elseif current_state == LEAVE then
        leaving_turns = leaving_turns - 1
        robot.wheels.set_velocity(10,10)
        if leaving_turns == 0 then
            Walk()
        elseif SenseObstacle() then
            Avoid()
        end
        
    end
    
    --log(getWord())
end

function Avoid()
    avoiding_turns = math.random(MAX_AVOID_TURNS)
end

function Stay()
    current_state = STAY
    staying_turns = STAYING_TURNS
    
    local data = math.random(255) --create a new word
    if #inventory > 0 then --or choose a word if we have some
        data = inventory[math.random(#inventory)]
    end
        robot.range_and_bearing.set_data(1,data) -- send the word to the other robots
end

function Leave()
    current_state = LEAVE
    robot.range_and_bearing.set_data(1,0) --stop sending
    leaving_turns = LEAVING_TURNS
end
    
function Walk()
    current_state = WALK
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
            

-- Count the number of robots sensed close to the robot
function CountRAB()
    number_robot_sensed = 0
	for i = 1, #robot.range_and_bearing do -- for each robot seen
        if UpdateLexicon(robot.range_and_bearing[i].data[1]) then
            number_robot_sensed = number_robot_sensed + 1 -- increase the counter
        else
            number_robot_sensed = 0
        end
	end
end

function stopProba(N)
    return BASE_STAY_PROBA * (number_robot_sensed + 1) -- +1 to account for the robot itself
    --[[
    if N==0 then
        return 0.03
    elseif N==1 then
        return 0.42
    elseif N==2 then
        return 0.5
    elseif N>=3 then
        return 0.51
    end--]]
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
   current_state = WALK
   avoiding_turns = 0
	number_robot_sensed = 0
	staying_turns = 0
	leaving_turns = 0
    inventory = {}
end
function reset()
	current_state = WALK
	avoiding_turns = 0
	number_robot_sensed = 0
	staying_turns = 0
	leaving_turns = 0
    inventory = {}
end
function destroy()
end
