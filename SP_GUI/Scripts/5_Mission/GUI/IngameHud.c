modded class IngameHud
{
	protected Widget							panelCustomGUI;
	
	protected ProgressBarWidget					HealthBar;
	protected ProgressBarWidget					BloodBar;
	protected ProgressBarWidget					HungerBar;
	protected ProgressBarWidget					ThirstBar;
	
	protected TextWidget						HealthText;
	protected TextWidget						BloodText;
	protected TextWidget						HungerText;
	protected TextWidget						ThirstText;
	
	override void InitBadgesAndNotifiers()
	{
		super.InitBadgesAndNotifiers();
		
		// For Thirsty, Hungry, Health, Blood, Temperature icons
		for ( int j = 0; j < m_StatesWidgetNames.Count(); j++ )
		{
			string wgt_name = m_StatesWidgetNames.GetElement(j);
			
			// This if statement is to add support for mods which add custom StatesWidgets
			if ( wgt_name == "Thirsty" || wgt_name == "Hungry" || wgt_name == "Health" || wgt_name == "Blood" )
			{
				ImageWidget wgt;
				Class.CastTo(wgt,  m_Notifiers.FindAnyWidget( String( "Icon" + wgt_name ) ) );
				wgt.Show( true );
				
				for ( int k = 0; k < 5; k++ )
				{
					wgt.LoadImageFile( k, "set:CustomGUI" + " image:Icon" + wgt_name );
				}
			}
		}
		
		// For SICK, BLEEDISH, FRACTURE, STUFFED, WETNESS icons
		for ( int m = 0; m < m_BadgesWidgetNames.Count(); m++ )
		{
			string badge_name = m_BadgesWidgetNames.GetElement( m );
			int key = m_BadgesWidgetNames.GetKey( m );
			ImageWidget badge_widget;
			Class.CastTo(badge_widget,  m_Badges.FindAnyWidget( badge_name ) );
			
			// Set image
			//if ( badge_name == "Heros/Bandits" )
			//	badge_widget.LoadImageFile( 0, "set:CustomGUI" + " image:Heros/Bandits" );
			
			m_BadgesWidgets.Set( key, badge_widget );
			badge_widget.Show( false );
			m_BadgesWidgetDisplay.Set( key, false );
		}
	}
	
	override void Init( Widget hud_panel_widget )
	{
		super.Init( hud_panel_widget );
		
		InitStatBars();
	}
	
	void InitStatBars()
	{
		CastTo( panelCustomGUI, GetGame().GetWorkspace().CreateWidgets( "SP_GUI/Layouts/CustomGUI.layout" ) );
		
		CastTo( HealthBar, panelCustomGUI.FindAnyWidget( "HealthBar" ) );
		CastTo( BloodBar, panelCustomGUI.FindAnyWidget( "BloodBar" ) );
		CastTo( HungerBar, panelCustomGUI.FindAnyWidget( "HungerBar" ) );
		CastTo( ThirstBar, panelCustomGUI.FindAnyWidget( "ThirstBar" ) );
		
		CastTo( HealthText, panelCustomGUI.FindAnyWidget( "HealthText" ) );
		CastTo( BloodText, panelCustomGUI.FindAnyWidget( "BloodText" ) );
		CastTo( HungerText, panelCustomGUI.FindAnyWidget( "HungerText" ) );
		CastTo( ThirstText, panelCustomGUI.FindAnyWidget( "ThirstText" ) );
		
		m_Notifiers.Show( false );
		m_Notifiers = panelCustomGUI.FindAnyWidget( "NotifiersPanel" );
		
		m_Badges.Show( false );
		m_Badges = panelCustomGUI.FindAnyWidget( "BadgesPanel" );
		
		m_BloodType.Show( false );
		m_BloodType = TextWidget.Cast( panelCustomGUI.FindAnyWidget("BloodType") );
		
		m_BloodPosType.Show( false );
		m_BloodPosType = TextWidget.Cast( panelCustomGUI.FindAnyWidget("BloodPosType") );
		
		m_HeatBufferPlus = ImageWidget.Cast( panelCustomGUI.FindAnyWidget( "HeatBuffer" ) );
		if ( m_HeatBufferPlus )	m_HeatBufferPlus.Show( false );
		
		InitBadgesAndNotifiers();
	}
	
	void UpdateMRTsStatBars()
	{
		PlayerBase player;
		
		if( !Class.CastTo( player, GetGame().GetPlayer() ) )	return;
		
		bool shouldShow = ( ( !m_HudHidePlayer && !m_HudHideUI && m_HudState ) || m_HudInventory );
		
		panelCustomGUI.Show( shouldShow );
		
		if ( !shouldShow )	return;
		
		int currentHealth = player.GetTransferValues().GetHealth() * 1000;		// Max is 100
		int currentBlood  = player.GetTransferValues().GetBlood()  * 1000;		// Max is 5000
		int currentHunger = player.GetTransferValues().GetHunger() * 1000;		// Max is 7500
		int currentThirst = player.GetTransferValues().GetThirst() * 1000;		// Max is 5000
		
		HealthBar.SetCurrent( currentHealth );
		BloodBar.SetCurrent( currentBlood );
		HungerBar.SetCurrent( currentHunger );
		ThirstBar.SetCurrent( currentThirst );
		
		/* Setting Colors Based on vanilla stat levels
		
		HealthBar.SetColor( m_Notifiers.FindAnyWidget( "IconHealth" ).GetColor() );
		BloodBar.SetColor( m_Notifiers.FindAnyWidget( "IconBlood" ).GetColor() );
		HungerBar.SetColor( m_Notifiers.FindAnyWidget( "IconHungry" ).GetColor() );
		ThirstBar.SetColor( m_Notifiers.FindAnyWidget( "IconThirsty" ).GetColor() );
		
		HealthText.SetColor( HealthBar.GetColor() );
		BloodText.SetColor( BloodBar.GetColor() );
		HungerText.SetColor( HungerBar.GetColor() );
		ThirstText.SetColor( ThirstBar.GetColor() );
		
		*/
		
		/* Setting Texts Based on vanilla stat levels
		
		EStatLevels statlevel;
		
		statlevel = player.GetStatLevel( currentHealth, PlayerConstants.SL_HEALTH_CRITICAL, PlayerConstants.SL_HEALTH_LOW, PlayerConstants.SL_HEALTH_NORMAL, PlayerConstants.SL_HEALTH_HIGH );
		if ( statlevel == EStatLevels.GREAT )		HealthText.SetText( "Healthy" );
		if ( statlevel == EStatLevels.HIGH )		HealthText.SetText( "Not So Healthy" );
		if ( statlevel == EStatLevels.MEDIUM )		HealthText.SetText( "Unhealthy" );
		if ( statlevel == EStatLevels.LOW )			HealthText.SetText( "Very Unhealthy" );
		if ( statlevel == EStatLevels.CRITICAL )	HealthText.SetText( "Close To Dying" );
		
		statlevel = player.GetStatLevel( currentBlood, PlayerConstants.SL_BLOOD_CRITICAL, PlayerConstants.SL_BLOOD_LOW, PlayerConstants.SL_BLOOD_NORMAL, PlayerConstants.SL_BLOOD_HIGH );
		if ( statlevel == EStatLevels.GREAT )		BloodText.SetText( "Full Blood" );
		if ( statlevel == EStatLevels.HIGH )		BloodText.SetText( "High Blood" );
		if ( statlevel == EStatLevels.MEDIUM )		BloodText.SetText( "Lost Blood" );
		if ( statlevel == EStatLevels.LOW )			BloodText.SetText( "Low Blood" );
		if ( statlevel == EStatLevels.CRITICAL )	BloodText.SetText( "Very Low Blood" );
		
		statlevel = player.GetStatLevel( currentHunger, PlayerConstants.SL_ENERGY_CRITICAL, PlayerConstants.SL_ENERGY_LOW, PlayerConstants.SL_ENERGY_NORMAL, PlayerConstants.SL_ENERGY_HIGH );
		if ( statlevel == EStatLevels.GREAT )		HungerText.SetText( "Very Well Fed" );
		if ( statlevel == EStatLevels.HIGH )		HungerText.SetText( "Well Fed" );
		if ( statlevel == EStatLevels.MEDIUM )		HungerText.SetText( "Hungry" );
		if ( statlevel == EStatLevels.LOW )			HungerText.SetText( "Very Hungry" );
		if ( statlevel == EStatLevels.CRITICAL )	HungerText.SetText( "Starving" );
		
		statlevel = player.GetStatLevel( currentThirst, PlayerConstants.SL_WATER_CRITICAL, PlayerConstants.SL_WATER_LOW, PlayerConstants.SL_WATER_NORMAL, PlayerConstants.SL_WATER_HIGH );
		if ( statlevel == EStatLevels.GREAT )		ThirstText.SetText( "Well Hydrated" );
		if ( statlevel == EStatLevels.HIGH )		ThirstText.SetText( "Hydrated" );
		if ( statlevel == EStatLevels.MEDIUM )		ThirstText.SetText( "Dehydrated" );
		if ( statlevel == EStatLevels.LOW )			ThirstText.SetText( "Very Dehydrated" );
		if ( statlevel == EStatLevels.CRITICAL )	ThirstText.SetText( "Dying Of Dehydration" );
		*/
		
		// Colorz ( use this if you want get colors https://hugabor.github.io/color-picker/ )
		// Parameters:		ARGB( 255, red, grn, blu )
		int stat_white =	ARGB( 255, 220, 220, 220 );
		int stat_yellow =	ARGB( 255, 255, 255, 000 );
		int stat_orange =	ARGB( 255, 255, 150, 000 );
		int stat_red =		ARGB( 255, 255, 000, 000 );
		
		string text, color;
		
		// Health
		if ( percentageHealth => 0.8 ) // 80% and higher
		{
			text = "Very Healthy";
			color = stat_white;
		}
		else if ( percentageHealth => 0.6 ) // 60% to 79.999%
		{
			text = "Healthy";
			color = stat_white;
		}
		else if ( percentageHealth => 0.4 ) // 40% to 59.999%
		{
			text = "Somewhat Healthy";
			color = stat_yellow;
		}
		else if ( percentageHealth => 0.2 ) // 20% to 39.999%
		{
			text = "Unhealthy";
			color = stat_orange;
		}
		else if ( percentageHealth => 0.0 ) // 0% to 19.999%
		{
			text = "Very Unhealthy";
			color = stat_red;
		}
		
		m_Notifiers.FindAnyWidget( "IconHealth" ).SetColor( color ); // Set vanilla icon color
		HealthText.SetText( text ); // Set StatBar Text
		HealthText.SetColor( color ); // Set StatBar Text Color
		HealthBar.SetColor( color ); // Set StatBar Color
		
		// Blood
		if ( percentageHealth => 0.8 ) // 80% and higher
		{
			text = "Full Blood";
			color = stat_white;
		}
		else if ( percentageHealth => 0.6 ) // 60% to 79.999%
		{
			text = "High Blood";
			color = stat_white;
		}
		else if ( percentageHealth => 0.4 ) // 40% to 59.999%
		{
			text = "Lost Blood";
			color = stat_yellow;
		}
		else if ( percentageHealth => 0.2 ) // 20% to 39.999%
		{
			text = "Low Blood";
			color = stat_orange;
		}
		else if ( percentageHealth => 0.0 ) // 0% to 19.999%
		{
			text = "Very Low Blood";
			color = stat_red;
		}
		
		m_Notifiers.FindAnyWidget( "IconBlood" ).SetColor( color ); // Set vanilla icon color
		BloodText.SetText( text ); // Set StatBar Text
		BloodText.SetColor( color ); // Set StatBar Text Color
		BloodBar.SetColor( color ); // Set StatBar Color
		
		// Hunger
		if ( percentageHealth => 0.8 ) // 80% and higher
		{
			text = "Very Well Fed";
			color = stat_white;
		}
		else if ( percentageHealth => 0.6 ) // 60% to 79.999%
		{
			text = "Well Fed";
			color = stat_white;
		}
		else if ( percentageHealth => 0.4 ) // 40% to 59.999%
		{
			text = "Hungry";
			color = stat_yellow;
		}
		else if ( percentageHealth => 0.2 ) // 20% to 39.999%
		{
			text = "Very Hungry";
			color = stat_orange;
		}
		else if ( percentageHealth => 0.0 ) // 0% to 19.999%
		{
			text = "Starving";
			color = stat_red;
		}
		
		m_Notifiers.FindAnyWidget( "IconHungry" ).SetColor( color ); // Set vanilla icon color
		HungerText.SetText( text ); // Set StatBar Text
		HungerText.SetColor( color ); // Set StatBar Text Color
		HungerBar.SetColor( color ); // Set StatBar Color
		
		// Thirst
		if ( percentageHealth => 0.8 ) // 80% and higher
		{
			text = "Well Hydrated";
			color = stat_white;
		}
		else if ( percentageHealth => 0.6 ) // 60% to 79.999%
		{
			text = "Hydrated";
			color = stat_white;
		}
		else if ( percentageHealth => 0.4 ) // 40% to 59.999%
		{
			text = "Thirsty";
			color = stat_yellow;
		}
		else if ( percentageHealth => 0.2 ) // 20% to 39.999%
		{
			text = "Very Thirsty";
			color = stat_orange;
		}
		else if ( percentageHealth => 0.0 ) // 0% to 19.999%
		{
			text = "Dying of Thirst";
			color = stat_red;
		}
		
		m_Notifiers.FindAnyWidget( "IconThirsty" ).SetColor( color ); // Set vanilla icon color
		ThirstText.SetText( text ); // Set StatBar Text
		ThirstText.SetColor( color ); // Set StatBar Text Color
		ThirstBar.SetColor( color ); // Set StatBar Color
	}
	
	override void Update( float timeslice )
	{
		super.Update( timeslice );
		
		UpdateMRTsStatBars();
		
		m_BadgeNotifierDivider.Show( false );
	}
}
