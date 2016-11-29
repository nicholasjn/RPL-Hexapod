package com.example.nicholasjn.imu_hexapodv4;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.hardware.SensorManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
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
    private SensorManager mSensorManager;
    private Sensor senAccelerometer;
    public BluetoothAdapter mBluetoothAdapter;
    private long lastUpdate = 0;
    private float last_x, last_y, last_z;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ArrayList mDevice = new ArrayList();


        //Bluetooth Setup

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
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
                IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
                registerReceiver(mReceiver, filter);
            }
        }

        //Set up accelerometer sensor
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        senAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

        while(true){
            //Write your code here
        }
    }

    public void onSensorChanged(SensorEvent sensorEvent){
        Sensor mSensor = sensorEvent.sensor;
        float x, y, z, moveThres;
        long curTime;

        if(mSensor.getType() == Sensor.TYPE_ACCELEROMETER){
            curTime = System.currentTimeMillis();
            if(curTime > 200){
                x = sensorEvent.values[0];
                y = sensorEvent.values[1];
                z = sensorEvent.values[2];
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
        BluetoothAdapter.cancelDiscovery();
        try {
            mmSocket.connect();
        } catch (IOException connectException){
            //Unable to connect; close the socket and get out
            try{
                mmSocket.close();
            } catch (IOException closeException){}
            return;
        }

//        manageConnectedSocket(mmSocket);
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
/*    public void readBt(){
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
*/
    //Method to shutdown connection.
    public void cancel(){
        try {
            mmSocket.close();
        } catch (IOException e){}
    }
}
/*
public void onSensorChanged(SensorEvent event){
    final float alpha = 0.8;

    gravity[0] = alpha * gravity[0] + (1 - alpha) * event.values[0];
    gravity[1] = alpha * gravity[1] + (1 - alpha) * event.values[1];
    gravity[2] = alpha * gravity[2] + (1 - alpha) * event.values[2];

    linear_acceleration[0] = event.values[0] - gravity[0];
    linear_acceleration[1] = event.values[1] - gravity[1];
    linear_acceleration[2] = event.values[2] - gravity[2];

}
*/