package ru.sgakerru.androidapp.notes

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.Button
import androidx.navigation.fragment.findNavController
import ru.sgakerru.androidapp.R
import ru.sgakerru.androidapp.databinding.FragmentNotesBinding
import ru.sgakerru.androidapp.databinding.NotesRecordBinding

private const val ARG_DB_TYPE = "dbType";

class NotesFragment : Fragment()
{
    private lateinit var binding: FragmentNotesBinding;

    /** Тип базы данных (SQLite или Firebase). */
    private lateinit var dbType: DbManagerType;

    /** Адаптер для заметок, чтобы поместить их в ListView */
    private var notesAdapter = NotesAdapter();

    private lateinit var dbManager: DbManager;

    /** Обработка нажатий на кнопки. */
    private val buttonOnClickListener = (View.OnClickListener
    { v: View ->
        when (v as Button)
        {
            binding.buttonAddNewNote ->
            {
                // Переходим на другую страницу,
                // где можно будет добавить новую запись
                var bundle = Bundle();
                bundle.putSerializable("dbType", dbType);
                findNavController().navigate(R.id.action_nav_notes_to_nav_notes_add_note, bundle);
            }
        }
    });

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState);

        arguments?.let {
            dbType = it.get(ARG_DB_TYPE) as DbManagerType;
        }

        dbManager = DbManager.create(dbType, context!!, notesAdapter);
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View?
    {
        binding = FragmentNotesBinding.inflate(inflater, container, false);

        // Обрабатываем нажатия на кнопки.
        binding.buttonAddNewNote.setOnClickListener(buttonOnClickListener);

        // Привязываем список к адаптеру.
        binding.lvNotes.adapter = notesAdapter;

        return binding.root;
    }

    /** Адаптер для списка заметок для ListView. */
    inner class NotesAdapter : BaseAdapter
    {
        public var notesList = ArrayList<Note>();

        constructor() : super()
        {
        }

        override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View
        {
            var binding = NotesRecordBinding.inflate(layoutInflater, null, false);
            var note = notesList[position];

            binding.textViewTitle.text = note.title;
            binding.textViewContent.text = note.content;

            binding.buttonDelete.setOnClickListener(View.OnClickListener
            {
                dbManager.remove(note.noteId!!);
            });

            binding.buttonEdit.setOnClickListener(View.OnClickListener
            {
                // Переходим на страницу редактирования
                // и передаём текущие данные заметки.
                var bundle = Bundle();
                bundle.putSerializable("dbType", dbType);
                bundle.putString("noteId", note.noteId!!);
                bundle.putString("title", note.title);
                bundle.putString("content", note.content);
                findNavController().navigate(
                    R.id.action_nav_notes_to_nav_notes_edit_note,
                    bundle
                );
            });

            return binding.root;
        }

        override fun getCount(): Int
        {
            return notesList.size;
        }

        override fun getItem(position: Int): Any
        {
            return notesList[position];
        }

        override fun getItemId(position: Int): Long
        {
            return position.toLong();
        }
    }
}