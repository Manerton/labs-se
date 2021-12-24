package ru.sgakerru.androidapp

import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import ru.sgakerru.androidapp.databinding.FragmentSensorBinding

class SensorFragment : Fragment()
{
    private lateinit var binding: FragmentSensorBinding;
    private var sensors = HashMap<Int, Sensor>();
    private lateinit var sensorManager: SensorManager;
    private lateinit var sensorEventListener: MySensorEventListener;

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState);
        sensorManager = context!!.getSystemService(Context.SENSOR_SERVICE) as SensorManager;
        sensors[Sensor.TYPE_LIGHT] = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);
        sensors[Sensor.TYPE_ACCELEROMETER] = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sensorEventListener = MySensorEventListener();
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View?
    {
        binding = FragmentSensorBinding.inflate(inflater, container, false);
        return binding.root;
    }

    override fun onResume()
    {
        super.onResume();

        for (sensor in sensors.values)
        {
            sensorManager.registerListener(sensorEventListener, sensor, SensorManager.SENSOR_DELAY_NORMAL);
        }
    }

    override fun onPause()
    {
        super.onPause();
        sensorManager.unregisterListener(sensorEventListener);
    }

    inner class MySensorEventListener : SensorEventListener
    {
        override fun onSensorChanged(event: SensorEvent?)
        {
            when (event?.sensor?.type)
            {
                Sensor.TYPE_LIGHT ->
                {
                    binding.textViewLightValue.text = event.values[0].toString();
                }
                Sensor.TYPE_ACCELEROMETER ->
                {
                    binding.textViewAcelXValue.text = event.values[0].toString();
                    binding.textViewAcelYValue.text = event.values[1].toString();
                    binding.textViewAcelZValue.text = event.values[2].toString();
                }
            }
        }

        override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int)
        {

        }
    }
}