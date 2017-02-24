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
end
    
function Approach()
    current_state = APPROACH
    Aim(true)
end

function Repel()
    current_state = REPEL
    Aim(false)
end

function Aim(front)
    quadrants = {}
    quadrants.front = {}
    quadrants.back = {}
    quadrants.left = {}
    quadrants.right = {}
	for i = 1, #robot.range_and_bearing do -- for each robot seen                                              
        if robot.range_and_bearing[i].horizontal_bearing <= math.pi/4 and robot.range_and_bearing[i].horizontal_bearing > -math.pi/4 then
            table.insert(quadrants.front,robot.range_and_bearing[i].range)
        elseif robot.range_and_bearing[i].horizontal_bearing <= 3*math.pi/4 and robot.range_and_bearing[i].horizontal_bearing > math.pi/4 then
            table.insert(quadrants.left,robot.range_and_bearing[i].range)
        elseif robot.range_and_bearing[i].horizontal_bearing <= -3*math.pi/4 and robot.range_and_bearing[i].horizontal_bearing > 3*math.pi/4 then
            table.insert(quadrants.back,robot.range_and_bearing[i].range)
        elseif robot.range_and_bearing[i].horizontal_bearing <= -math.pi/4 and robot.range_and_bearing[i].horizontal_bearing > -3*math.pi/4 then
            table.insert(quadrants.right,robot.range_and_bearing[i].range)
        end
        
        if robot.range_and_bearing[i].range < 20 then
            robot_close = true
        end
	end
    
    if(not robot_close) then 
        bestQuadrant = ""
        best = 0
        sides = {'front','back','left','right'}
        for i=1,4 do
            side = sides[i]
            sum = 0
            for msg = 1, #quadrants[side] do
                x = quadrants[side][msg]
                sum = sum + 1--50/x--math.exp^(-(x*x)/(2*(30*30)))
            end
            if sum > best then
                best = sum
                bestQuadrant = side
            end
        end
        
        if best==0 then
            if front then
                bestQuadrant = "front"
            else
                bestQuadrant = "back"
            end
        end
        
        if (front and bestQuadrant == "front") or (not front and bestQuadrant == "back") then
            robot.wheels.set_velocity(10,10)
        elseif (front and (bestQuadrant == "back" or bestQuadrant == "left")) or (not front and (bestQuadrant == "front" or bestQuadrant == "right")) then
            robot.wheels.set_velocity(0,10)
        elseif (front and bestQuadrant == "right") or (not front and bestQuadrant == "left") then
            robot.wheels.set_velocity(10,0)
        end
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

function getWord()
    return "_"
end

-- init/reset/destroy
function init()
   current_state = APPROACH
   robot_close = false
    avoiding = false
	avoiding_turns = 0
   robot.range_and_bearing.set_data(1,255)
   robot.wheels.set_velocity(0,0)
end
function reset()
	current_state = APPROACH
    robot_close = false
    avoiding = false
	avoiding_turns = 0
    robot.range_and_bearing.set_data(1,255)
    robot.wheels.set_velocity(0,0)
end
function destroy()
end
