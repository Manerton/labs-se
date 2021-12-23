package ru.sgakerru.androidapp.notes

import android.content.ContentValues
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Toast
import androidx.navigation.fragment.findNavController
import ru.sgakerru.androidapp.R
import ru.sgakerru.androidapp.databinding.FragmentAddNoteBinding

class AddNoteFragment : Fragment()
{
    private lateinit var binding: FragmentAddNoteBinding;

    /** Обработка нажатий на кнопки. */
    private val buttonOnClickListener = (View.OnClickListener
    { v: View ->
        when (v as Button)
        {
            // Создать (добавить) новую заметку.
            binding.buttonCreate ->
            {
                // Класс для работы с БД.
                val dbManager = DbManager(context!!);

                // Добавляем запись в БД.
                val note = Note(null,
                    binding.editTextTitle.text.toString(),
                    binding.editTextContent.text.toString()
                );

                val id = dbManager.insert(note);

                if (id > 0)
                {
                    Toast.makeText(context, "Была добавлена новая заметка!", Toast.LENGTH_LONG).show();
                }
                else
                {
                    Toast.makeText(context, "Не удалось добавить новую заметку!", Toast.LENGTH_LONG).show();
                }
            }
        }
    });

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View?
    {
        binding = FragmentAddNoteBinding.inflate(inflater, container, false);

        binding.buttonCreate.setOnClickListener(this.buttonOnClickListener);

        return binding.root;
    }
}