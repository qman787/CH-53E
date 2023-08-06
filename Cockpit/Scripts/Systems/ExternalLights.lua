dofile(LockOn_Options.script_path.."command_defs.lua")

local dev = GetSelf()
local update_time_step = 0.01 --update will be called 10 times per second
make_default_activity(update_time_step)

elec_dc_ok  = get_param_handle("DC_POWER_AVAIL")

function post_initialize()

end
--PlaneLightsOnOff

dev:listen_command(Keys.LandingLight)
dev:listen_command(Keys.PositionLights)
dev:listen_command(Keys.PlaneLightsOnOff)
dev:listen_command(Keys.BladetipOnOff)
dev:listen_command(device_commands.LandingLightSw)
dev:listen_command(device_commands.PositionLightSw)
dev:listen_command(device_commands.BladetipSw)
dev:listen_command(device_commands.PlaneLightsOnOffSw)
dev:listen_command(device_commands.Formation)
dev:listen_command(device_commands.Beacon)


local extLDG = 0  		
local Bladetip = 0
local formationBrightness = 0
local PosLts = 0
local PlaneLights = 0 

function SetCommand(command,value)
    if command == device_commands.LandingLightSw then
        extLDG = value
	elseif command == device_commands.PlaneLightsOnOffSw then
        PlaneLights = value
    elseif command == device_commands.PositionLightSw then
		PosLts = value
	elseif command == device_commands.BladetipSw then
		Bladetip = value	
		
	elseif command == Keys.LandingLight then
		dev:performClickableAction(device_commands.LandingLightSw, (1-extLDG),true)
	elseif command == Keys.PositionLights then
		dev:performClickableAction(device_commands.PositionLightSw, (1-PosLts),true)
	elseif command == Keys.PlaneLightsOnOff then
		dev:performClickableAction(device_commands.PlaneLightsOnOffSw, (1-PlaneLights),true)
	elseif command == Keys.BladetipOnOff then
		dev:performClickableAction(device_commands.BladetipSw, (1-Bladetip),true)
	end
end


function update() 
--local lightState = get_aircraft_draw_argument_value(208)    
 --print_message_to_user(":"..lightState)
	if elec_dc_ok:get()==1 then
	--	set_aircraft_draw_argument_value(51,extlight_taxi) -- 51 is animation to move landing lights open, 208 for actual light beam
		set_aircraft_draw_argument_value(26,extLDG) 
		set_aircraft_draw_argument_value(13,PosLts)
		set_aircraft_draw_argument_value(190,Bladetip)
		set_aircraft_draw_argument_value(14,PlaneLights)
	--	set_aircraft_draw_argument_value(88,formationBrightness)
	else		-- no electrical power, turn lights off
	--	set_aircraft_draw_argument_value(51,0) 
		set_aircraft_draw_argument_value(26,0)
		set_aircraft_draw_argument_value(13,0)		
		set_aircraft_draw_argument_value(190,0)
		set_aircraft_draw_argument_value(14,0)
	--	set_aircraft_draw_argument_value(88,0)
	end
end

need_to_be_closed = false 