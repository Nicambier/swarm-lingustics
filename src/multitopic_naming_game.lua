-- Put your global variables here



--[[ This function is executed every time you press the 'execute'
     button ]]
function init()
	ns = 10
   nt = robot.random.uniform_int(0,ns+1)
   nm = 100
   turning = 0
   inventory = {}
   inventory["blue"] = {}
   inventory["red"] = {}
	robot.wheels.set_velocity(5,5)
	robot.range_and_bearing.set_data({0,0,0,0,0,0,0,0,0,0})
    robot.colored_blob_omnidirectional_camera.enable()
    topic = ""
end



--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
	walk()
        
        if #robot.colored_blob_omnidirectional_camera > 0 then
            if robot.colored_blob_omnidirectional_camera[1].color.red > 0 then
                topic = "red"
            else
                topic = "blue"
            end
        end
            
        if #topic > 0 then
            play()
        end
end

function play()
        local received = table.shuffle(robot.range_and_bearing)
	for i = 1,#received do
		local w = received[i].data
		if w[i] ~= 0 then
			update_inventory(table_to_word(w))
		end
	end

	if nt % ns == 0 then	
		local selectedW
		if #inventory[topic]==0 then
			selectedW = create_word()
		else
			selectedW = inventory[topic][robot.random.uniform_int(1,#inventory+1)]
		end	
		robot.range_and_bearing.set_data(word_to_table(selectedW))
	else
		robot.range_and_bearing.set_data({0,0,0,0,0,0,0,0,0,0})
	end

	if #inventory[topic]==1 then
		log(topic.." = "..inventory[topic][1].." (red: "..#inventory["red"]..", blue: "..#inventory["blue"]..")")
	else
		log(topic.." = ".."no convergence")
	end
end

function update_inventory(w)
	local inside = false
	for i = 1,#inventory[topic] do
		if inventory[topic][i]==w then
			inside = true
		end
	end

	if inside then
		inventory[topic] = {w}
	else
		inventory[topic][#inventory+1] = w
	end	
end

function create_word()
	local length = robot.random.uniform_int(1,11)
	local w = ""
	for i = 1,length do
		w = w..string.char(robot.random.uniform_int(97,123))
	end

	return w
end

function word_to_table(w)
	local t = {0,0,0,0,0,0,0,0,0,0}
	for i = 1,#w do
		t[i] = string.byte(w:sub(i,i))
	end

	return t
end

function table_to_word(t)
	local w = ""
	for i = 1,#t do
		w = w .. string.char(t[i])
	end

	return w
end

function walk()
	if(turning>0) then
   	turning = turning-1
		if(turning==0) then
			nt = nt + 1
		end
   elseif(nt%nm==0) then
      robot.wheels.set_velocity(-10,10)
      turning = robot.random.uniform_int(0,40)
   else
      nt = nt + 1
      robot.wheels.set_velocity(5,5)
   end
end

function table.copy(t)
   local t2 = {}
   	for key,value in pairs(t) do
      	t2[key] = value
   	end
   return t2
end

function table.shuffle(t)
	local t2 = table.copy(t)
   local j
   for i = #t2, 2, -1 do
       j = robot.random.uniform_int(1,i)
       t2[i], t2[j] = t2[j], t2[i]
   end
	return t2
end

--[[ This function is executed every time you press the 'reset'
     button in the GUI. It is supposed to restore the state
     of the controller to whatever it was right after init() was
     called. The state of sensors and actuators is reset
     automatically by ARGoS. ]]
function reset()
   init()
end



--[[ This function is executed only once, when the robot is removed
     from the simulation ]]
function destroy()
   -- put your code here
end
