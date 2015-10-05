<<<<<<< HEAD

=======
>>>>>>> eb432598125f0898fb266f9b2fa9d1b8d6eacea1
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import java.net.Socket;
import java.util.ArrayList;
<<<<<<< HEAD

=======
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.File;
import java.io.BufferedOutputStream;
import java.io.BufferedInputStream;
import java.net.ServerSocket;
>>>>>>> eb432598125f0898fb266f9b2fa9d1b8d6eacea1
class Client
{
	public static void main(String[] args)
	{
		// Connect the socket
		Socket s = null;
		DataOutputStream dos = null;
		DataInputStream dis = null;
<<<<<<< HEAD

=======
		FileOutputStream fos= null;
		FileInputStream fis = null;
		
>>>>>>> eb432598125f0898fb266f9b2fa9d1b8d6eacea1
		try {
			s = new Socket("127.0.0.1", 5000);
		}
		catch(Exception e) {
			System.out.println("Socket Error:" + e.toString());
		}
		System.out.println("Connected to socket");

		try {
			// Get the input and output streams
			dos = new DataOutputStream(s.getOutputStream());
			dis = new DataInputStream(s.getInputStream());
<<<<<<< HEAD
		}
		catch (Exception e) {
			System.out.println("dos/dis Error:" + e.toString());
=======
			
		}
		catch (Exception e) {
			System.out.println("input/output stream Error:" + e.toString());
>>>>>>> eb432598125f0898fb266f9b2fa9d1b8d6eacea1
		}
		System.out.println("Obtained dis and dos");

		sendAndRecieve("ls", dos, dis);
	    sendAndRecieve("mkdir newFolder", dos, dis);
	    sendAndRecieve("cd newFolder", dos, dis);
<<<<<<< HEAD
=======
	    
	    //putFile(s, dos, dis, "fromclient.txt");
	    getFile(s, dos, dis, "fromserver.txt");
	    putFile(s, dos, dis, "flower.jpg");
	    getFile(s, dos, dis, "stickman.png");
>>>>>>> eb432598125f0898fb266f9b2fa9d1b8d6eacea1
		closeSocket(dos, dis);
	}

	public static void sendAndRecieve(String message, DataOutputStream dos, DataInputStream dis)
	{
	    // final String messageToSend = message;
		//
	    // new Thread(new Runnable()
	    // {
	    //     public void run()
	    //     {
	            try
	            {
					// Write to the socket
					dos.writeUTF(message);
					dos.flush();
					System.out.println("Sent the message: " + message);

					System.out.println("Waiting for response");
					while(dis.available() <= 0) { }

					System.out.println("\nMessage:");

					String response = "";
					while(dis.available() > 0)
					{
						response += (char) dis.readByte();
					}

					System.out.println(response + "\n\n");
	            }
	            catch(Exception e) { System.out.println("Error:" + e.toString()); }
	    //     }
	    // }).start();
	}
<<<<<<< HEAD

=======
	public static void putFile(Socket s, DataOutputStream dos, DataInputStream dis, String f)
	{
		try
	    {
	    	File myFile = new File(f);
	    	byte[] mybytearray = new byte[(int) myFile.length()];
	    	BufferedInputStream bis = new BufferedInputStream(new FileInputStream(myFile));
	    	
	    	dos.writeUTF("put " + f);
			dos.flush();
			
			System.out.println("Sent the message: put " + f);
      		
      		bis.read(mybytearray, 0, mybytearray.length);
      		OutputStream os = s.getOutputStream();
      		
      		os.write(mybytearray, 0, mybytearray.length);
      		os.flush();
      		
      			dos.writeUTF("&endm;");
      			dos.flush();
      		
    	}
    	catch(Exception e) { System.out.println("Error:" + e.toString()); }
	}
	public static void getFile(Socket s, DataOutputStream dos, DataInputStream dis, String f)
	{
		try
	    {
			byte[] mybytearray = new byte[1024];
    		InputStream is = s.getInputStream();
    		
	    	dos.writeUTF("get " + f);
			dos.flush();
			System.out.println("Sent the message: get " + f);
			
    		FileOutputStream fos = new FileOutputStream(f);
    		
    		BufferedOutputStream bos = new BufferedOutputStream(fos);
    		
    		int bytesRead = is.read(mybytearray, 0, mybytearray.length);
    		
    		bos.write(mybytearray, 0, bytesRead);
    		bos.close();
		}
		catch(Exception e) { System.out.println("Error:" + e.toString()); }
	}
>>>>>>> eb432598125f0898fb266f9b2fa9d1b8d6eacea1
	public static void closeSocket(DataOutputStream dos, DataInputStream dis)
	{
		try
		{
			// Write to the socket
			dos.writeUTF("exit");
			dos.flush();

			dis.close();
			dos.close();
		}
		catch(Exception e) { System.out.println("Error:" + e.toString()); }
	}
}
