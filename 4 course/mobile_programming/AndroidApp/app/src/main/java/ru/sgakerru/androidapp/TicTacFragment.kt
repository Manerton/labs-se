package ru.sgakerru.androidapp

import android.graphics.Color
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager
import android.widget.Button
import android.widget.Toast
import ru.sgakerru.androidapp.databinding.FragmentTicTacBinding;

class TicTacFragment : Fragment()
{
    private lateinit var binding: FragmentTicTacBinding;

    private var firstPlayerActions = ArrayList<Int>();
    private var secondPlayerActions = ArrayList<Int>();
    private var isFirstPlayerActive = true;

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState);
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View?
    {
        binding = FragmentTicTacBinding.inflate(inflater, container, false);

        binding.button1.setOnClickListener(buttonOnClickListener);
        binding.button2.setOnClickListener(buttonOnClickListener);
        binding.button3.setOnClickListener(buttonOnClickListener);
        binding.button4.setOnClickListener(buttonOnClickListener);
        binding.button5.setOnClickListener(buttonOnClickListener);
        binding.button6.setOnClickListener(buttonOnClickListener);
        binding.button7.setOnClickListener(buttonOnClickListener);
        binding.button8.setOnClickListener(buttonOnClickListener);
        binding.button9.setOnClickListener(buttonOnClickListener);

        return binding.root;
    }

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
            Toast.makeText(activity, "Player 1 won the game!", Toast.LENGTH_LONG).show();
        }
        else if (winner == 2)
        {
            Toast.makeText(activity, "Player 2 won the game!", Toast.LENGTH_LONG).show();
        }
    }
}