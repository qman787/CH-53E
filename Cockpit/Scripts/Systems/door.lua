local dev = GetSelf() 							-- naming the device after the file name... ie "door"

dofile(LockOn_Options.script_path.."command_defs.lua") -- making sure DCS has the commands loaded

local update_time_step = 0.01 			-- this means the device will update avery 1/100th of a second -ish
make_default_activity(update_time_step)  

elec_dc_ok  = get_param_handle("DC_POWER_AVAIL") -- this is needed if you need/want to ensure electric system working

local opendoor = 0			-- defining for later
local door = 0				-- defining for later

dev:listen_command(Keys.door)			-- here is the key we defined in the command_defs.lua
dev:listen_command(device_commands.doorsw)  -- -- here is the device command we defined in the command_defs.lua

function post_initialize()			-- not needed in this script

end	

function SetCommand(command,value)
			
			if command == device_commands.doorsw then -- when device_doorsw has been called (by mouse click or a keyboard)
				door = value							--door value is now either 0 or 1
			elseif command == Keys.door then 	--listen for the key "door" defined in the "inputs folder" in keyboard default.lua
			dev:performClickableAction(device_commands.doorsw, (1-door),true) -- makes the switch inside the cockpit move
end
end

function anim_door()					--this is a simple animation that increase the number "opendoor" by 0.005 every 1/100th of a second
		if  door == 1 then 				
			opendoor =  opendoor + 0.005
		
		if opendoor >= 1 then			-- once the door is fully open (ie the arg is 1) then stop moving
				opendoor = 1
		
			end
		
		elseif door == 0 then
				opendoor = opendoor - 0.005  -- same animation decreases the number "opendoor" by 0.005 every 1/100th of a second
			if opendoor <= 0 then   -- once the door is fully closed (ie the arg is 0) then stop moving
			opendoor = 0
		end
		end
		end

function update()

if elec_dc_ok:get()==1 then -- check that the elctric is on before this animation will work
anim_door()
set_aircraft_draw_argument_value(38, opendoor) -- the value "opendoor" is used to tell the arg 38 how far to move		 
end
end

need_to_be_closed = false -- close lua 
