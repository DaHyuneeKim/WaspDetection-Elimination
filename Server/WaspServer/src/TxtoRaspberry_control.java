import java.io.IOException;
import java.io.OutputStreamWriter;

public class TxtoRaspberry_control extends Thread {
	public void run()
	{
	try 
	{ 
		int count = 0;
		while (count!=1000000) 
		{
			Thread.sleep(20);
				if(main.bufferQ.Queue_control.isEmpty())
				{
					//System.out.println("이까지 진입");
					continue;
				}
				else
				{
					synchronized(main.bufferQ.Queue_control) 
					{
							int send_control = main.bufferQ.Queue_control.remove();
								main.outWriter_Rasp.write(send_control);
								main.outWriter_Rasp.flush();
								System.out.println("send_control : "+send_control);

					}
					count++;
				}
		}
	}
	catch(Exception e)
	{
		System.out.println(e.getMessage());
	}
		
			
			
	}

}
