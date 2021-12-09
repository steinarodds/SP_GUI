
modded class HeroesAndBanditsStatusBarIconUI
{	
	override Widget Init()
    {
		layoutRoot 	= GetGame().GetWorkspace().CreateWidgets( "SP_GUI/Layouts/Modded_HeroesAndBanditsStatusBar.layout" );
		m_Icon		= ImageWidget.Cast( layoutRoot.FindAnyWidget( "HeroesAndBanditsStatusBarIcon" ) );
		m_Icon.LoadImageFile( 0, g_HeroesAndBanditsPlayer.getLevel().LevelImage , true );
		
        return layoutRoot;
    }
}