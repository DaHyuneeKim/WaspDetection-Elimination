import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.io.InputStreamReader;
public class connectSocket_Rasp extends Thread {
	public void run()
	{
		try 
		{
			main.serverSocket_Rasp = new ServerSocket(55555); // Server - Rasp port : 55555
			System.out.println("Raspberry Ŭ���̾�Ʈ ���� �����");
			main.RaspSocket = main.serverSocket_Rasp.accept(); // Ŭ���̾�Ʈ�κ��� �����Ͱ� ���°��� �����Ѵ�.
			System.out.println("Raspberry Ŭ���̾�Ʈ ���� (����� IP : " + main.RaspSocket.getInetAddress() + ", Port : "+main.RaspSocket.getPort() + ")");// Ŭ���̾�Ʈ�� ���� �����͸� �޴´�.
			main.out_Rasp = main.RaspSocket.getOutputStream(); // String Ÿ���� stream ���·� ��ȯ�Ͽ� //�����Ѵٴ� ��.
			main.in_Rasp = main.RaspSocket.getInputStream(); // ���Ͽ��� �ѿ��� stream ������ ���ڸ� ����																		// �� �о� �� bufferstream ���·�
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
