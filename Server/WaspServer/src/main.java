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
	
	public static ServerSocket serverSocket_Rasp = null; // 서버소켓 생성
	public static Socket RaspSocket = null; // 서버소켓이 지정한 포트를 타고온 상대 ip를 저장
	public static OutputStream out_Rasp = null; // String 타입의 문자를 보낼수 있는 함수.
	public static InputStream in_Rasp = null;// stream 타입의 문자를 읽어서 저장할 수 있는 함수.
	public static OutputStreamWriter outWriter_Rasp = null;
	public static InputStreamReader inReader_Rasp = null;
	
	public static ServerSocket serverSocket_App = null; // 서버소켓 생성
	public static Socket appclientSocket = null; // 서버소켓이 지정한 포트를 타고온 상대 ip를 저장
	public static OutputStream out_App = null; // String 타입의 문자를 보낼수 있는 함수.
	public static InputStream in_App = null;// stream 타입의 문자를 읽어서 저장할 수 있는 함수.
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
	
	public static void main(String[] args) { // 메인함수, IOException
		
		
		connectSocket_app.start();
		connectSocket_rasp.start();


		Rxfrom_Rasp.start();
		TxtoApp_Temp.start();
	}

}