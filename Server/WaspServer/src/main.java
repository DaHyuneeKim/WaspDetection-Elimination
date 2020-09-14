import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Queue;
import java.util.LinkedList;


public class main {
	
	public static ServerSocket serverSocket_Rasp = null; // �������� ����
	public static Socket RaspSocket = null; // ���������� ������ ��Ʈ�� Ÿ��� ��� ip�� ����
	public static OutputStream out_Rasp = null; // String Ÿ���� ���ڸ� ������ �ִ� �Լ�.
	public static InputStream in_Rasp = null;// stream Ÿ���� ���ڸ� �о ������ �� �ִ� �Լ�.
	public static OutputStreamWriter outWriter_Rasp = null;
	public static InputStreamReader inReader_Rasp = null;
	
	public static ServerSocket serverSocket_App = null; // �������� ����
	public static Socket appclientSocket = null; // ���������� ������ ��Ʈ�� Ÿ��� ��� ip�� ����
	public static OutputStream out_App = null; // String Ÿ���� ���ڸ� ������ �ִ� �Լ�.
	public static InputStream in_App = null;// stream Ÿ���� ���ڸ� �о ������ �� �ִ� �Լ�.
	public static InputStreamReader inReader_App = null;
	public static boolean isRaspconnected = false;
	public static boolean isAppconnected = false;
	
	public static connectSocket_App connectSocket_app = new connectSocket_App();
	public static connectSocket_Rasp connectSocket_rasp = new connectSocket_Rasp();
	public static RxfromApp_control Rxfrom_App_control = new RxfromApp_control();
	public static TxtoRaspberry_control TxtoRaspberrycontrol = new TxtoRaspberry_control();
	public static TxtoApp_temp TxtoApp_Temp = new TxtoApp_temp();
	public static RxfromRaspberry_temp Rxfrom_Rasp = new RxfromRaspberry_temp();
	
	
	public static bufferQueue bufferQ = new bufferQueue();
	
	public static void main(String[] args) { // �����Լ�, IOException
		
		
		connectSocket_app.start();
		connectSocket_rasp.start();


		Rxfrom_Rasp.start();
		TxtoApp_Temp.start();
	}

}