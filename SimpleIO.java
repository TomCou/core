/*
 * used by UNBRobotics - core - v1.0.0
 * oct-4-2015
 */

package UnbRoboticsCompetionv31;

import java.io.*;
import gnu.io.*;

/**
 * An interface from multiple objects to send/receive from a single serial port
 * 
 * @author Frank Lasard
 * @author Adam Doyle
 * @version 2.1
 * @since 2013-10-01
 */
class SimpleIO {
    static CommPortIdentifier portId = null;
    static String message = "Hello, world";
    private SerialPort serialPort;
    protected InputStream input;
    private OutputStream output;	
    protected boolean portFound = false;

    SimpleIO(String port){
        connect(port);
    }

    /**
     * A synchronized method for multiple devices to write to a serial port
     * @param data An 8 bit value to be written to the serial port
     */
    protected synchronized void writeByte(Byte data){
        try {
            output.write(data);
            Thread.sleep(15);
        } 
        catch (IOException iOE) {
            System.out.println("Error Writing to serial port: " + iOE.getMessage());
        }
        catch (InterruptedException iE){
            System.out.println("Interrupted while writing to serial port: " + iE.getMessage());
        }
        catch (Exception e){
            System.out.println("Unknown error while writing to serial port: " + e.getMessage());
        }
    }


    /**
     * When called this method will attempt to open and configure a serial port
     * at the provided mount point. It will then configure it to a known 
     * serial port configuration (57600, 8, 1, None).
     * 
     * @param defaultPort The default OS mount point to connect to
     */
    private synchronized void connect(String defaultPort){

        //Tell gnu.io to look at default mount points
        System.setProperty("gnu.io.rxtx.SerialPorts", defaultPort);

        try{
            //Get Default Port Id
            portId = CommPortIdentifier.getPortIdentifier(defaultPort);
        }
        catch(NoSuchPortException nSPE){
            System.out.println("No Port Identifier for default port " + defaultPort + " found.");
        }

        if(portId != null){
            //Verify that possible port is actually a serial port
            if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {  
                try {
                    //Try to open port
                    serialPort = (SerialPort) portId.open("SimpleIO", 2000);
                    if (SystemInterface.debug) System.out.println("SimpleIO:Opened Default Serial Port.");

                    //Try to configure port
                    serialPort.setSerialPortParams(57600, 
                                                SerialPort.DATABITS_8, 
                                                SerialPort.STOPBITS_1, 
                                                SerialPort.PARITY_NONE);
                    if (SystemInterface.debug) System.out.println("SimpleIO:Serial Port Configured.");

                    //Try to open output stream
                    output = serialPort.getOutputStream();
                    if (SystemInterface.debug) System.out.println("SimpleIO:Output Stream Opened");

                    //Try to open input stream
                    input = serialPort.getInputStream();
                    if (SystemInterface.debug) System.out.println("SimpleIO:Input Stream Opened.");

                    if (SystemInterface.debug) System.out.println("SimpleIO:Serial Port " + defaultPort +" Ready");
                    
                    portFound = true;
                } 
                //If the port is in use, exit
                catch (PortInUseException pIUE) {
                    System.out.println("Default Port in use: " + pIUE.getMessage());
                } 
                catch (UnsupportedCommOperationException uSCOE) {
                    System.out.println("Serial Port Configuration Failed: " + uSCOE.getMessage());
                }
                catch (IOException iOE) {
                    System.out.println("Input/Output Stream Could Not Be Opened: " + iOE.getMessage());
                }
                catch (Exception e){
                    System.out.println("Unknow error while opening default comm port: " + e.getMessage());
                } 
            }
        }

        //Port was not found
        if (!portFound) {
            System.out.println("Default Port " + defaultPort + " not found.");
        } 
    }
    
    protected synchronized void disconnect(){
        if (serialPort != null){
            try{
                output.close();
                input.close();
            }
            catch(Exception e){
                System.out.println("Unable to close Input/Output Streams properly: " + e.getMessage());
            }

            serialPort.close();
        }	
    }
}
