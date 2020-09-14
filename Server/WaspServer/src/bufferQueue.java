import java.util.LinkedList;
import java.util.Queue;

public class bufferQueue {
	Queue<Integer> Queue_control = new LinkedList <Integer>();
	Queue<String> Tx_QueuetoAndroid_temp = new LinkedList <String>();
	Queue<String> Rx_QueuefromRaspberry_temp = new LinkedList <String>();
	Queue<videoPacket> Rx_QueuefromRaspberry_video = new LinkedList <videoPacket>();
	Queue<videoPacket> Tx_QueuetoAndroid_video = new LinkedList <videoPacket>();
}
