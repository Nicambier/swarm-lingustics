-- STATES
WALK = "walk"
STAY = "stay"
AVOID = "avoid"

-- GLOBAL VAR
current_state = WALK
leaving_turns = 0
staying_turns = 0
avoiding_turns = 0
number_robot_sensed = 0

-- CONST
STAYING_TURNS = 200
LEAVING_TURNS = 50
MAX_AVOID_TURNS = 20
BASE_STAY_PROBA = 0.18
SENSE_THRESH = 0.5

-- the main loop
function step()
    
    if current_state == WALK then
        robot.wheels.set_velocity(10,10)
        if OnSpot() then
            Stay()
        elseif SenseObstacle() then
            Avoid()
        end
    elseif current_state == LEAVE then
        leaving_turns = leaving_turns - 1
        robot.wheels.set_velocity(10,10)
        if SenseObstacle() then
            Avoid()
        elseif leaving_turns == 0 then
            current_state = WALK
        end
    elseif current_state == AVOID then
        avoiding_turns = avoiding_turns - 1
        robot.wheels.set_velocity(10,0)
        if avoiding_turns == 0 then
            if leaving_turns > 0 then
                current_state = LEAVE
            else
                current_state = WALK
            end
        end
    elseif current_state == STAY then
        staying_turns = staying_turns - 1
        robot.wheels.set_velocity(0,0)
        
        robot.range_and_bearing.set_data(1,1) -- send the word to the other robots
        
        if staying_turns == 0 then
            CountRAB()
            local proba = BASE_STAY_PROBA * (number_robot_sensed + 1) -- +1 to account for the robot itself
            if robot.random.uniform() < proba then -- stay
                Stay()
            else
                Leave()
            end
        end
    end
    
    --log(getWord())
end

function Stay()
    current_state = STAY
    staying_turns = STAYING_TURNS
end

function Leave()
    current_state = LEAVE
    robot.range_and_bearing.set_data(1,0) --stop sending
    leaving_turns = LEAVING_TURNS
end

function Avoid()
    current_state = AVOID
    avoiding_turns = math.random(MAX_AVOID_TURNS)
end
    
function Walk()
    current_state = WALK
end

function OnSpot()
    local s = robot.motor_ground
	max = 0
	max_id = 0
	for i = 1,#s do
		if s[i].value < 0.8 then
			return true
		end
	end
	return false
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
            

function CountRAB()
    local converged = false
    number_robot_sensed = 0
	for i = 1, #robot.range_and_bearing do -- for each robot seen                                              
		if robot.range_and_bearing[i].range < 150 and robot.range_and_bearing[i].data[1] ~= 0 then
                    number_robot_sensed = number_robot_sensed + 1 -- increase the counter
		end
	end
end

function getWord()
    return "_"
end

-- init/reset/destroy
function init()
   current_state = WALK
end

function reset()
	current_state = WALK
	avoiding_turns = 0
	number_robot_sensed = 0
	staying_turns = 0
	leaving_turns = 0
end
function destroy()
end