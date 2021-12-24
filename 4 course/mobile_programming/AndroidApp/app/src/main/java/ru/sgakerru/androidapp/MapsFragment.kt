package ru.sgakerru.androidapp

import android.content.Context
import android.content.pm.PackageManager
import android.location.Location
import android.location.LocationListener
import android.location.LocationManager
import android.os.Build
import androidx.fragment.app.Fragment

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.core.app.ActivityCompat

import com.google.android.gms.maps.CameraUpdateFactory
import com.google.android.gms.maps.GoogleMap
import com.google.android.gms.maps.OnMapReadyCallback
import com.google.android.gms.maps.SupportMapFragment
import com.google.android.gms.maps.model.LatLng
import com.google.android.gms.maps.model.MarkerOptions

class MapsFragment : Fragment()
{
    /// Код реквеста на разрешение к геолокации.
    private val requestAccessLocationCode = 1;

    /// Гугл карта.
    private var googleMap: GoogleMap? = null;

    private val callback = OnMapReadyCallback { map ->
        this.googleMap = map;

        checkPermission();
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View?
    {
        return inflater.inflate(R.layout.fragment_maps, container, false);
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?)
    {
        super.onViewCreated(view, savedInstanceState);
        val mapFragment = childFragmentManager.findFragmentById(R.id.map) as SupportMapFragment?;
        mapFragment?.getMapAsync(callback);
    }

    /// Проверяем разрешение к геолокации.
    private fun checkPermission()
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)
        {
            if (ActivityCompat.checkSelfPermission
                    (activity!!, android.Manifest.permission.ACCESS_FINE_LOCATION)
                != PackageManager.PERMISSION_GRANTED
            )
            {
                requestPermissions(
                    arrayOf(android.Manifest.permission.ACCESS_FINE_LOCATION),
                    requestAccessLocationCode
                );
                return;
            }
        }

        handleUserLocation();
    }

    /// Результат реквеста на разрешение доступа к геолокации.
    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    )
    {
        if (requestCode == requestAccessLocationCode)
        {
            if (grantResults[0] == PackageManager.PERMISSION_GRANTED)
            {
                handleUserLocation();
            }
            else
            {
                Toast.makeText(
                    activity,
                    "Не удалось получить доступ к вашей геолокации!",
                    Toast.LENGTH_LONG
                ).show();
            }
        }

        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    /// Переместить карту к новой точке.
    private fun moveMapToNewLocation(location: Location)
    {
        googleMap!!.clear();
        val locationPos = LatLng(location.latitude, location.longitude);
        googleMap!!.addMarker(
            MarkerOptions()
                .position(locationPos)
                .title("Это вы!")
        );
        googleMap!!.moveCamera(CameraUpdateFactory.newLatLngZoom(locationPos, 15f));
    }

    /// Привязываемся к изменениям геолокации юзера.
    private fun handleUserLocation()
    {
        val myLocation = MyLocationListener();
        val locationManager: LocationManager =
            activity!!.getSystemService(Context.LOCATION_SERVICE) as LocationManager;
        // Привязываемся к геолокации по сети.
        locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 3, 5f, myLocation);
        // Привязываемся к GPS геолокации.
        locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 3, 5f, myLocation);
    }

    inner class MyLocationListener : LocationListener
    {
        /// Текущая геолокация.
        private var location: Location? = null;

        constructor()
        {
            location = Location("Start");
            location!!.latitude = 0.0;
            location!!.longitude = 0.0;
        }

        override fun onLocationChanged(loc: Location?)
        {
            location = loc;
            moveMapToNewLocation(location!!);
            Log.i("MYLOG", "onLocationChanged: ${loc.toString()}");
            if (activity != null)
            {
                Toast.makeText(activity, "onLocationChanged: ${loc.toString()}", Toast.LENGTH_LONG).show();
                Toast.makeText(activity, "onLocationChanged: ${loc.toString()}", Toast.LENGTH_LONG).show();
            }
        }

        override fun onStatusChanged(provider: String?, status: Int, extras: Bundle?)
        {
            Log.i("MYLOG", "onStatusChanged: $provider, $status");
            if (activity != null)
            {
                Toast.makeText(activity, "onStatusChanged: $provider, $status", Toast.LENGTH_LONG).show();
            }
        }

        override fun onProviderEnabled(provider: String?)
        {
            Log.i("MYLOG", "onProviderEnabled: $provider");
            if (activity != null)
            {
                Toast.makeText(activity, "onProviderEnabled: $provider", Toast.LENGTH_LONG).show();
            }
        }

        override fun onProviderDisabled(provider: String?)
        {
            Log.i("MYLOG", "onProviderDisabled: $provider");
            if (activity != null)
            {
                Toast.makeText(activity, "onProviderDisabled: $provider", Toast.LENGTH_LONG).show();
            }
        }
    }
}
