package com.example.wasp2;

import android.app.Activity;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.renderscript.ScriptGroup;
import android.support.annotation.StringRes;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Queue;
import java.util.LinkedList;



public class MainActivity extends AppCompatActivity {
    private Socket socket;  //소켓생성
    private Socket videosocket;
    InputStream in;        //서버로부터 온 데이터를 읽는다.
    OutputStream out;        //서버에 데이터를 전송한다.
    InputStream in_video;
    Button button;          //화면구성
    TextView temp;        //화면구성
    Button button2;          //화면구성
    Button button3;          //화면구성
    boolean isserverConnected = false;
    public static Queue<Integer> TxQueue = new LinkedList <Integer>();
    public static Queue<videoPacket> RxQueue_video = new LinkedList <videoPacket>();
    public static String temp_String;
    public static  Bitmap imageBitmap;
    public static Canvas CacheCanvas ;
    public static Paint mPaint;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        button = (Button) findViewById(R.id.button); // 말벌퇴치 버튼
        button2 =  (Button) findViewById(R.id.button2); // 말벌 치우기 버튼

        temp = (TextView) findViewById(R.id.text2); //온도 정보 받아서 띄우기




        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { //말벌퇴치버튼이 클릭되면 소켓에 데이터를 출력한다.
                new Thread() {
                    public void run() {
                        int data_Work = 0;
                        synchronized(TxQueue) {
                            TxQueue.add(data_Work);
                        }

                    }
                }.start();
            }


        });
        button2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) { //말벌 치우기 버튼이 클릭되면 소켓에 데이터를 출력한다.
                new Thread() {
                    public void run() {
                        int data_Work = 1;
                        synchronized(TxQueue) {
                            TxQueue.add(data_Work);
                        }

                    }
                }.start();
            }
        });





        Thread worker = new Thread() {    //worker 를 Thread 로 생성
            public void run() { //스레드 실행구문
//소켓을 생성하고 입출력 스트립을 소켓에 연결한다.
                try {
                    String serverIP = "192.168.0.102";
                    int port = 50000;
                    int video_port = 57777;
                    socket = new Socket(serverIP,port); //소켓생성
                    videosocket = new Socket(serverIP,video_port);
                    in = socket.getInputStream();
                    in_video = videosocket.getInputStream();
                    out = socket.getOutputStream(); //데이터를 전송시 stream 형태로 변환하여 전송한다
                    isserverConnected = true;
                    OutputStreamWriter outReader = new OutputStreamWriter(out);
                    outReader.flush();
                    while (true) {
                        int transfer_data;
                        if (TxQueue.isEmpty()) {
                            continue;
                        }
                        synchronized (TxQueue) {
                            transfer_data = TxQueue.remove();
                        }
                        try {

                            outReader.write(transfer_data);
                            outReader.flush();
                        } catch (IOException e) {
                            System.out.println("데이터 전송 실패\n");
                        }

                    }
                }catch(Exception e)
                {

                }

            }
        };

        Thread getTempfromRasp = new Thread()
        {
            public void run()
            {
                try {
                    byte temper_byte[] = new byte[8];

                        while(true)
                        {
                            sleep(20);
                            if(isserverConnected == true)
                            {
                                in.read(temper_byte,0,8);
                                temp_String = new String(temper_byte);
                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        temp.setText(temp_String.toString());
                                        temp.invalidate();
                                    }
                                });
                             }
                        }
                }
                catch(Exception e)
                {
                    System.out.println(e.getMessage());
                }
            }

        };
        Thread video_get = new Thread()
        {
            public void run()
            {
                try{ //이미지 띄우기 코드
                    // is=am.open("Bee1.jpeg");
                    sleep(50);
                    int count = 0;
                    videoPacket videoPk = new videoPacket();
                    while (true) {
                        in_video.read(videoPk.imagedata, 0, 1020);
                        synchronized (RxQueue_video) {
                            RxQueue_video.add(videoPk);
                           // System.out.println("put : " + count);
                            count++;
                        }
                    }

                }catch(Exception e){
                    e.printStackTrace();
                }
            }

        };

        Thread video_print = new Thread(){
            public void run()
            {
                try {
                    sleep(50);
                    videoPacket videoPk = new videoPacket();
                    int over= 0;
                    byte video [] = new byte[51000];
                    while (true)
                    {
                        sleep(1);
                        if (over == 1) {
                            break;
                        }
                        int count = 0;
                        for(int i = 0; i<50;)
                        {
                           // System.out.println("queueisempty: "+RxQueue_video.isEmpty());
                            if (RxQueue_video.isEmpty() == false)
                            {
                                synchronized (RxQueue_video)
                                {
                                    //sleep(1);
                                    videoPk = RxQueue_video.remove();
                                   // System.out.println("pop : " + i);
                                    i++;
                                   // System.out.println(i);
                                }
                                for (int j = 0; j < 1020; j++)
                                {
                                    video[(i-1) * 1020 + j] = videoPk.imagedata[j];
                                   // System.out.println("냥 :" + j);
                                }
                               // System.out.println("이 for문 끝남");
                            }
                        }
                        //System.out.println("이까지");
                            int nrOfPixels = video.length / 3;
                            int pixels[] = new int[nrOfPixels];
                            for (int i = 0; i < nrOfPixels; i++) {
                                int b = (int) (0xFF & video[3 * i]);
                                int g = (int) (0xFF & video[3 * i + 1]);
                                int r = (int) (0xFF) & video[3 * i + 2];
                                pixels[i] = Color.rgb(r, g, b);
                            }

                            imageBitmap = Bitmap.createBitmap(pixels, 170, 100, Bitmap.Config.ARGB_4444);
                            // runOnUiThread(new Runnable() {
                            //   @Override
                            //  public void run() {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                ImageView imageView = (ImageView) findViewById(R.id.image2);
                                imageView.setImageBitmap(imageBitmap);
                            }
                        });
                            // CacheCanvas = new Canvas();
                            // mPaint = new Paint();
                            // CacheCanvas.setBitmap(imageBitmap);
                            // CacheCanvas.drawBitmap(imageBitmap,0,0,mPaint);
                            //    }
                            // });
                        }
                }

                catch(Exception e)
                {
                    System.out.println(e.getMessage());
                }
            }
        };

        worker.start();
        getTempfromRasp.start();
        video_get.start();
        video_print.start();


    }

    protected void onStop() {  //앱 종료시
        super.onStop();
        try {
            socket.close(); //소켓을 닫는다.
            out.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

