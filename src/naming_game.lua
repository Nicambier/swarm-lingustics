-- Put your global variables here



--[[ This function is executed every time you press the 'execute'
     button ]]
function init()
	ns = 10
   nt = robot.random.uniform_int(0,ns+1)
	inventory = {}
	robot.wheels.set_velocity(5,5)
	robot.range_and_bearing.set_data({0,0,0,0,0,0,0,0,0,0})
end



--[[ This function is executed at each time step
     It must contain the logic of your controller ]]
function step()
	nt = nt + 1
	walk()
	local received = table.shuffle(robot.range_and_bearing)
	for i = 1,#received do
		local w = received[i].data
		if w[i] ~= 0 then
			update_inventory(table_to_word(w))
		end
	end

	if nt % ns == 0 then	
		local selectedW
		if #inventory==0 then
			selectedW = create_word()
		else
			selectedW = inventory[robot.random.uniform_int(1,#inventory+1)]
		end	
		robot.range_and_bearing.set_data(word_to_table(selectedW))
	else
		robot.range_and_bearing.set_data({0,0,0,0,0,0,0,0,0,0})
	end
end

function update_inventory(w)
	local inside = false
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
	local p = robot.proximity
	max = 0
	max_id = 0
	for i = 1,6 do
		if p[i].value > max then
			max = p[i].value
			max_id = i
		end
	end
	for i = 19,24 do
		if p[i].value > max then
			max = p[i].value
			max_id = i
		end
	end
	if max > 0 then
		robot.wheels.set_velocity(5*p[max_id].angle + 1,-5*p[max_id].angle + 1)
	else
		robot.wheels.set_velocity(5,5)
	end
end

function getWord()
    if #inventory==1 then
        return inventory[1]
    else
        return "_"
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
