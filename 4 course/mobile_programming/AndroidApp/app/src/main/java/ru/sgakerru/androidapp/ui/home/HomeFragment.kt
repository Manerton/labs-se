package ru.sgakerru.androidapp.ui.home

import android.arch.lifecycle.MutableLiveData
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import android.support.v4.app.Fragment
import android.arch.lifecycle.Observer
import android.arch.lifecycle.ViewModelProvider
import android.widget.Button
import android.widget.Toast
import ru.sgakerru.androidapp.R
import ru.sgakerru.androidapp.databinding.FragmentHomeBinding

class HomeFragment : Fragment()
{
    private lateinit var homeViewModel: HomeViewModel;
    private var _binding: FragmentHomeBinding? = null;

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!;

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View?
    {
        homeViewModel =
            ViewModelProvider(
                this,
                ViewModelProvider.NewInstanceFactory()
            ).get(HomeViewModel::class.java);

        _binding = FragmentHomeBinding.inflate(inflater, container, false);
        val root: View = binding.root;

        val textView: TextView = binding.textHome;
        homeViewModel.text.observe(viewLifecycleOwner, Observer {
            textView.text = it
        });

        return root;
    }

    override fun onDestroyView()
    {
        super.onDestroyView();
        _binding = null;
    }
}