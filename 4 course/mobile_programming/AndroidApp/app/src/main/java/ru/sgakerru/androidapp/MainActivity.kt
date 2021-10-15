package ru.sgakerru.androidapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import ru.sgakerru.androidapp.databinding.ActivityMainBinding;
import android.view.View;
import android.widget.Toast;
import java.util.Calendar;

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding;

    private val buttonAgeOnClickListener = (View.OnClickListener {
        val year : Int? = this.binding.editTextDateAge.text.toString().toIntOrNull();

        if (year != null)
        {
            val res: Int = Calendar.getInstance().get(Calendar.YEAR) - year;
            this.binding.txtViewAge.text = res.toString();
            Toast.makeText(applicationContext, "Возраст подсчитан: $res!", Toast.LENGTH_SHORT).show();
        }
    });

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState);
        this.binding = ActivityMainBinding.inflate(layoutInflater);
        this.binding.buttonAge.setOnClickListener(buttonAgeOnClickListener);
        this.setContentView(binding.root);
    }
}