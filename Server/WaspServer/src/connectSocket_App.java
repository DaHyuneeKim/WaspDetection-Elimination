import java.io.InputStreamReader;
import java.net.ServerSocket;

public class connectSocket_App extends Thread {
	public void run()
	{
		try 
		{
			main.serverSocket_App = new ServerSocket(50000); // Server - App Port : 50000
			System.out.println("App Ŭ���̾�Ʈ ���� �����");
			main.appclientSocket = main.serverSocket_App.accept(); // Ŭ���̾�Ʈ�κ��� �����Ͱ� ���°��� �����Ѵ�.
			System.out.println("App Ŭ���̾�Ʈ ���� (����� IP : " + main.appclientSocket.getInetAddress() + ", Port : "+main.appclientSocket.getPort() + ")");// Ŭ���̾�Ʈ�� ���� �����͸� �޴´�.
			main.out_App = main.appclientSocket.getOutputStream(); // String Ÿ���� stream ���·� ��ȯ�Ͽ� //�����Ѵٴ� ��.
			main.in_App = main.appclientSocket.getInputStream(); // ���Ͽ��� �ѿ��� stream ������ ���ڸ� ����																		// �� �о� �� bufferstream ���·�
			main.inReader_App = new InputStreamReader(main.in_App);	// in �� ����.	
			main.isAppconnected = true;
			main.Rxfrom_App_control.start();
			Thread.sleep(20);
		}
		catch(Exception e)
		{// �� �о� �� bufferstream ���·�
			System.out.println(e.getMessage());
		}
	}

}
