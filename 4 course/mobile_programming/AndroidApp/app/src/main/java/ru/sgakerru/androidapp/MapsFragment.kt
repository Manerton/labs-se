package ru.sgakerru.androidapp

import android.content.pm.PackageManager
import android.os.Build
import androidx.fragment.app.Fragment

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.core.app.ActivityCompat

import com.google.android.gms.maps.CameraUpdateFactory
import com.google.android.gms.maps.OnMapReadyCallback
import com.google.android.gms.maps.SupportMapFragment
import com.google.android.gms.maps.model.LatLng
import com.google.android.gms.maps.model.MarkerOptions

class MapsFragment : Fragment()
{
    private val requestAccessLocationCode = 123;

    private val callback = OnMapReadyCallback { googleMap ->
        val sydney = LatLng(-34.0, 151.0);
        googleMap.addMarker(MarkerOptions().position(sydney).title("Marker in Sydney"));
        googleMap.moveCamera(CameraUpdateFactory.newLatLng(sydney));
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

        checkPermission();
    }

    private fun checkPermission()
    {
        if (Build.VERSION.SDK_INT >= 23)
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
            }
            return;
        }

        getUserLocation();
    }

    private fun getUserLocation()
    {
        Toast.makeText(activity, "Геолокация.", Toast.LENGTH_LONG).show();
        // TODO: ...
    }

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
                getUserLocation();
            }
            else
            {
                Toast.makeText(activity, "Не удалось получить доступ к вашей геолокации!", Toast.LENGTH_LONG).show();
            }
        }

        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
    }
}