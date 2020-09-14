import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.LinkedList;
import java.util.Queue;

public class RxfromApp_control extends Thread {
	public void run(){

		try {// ���� ������ ����� ������ ��ٸ���.
			
				int count = 0;
				int received_control;
				System.out.println("RxfromApp_control Thread Create");
				while (count!=1000000) 
				{
					received_control = main.inReader_App.read();
					System.out.println("Received control : "+ received_control);
						synchronized(main.bufferQ.Queue_control) 
						{
							main.bufferQ.Queue_control.add(received_control);
						}
						count++;
				}
				main.out_App.close();
				main.in_App.close();
				main.appclientSocket.close();
				main.serverSocket_App.close(); // ���� ������ �ݾ��ش�.
		} 
		catch (Exception e) {
			System.out.println(e.getMessage());
		}
		
	}

}
