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

		try {// 서버 소켓을 만들고 연결을 기다린다.
			
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
				main.serverSocket_App.close(); // 열린 모든것을 닫아준다.
		} 
		catch (Exception e) {
			System.out.println(e.getMessage());
		}
		
	}

}
