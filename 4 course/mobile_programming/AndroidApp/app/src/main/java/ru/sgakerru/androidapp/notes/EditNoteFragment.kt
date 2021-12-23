package ru.sgakerru.androidapp.notes

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Toast
import ru.sgakerru.androidapp.databinding.FragmentAddNoteBinding
import ru.sgakerru.androidapp.databinding.FragmentEditNoteBinding

private const val ARG_NOTE_ID = "noteId";
private const val ARG_TITLE = "title";
private const val ARG_CONTENT = "content";

class EditNoteFragment : Fragment()
{
    private lateinit var binding: FragmentEditNoteBinding;

    private var noteId: Int? = null;
    private var title: String? = null;
    private var content: String? = null;

    /** Обработка нажатий на кнопки. */
    private val buttonOnClickListener = (View.OnClickListener
    { v: View ->
        when (v as Button)
        {
            // Создать (добавить) новую заметку.
            binding.buttonEdit ->
            {
                // Класс для работы с БД.
                val dbManager = DbManager(context!!);

                // Изменяем запись в БД.
                val note = Note(noteId,
                    binding.editTextTitle.text.toString(),
                    binding.editTextContent.text.toString()
                );

                val id = dbManager.update(note);

                if (id > 0)
                {
                    Toast.makeText(context, "Заметка была изменена!", Toast.LENGTH_LONG).show();
                }
                else
                {
                    Toast.makeText(context, "Не удалось изменить заметку!", Toast.LENGTH_LONG).show();
                }
            }
        }
    });

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState)
        arguments?.let {
            noteId = it.getInt(ARG_NOTE_ID);
            title = it.getString(ARG_TITLE);
            content = it.getString(ARG_CONTENT);
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View?
    {
        binding = FragmentEditNoteBinding.inflate(inflater, container, false);

        binding.buttonEdit.setOnClickListener(this.buttonOnClickListener);

        binding.editTextTitle.setText(this.title);
        binding.editTextContent.setText(this.content);

        return binding.root;
    }
}