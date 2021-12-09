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
		
		int currentHealth = player.GetTransferValues().GetHealth() * 100;		// Max is 100
		int currentBlood  = player.GetTransferValues().GetBlood()  * 5000;		// Max is 5000
		int currentHunger = player.GetTransferValues().GetHunger() * 7500;		// Max is 7500
		int currentThirst = player.GetTransferValues().GetThirst() * 5000;		// Max is 5000
		
		HealthBar.SetCurrent( currentHealth );
		BloodBar.SetCurrent( currentBlood );
		HungerBar.SetCurrent( currentHunger );
		ThirstBar.SetCurrent( currentThirst );
		
		HealthBar.SetColor( m_Notifiers.FindAnyWidget( "IconHealth" ).GetColor() );
		BloodBar.SetColor( m_Notifiers.FindAnyWidget( "IconBlood" ).GetColor() );
		HungerBar.SetColor( m_Notifiers.FindAnyWidget( "IconHungry" ).GetColor() );
		ThirstBar.SetColor( m_Notifiers.FindAnyWidget( "IconThirsty" ).GetColor() );
		
		HealthText.SetColor( HealthBar.GetColor() );
		BloodText.SetColor( BloodBar.GetColor() );
		HungerText.SetColor( HungerBar.GetColor() );
		ThirstText.SetColor( ThirstBar.GetColor() );
		
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
	}
	
	override void Update( float timeslice )
	{
		super.Update( timeslice );
		
		UpdateMRTsStatBars();
		
		m_BadgeNotifierDivider.Show( false );
	}
}
