package com.example.nicholasjn.imu_hexapodv4;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.Intent;
import android.content.IntentFilter;
import android.view.View;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    private int i;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ArrayList mDevice = new ArrayList();
        //Bluetooth Setup
        public BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(mBluetoothAdapter == null){
            //This device does'n support bluetooth
            //Insert your error command below
        }
        else{
            if(!mBluetoothAdapter.isEnabled()){
                Intent enableBtIntent = new Intent((BluetoothAdapter.ACTION_REQUEST_ENABLE));
                startActivityForResult(enableBtIntent, 1);
            }
            Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
            if(pairedDevices.size() > 0){
                for(BluetoothDevice device : pairedDevices){
                    mDevice = device;
                }
            }
            else{
                private final BroadcastReceiver mReceiver = new BroadcastReceiver(){
                    public void onReceive(Context context, Intent intent){
                        String action = intent.getAction();
                        //When discovery finds a device
                        if(BluetoothDevice.ACTION_FOUND.equals(action)){
                            //Get the BluetoothDevice object from the Intent
                            BluetoothDevice device = intent.getParcelableArrayExtra(BluetoothDevice.EXTRA_DEVICE);
                            //add the name and address to mDevice list
                            mDevice = device;
                        }
                    }
                };

                //Register the BroadcastReceiver
                IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND)'
                registerReceiver(mReceiver, filter);
            }
        }


    }
}

//Class to establish a connection to paired device in separate thread
private class ConnectThread extends Thread {
    private final BluetoothSocket mmSocket;
    private final BluetoothDevice mmDevice;
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-" +
            "00805f9b34fb");

    public ConnectThread(BluetoothDevice device){
        //Use temp variabel because mmDevice is final
        BluetoothSocket temp = null;
        mmDevice = device;

        try{
            //I assigned UUID that contain string as above to MY_UUID
            temp = device.createRfcommSocketToServiceRecord(MY_UUID);
        } catch (IOException e){}
        mmSocket = temp;
    }

    public void run(){
        mBluetoothAdapter.cancelDiscovery();

        try {
            mmSocket.connect();
        } catch (IOException connectException){
            //Unable to connect; close the socket and get out
            try{
                mmSocket.close();
            } catch (IOException closeException){}
            return;
        }

        manageConnectedSocket(mmSocket);
    }

    public void cancel(){
        try {
            mmSocket.close();
        } catch (IOException e){}
    }
}

private class ConnectedThread extends Thread {
    private final BluetoothSocket mmSocket;
    private final InputStream mmInStream;
    private final OutputStream mmOutStream;

    public ConnectedThread(BluetoothSocket socket){
        mmSocket = socket;
        InputStream tempIn = null;
        OutputStream tempOut = null;

        //Use temp object because the real is final
        try {
            tempIn = socket.getInputStream();
            tempOut = socket.getOutputStream();
        } catch (IOException e) {}

        mmInStream = tempIn;
        mmOutStream = tempOut;
    }

    //Method to send data to robot.
    public void writeBt(byte[] bytes){
        try {
            mmOutStream.write(bytes);
        } catch (IOException e){}
    }

    //Method to read data from robot
    public void readBt(){
        byte[] buffer = new byte[1024];
        int bytes;

        while(1){
            try{
                bytes = mmInStream.read(buffer);
                mHandler.obtainMessage(MESSAGE_READ, bytes -1, buffer);
                        .sendToTarget();
            } catch (IOException e){
                break;
            }
        }
    }

    //Method to shutdown connection.
    public void cancel(){
        try {
            mmSocket.close();
        } catch (IOException e){}
    }
}