/*
 * use by UNBRobotics as core - v1.0.0
 *
 */

package UnbRoboticsCompetionv31;

import net.java.games.input.*;
import java.io.*;
import java.util.*;

/**
 * The Controller Handler will take care of gathering all the needed information
 * from the user preference file and the OS, valid the data, and create all the 
 * needed objects. As a controller from the OS is found that matches one in the
 * user preferences, a new RemoteController object is created.
 * 
 * @author Frank Lasard
 * @author Jonathan Miller
 * @author Adam Doyle
 * @version 3.1
 * @since 2012-01-01
 */
class ControllerHandler extends Thread{
    
    private Properties props;                                                   //Properties that hold the user defined variables
    private File propsFile = new File("");                                      //File that holds the user defined variable
    private String os = getOS();                                                //Operations System
    private String comPort;                                                     //Comm Port that is ued for serial communications
    private ArrayList<String> controllerNames;                                  //The list of parsed controllers names 
    private boolean foundController;                                            //Did the system find a valid connected controller
    private RemoteController[] controllers;                                     //Currently running Remote Controller threads
    private Controller[] cs;                                                    //Controllers that are reported from JInput/OS
    private SimpleIO io;                                                        //The serial communication object
    private final int[] baseArray = {1,31,61,91,121,151,181,211};               //Base bytes for the 8 robots
    
        
    ControllerHandler(){
        
    }
    
    public void run(){
        
        //Setup the environment
        setup();

        //Test the IO port, if not mark the serial port and not found, causing the program to close
        try{
            io.writeByte((byte)249);
        }
        catch(Exception e){
            System.out.println(e.getClass());
            System.out.println(e.getMessage());
            System.out.println("Serial IO not initialized Properly");
            io.portFound = false;
        }
        if (SystemInterface.debug) System.out.println("ControllerHandler:Serial IO passed Test");

        //Test if valid serial port was found 
        if(io.portFound){
            
            //Test if at least 1 valid controller was found
            if(foundController){
                if(SystemInterface.debug) System.out.println("ControllerHandler:Playstation DualShock 3 Connected");

                //If a serial port and valid controller(s) were found run the until the close signal is given
                while(!SystemInterface.close){

                    //If the ebug flag is set, read in the input from all the ardunios and output it to the terminal
                    if(SystemInterface.debug){
                        try{
                            if (io.input.available() != 0){
                                    System.out.print("ControllerHandler:Ardunio Return " + io.input.read()+"\n");
                            }
                        }
                        catch(IOException e){
                            System.out.println("ControllerHandler:Error Reading from serial port");
                            break;
                        }
                        catch(Exception e){
                            System.out.println("ControllerHandler:Unknow Communication Error");
                            break;
                        }
                    }
                }
            }
            else{
                System.out.println("No Valid Controller Found");
                System.out.println("Please Connect PlayStation DualShock 3 Controller(s)"); 
            }
        }
        else{
            System.out.println("No Xbee Tranciever Found");
            System.out.println("Please Connect an Xbee Transciever");
        }

        close();
    }
    
    /**
     * The setup method will first determine the location of the user preference
     * file, open it, and parse the contents. Once it has the user input it will
     * attempt to open a serial connection with either the user defined value, 
     * or the OS default. Then it will call the controller setup routine.
     */
    private void setup(){
        
        if (os.equals("windows")){
            propsFile = new File(System.getProperty("user.home")+"/AppData/Local/UnbRobotics/ControllerProperties.txt");
        }else if (os.equals("unix")){
            propsFile = new File(System.getProperty("user.home")+"/.UnbRobotics/ControllerProperties.txt");
        }else if (os.equals("mac")){
            propsFile = new File(System.getProperty("user.home")+"/.UnbRobotics/ControllerProperties.txt");
        }else if (os.equals("unknown")){
            System.out.println("Unsupported OS");
            close();
        }

        props = openProperties(propsFile);
        
        String[] propsNames = props.stringPropertyNames().toArray(new String[props.stringPropertyNames().size()]);
        controllerNames = new ArrayList<String>(propsNames.length);
        
        for (String propsName : propsNames) {
            controllerNames.add(null);
        }
        
        

        for (String contPropName : propsNames) {
            if (contPropName.contains("Controller")) {
                controllerNames.set((Integer.valueOf(contPropName.replaceAll("[^0-9]",""))-1), props.getProperty(contPropName));
            }
            if (contPropName.contains("comPort")){
                comPort = props.getProperty("comPort").trim();
                
            }
        }
        
        controllerNames.removeAll(Collections.singleton(null)); //remove all unused slots in the controller list
        
        if(SystemInterface.debug) for (String controllerName : controllerNames) System.out.println("ControllerHandler:"+controllerName);
        
        controllers = new RemoteController[controllerNames.size()];  //Create an array to store remote controller threads

        if (SystemInterface.debug) System.out.println("ControllerHandler:Were Controllers Found:" + foundController);
        
        if (SystemInterface.debug) System.out.println("ControllerHandler:ComPort pre validation: " + comPort);
                
        if((comPort == null) || (comPort.isEmpty())){
            System.out.println("No Serial Port specified, using system defualt");
            
            if (os.equals("windows")){
                props.setProperty("comPort", "COM1");
            }else if (os.equals("unix")){
                props.setProperty("comPort", "/dev/ttyUSB0");
            }else if (os.equals("mac")){
                props.setProperty("comPort", "/dev/ttyUSB0");
            }
        }
        
        comPort = props.getProperty("comPort");
        
        if(SystemInterface.debug) System.out.println("ControllerHandler:ComPort post validation: " + comPort);
        
        io = new SimpleIO(comPort);
        
        foundController = false;	//Inialize Controller found status to default 
        
        //Controllers must be connected after the serial port is created.
        connectControllers();  //Attempt to connect controllers
    }
    
    /**
     * The close method will close out the program, by telling the Remote 
     * Controllers to disconnect, then the serial port, and then save the
     * properties out to the users home directory.
     */
    private void close(){
        
        //Close Remote Controllers
        if(controllers != null){
            System.out.println("Telling Controllers to Disconnect");
            for (RemoteController controller : controllers) {
                if (controller != null) {
                   controller.setThreadStop(true);
                }
            }
        }

        //Close Serial Comm Port
        if (io != null){
            System.out.println("Disconnecting Xbee Transiever");
            io.disconnect();
        }
        
        //Save the properties back to the user home directory
        if(props != null){
            System.out.println("Writing Properties to files");
            closeProperties(props, propsFile);
        }

        
    }
    
    /**
     * All the joystick that are connected to the OS are compared to the ones
     * specified in the user preference file. If a match is made a new Remote
     * Controller is started and moved to it's own thread.
     */
    private void connectControllers(){
        try{
            //Setup Controller Environment
            if(!controllerNames.isEmpty()){
                cs = ControllerEnvironment.getDefaultEnvironment().getControllers();                                //Get the current connected controllers
            } 
            else{
                System.out.println("Please configure controller(s) in preference file");
                close();
            }

            for (int i = 0; ((i < cs.length) && (i < 8)); i++){
                if(cs[i].getName().contains("PLAYSTATION(R)3")){
                    for (int j = 0; j<controllerNames.size();j++){
                        if (cs[i].getName().contains(controllerNames.get(j))){
                            if(controllers[j] == null){
                                foundController = true;
                                //Create a new controller object
                                RemoteController t = new RemoteController (io, cs[i],baseArray[j]);
                                //Store the controller thread reference
                                controllers[j] = t;
                                //Allocate a thread to each new controller
                                t.start();
                            }
                        }
                    }
                }
            }
        }
        catch(Exception e){
            System.out.println("Failed to create controllers " + e.getMessage());
        }
    }
    
    private String getOS(){
        
        String OS = System.getProperty("os.name").toLowerCase();
        
        if(OS.contains("win"))
            return "windows";
        else if((OS.contains("nix"))|| (OS.contains("nux"))|| (OS.contains("aix")))
            return "unix";
        else if(OS.contains("mac"))
            return "mac";
        else
            return "unknown";
        
    }
    
    private Properties openProperties(File propsFile){
        Properties props = new Properties();
        if(!(propsFile.exists() && !propsFile.isDirectory())){
            System.out.println(propsFile.toString() + " Does not Exist, Creating it");
            try{
                propsFile.getParentFile().mkdirs();
                propsFile.createNewFile();
                System.out.println("File Created");
            }
            catch (SecurityException sE){
                System.out.println("Security Exception: " + sE.getMessage());
            }
            catch (IOException iOE){
                System.out.println("Could not creat file: " + iOE.getMessage());
            }
            catch (Exception e){
                System.out.println("Shit hit the fan:" + e.getMessage());
            }
        }

        try{
            FileInputStream propsFIS = new FileInputStream(propsFile);
            props.load(propsFIS);
            propsFIS.close();
        }
        catch (SecurityException sE){
            System.out.println("Security Exception: " + sE.getMessage());
        }
        catch (IllegalArgumentException iAE){
            System.out.println("Invalid input in file: "+ iAE.getMessage());
        }
        catch (FileNotFoundException fNFE){
            System.out.println("File not Found: "+fNFE.getMessage());
        }
        catch (IOException iOE){
            System.out.println("IO Error loading properties: " + iOE.getMessage());
        }
        catch (Exception e){
            System.out.println("Shit hit the fan:" + e.getMessage());
        }
        
        return props;
    }
    
    private void closeProperties(Properties props, File propsFile){
        try{
            FileOutputStream propsFOS = new FileOutputStream(propsFile);
            props.store(propsFOS, "---closing---");
            propsFOS.flush();
            propsFOS.close();
        }
        catch (SecurityException sE){
            System.out.println("Security Exception: " + sE.getMessage());
        }
        catch (IllegalArgumentException iAE){
            System.out.println("Invalid input in file: "+ iAE.getMessage());
        }
        catch (IOException iOE){
            System.out.println("IO Error loading properties: " + iOE.getMessage());
        }
        catch (Exception e){
            System.out.println("Shit hit the fan, while writting properties:" + e.getMessage());
        }
    }
}
