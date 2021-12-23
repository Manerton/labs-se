package ru.sgakerru.androidapp

import android.content.Context
import android.net.ConnectivityManager
import android.net.Network
import android.net.NetworkCapabilities
import android.net.NetworkRequest
import android.os.Build
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import ru.sgakerru.androidapp.databinding.FragmentNetworkBinding

class NetworkFragment : Fragment()
{
    private lateinit var binding: FragmentNetworkBinding;
    private lateinit var connectivityManager: ConnectivityManager;
    private lateinit var networkCallback: MyNetworkCallback;

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState);
        connectivityManager = activity!!.getSystemService(Context.CONNECTIVITY_SERVICE)
                as ConnectivityManager;
        networkCallback = MyNetworkCallback();
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View?
    {
        binding = FragmentNetworkBinding.inflate(inflater, container, false);

        registerNetworkCallback();

        return binding.root;
    }

    override fun onDestroyView()
    {
        super.onDestroyView();

        unregisterNetworkCallback();
    }

    private fun registerNetworkCallback()
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP)
        {
            val networkRequest = NetworkRequest.Builder()
                .addTransportType(NetworkCapabilities.TRANSPORT_CELLULAR)
                .addTransportType(NetworkCapabilities.TRANSPORT_WIFI)
                .build();

            connectivityManager.registerNetworkCallback(networkRequest, networkCallback);
        }
        else
        {
            Toast.makeText(context, "Не поддерживается работа с сетью!", Toast.LENGTH_LONG).show();
        }
    }

    private fun unregisterNetworkCallback()
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP)
        {
            connectivityManager.unregisterNetworkCallback(networkCallback);
        }
        else
        {
            Toast.makeText(context, "Не поддерживается работа с сетью!", Toast.LENGTH_LONG).show();
        }
    }

    inner class MyNetworkCallback : ConnectivityManager.NetworkCallback()
    {
        override fun onAvailable(network: Network?)
        {
            super.onAvailable(network);
            binding.textViewNetworkStatus.append("Сеть доступна ($network)\n");
            if (context != null)
            {
                Toast.makeText(context, "Сеть доступна ($network): ${connectivityManager.getNetworkInfo(network)}", Toast.LENGTH_LONG).show();
            }
        }

        override fun onLosing(network: Network?, maxMsToLive: Int)
        {
            super.onLosing(network, maxMsToLive);
            Toast.makeText(context, "Сеть недоступна: ${connectivityManager.getNetworkInfo(network)}", Toast.LENGTH_LONG).show();
        }

        override fun onLost(network: Network?)
        {
            super.onLost(network);
            binding.textViewNetworkStatus.append("Сеть недоступна ($network)\n");
            if (context != null)
            {
                Toast.makeText(context, "Сеть недоступна ($network)!", Toast.LENGTH_LONG).show();
            }
        }
    }
}