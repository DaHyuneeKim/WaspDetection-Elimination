import java.io.InputStreamReader;
import java.net.ServerSocket;

public class connectSocket_App extends Thread {
	public void run()
	{
		try 
		{
			main.serverSocket_App = new ServerSocket(50000); // Server - App Port : 50000
			System.out.println("App 클라이언트 연결 대기중");
			main.appclientSocket = main.serverSocket_App.accept(); // 클라이언트로부터 데이터가 오는것을 감지한다.
			System.out.println("App 클라이언트 연결 (연결된 IP : " + main.appclientSocket.getInetAddress() + ", Port : "+main.appclientSocket.getPort() + ")");// 클라이언트로 부터 데이터를 받는다.
			main.out_App = main.appclientSocket.getOutputStream(); // String 타입을 stream 형태로 변환하여 //전송한다는 뜻.
			main.in_App = main.appclientSocket.getInputStream(); // 소켓에서 넘오는 stream 형태의 문자를 얻은																		// 후 읽어 들어서 bufferstream 형태로
			main.inReader_App = new InputStreamReader(main.in_App);	// in 에 저장.	
			main.isAppconnected = true;
			main.Rxfrom_App_control.start();
			Thread.sleep(20);
		}
		catch(Exception e)
		{// 후 읽어 들어서 bufferstream 형태로
			System.out.println(e.getMessage());
		}
	}

}
