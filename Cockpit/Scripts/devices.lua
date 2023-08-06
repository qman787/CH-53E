local count = 0
local function counter()
	count = count + 1
	return count
end
-------DEVICE ID-------
devices = {}
devices["ELECTRIC_SYSTEM"]	= counter()
devices["WEAPON_SYSTEM"]	= counter()
devices["RWR"] 				= counter()
devices["EXTLIGHTS"]		= counter()
devices["AVIONICS"]			= counter()
devices["DIGITAL_CLOCK"]	= counter()
devices["EFM_HELPER"]		= counter()
devices["FCS"]				= counter()
devices["INTERCOM"]			= counter() 
devices["UHF_RADIO"]		= counter()
devices["HELMET_DEVICE"] 	= counter() 
devices["OILPRESSURE"]		= counter() 
devices["CLOCK"]			= counter() 
devices["WINCH"]			= counter() 
devices["SYSTEM"]			= counter()
devices["GEAR"]				= counter()  
devices["WIPERS"]			= counter()  
devices["GUNNERS"]			= counter()  
devices["OPENTAIL"]			= counter()  
devices["REFUELPROBE"]		= counter()
devices["RAMP"]				= counter()
devices["ELECWARN"]			= counter()
devices["COCKSHAKE"] 		= counter()
devices["FLIGHT_CONTROLS"] 		= counter()
