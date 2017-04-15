package com.athenajth.drot;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;

import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.os.AsyncTask;
import java.io.IOException;
import java.util.UUID;

public class ledControl extends AppCompatActivity
{
    Button btnOn, btnOff, btnDis;
    SeekBar brightness;
    String address = null;
    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb"); //ssp uiud

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        //receive addr of bluetooth device got in DeviceList class
        Intent newint = getIntent();
        address = newint.getStringExtra(DeviceList.EXTRA_ADDRESS);

        //view of ledControl layout
        setContentView(R.layout.activity_led_control);

        //call widgets
        btnOn = (Button)findViewById(R.id.button_led_on);
        btnOff = (Button)findViewById(R.id.button_led_off);
        btnDis = (Button)findViewById(R.id.button_led_disconnect);
        brightness = (SeekBar)findViewById(R.id.seekBar);

//        brightness.setOnSeekBarChangeListener(
//                new SeekBar.OnSeekBarChangeListener()
//                {
//                    @Override
//                    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser)
//                    {
//                        if(fromUser == true)
//                        {
//                            lumn.setText(String.valueOf(progress));
//                            try
//                            {
//                                btSocket.getOutputStream().write(String.valueOf(progress).getBytes());
//                            }
//                            catch (IOException e) {}
//                        }
//                    }
//
//                    @Override
//                    public void onStartTrackingTouch(SeekBar seekBar) {}
//
//                    @Override
//                    public void onStopTrackingTouch(SeekBar seekBar) {}
//                }
//
//        );

//        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
//        setSupportActionBar(toolbar);
//
//        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
//        fab.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAction("Action", null).show();
//            }
//        });
    }


    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
    }

    private void Disconnect()
    {
        if(btSocket != null)    //if btsocket's busy
        {
            try
            {
                btSocket.close();   //close connection
            }
            catch (IOException e)
            {
                msg("Error in disconnect");
            }
            finish();   //return to the first layout
        }
    }

    private void turnOffLed()
    {
        if(btSocket != null)
        {
            try
            {
                btSocket.getOutputStream().write("TF".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error in turnOffLED");
            }
        }
    }

    private void turnOnLed()
    {
        if(btSocket != null)
        {
            try
            {
                btSocket.getOutputStream().write("TO".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("error in turnOnLed");
            }
        }
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void>    //UI thread
    {
        private boolean ConnectSuccess = true;

        @Override   //intent to override method in superclass
        protected void onPreExecute()
        {
            progress = ProgressDialog.show(ledControl.this, "Connecting...", "Please wait!");
        }

        @Override
        protected Void doInBackground(Void... devices)  //zero or more devices can be passed as parameter
        {
            try
            {
                if(btSocket == null || !isBtConnected)
                {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();
                }
            }
            catch(IOException e)    //if something in try block throws exception
            {
                ConnectSuccess = false;
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result)
        {
            super.onPostExecute(result);
            if(!ConnectSuccess)
            {
                msg("Connection Failed. Is it a SPP Bluetooth");
                finish();
            }
            else
            {
                msg("Connected. ");
                isBtConnected = true;
            }
            progress.dismiss();
        }
    }

}
