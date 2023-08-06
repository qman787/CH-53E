dofile(LockOn_Options.common_script_path.."elements_defs.lua")

SetScale(METERS) 

local font7segment = MakeFont({used_DXUnicodeFontData = "font7segment"},{0,255,0,215}) --(R,G,B,opacity)
local font7segment_yellow = MakeFont({used_DXUnicodeFontData = "font7segment"},{255,255,0,215}) --(R,G,B,opacity)
local font7segment_red = MakeFont({used_DXUnicodeFontData = "font7segment"},{244,0,0,215}) --(R,G,B,opacity)
local font7segment_purple = MakeFont({used_DXUnicodeFontData = "font7segment"},{255,0,255,215}) --(R,G,B,opacity)
local font7segment_blue = MakeFont({used_DXUnicodeFontData = "font7segment"},{0,127,200,215}) --(R,G,B,opacity)
local font7segment_orange = MakeFont({used_DXUnicodeFontData = "font7segment"},{255, 165, 0,215}) --(R,G,B,opacity)

--local font7segment = MakeFont({used_DXUnicodeFontData = "font_verdana_7"},{0,127,200,215}) --(R,G,B,opacity)
local center = {0,0,0}  --- {L/R,U/D,forward/back}

verts = {}
dx=.0125
dy=.0075
verts [1]= {-dx,-dy}
verts [2]= {-dx,dy}
verts [3]= {dx,dy}
verts [4]= {dx,-dy}
	
local base 			 = CreateElement "ceMeshPoly"
base.name 			 = "base"
base.vertices 		 = verts
base.indices 		 = {0,1,2,2,3,0}
base.init_pos		 = center   
base.material		 = MakeMaterial(nil,{3,3,30,255})
base.h_clip_relation = h_clip_relations.REWRITE_LEVEL 
base.level			 = 5
base.isdraw			 = true
base.change_opacity  = false
base.isvisible		 = false
base.element_params  = {"DC_POWER_AVAIL"}  
base.controllers     = {{"parameter_in_range",0,0.9,1.1}} 
Add(base)

local TurbineOutletTemp           = CreateElement "ceStringPoly"
TurbineOutletTemp.name            = create_guid_string()
TurbineOutletTemp.material        = font7segment	
TurbineOutletTemp.alignment       = "CenterCenter"
TurbineOutletTemp.init_pos		  = {-0.012, 0, 0}
TurbineOutletTemp.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
TurbineOutletTemp.formats         = {"%.0f"} 
TurbineOutletTemp.element_params  = {"TOT"}
TurbineOutletTemp.controllers     = {{"text_using_parameter",0,0}}
TurbineOutletTemp.h_clip_relation = h_clip_relations.compare
TurbineOutletTemp.level			  = 6
TurbineOutletTemp.parent_element  = base.name
Add(TurbineOutletTemp)

local Torque           = CreateElement "ceStringPoly"
Torque.name            = create_guid_string()
Torque.material        = font7segment	
Torque.alignment       = "CenterCenter"
Torque.init_pos		   = {0.012, 0, 0}
Torque.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Torque.formats         = {"%.0f"} 
Torque.element_params  = {"TRQ"}
Torque.controllers     = {{"text_using_parameter",0,0}}
Torque.h_clip_relation = h_clip_relations.compare
Torque.level		   = 6
Torque.parent_element  = base.name
Add(Torque)  -- not finished yet

local Debug1           = CreateElement "ceStringPoly"
Debug1.name            = create_guid_string()
Debug1.material        = font7segment_red	
Debug1.alignment       = "CenterCenter"
Debug1.init_pos		   = {0.000, 0.112, 0}
Debug1.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Debug1.formats         = {"%.2f"} 
Debug1.element_params  = {"TEST_PARAM_THRUST_REQUIRED"}
Debug1.controllers     = {{"text_using_parameter",0,0}}
Debug1.h_clip_relation = h_clip_relations.compare
Debug1.level		   = 6
Debug1.parent_element  = base.name
Add(Debug1)  -- not finished yet

local Debug2           = CreateElement "ceStringPoly"
Debug2.name            = create_guid_string()
Debug2.material        = font7segment_orange	
Debug2.alignment       = "CenterCenter"
Debug2.init_pos		   = {0.036, 0.024, 0}
Debug2.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Debug2.formats         = {"%.2f"} 
Debug2.element_params  = {"TEST_PARAM_2"}
Debug2.controllers     = {{"text_using_parameter",0,0}}
Debug2.h_clip_relation = h_clip_relations.compare
Debug2.level		   = 6
Debug2.parent_element  = base.name
Add(Debug2)  -- not finished yet

local Debug3           = CreateElement "ceStringPoly"
Debug3.name            = create_guid_string()
Debug3.material        = font7segment_blue	
Debug3.alignment       = "CenterCenter"
Debug3.init_pos		   = {0.000, 0.132, 0}
Debug3.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Debug3.formats         = {"%.2f"} 
Debug3.element_params  = {"TEST_PARAM_THRUST_PRODUCED"}
Debug3.controllers     = {{"text_using_parameter",0,0}}
Debug3.h_clip_relation = h_clip_relations.compare
Debug3.level		   = 6
Debug3.parent_element  = base.name
Add(Debug3)  -- not finished yet

local Debug4           = CreateElement "ceStringPoly"
Debug4.name            = create_guid_string()
Debug4.material        = font7segment_yellow	
Debug4.alignment       = "CenterCenter"
Debug4.init_pos		   = {-0.048, 0.132, 0}
Debug4.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Debug4.formats         = {"%.2f"} 
Debug4.element_params  = {"TEST_PARAM_THRUST_AVAILABLE"}
Debug4.controllers     = {{"text_using_parameter",0,0}}
Debug4.h_clip_relation = h_clip_relations.compare
Debug4.level		   = 6
Debug4.parent_element  = base.name
Add(Debug4)  -- not finished yet

local Debug5           = CreateElement "ceStringPoly"
Debug5.name            = create_guid_string()
Debug5.material        = font7segment_yellow	
Debug5.alignment       = "CenterCenter"
Debug5.init_pos		   = {-0.060, 0.024, 0}
Debug5.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Debug5.formats         = {"%.2f"} 
Debug5.element_params  = {"TEST_PARAM_AUTOPILOT_INC"}
Debug5.controllers     = {{"text_using_parameter",0,0}}
Debug5.h_clip_relation = h_clip_relations.compare
Debug5.level		   = 6
Debug5.parent_element  = base.name
Add(Debug5)  -- not finished yet

local Debug6           = CreateElement "ceStringPoly"
Debug6.name            = create_guid_string()
Debug6.material        = font7segment_yellow	
Debug6.alignment       = "CenterCenter"
Debug6.init_pos		   = {-0.024, 0.024, 0}
Debug6.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Debug6.formats         = {"%.2f"} 
Debug6.element_params  = {"TEST_PARAM_AUTOPILOT_YAW"}
Debug6.controllers     = {{"text_using_parameter",0,0}}
Debug6.h_clip_relation = h_clip_relations.compare
Debug6.level		   = 6
Debug6.parent_element  = base.name
Add(Debug6)  -- not finished yet

local DebugPitch           = CreateElement "ceStringPoly"
DebugPitch.name            = create_guid_string()
DebugPitch.material        = font7segment	
DebugPitch.alignment       = "CenterCenter"
DebugPitch.init_pos		   = {-0.036, 0.036, 0}
DebugPitch.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugPitch.formats         = {"%.0f"} 
DebugPitch.element_params  = {"TEST_PARAM_BODY_PITCH"}
DebugPitch.controllers     = {{"text_using_parameter",0,0}}
DebugPitch.h_clip_relation = h_clip_relations.compare
DebugPitch.level		   = 6
DebugPitch.parent_element  = base.name
Add(DebugPitch)  -- not finished yet

local DebugRoll           = CreateElement "ceStringPoly"
DebugRoll.name            = create_guid_string()
DebugRoll.material        = font7segment	
DebugRoll.alignment       = "CenterCenter"
DebugRoll.init_pos		   = {-0.012, 0.036, 0}
DebugRoll.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugRoll.formats         = {"%.0f"} 
DebugRoll.element_params  = {"TEST_PARAM_BODY_ROLL"}
DebugRoll.controllers     = {{"text_using_parameter",0,0}}
DebugRoll.h_clip_relation = h_clip_relations.compare
DebugRoll.level		   = 6
DebugRoll.parent_element  = base.name
Add(DebugRoll)  -- not finished yet

local DebugYaw           = CreateElement "ceStringPoly"
DebugYaw.name            = create_guid_string()
DebugYaw.material        = font7segment	
DebugYaw.alignment       = "CenterCenter"
DebugYaw.init_pos		   = {0.012, 0.036, 0}
DebugYaw.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugYaw.formats         = {"%.0f"} 
DebugYaw.element_params  = {"TEST_PARAM_BODY_YAW"}
DebugYaw.controllers     = {{"text_using_parameter",0,0}}
DebugYaw.h_clip_relation = h_clip_relations.compare
DebugYaw.level		   = 6
DebugYaw.parent_element  = base.name
Add(DebugYaw)  -- not finished yet

local DebugPidTime           = CreateElement "ceStringPoly"
DebugPidTime.name            = create_guid_string()
DebugPidTime.material        = font7segment	
DebugPidTime.alignment       = "CenterCenter"
DebugPidTime.init_pos		   = {-0.089, 0.276, 0}
DebugPidTime.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugPidTime.formats         = {"%.2f"} 
DebugPidTime.element_params  = {"TEST_PARAM_PID_TIME"}
DebugPidTime.controllers     = {{"text_using_parameter",0,0}}
DebugPidTime.h_clip_relation = h_clip_relations.compare
DebugPidTime.level		   = 6
DebugPidTime.parent_element  = base.name
Add(DebugPidTime)  -- not finished yet

local DebugPidP           = CreateElement "ceStringPoly"
DebugPidP.name            = create_guid_string()
DebugPidP.material        = font7segment	
DebugPidP.alignment       = "CenterCenter"
DebugPidP.init_pos		   = {-0.060, 0.276, 0}
DebugPidP.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugPidP.formats         = {"%.0f"} 
DebugPidP.element_params  = {"TEST_PARAM_PID_P"}
DebugPidP.controllers     = {{"text_using_parameter",0,0}}
DebugPidP.h_clip_relation = h_clip_relations.compare
DebugPidP.level		   = 6
DebugPidP.parent_element  = base.name
Add(DebugPidP)  -- not finished yet

local DebugPidI           = CreateElement "ceStringPoly"
DebugPidI.name            = create_guid_string()
DebugPidI.material        = font7segment	
DebugPidI.alignment       = "CenterCenter"
DebugPidI.init_pos		   = {-0.024, 0.276, 0}
DebugPidI.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugPidI.formats         = {"%.0f"} 
DebugPidI.element_params  = {"TEST_PARAM_PID_I"}
DebugPidI.controllers     = {{"text_using_parameter",0,0}}
DebugPidI.h_clip_relation = h_clip_relations.compare
DebugPidI.level		   = 6
DebugPidI.parent_element  = base.name
Add(DebugPidI)  -- not finished yet


local DebugPidD           = CreateElement "ceStringPoly"
DebugPidD.name            = create_guid_string()
DebugPidD.material        = font7segment	
DebugPidD.alignment       = "CenterCenter"
DebugPidD.init_pos		   = {0.012, 0.276, 0}
DebugPidD.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugPidD.formats         = {"%.0f"} 
DebugPidD.element_params  = {"TEST_PARAM_PID_D"}
DebugPidD.controllers     = {{"text_using_parameter",0,0}}
DebugPidD.h_clip_relation = h_clip_relations.compare
DebugPidD.level		   = 6
DebugPidD.parent_element  = base.name
Add(DebugPidD)  -- not finished yet

local DebugCx           = CreateElement "ceStringPoly"
DebugCx.name            = create_guid_string()
DebugCx.material        = font7segment_yellow	
DebugCx.alignment       = "CenterCenter"
DebugCx.init_pos		   = {-0.060, 0.264, 0}
DebugCx.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugCx.formats         = {"%.3f"} 
DebugCx.element_params  = {"TEST_PARAM_CX"}
DebugCx.controllers     = {{"text_using_parameter",0,0}}
DebugCx.h_clip_relation = h_clip_relations.compare
DebugCx.level		   = 6
DebugCx.parent_element  = base.name
Add(DebugCx)  -- not finished yet

local DebugCy           = CreateElement "ceStringPoly"
DebugCy.name            = create_guid_string()
DebugCy.material        = font7segment_yellow	
DebugCy.alignment       = "CenterCenter"
DebugCy.init_pos		   = {-0.024, 0.264, 0}
DebugCy.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugCy.formats         = {"%.3f"} 
DebugCy.element_params  = {"TEST_PARAM_CY"}
DebugCy.controllers     = {{"text_using_parameter",0,0}}
DebugCy.h_clip_relation = h_clip_relations.compare
DebugCy.level		   = 6
DebugCy.parent_element  = base.name
Add(DebugCy)  -- not finished yet


local DebugCz           = CreateElement "ceStringPoly"
DebugCz.name            = create_guid_string()
DebugCz.material        = font7segment_yellow	
DebugCz.alignment       = "CenterCenter"
DebugCz.init_pos		   = {0.012, 0.264, 0}
DebugCz.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugCz.formats         = {"%.3f"} 
DebugCz.element_params  = {"TEST_PARAM_CZ"}
DebugCz.controllers     = {{"text_using_parameter",0,0}}
DebugCz.h_clip_relation = h_clip_relations.compare
DebugCz.level		   = 6
DebugCz.parent_element  = base.name
Add(DebugCz)  -- not finished yet

local DebugCl           = CreateElement "ceStringPoly"
DebugCl.name            = create_guid_string()
DebugCl.material        = font7segment_purple	
DebugCl.alignment       = "CenterCenter"
DebugCl.init_pos		   = {-0.060, 0.252, 0}
DebugCl.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugCl.formats         = {"%.3f"} 
DebugCl.element_params  = {"TEST_PARAM_CL"}
DebugCl.controllers     = {{"text_using_parameter",0,0}}
DebugCl.h_clip_relation = h_clip_relations.compare
DebugCl.level		   = 6
DebugCl.parent_element  = base.name
Add(DebugCl)  -- not finished yet

local DebugCm           = CreateElement "ceStringPoly"
DebugCm.name            = create_guid_string()
DebugCm.material        = font7segment_purple
DebugCm.alignment       = "CenterCenter"
DebugCm.init_pos		   = {-0.024, 0.252, 0}
DebugCm.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugCm.formats         = {"%.3f"} 
DebugCm.element_params  = {"TEST_PARAM_CM"}
DebugCm.controllers     = {{"text_using_parameter",0,0}}
DebugCm.h_clip_relation = h_clip_relations.compare
DebugCm.level		   = 6
DebugCm.parent_element  = base.name
Add(DebugCm)  -- not finished yet


local DebugCn           = CreateElement "ceStringPoly"
DebugCn.name            = create_guid_string()
DebugCn.material        = font7segment_purple
DebugCn.alignment       = "CenterCenter"
DebugCn.init_pos		   = {0.012, 0.252, 0}
DebugCn.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
DebugCn.formats         = {"%.3f"} 
DebugCn.element_params  = {"TEST_PARAM_CN"}
DebugCn.controllers     = {{"text_using_parameter",0,0}}
DebugCn.h_clip_relation = h_clip_relations.compare
DebugCn.level		   = 6
DebugCn.parent_element  = base.name
Add(DebugCn)  -- not finished yet

local Autopilot1           = CreateElement "ceStringPoly"
Autopilot1.name            = create_guid_string()
Autopilot1.material        = font7segment_orange	
Autopilot1.alignment       = "CenterCenter"
Autopilot1.init_pos		   = {0.080, 0.276, 0}
Autopilot1.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Autopilot1.formats         = {"%.2f"} 
Autopilot1.element_params  = {"TEST_PARAM_AP_1"}
Autopilot1.controllers     = {{"text_using_parameter",0,0}}
Autopilot1.h_clip_relation = h_clip_relations.compare
Autopilot1.level		   = 6
Autopilot1.parent_element  = base.name
Add(Autopilot1)  -- not finished yet

local Autopilot2           = CreateElement "ceStringPoly"
Autopilot2.name            = create_guid_string()
Autopilot2.material        = font7segment_orange	
Autopilot2.alignment       = "CenterCenter"
Autopilot2.init_pos		   = {0.104, 0.276, 0}
Autopilot2.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Autopilot2.formats         = {"%.2f"} 
Autopilot2.element_params  = {"TEST_PARAM_AP_2"}
Autopilot2.controllers     = {{"text_using_parameter",0,0}}
Autopilot2.h_clip_relation = h_clip_relations.compare
Autopilot2.level		   = 6
Autopilot2.parent_element  = base.name
Add(Autopilot2)  -- not finished yet

local Autopilot3           = CreateElement "ceStringPoly"
Autopilot3.name            = create_guid_string()
Autopilot3.material        = font7segment_orange	
Autopilot3.alignment       = "CenterCenter"
Autopilot3.init_pos		   = {0.138, 0.276, 0}
Autopilot3.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Autopilot3.formats         = {"%.2f"} 
Autopilot3.element_params  = {"TEST_PARAM_AP_3"}
Autopilot3.controllers     = {{"text_using_parameter",0,0}}
Autopilot3.h_clip_relation = h_clip_relations.compare
Autopilot3.level		   = 6
Autopilot3.parent_element  = base.name
Add(Autopilot3)  -- not finished yet

local Autopilot4           = CreateElement "ceStringPoly"
Autopilot4.name            = create_guid_string()
Autopilot4.material        = font7segment_orange	
Autopilot4.alignment       = "CenterCenter"
Autopilot4.init_pos		   = {0.138, 0.288, 0}
Autopilot4.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Autopilot4.formats         = {"%.0f"} 
Autopilot4.element_params  = {"TEST_PARAM_AP_4"}
Autopilot4.controllers     = {{"text_using_parameter",0,0}}
Autopilot4.h_clip_relation = h_clip_relations.compare
Autopilot4.level		   = 6
Autopilot4.parent_element  = base.name
Add(Autopilot4)  -- not finished yet

local Autopilot5           = CreateElement "ceStringPoly"
Autopilot5.name            = create_guid_string()
Autopilot5.material        = font7segment_orange	
Autopilot5.alignment       = "CenterCenter"
Autopilot5.init_pos		   = {0.104, 0.288, 0}
Autopilot5.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Autopilot5.formats         = {"%.0f"} 
Autopilot5.element_params  = {"TEST_PARAM_AP_5"}
Autopilot5.controllers     = {{"text_using_parameter",0,0}}
Autopilot5.h_clip_relation = h_clip_relations.compare
Autopilot5.level		   = 6
Autopilot5.parent_element  = base.name
Add(Autopilot5)  -- not finished yet

local Autopilot6           = CreateElement "ceStringPoly"
Autopilot6.name            = create_guid_string()
Autopilot6.material        = font7segment_orange	
Autopilot6.alignment       = "CenterCenter"
Autopilot6.init_pos		   = {0.080, 0.288, 0}
Autopilot6.stringdefs      = {0.007,0.75*0.007, 0, 0}  -- {size vertical, horizontal, 0, 0}
Autopilot6.formats         = {"%.0f"} 
Autopilot6.element_params  = {"TEST_PARAM_AP_6"}
Autopilot6.controllers     = {{"text_using_parameter",0,0}}
Autopilot6.h_clip_relation = h_clip_relations.compare
Autopilot6.level		   = 6
Autopilot6.parent_element  = base.name
Add(Autopilot6)  -- not finished yet




local Xsize = 0.002
local Ysize = Xsize*0.52
function addSegment(element)
	element.vertices	   	= {{-Xsize , Ysize}, 
							   { Xsize , Ysize},
							   { Xsize ,-Ysize},
							   {-Xsize ,-Ysize}}
	element.indices	   		= {0,1,2,2,3,0}
	element.material    	= MakeMaterial(nil,{0,255,0,215})
	element.h_clip_relation = h_clip_relations.REWRITE_LEVEL
	element.level 			= 6
	element.parent_element 	= base.name
	Add(element)
end

local numSegments = 26 
for i = 0,numSegments do
	local TOTbar		   = CreateElement "ceMeshPoly"
	TOTbar.name		 	   = "segment_"..i
	TOTbar.init_pos	 	   = { -0.012, -0.085 + i*0.0029, 0}
	TOTbar.controllers 	   = {{"parameter_in_range",0,i*25+300,1200}}
	TOTbar.element_params  = {"TOT"}	
	addSegment(TOTbar)
end

local numSegments = 26 
for i = 0,numSegments do
	local TRQbar		   = CreateElement "ceMeshPoly"
	TRQbar.name		 	   = "segment_"..i
	TRQbar.init_pos	 	   = { 0.012, -0.085 + i*0.0029, 0}
	TRQbar.controllers 	   = {{"parameter_in_range",0,i*1.85+20,85}}
	TRQbar.element_params  = {"TRQ"}	
	addSegment(TRQbar)
end