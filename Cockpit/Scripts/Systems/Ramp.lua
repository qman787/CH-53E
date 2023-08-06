local dev = GetSelf()

dofile(LockOn_Options.script_path.."command_defs.lua")

local update_time_step = 0.01
make_default_activity(update_time_step)

elec_dc_ok  = get_param_handle("DC_POWER_AVAIL")


local openramp = 0
local ramp = 0


dev:listen_command(Keys.Ramp)
dev:listen_command(device_commands.Rampsw)



function post_initialize()

end	

function SetCommand(command,value)
			
			if command == device_commands.Rampsw then
				ramp = value
			elseif command == Keys.Ramp then
					
			dev:performClickableAction(device_commands.Rampsw, (1-ramp),true) -- makes the switch inside cockpit move
end
end


function anim_ramp()
		if  ramp == 1 then 
			openramp =  openramp + 0.005
		
		if openramp >= 1 then
				openramp = 1
		
			end
		

		
		elseif ramp == 0 then
				openramp = openramp - 0.005
			if openramp <= 0 then
			openramp = 0
		end
		end
		end

	



function update()

if elec_dc_ok:get()==1 then
anim_ramp()
set_aircraft_draw_argument_value(86, openramp) --animation to external model		 
end
end
	
		

need_to_be_closed = false -- close lua 
