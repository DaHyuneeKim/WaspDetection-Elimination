package com.example.wasp2;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import java.net.Socket;

public class SplashActivity extends Activity {
    //static Socket socket;  //소켓생성
    @Override
    protected  void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);

        try {
            Thread.sleep(3500);
        }catch(InterruptedException e){
            e.printStackTrace();

        }

        startActivity(new Intent(this,MainActivity.class));
        finish();
    }

}
