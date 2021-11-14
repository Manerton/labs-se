package ru.sgakerru.androidapp;

import android.graphics.Color
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import ru.sgakerru.androidapp.databinding.ActivityMainBinding;
import android.view.View;
import android.view.WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE
import android.widget.Toast;
import android.widget.Button;

class MainActivity : AppCompatActivity()
{
    private lateinit var binding: ActivityMainBinding;

    private var firstPlayerActions = ArrayList<Int>();
    private var secondPlayerActions = ArrayList<Int>();
    private var isFirstPlayerActive = true;

    private val buttonOnClickListener = (View.OnClickListener
    { v: View ->
        val selectedBtn = v as Button;
        var cellId = 0;

        when (selectedBtn.id)
        {
            R.id.button1 -> cellId = 1
            R.id.button2 -> cellId = 2
            R.id.button3 -> cellId = 3
            R.id.button4 -> cellId = 4
            R.id.button5 -> cellId = 5
            R.id.button6 -> cellId = 6
            R.id.button7 -> cellId = 7
            R.id.button8 -> cellId = 8
            R.id.button9 -> cellId = 9
        }

        this.playGame(cellId, selectedBtn);
    });

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState);
        this.binding = ActivityMainBinding.inflate(layoutInflater);

        this.binding.button1.setOnClickListener(buttonOnClickListener);
        this.binding.button2.setOnClickListener(buttonOnClickListener);
        this.binding.button3.setOnClickListener(buttonOnClickListener);
        this.binding.button4.setOnClickListener(buttonOnClickListener);
        this.binding.button5.setOnClickListener(buttonOnClickListener);
        this.binding.button6.setOnClickListener(buttonOnClickListener);
        this.binding.button7.setOnClickListener(buttonOnClickListener);
        this.binding.button8.setOnClickListener(buttonOnClickListener);
        this.binding.button9.setOnClickListener(buttonOnClickListener);

        this.setContentView(binding.root);
    }

    private fun playGame(cellId: Int, btn: Button)
    {
        if (isFirstPlayerActive)
        {
            btn.text = "X";
            btn.setBackgroundColor(Color.GREEN);
            firstPlayerActions.add(cellId);
        }
        else
        {
            btn.text = "O";
            btn.setBackgroundColor(Color.RED);
            secondPlayerActions.add(cellId);
        }

        isFirstPlayerActive = !isFirstPlayerActive;
        btn.isEnabled = false;

        this.checkWinner();
    }

    private fun checkWinner()
    {
        var winner = 0;

        val checkOne = fun(playerActions: ArrayList<Int>): Boolean
        {
            return (playerActions.containsAll(listOf(1, 2, 3))
                    || playerActions.containsAll(listOf(4, 5, 6))
                    || playerActions.containsAll(listOf(7, 8, 9))
                    || playerActions.containsAll(listOf(1, 4, 7))
                    || playerActions.containsAll(listOf(2, 5, 8))
                    || playerActions.containsAll(listOf(3, 6, 9))
                    || playerActions.containsAll(listOf(1, 5, 9))
                    || playerActions.containsAll(listOf(3, 5, 7))
                    );
        };

        if (checkOne(firstPlayerActions))
        {
            winner = 1;
        }
        else if (checkOne(secondPlayerActions))
        {
            winner = 2;
        }

        if (winner == 1)
        {
            Toast.makeText(this, "Player 1 won the game!", Toast.LENGTH_LONG).show();
            window.setFlags(
                FLAG_NOT_TOUCHABLE,
                FLAG_NOT_TOUCHABLE
            );
        }
        else if (winner == 2)
        {
            Toast.makeText(this, "Player 2 won the game!", Toast.LENGTH_LONG).show();
        }
    }
}