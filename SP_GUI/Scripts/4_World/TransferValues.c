modded class TransferValues
{
	const int TYPE_HUNGER 	= 3;
	const int TYPE_THIRST 	= 4;
	
	float m_LastHungerUpdate;
	float m_HungerMaxValue;
	float m_HungerClient;
	
	float m_LastThirstUpdate;
	float m_ThirstMaxValue;
	float m_ThirstClient;
	
	override void Init()
	{
		super.Init();
		
		// Max Values have been known to change on dayz devs' whims...
		
		m_LastHungerUpdate 	= 0;
		m_HungerMaxValue 	= 7500; // m_Player.GetStatEnergy().GetMax() not correct...
		m_HungerClient 		= 0;
		
		m_LastThirstUpdate 	= 0;
		m_ThirstMaxValue 	= m_Player.GetStatWater().GetMax(); // 5000
		m_ThirstClient 		= 0;
	}
	
	override void CheckValues()
	{
		super.CheckValues();
		
		CheckHunger();
		CheckThirst();
	}
	
	float GetHunger()
	{
		return m_HungerClient;
	}
	
	float GetThirst()
	{
		return m_ThirstClient;
	}
	
	void CheckHunger()
	{
		float hunger_current = m_Player.GetStatEnergy().Get();
		float hunger_normalized = hunger_current / m_HungerMaxValue;
		float difference_normalized = hunger_normalized - m_LastHungerUpdate;
		float diff_abs = Math.AbsFloat( difference_normalized );
		
		if( diff_abs > ( SENSITIVTY_PERCENTAGE / 100 ) )
		{
			SendValue( TYPE_HUNGER, hunger_normalized );
			m_LastHungerUpdate = hunger_normalized;
		}
	}
	
	void CheckThirst()
	{
		float thirst_current = m_Player.GetStatWater().Get();
		float thirst_normalized = thirst_current / m_ThirstMaxValue;
		float difference_normalized = thirst_normalized - m_LastThirstUpdate;
		float diff_abs = Math.AbsFloat( difference_normalized );
		
		if( diff_abs > ( SENSITIVTY_PERCENTAGE / 100 ) )
		{
			SendValue( TYPE_THIRST, thirst_normalized );
			m_LastThirstUpdate = thirst_normalized;
		}
	}
	
	override void SendInitValues()
	{
		super.SendInitValues();
		
		// Hunger
		float hunger_current = m_Player.GetStatEnergy().Get();
		float hunger_normalized = hunger_current / m_HungerMaxValue;
		SendValue( TYPE_HUNGER, hunger_normalized);
		m_LastHungerUpdate = hunger_normalized;
		
		// Thirst
		float thirst_current = m_Player.GetStatWater().Get();
		float thirst_normalized = thirst_current / m_ThirstMaxValue;
		SendValue( TYPE_THIRST, thirst_normalized );
		m_LastThirstUpdate = thirst_normalized;
	}

	override void ReceiveValue( int value_type, float value )
	{
		super.ReceiveValue( value_type, value );
		
		if( value_type == TYPE_HUNGER )
		{
			m_HungerClient = value;
		}
		else if( value_type == TYPE_THIRST )
		{
			m_ThirstClient = value;
		}
	}
}
