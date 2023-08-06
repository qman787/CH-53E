

declare_loadout({
	category		 = CAT_PODS,                                                
	CLSID			 = "AMMO_CH53",
	attribute		 =  {wsType_Air,wsType_Free_Fall,wsType_FuelTank,WSTYPE_PLACEHOLDER},
	Picture			 = "Ammunition.png",
	displayName		 = _("AMMO Cargo"),
	Weight_Empty	 = 12,
	Weight			 = 800,
	Cx_pil			 = 0.002,
	shape_table_data = 
	{
		{
			name 	= "AMMO Cargo",
			file	= "ammo_box_cargo";
			life	= 1;                 
			fire	= { 0, 1};
			username	= "AMMO Cargo";
			index	= WSTYPE_PLACEHOLDER;
		},
	},
	Elements	= 
	{
		{
			ShapeName	= "ammo_box_cargo",
			
			
		}, 
	}, 
})


declare_loadout({
	category		 = CAT_PODS,                                                
	CLSID			 = "MARINES_CH53",
	attribute		 =  {wsType_Air,wsType_Free_Fall,wsType_FuelTank,WSTYPE_PLACEHOLDER},
	Picture			 = "PARA TROOP.png",
	displayName		 = _("Marines"),
	Weight_Empty	 = 1,
	Weight			 = 1600,
	Cx_pil			 = 0.002,
	shape_table_data = 
	{
		{
			name 	= "Marines",
			file	= "PARATROOPdummy";
			life	= 1;                 
			fire	= { 0, 1};
			username	= "Marines";
			index	= WSTYPE_PLACEHOLDER;
		},
	},
	Elements	= 
	{
		{
			ShapeName	= "PARATROOPdummy",
		}, 
	}, 
})





declare_loadout({
	category		 = CAT_PODS,                                                
	CLSID			 = "TANK_AC130",
	attribute		 =  {wsType_Air,wsType_Free_Fall,wsType_FuelTank,WSTYPE_PLACEHOLDER},
	Picture			 = "Fuels Cargo.png",
	displayName		 = _("Fuels Cargo"),
	Weight_Empty	 = 12,
	Weight			 = 500,
	Cx_pil			 = 0.002,
	shape_table_data = 
	{
		{
			name 	= "Fuels Cargo",
			file	= "Fuels Cargo";
			life	= 1;                 
			fire	= { 0, 1};
			username	= "Fuels Cargo";
			index	= WSTYPE_PLACEHOLDER;
		},
	},
	Elements	= 
	{
		{
			ShapeName	= "Fuels Cargo",
			
			
		}, 
	}, 
})

declare_loadout({
	category		 = CAT_PODS,                                                
	CLSID			 = "Container_CH53",
	attribute		 =  {wsType_Air,wsType_Free_Fall,wsType_FuelTank,WSTYPE_PLACEHOLDER},
	Picture			 = "Container Cargo.png",
	displayName		 = _("Container Cargo"),
	Weight_Empty	 = 12,
	Weight			 = 12 +  0.0775 * 70,
	Cx_pil			 = 0.002,
	shape_table_data = 
	{
		{
			name 	= "Container Cargo",
			file	= "iso_container_small_cargo.EDM";
			life	= 1;                 
			fire	= { 0, 1};
			username	= "Container Cargo";
			index	= WSTYPE_PLACEHOLDER;
		},
	},
	Elements	= 
	{
		{
			ShapeName	= "iso_container_small_cargo",
			
			
		}, 
	}, 
})


declare_loadout({
	category		 = CAT_PODS,                                                
	CLSID			 = "ab-212_cargo",
	attribute		 =  {wsType_Air,wsType_Free_Fall,wsType_FuelTank,WSTYPE_PLACEHOLDER},
	Picture			 = "Container Cargo.png",
	displayName		 = _("Hook Cargo"),
	Weight_Empty	 = 12,
	Weight			 = 12 +  0.0775 * 70,
	Cx_pil			 = 0.002,
	shape_table_data = 
	{
		{
			name 	= "ab-212_cargo",
			file	= "ab-212_cargo";
			life	= 1;                 
			fire	= { 0, 1};
			username	= "Hook Cargo";
			index	= WSTYPE_PLACEHOLDER;
		},
	},
	Elements	= 
	{
		{
			ShapeName	= "ab-212_cargo",
			
			
		}, 
	}, 
})

declare_loadout({
	category		 = CAT_PODS,                                                
	CLSID			 = "bw_container_cargo",
	attribute		 =  {wsType_Air,wsType_Free_Fall,wsType_FuelTank,WSTYPE_PLACEHOLDER},
	Picture			 = "Container Cargo.png",
	displayName		 = _("Small Container"),
	Weight_Empty	 = 12,
	Weight			 = 12 +  0.0775 * 70,
	Cx_pil			 = 0.002,
	shape_table_data = 
	{
		{
			name 	= "bw_container_cargo",
			file	= "bw_container_cargo";
			life	= 1;                 
			fire	= { 0, 1};
			username	= "Hook Cargo2";
			index	= WSTYPE_PLACEHOLDER;
		},
	},
	Elements	= 
	{
		{
			ShapeName	= "bw_container_cargo",
			
			
		}, 
	}, 
})


declare_loadout({
	category		 = CAT_PODS,                                                
	CLSID			 = "m117_cargo",
	attribute		 =  {wsType_Air,wsType_Free_Fall,wsType_FuelTank,WSTYPE_PLACEHOLDER},
	Picture			 = "Ammunition.png",
 displayName = _("Bombs"),
	Weight_Empty	 = 0,
	Weight			 = 1000,
	Cx_pil			 = 0.002,
	shape_table_data = 
	{
		{
			name 	= "m117_cargo",
			file	= "m117_cargo";
			life	= 1;                 
			fire	= { 0, 1};
			username	= "Bombs";
			index	= WSTYPE_PLACEHOLDER;
		},
	},
	Elements	= 
	{
		{
			ShapeName	= "m117_cargo",
			
			
		}, 
	}, 
})




declare_loadout({
	category		 = CAT_PODS,                                                
	CLSID			 = "barrels_cargo",
	attribute		 =  {wsType_Air,wsType_Free_Fall,wsType_FuelTank,WSTYPE_PLACEHOLDER},
	Picture			 = "Fuels Cargo.png",
 displayName = _("Barrels Cargo"),
	Weight_Empty	 = 0,
	Weight			 = 1000,
	Cx_pil			 = 0.002,
	shape_table_data = 
	{
		{
			name 	= "barrels_cargo",
			file	= "barrels_cargo";
			life	= 1;                 
			fire	= { 0, 1};
			username	= "Barrels";
			index	= WSTYPE_PLACEHOLDER;
		},
	},
	Elements	= 
	{
		{
			ShapeName	= "barrels_cargo",
			
			
		}, 
	}, 
})

declare_loadout({
	category		 = CAT_PODS,                                                
	CLSID			 = "oiltank_cargo",
	attribute		 =  {wsType_Air,wsType_Free_Fall,wsType_FuelTank,WSTYPE_PLACEHOLDER},
	Picture			 = "Fuels Cargo.png",
 displayName = _("Oiltank Cargo"),
	Weight_Empty	 = 0,
	Weight			 = 1000,
	Cx_pil			 = 0.002,
	shape_table_data = 
	{
		{
			name 	= "oiltank_cargo",
			file	= "oiltank_cargo";
			life	= 1;                 
			fire	= { 0, 1};
			username	= "Oiltank Cargo";
			index	= WSTYPE_PLACEHOLDER;
		},
	},
	Elements	= 
	{
		{
			ShapeName	= "oiltank_cargo",
			
			
		}, 
	}, 
})


