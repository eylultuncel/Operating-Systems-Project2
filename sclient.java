import java.io.IOException;
import java.net.Socket;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;

public class sclient {

    public static void main(String[] args) throws IOException, InterruptedException {

	//create new socket
        Socket socket = new Socket(args[1], Integer.parseInt(args[3]));
        Scanner in = new Scanner(socket.getInputStream());

        String buffer = "";
        int i =0;
	
	//after connecting with server, endless streaming of video starts
	//streaming doesn't end unless the client ends it 
        while (true){
	    //get every line in and concatenate 14 lines (1 frame)
            buffer = buffer.concat(in.nextLine());
            buffer = buffer.concat("\n");
            i++;
	    
	    //when there is 14 lines concetanated ended up to frame
 	    //then print the frame out to the terminal in endless way
            if(i==14){
                System.out.print(buffer);
                TimeUnit.MILLISECONDS.sleep(50);
                System.out.println("\033\143");
                buffer="";
                i=0;
            }
        }
    }
}
