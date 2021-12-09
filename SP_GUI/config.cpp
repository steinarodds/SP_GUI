class CfgPatches
{
	class SP_GUI
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};

class CfgMods
{
	class SP_GUI
	{
		name="SP_GUI";
		dir="SP_GUI";
		type="mod";
		author="Mrtea101";
		credits="Mrtea101";
		hideName=1;
		hidePicture=1;
		
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		
		class defs
		{
			class imageSets
			{
				files[]=
				{
					"SP_GUI\ImageSets\CustomGUI.imageset"
				};
			};
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"SP_GUI\Scripts\3_Game",
					"SP_GUI\Scripts\Common"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"SP_GUI\Scripts\4_World",
					"SP_GUI\Scripts\Common"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"SP_GUI\Scripts\5_Mission",
					"SP_GUI\Scripts\Common"
				};
			};
		};
	};
};