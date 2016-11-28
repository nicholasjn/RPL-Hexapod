package com.example.nicholasjn.imu_hexapodv4;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.content.IntentFilter;
import android.view.View;
import java.util.ArrayList;
import java.util.Set;
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
        BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
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

