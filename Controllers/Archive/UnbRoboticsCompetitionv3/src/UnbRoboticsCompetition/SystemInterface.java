/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package UnbRoboticsCompetition;

/**
 *
 * @author adam
 */
public class SystemInterface {
    
    protected static boolean debug = false;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        ControllerSetup ContSet = new ControllerSetup();
        
        //To-Do have setup return -1 0 1 for status and verify here
        ContSet.setup();
        
        //To-Do change run() to be exitable from here or by calling the close() method
        ContSet.run();
    }
}