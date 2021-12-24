package ru.sgakerru.androidapp.notes

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Toast
import ru.sgakerru.androidapp.databinding.FragmentAddNoteBinding

private const val ARG_DB_TYPE = "dbType";

class AddNoteFragment : Fragment()
{
    private lateinit var binding: FragmentAddNoteBinding;

    private lateinit var dbManager: DbManager;

    /** Обработка нажатий на кнопки. */
    private val buttonOnClickListener = (View.OnClickListener
    { v: View ->
        when (v as Button)
        {
            // Создать (добавить) новую заметку.
            binding.buttonCreate ->
            {
                // Добавляем запись в БД.
                val note = Note(null,
                    binding.editTextTitle.text.toString(),
                    binding.editTextContent.text.toString()
                );

                val result = dbManager.insert(note);

                if (result)
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
        super.onCreate(savedInstanceState);
        arguments?.let {
            val dbManagerType = it.get(ARG_DB_TYPE) as DbManagerType;
            dbManager = DbManager.getInstance(dbManagerType)!!;
        }
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