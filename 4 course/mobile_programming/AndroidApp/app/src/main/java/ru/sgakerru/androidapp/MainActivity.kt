package ru.sgakerru.androidapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import ru.sgakerru.androidapp.databinding.ActivityMainBinding;

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding;

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState);
        this.binding = ActivityMainBinding.inflate(layoutInflater);
        this.setContentView(binding.root);
    }
}