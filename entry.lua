local self_ID = "CH_53"
declare_plugin(self_ID,
{
image     	 = "CH_53.bmp",
installed 	 = true, -- if false that will be place holder , or advertising
dirName	  	 = current_mod_path,
creditsFile = "credits.txt", 
fileMenuName = _("CH_53"),
version		 = "DCS 2.7",		 
state		 = "installed",
info		 = _("The Sikorsky CH-53E Super Stallion, designated S-80E by the manufacturer, is a heavy transport helicopter created by the American company Sikorsky Aircraft Corporation for the United States Marine Corps. It is the largest helicopter in service with the United States Armed Forces, and is in fact the largest US-made aircraft of this type to date."),

binaries 	 = 
{'CH53',
},

InputProfiles =
	{
		["CH_53"]     	 = current_mod_path..'/Input/CH_53',
		--["CH_53_copilot"] = current_mod_path..'/Input/CH_53_co',
		--["CH_53_Gunner1"] = current_mod_path..'/Input/CH_53_Gunner1',
		--["CH_53_Gunner2"] = current_mod_path..'/Input/CH_53_Gunner2',
	},

Skins	= 
{
	{
		name	= "CH_53",
		dir		= "Theme"
	},
},

Missions =
	{
		{
			name		= _("CH_53"),
			dir			= "Missions",

		},
	},
LogBook =
	{
		{
			name		= _("CH_53"),
			type		= "CH_53",
		},
	},	

})


mount_vfs_texture_path(current_mod_path ..  "/Theme/ME")
mount_vfs_model_path	(current_mod_path.."/Cockpit/Shape")
mount_vfs_model_path	(current_mod_path.."/Shapes")
mount_vfs_liveries_path (current_mod_path.."/Liveries")
-- mount_vfs_texture_path  (current_mod_path.."/Textures/CH53-Textures.zip")
-- mount_vfs_texture_path  (current_mod_path.."/Textures/Avionics.zip")
mount_vfs_texture_path  (current_mod_path.."/Textures/CH53-Textures")
mount_vfs_texture_path  (current_mod_path.."/Textures/Avionics")
mount_vfs_texture_path  (current_mod_path ..  "/Skins/1/ME")

dofile(current_mod_path.."/STATICS_CH_53.lua")
dofile(current_mod_path..'/Weapons/CH_53_Weapons.lua')


dofile(current_mod_path..'/CH_53.lua')


local cfg_path = current_mod_path .. '/FM/CH_53_config.lua'
dofile(cfg_path)
CH_53[1] = self_ID
CH_53[2] = "CH53"
CH_53.config_path = cfg_path


dofile(current_mod_path.."/Views.lua")
make_view_settings('CH_53', ViewSettings, SnapViews)


make_flyable('CH_53',current_mod_path..'/Cockpit/Scripts/', CH_53, current_mod_path..'/comm.lua')

plugin_done() 
