dofile(LockOn_Options.script_path.."command_defs.lua")

local CockShake = GetSelf()
local sensor_data = get_base_data()
local update_time_step = 0.03  
make_default_activity(update_time_step)

local current_hdg = get_param_handle("CURRENT_HDG")
local current_Ralt = get_param_handle("CURRENT_RALT")
local alt_1k = get_param_handle("ALT_1000")
local MySpeed = sensor_data.getTrueAirSpeed() 
local pitch = sensor_data.getPitch()
local roll = sensor_data.getRoll()
local VV = sensor_data.getVerticalVelocity()

local panelshakez = 0
local panelshakey = 0
local panelshakex = 0

PANELSHAKEZ = get_param_handle("PANELSHAKEZ")
PANELSHAKEY = get_param_handle("PANELSHAKEY")
PANELSHAKEX = get_param_handle("PANELSHAKEX")

function SetCommand(command,value)

	end

function update()
update_shake()
	--print_message_to_user("ShakeShake  "..panelshakez)
PANELSHAKEZ:set(panelshakez)
PANELSHAKEX:set(panelshakex)
PANELSHAKEY:set(panelshakey)

end


function update_shake()
local MySpeed = sensor_data.getTrueAirSpeed()
local VV = sensor_data.getVerticalVelocity()

if MySpeed < 10 and VV < -1.5 then 
	panelshakez = panelshakez + 0.35
	panelshakex = panelshakex + 0.1
	panelshakey = panelshakey + 0.45

end
if panelshakez >= 1 then 
panelshakez = -1
end

if panelshakex >= 1 then 
panelshakex = -1
end

if panelshakey >= 1 then 
panelshakey = -1
end
end




need_to_be_closed = false -- close lua state after initialization