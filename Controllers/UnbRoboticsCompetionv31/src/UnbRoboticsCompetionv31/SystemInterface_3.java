/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package UnbRoboticsCompetionv31;

import java.util.Scanner;

/**
 * A System to read in multiple Playstation DualShock 3 controllers and output
 * to a single serial port.
 * 
 * @author Adam Doyle
 * @version 2.0
 */
public class SystemInterface {
    
    protected static boolean debug = false;                                     //System wide debug, set to true for additionally logging
    protected static boolean close = false;                                     //System wide close, set to true to have program end

    /**
     * Starts the program by creating a handler for the controller input and
     * then waits for the user to terminate the program.
     * 
     * @param args command line arguments, first for debug (1 or 0)
     */
    public static void main(String[] args) {
        
        //Check and valid the first command line argument, if 1 turn debgging on
        if(args.length != 0){
            try{
                if(Integer.parseInt(args[0]) == 1)
                    debug = true;
                else if (Integer.parseInt(args[0]) == 0)
                    debug = false;
                else
                    debug = false;
            }
            catch (NumberFormatException nFE){
                System.out.println("Invalid Argument, defaulting debug to false");
                debug = false;
            }
            catch (Exception e){
                System.out.println("Invalid Argument, defaulting debug to false");
                debug = false;
            }
        }
        
        //Create the Controller Handler
        if(debug) System.out.println("SystemInterface: Building new Controller Handler.");
        ControllerHandler contHand = new ControllerHandler();
        
        //Split Controller Handler into it's own thread
        if(debug) System.out.println("SystemInterface: Moving Handler to seperate Thread.");
        contHand.start();
        
        //Create a scanner to allow user input
        if(debug) System.out.println("SystemInterface: Asking User to end program.");
        Scanner userIn = new Scanner(System.in);	//Open a Scanner for user input
        String s;
        
        //While the Controller Handler is alive continue waiting for user to terminate the program
        while(contHand.isAlive()){
            try{
                //If the user hasn't asked to close the program yet, ask if they want to and wait for input
                if(!close){
                    System.out.println("Exit Program? (Y/N)");
                    if(userIn.hasNextLine() && !close){
                        s = userIn.nextLine();
                        if (s.equalsIgnoreCase("Y") || s.equals("YES")){
                            close = true;
                        }
                    }
                }
            }
            catch(Exception e){
                System.out.println("Getting User Input F***ed Up");
                close=true;
                break;
            }
        }
        
        //Once the COntroller Handler is dead, close the user input scanner
        if(debug) System.out.println("SystemInterface: Closing User Input");
        if(userIn != null){
            userIn.close();
        }
        
        //Once everything else is closed exit the program cleanly
        System.out.println("Exiting Program");
        System.exit(0);
    }//end main
}//end SystemInterface