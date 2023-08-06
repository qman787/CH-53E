dofile(LockOn_Options.script_path.."devices.lua")
dofile(LockOn_Options.common_script_path.."tools.lua")
	
MainPanel = {"ccMainPanel",LockOn_Options.script_path.."mainpanel_init.lua"}
	
creators = {}
creators[devices.ELECTRIC_SYSTEM]	= {"avSimpleElectricSystem"} -- needed for simpleRWR to work
creators[devices.WEAPON_SYSTEM]		= {"avSimpleWeaponSystem"	,LockOn_Options.script_path.."Systems/weapon_system.lua"} --{"avSimpleWeaponSystem"	}
creators[devices.RWR]	 			= {"avSimpleRWR"			,LockOn_Options.script_path.."RWR/device/RWR_device.lua"}	
creators[devices.EXTLIGHTS]			= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/ExternalLights.lua"}
creators[devices.AVIONICS]    		= {"avLuaDevice"            ,LockOn_Options.script_path.."Systems/Avionics.lua"}
creators[devices.DIGITAL_CLOCK]    	= {"avLuaDevice"            ,LockOn_Options.script_path.."M880A_digitalClock/M880A_device.lua"}
creators[devices.EFM_HELPER]    	= {"avLuaDevice"            ,LockOn_Options.script_path.."Systems/EFM_Helper.lua"} 
creators[devices.FCS]    			= {"avLuaDevice"            ,LockOn_Options.script_path.."Systems/FCS.lua"} 
creators[devices.INTERCOM]     		= {"avIntercom"             ,LockOn_Options.script_path.."Systems/Intercom.lua", {devices.UHF_RADIO} }
creators[devices.UHF_RADIO]     	= {"avUHF_ARC_164"          ,LockOn_Options.script_path.."Systems/UHF_radio.lua", {devices.INTERCOM, devices.ELECTRIC_SYSTEM} } 
creators[devices.HELMET_DEVICE] 	= {"avNightVisionGoggles"}
creators[devices.OILPRESSURE] 		= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/oilpressure.lua"}
creators[devices.CLOCK]			 	= {"avLuaDevice"			 ,LockOn_Options.script_path.."clock.lua"}
creators[devices.WINCH] 			= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/winch.lua"}
creators[devices.SYSTEM]			= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/init.lua"}
creators[devices.GEAR]				= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/gear.lua"}
creators[devices.WIPERS]			= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/Wipers.lua"}
creators[devices.GUNNERS]			= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/Gunners.lua"}
creators[devices.OPENTAIL]			= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/Opentail.lua"}
creators[devices.REFUELPROBE]		= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/Refuelprobe.lua"}
creators[devices.RAMP]				= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/Ramp.lua"}
creators[devices.ELECWARN]			= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/ElectricWarnings.lua"}
creators[devices.COCKSHAKE]			= {"avLuaDevice"			,LockOn_Options.script_path.."Systems/Cockshake.lua"}




indicators = {}
indicators[#indicators + 1] = {"ccIndicator", LockOn_Options.script_path.."RPM_Display/init.lua", nil,{{"ENG_PNT1_CENTER",nil,nil}, {rz=-14},2},2} 
indicators[#indicators + 1] = {"ccIndicator", LockOn_Options.script_path.."tempTorqDisplay/init.lua", nil,{{"ENG_PNT2_CENTER",nil,nil}, {rz=-14},2},2}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."M880A_digitalClock/init.lua",nil,{{"ENG_PNT3_CENTER",nil,nil}, {rz=-14},2},2}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."RadarAltitude/init.lua",nil, {{"RADALT",nil,nil}, {rz=-14},2},2}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."RWR/Indicator/init.lua",nil,{{"RWR001",nil,nil}, {rz=-14},2},2}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."BearingIndicator/init.lua",nil,{{"BEARING1",nil,nil}, {rz=-14},2},2}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."FuelIndicator/init.lua",nil,{{"FUELDIG",nil,nil}, {rz=-14},2},2}
indicators[#indicators + 1] = {"ccIndicator", LockOn_Options.script_path.."RPM_Display/init.lua", nil,{{"RPMTAQ",nil,nil}, {rz=-14},2},2} 
indicators[#indicators + 1] = {"ccIndicator", LockOn_Options.script_path.."tempTorqDisplay/init.lua", nil,{{"TORQ",nil,nil}, {rz=-14},2},2}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."M880A_digitalClock/init.lua",nil,{{"DigClock",nil,nil}, {rz=-14},2},2}
indicators[#indicators + 1] = {"ccControlsIndicatorBase", LockOn_Options.script_path.."ControlsIndicator/ControlsIndicator.lua", nil}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."BearingIndicator/init.lua",nil,{{"BEARING2",nil,nil}, {rz=-14},2},2}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."BearingIndicator/init.lua",nil,{{"BEARING3",nil,nil}, {rz=-14},2},2}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."FuelIndicator/init.lua",nil,{{"FUELDIG1",nil,nil}, {rz=-14},2},2}
indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."RadarAltitude/init.lua",nil, {{"RADALT.R",nil,nil}, {rz=-14},2},2}

--indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."RWR/Indicator/init.lua",nil}
--indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."BearingIndicator/init.lua",nil}
--indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."FuelIndicator/init.lua",nil}
--indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."RadarAltitude/init.lua",nil}



--[[indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."HMD/init.lua", nil, nil ,--id of parent device
  {{"ENG_PNT5_CENTER",nil,nil}, {nil}}
}
]]
--[[indicators[#indicators + 1] = {"ccIndicator", LockOn_Options.script_path .. "CrewIndicator/crew_indicator_init.lua",--id of parent device
 {{"ENG_PNT6_CENTER",nil,nil}, {nil}}
} 
]]

--[[indicators[#indicators + 1] = {"ccIndicator" ,LockOn_Options.script_path.."EngineDisplay/Indicator/init.lua",nil,--id of parent device
  {{"ENG_PNT8_CENTER",nil,nil}, {nil}}
} 
]]


dofile(LockOn_Options.common_script_path.."KNEEBOARD/declare_kneeboard_device.lua")
