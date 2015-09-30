package UnbRoboticsCompetionv31;

import net.java.games.input.Component;
import net.java.games.input.Controller;


/**
 * An instance of this class will monitor and re-act to a controller. As input
 * is received it will send the corresponding code out over the serial port.
 * 
 * @author Frank Lesard
 * @author Adam Doyle
 * @version 1.3
 * @since 2013-10-01
 */
public class RemoteController extends Thread {
	
    //Jinput output to PS DualShock2 button correlations
    private static final int SELECT = 0;
    //private static final int LEFT_STICK_PUSH = 1;
    //private static final int RIGHT_STICK_PUSH = 2;
    private static final int START = 3;
    private static final int UP_D = 4;
    private static final int RIGHT_D = 5;
    private static final int DOWN_D = 6;
    private static final int LEFT_D = 7;
    private static final int L2_D = 8;
    private static final int R2_D = 9;
    private static final int L1_D = 10;
    private static final int R1_D = 11;
    private static final int GREEN_TRIANGLE_D = 12;
    private static final int RED_CIRCLE_D = 13;
    private static final int BLUE_X_D = 14;
    private static final int PURPLE_SQUARE_D = 15;
    //private static final int PS = 16;
    //private static final int LEFT_HOR = 17;
    private static final int LEFT_VER = 18;
    //private static final int RIGHT_HOR = 19;
    private static final int RIGHT_VER = 20;
    //private static final int XZ_PLANE = 21;
    //private static final int YZ_PLANE = 22;
    //private static final int FACE = 23;
    //private static final int UP_A = 25;
    //private static final int RIGHT_A = 26;
    //private static final int DOWN_A = 27;
    //private static final int LEFT_A = 28;
    //private static final int L2_A = 29;
    //private static final int R2_A = 30;
    //private static final int L1_A = 31;
    //private static final int R1_A = 32;
    //private static final int GREEN_TRIANGLE_A = 33;
    //private static final int RED_CIRCLE_A = 34;
    //private static final int BLUE_X_A = 35;
    //private static final int PURPLE_SQUARE_A = 36;

    //Runtime variables
    private Controller controller;
    private String name;
    private Component[] cps;
    private SimpleIO outwrite;
    private int myBase;
    private int motorLeft;
    private int motorRight;
    private boolean threadStop;


    /**
     * Connect a PS DualShock3 to the desired serial port. It will also 
     * associate a given unique address space to each controller.
     * 
     * @param writer The serial port object that is used to communicate to a serial port
     * @param ctr A controller object that was represents the output of Jinput/PS2 Dual shock3
     * @param startbyte The starting address for the RemoteController instance
     */
    RemoteController(SimpleIO writer, Controller ctr, int startByte)
    {
        this.controller = ctr;
        this.name = ctr.getName();
        this.outwrite = writer;
        this.myBase = startByte;
        this.threadStop = false;
        if (SystemInterface.debug) System.out.println("RemoteController:Hello, My Name is " + this.name + ", my start byte is "+ this.myBase);  
    }


    /**
     * This method describes the runtime actions of a Controller. It will 
     * continually read the input from the controller and send the 
     * corresponding address/action to the serial port
     * 
     * @see java.lang.Thread#run()
     */
    public void run()
    {        
        while(!threadStop){
            
            //Get the current controller input and break it into components
            try{
                
                if(controller.poll()){

                    cps = controller.getComponents();

                    //Left Stick (Map vertical input to -100 to 100
                    motorLeft = (int)(cps[LEFT_VER].getPollData()*100);

                    //Respond to Left Stick/Motor (in only one direction at a time)
                    if ((motorLeft <= 100) && (motorLeft > 79))
                    {   outwrite.writeByte((byte)myBase);}
                    else if ((motorLeft <= 79) && (motorLeft > 44))
                    {   outwrite.writeByte((byte)(myBase+1));}
                    else if ((motorLeft <= 44) && (motorLeft > 9))
                    {    outwrite.writeByte((byte)(myBase+2));}
                    else if ((motorLeft < -9) && (motorLeft >= -44))
                    {    outwrite.writeByte((byte)(myBase+3));}
                    else if ((motorLeft < -44) && (motorLeft >= -79))
                    {    outwrite.writeByte((byte)(myBase+4));}
                    else if ((motorLeft < -79) && (motorLeft >= -100))
                    {    outwrite.writeByte((byte)(myBase+5));}

                    //Right Stick (Map vertical input to -100 to 100)
                    motorRight = (int)(cps[RIGHT_VER].getPollData()*100);

                    //Respond to Right Stick/Motor (in only one direction at a time)
                    if ((motorRight <= 100) && (motorRight > 79))
                    {    outwrite.writeByte((byte)(myBase+6));}
                    else if ((motorRight <= 79) && (motorRight > 44))
                    {    outwrite.writeByte((byte)(myBase+7));}
                    else if ((motorRight <= 44) && (motorRight > 9))
                    {    outwrite.writeByte((byte)(myBase+8));}
                    else if ((motorRight < -9) && (motorRight >= -44))
                    {    outwrite.writeByte((byte)(myBase+9));}
                    else if ((motorRight < -44) && (motorRight >= -79))
                    {    outwrite.writeByte((byte)(myBase+10));}
                    else if ((motorRight < -79) && (motorRight >= -100))
                    {    outwrite.writeByte((byte)(myBase+11));}

                    //Respond to AUX Motor 1 (in only one direction at a time)
                    if (cps[L1_D].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+12));}
                    else if (cps[L2_D].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+13));}

                    //Respond to AUX Motor 2 (in only one direction at a time)
                    if (cps[R1_D].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+14));}
                    else if (cps[R2_D].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+15));}

                    //Respond to Start
                    if (cps[START].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+16));}

                    //Respond to Select
                    if(cps[SELECT].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+17));}

                    //Respond to digital Up
                    if(cps[UP_D].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+18));}

                    //Respond to digital Right
                    if(cps[RIGHT_D].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+19));}

                    //Respond to digital Down
                    if(cps[DOWN_D].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+20));}

                    //Respond to digital Left
                    if(cps[LEFT_D].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+21));}

                    //Respond to digital Triangle
                    if(cps[GREEN_TRIANGLE_D].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+22));}

                    //Respond to digital Circle
                    if(cps[RED_CIRCLE_D].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+23));}

                    //Respond to digital X
                    if(cps[BLUE_X_D].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+24));}

                    //Respond to digital Square
                    if(cps[PURPLE_SQUARE_D].getPollData() == 1)
                    {    outwrite.writeByte((byte)(myBase+25));}
                    
                    Thread.sleep(35);

                }
                else{
                    System.out.println(this.name+":Polling of controller failed");
                    this.threadStop = true;
                }
            }
            catch(Exception e){
                System.out.println(e.getClass());
                System.out.println(e.getMessage());
                System.out.println(this.name+":Processing input from Controller failed");
                this.threadStop = true;
            }
        }
        System.out.println(this.name + " disconnected.");
        
        
    }

    /**
     * This method can be called with a parameter of true to stop a controller.
     * @param a A boolean value that can force a controller to disconnect
     */
    protected void setThreadStop(boolean a){
        this.threadStop = a;
    }
    
    /**
     * This method will return the name of the controller
     * @return the name of the controller
     */
    protected String getCtrName(){
        return this.name;
    }
}