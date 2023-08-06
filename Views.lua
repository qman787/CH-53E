ViewSettings = {
	Cockpit = {
	[1] = {-- player slot 1 (pilot)
		CameraViewAngleLimits  = {20,140},-- FOV Min Max (how much zoom in/out)
		CockpitLocalPoint      = {0.0,0.01,0.0},	--where to line up center of cockpit model  {forward/back,up/down,left/right}
		CameraAngleRestriction = {false,90,0.50}, 
		CameraAngleLimits      = {200,-90,90}, -- View right/left, View down/up
		EyePoint               = {0.1,0.10,0.0}, -- Distance from pivot point. To simulate pivot around human neck, use {0.16,0.1,0}
		ShoulderSize		   = 0.25,
		Allow360rotation	   = false,
		limits_6DOF            = {x = {-0.50,10},y ={-10,10},z = {-10,10},roll = 90.000000},-- movement back/front bottom/top right/left
	},
		[2] = {-- player slot 1 (pilot)
		CameraViewAngleLimits  = {20,140},-- FOV Min Max (how much zoom in/out)
		CockpitLocalPoint      = {4.6,2.4,-0.55},	--where to line up view {forward/back,up/down,left/right}
		CameraAngleRestriction = {false,90,0.50}, 
		CameraAngleLimits      = {200,-90,90}, -- View right/left, View down/up
		EyePoint               = {0.1,0.10,0.0}, -- Distance from pivot point. To simulate pivot around human neck, use {0.16,0.1,0}
		ShoulderSize		   = 0.25,
		Allow360rotation	   = false,
		limits_6DOF            = {x = {-0.50,10},y ={-10,10},z = {-10,10},roll = 90.000000},-- movement back/front bottom/top right/left
	},
		[3] = {-- player slot 3 GunnerRight
		CameraViewAngleLimits  = {20,140},-- FOV Min Max (how much zoom in/out)
		CockpitLocalPoint      = {3.0,2.0,0.50},	--where to line up view  {forward/back,up/down,left/right}
		CockpitLocalPointAzimuth  = -90,
		CameraAngleRestriction = {false,90,0.50}, 
		CameraAngleLimits      = {200,-90,90}, -- View right/left, View down/up
		EyePoint               = {0.1,0.10,0.0}, -- Distance from pivot point. To simulate pivot around human neck, use {0.16,0.1,0}
		ShoulderSize		   = 0.25,
		Allow360rotation	   = false,
		limits_6DOF            = {x = {-0.50,10},y ={-10,10},z = {-10,10},roll = 90.000000},-- movement back/front bottom/top right/left
	},
		[4] = {-- player slot 4 Gunnerleft
		CameraViewAngleLimits  = {20,140},-- FOV Min Max (how much zoom in/out)
		CockpitLocalPoint      = {3.0,2.0,-0.50},	--where to line up view {forward/back,up/down,left/right}
		CockpitLocalPointAzimuth  = 90,
		CameraAngleRestriction = {false,90,0.50}, 
		CameraAngleLimits      = {200,-90,90}, -- View right/left, View down/up
		EyePoint               = {0.1,0.10,0.0}, -- Distance from pivot point. To simulate pivot around human neck, use {0.16,0.1,0}
		ShoulderSize		   = 0.25,
		Allow360rotation	   = false,
		limits_6DOF            = {x = {-0.50,10},y ={-10,10},z = {-10,10},roll = 90.000000},-- movement back/front bottom/top right/left
	},
		[5] = {-- player slot 5 Ramp Gunner
		CameraViewAngleLimits  = {20,140},-- FOV Min Max (how much zoom in/out)
		CockpitLocalPoint      = {-4.0,2.0,0.0},	--where to line up view {forward/back,up/down,left/right}
		CockpitLocalPointAzimuth  = 180,
		CameraAngleRestriction = {false,90,0.50}, 
		CameraAngleLimits      = {200,-90,90}, -- View right/left, View down/up
		EyePoint               = {0.1,0.10,0.0}, -- Distance from pivot point. To simulate pivot around human neck, use {0.16,0.1,0}
		ShoulderSize		   = 0.25,
		Allow360rotation	   = false,
		limits_6DOF            = {x = {-0.50,10},y ={-10,10},z = {-10,10},roll = 90.000000},-- movement back/front bottom/top right/left
	},
	}, 
	
	Chase = { -- F4 view
		LocalPoint      = {-0.4,-1.0,2},
		AnglesDefault   = {0.000000,0.000000},
	},  
	Arcade = {
		LocalPoint      = {-21.500000,5.000,0.000000},
		AnglesDefault   = {0.000000,-8.000000},
	}, 
}

local function head_pos_default(tab) -- this used for placing pilot head 
    if not tab then
	   tab = {}
	end

	tab.viewAngle = tab.viewAngle  or 75 -- FOV
	tab.hAngle    = tab.hAngle     or  0
	tab.vAngle    = tab.vAngle     or -8 -- angles down pilot view
	tab.x_trans   = tab.x_trans    or  4.9 --0.725 --moves pilot view forward .725
	tab.y_trans   = tab.y_trans    or  2.4 -- moves view up
	tab.z_trans   = tab.z_trans    or  0.55	-- 0.35 moves view to right seat
	tab.rollAngle = tab.rollAngle  or  0
	return tab
end



SnapViews = {
[1] = {-- player slot 1 (pilot)
		[1]  = head_pos_default({
					viewAngle = 70,
					vAngle	 = 0,
					x_trans	 = 0.0,
					y_trans	 = 0.0,
					z_trans	 = 0.0,
		}),--Num 0
		[2]  = head_pos_default({}),--LWin + Num1 : Snap View 1 - 
		[3]  = head_pos_default({}),--LWin + Num2 : Snap View 2 - 
		[4]  = head_pos_default({}),--LWin + Num3 : Snap View 3 - 
		[5]  = head_pos_default({}),--LWin + Num4 : Snap View 4 - 
		[6]  = head_pos_default({}),--LWin + Num5 : Snap View 5 - Default view
		[7]  = head_pos_default({}),--LWin + Num6 : Snap View 6 - 
		[8]  = head_pos_default({}),--LWin + Num7 : Snap View 7 - 
		[9]  = head_pos_default({}),--LWin + Num8 : Snap View 8 -
		[10] = head_pos_default({}),--Num 9
		[11] = head_pos_default({}),--look at left  mirror
		[12] = head_pos_default({}),--look at right mirror
		[13] = head_pos_default({})--default view
},

}
