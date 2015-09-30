package UnbRoboticsCompetition;

import net.java.games.input.*;
import java.io.*;
import java.util.*;

/**
 * @author Frank Lasard
 * @author Adam Doyle
 * @version 3.1
 * @since 2012-01-01
 */
public class ControllerSetup{
    
    //TO-DO merge multiple conf files into one and use regex to parse out what I want into multiple string arrays
    protected Properties props;
    protected File propsFile = new File("");
    protected String os = getOS();
    protected String comPort;
    protected ArrayList<String> controllerNames;
    protected boolean foundController;
    protected RemoteController[] controllers;
    protected Controller[] cs;
    protected SimpleIO io;
    protected final int[] baseArray = {1,31,61,91,121,151,181,211};	//Base bytes for the 8 robots
    protected Scanner userIn;
        
    protected ControllerSetup(){
        
    }
    
    protected void setup(){
        
        if (os.equals("windows")){
            propsFile = new File(System.getProperty("user.home")+"/AppData/Local/UnbRobotics/ControllerProperties.txt"); //Verify this file path may be /Roaming/AppData/...
        }else if (os.equals("unix")){
            propsFile = new File(System.getProperty("user.home")+"/.UnbRobotics/ControllerProperties.txt");
        }else if (os.equals("mac")){
            propsFile = new File(System.getProperty("user.home")+"/.UnbRobotics/ControllerProperties.txt");
        }else if (os.equals("unknown")){
            System.out.println("Unsupported OS");
            System.exit(0);
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
                comPort = props.getProperty("comPort");
            }
        }
        
        controllerNames.removeAll(Collections.singleton(null)); //remove all unused slots in the controller list
        
        if(SystemInterface.debug) for (String controllerName : controllerNames) System.out.println(controllerName);
        
        if (SystemInterface.debug) System.out.println("ComPort pre validation: " + comPort);
                
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
        
        if(SystemInterface.debug) System.out.println("ComPort post validation: " + comPort);
            
        io = new SimpleIO(comPort);

        foundController = false;	//Controller found status

        //Setup PS2 Controller Environment
        if(controllerNames.size() != 0){
            controllers = new RemoteController[controllerNames.size()];                     //Create an array to store remote controller threads
            ControllerEnvironment ce = ControllerEnvironment.getDefaultEnvironment();       //Get the current controller environment
            cs = ce.getControllers();                                                       //Get a list of all possible connected controllers
        } 
        else{
            System.out.println("Please configure controller(s) in preference file");
            close();
        }
            
    }
    
    protected void run(){
        userIn = new Scanner(System.in);	//Open a Scanner for user input

        //Check each controller to verify that it is a DualShock3 and start a corresponding thread if so
        if(cs.length != 0){
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

            //If a valid controller(s) was found continue operations
            if(foundController){
                System.out.println("Playstation DualShock 3 Connected");

                //If valid Xbee serial port found continue operations
                if(io.portFound){

                    //Place holder string
                    String s;

                    while(true){

                        if(SystemInterface.debug){
                            try{
                                if (io.input.available() != 0){
                                        System.out.print("Ardunio Return " + io.input.read()+"\n");
                                }
                            }
                            catch(IOException e){
                                System.out.println("Error Reading from Xbee Network");
                                break;
                            }
                            catch(Exception e){
                                System.out.println("Unknow Xbee Communication Error");
                                break;
                            }
                        }
                        else{
                            System.out.println("Exit Program? (Y/N)");
                            s = userIn.nextLine();
                            if (s.equals("Y") || s.equals("y")){
                                break;
                            }
                        }
                    }
                }
                else{
                    System.out.println("No Xbee Tranciever Found");
                    System.out.println("Please Connect an Xbee Transciever");
                }
            }
            else{
                System.out.println("No Valid Controller Found");
                System.out.println("Please Connect PlayStation DualShock 3 Controller(s)");
            }
        }
        else{
            System.out.println("No Controller Found");
            System.out.println("Please Connect a PlayStation DualShock 3 Controller");
        }

        close();
    }
    
    protected void close(){
        
        //Close user input Scanner
        if(userIn != null){
            userIn.close();
        }
          
        //Close Xbee Transceiver
        if (io != null){
            System.out.println("Disconnecting Xbee Transiever");
            io.disconnect();
        }
        
        if(props != null){
            System.out.println("Writing Properties to files");
            closeProperties(props, propsFile);
        }

        //Close Remote Controllers
        if(controllers != null){
            System.out.println("Telling Controllers to Disconnect");
            for (RemoteController controller : controllers) {
                if (controller != null) {
                   controller.setThreadStop(true);
                }
            }
        }

        System.out.println("Exiting Program");
        System.exit(0);
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