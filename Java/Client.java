
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

class Client
{
	public static void main(String[] args)
	{
		// Connect the socket
		Socket s = null;
		DataOutputStream dos = null;
		DataInputStream dis = null;

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
		}
		catch (Exception e) {
			System.out.println("dos/dis Error:" + e.toString());
		}
		System.out.println("Obtained dis and dos");

		sendAndRecieve("ls", dos, dis);
	    //sendAndRecieve("cd foldername");
	    //sendAndRecieve("mkdir newFolder");
	}

	public static void sendAndRecieve(String message, DataOutputStream dos, DataInputStream dis)
	{
	    final String messageToSend = message;

	    new Thread(new Runnable()
	    {
	        public void run()
	        {
	            try
	            {
					// Write to the socket
					dos.writeUTF(message);
					dos.flush();
					System.out.println("Sent the message: " + message);

					String message = "";
					while(dis.available() <= 0)
					{
						System.out.print("#");
					}

					System.out.println("\n\nMessage:");
					
					while(dis.available() > 0)
					{
						message += (char) dis.readByte();
					}

					System.out.println(message);

					dis.close();
					dos.close();
	            }
	            catch(Exception e)
	            {
	                System.out.println("Error:" + e.toString());
	            }
	        }
	    }).start();
	}
}
