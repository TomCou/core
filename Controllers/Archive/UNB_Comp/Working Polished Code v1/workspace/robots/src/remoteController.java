import net.java.games.input.Component;
import net.java.games.input.Controller;


public class remoteController implements Runnable {
	
	
	private Controller controller;
	private Component[] cps;
	private SimpleWrite2 outwrite;
	private int mybyte;
	
	remoteController(SimpleWrite2 writer, Controller ctr, int startbyte)
	{
		this.controller = ctr;
		this.outwrite = writer;
		this.mybyte = startbyte;
		System.out.println("Hello " + ctr.getName());
	}
	
	
	public void run()
	{
		while(true)
		{
			controller.poll();
			
			cps = controller.getComponents();

			
	 		//Green triangle = 33
	 		//Red Circle = 34
	 		//Purple X = 35
	 		//Square  = 36
	 		
	 		//Forward = 4
	 		//Right = 5
	 		//Back = 6
	 		//Left = 7
	 		
	 		//Start = 3
			
			if(cps[33].getPollData() == 1)//Green Triangle
			{
				//System.out.println("Gree Button Pressed");
				//System.out.println(mybyte);
				outwrite.writeByte((byte)mybyte);
			}
			if(cps[34].getPollData() == 1)
			{
				outwrite.writeByte((byte)(mybyte+1));
				//System.out.println(mybyte);
			}
					
			if(cps[35].getPollData() == 1)	
			{
				outwrite.writeByte((byte)(mybyte+2));
				//System.out.println(mybyte);
			}
				
			if(cps[36].getPollData() == 1)
			{
				outwrite.writeByte((byte)(mybyte+3));
				//System.out.println(mybyte);
			}
				
				
			if(cps[4].getPollData() == 1)
			{
				outwrite.writeByte((byte)(mybyte+4));
				//System.out.println(mybyte);
			}
					
			if(cps[5].getPollData() == 1)
			{
				outwrite.writeByte((byte)(mybyte+5));
				//System.out.println(mybyte);
			}
							
			if(cps[6].getPollData() == 1)	
			{
				outwrite.writeByte((byte)(mybyte+6));
				//System.out.println(mybyte);
			}
						
			if(cps[7].getPollData() == 1)	
			{
				outwrite.writeByte((byte)(mybyte+7));
				//System.out.println(mybyte);
			}
				
			if(cps[3].getPollData() == 1)
			{
				outwrite.writeByte((byte)(mybyte+8));
				//System.out.println(mybyte);
			}

		}
		
	}

}
