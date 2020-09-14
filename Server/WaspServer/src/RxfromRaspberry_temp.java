import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.LinkedList;
import java.util.Queue;

public class RxfromRaspberry_temp extends Thread {
	
	public void run() {
	
		
		try {
		byte[] temp = new byte[8];
		int count = 0;
		while(count!=100000)
		{
			sleep(20);
			if(main.isRaspconnected == true )
			{
				main.in_Rasp.read(temp,0,8);
				String temper = new String(temp);
				System.out.println("Received Temperature : "+ temper);
			}
		}
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage());
		}
	}


}
