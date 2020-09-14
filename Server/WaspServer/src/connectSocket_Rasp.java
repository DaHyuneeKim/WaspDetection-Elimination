import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.io.InputStreamReader;
public class connectSocket_Rasp extends Thread {
	public void run()
	{
		try 
		{
			main.serverSocket_Rasp = new ServerSocket(55555); // Server - Rasp port : 55555
			System.out.println("Raspberry 클라이언트 연결 대기중");
			main.RaspSocket = main.serverSocket_Rasp.accept(); // 클라이언트로부터 데이터가 오는것을 감지한다.
			System.out.println("Raspberry 클라이언트 연결 (연결된 IP : " + main.RaspSocket.getInetAddress() + ", Port : "+main.RaspSocket.getPort() + ")");// 클라이언트로 부터 데이터를 받는다.
			main.out_Rasp = main.RaspSocket.getOutputStream(); // String 타입을 stream 형태로 변환하여 //전송한다는 뜻.
			main.in_Rasp = main.RaspSocket.getInputStream(); // 소켓에서 넘오는 stream 형태의 문자를 얻은																		// 후 읽어 들어서 bufferstream 형태로
			main.outWriter_Rasp = new OutputStreamWriter(main.out_Rasp);
			main.isRaspconnected = true;
			main.TxtoRaspberrycontrol.start();
			Thread.sleep(20);
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage());
		}
	}

}
