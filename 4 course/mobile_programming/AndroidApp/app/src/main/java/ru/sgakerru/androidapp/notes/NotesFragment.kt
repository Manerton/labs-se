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

class NotesFragment : Fragment()
{
    private lateinit var binding: FragmentNotesBinding;

    /** Список заметок. */
    private var notesList = ArrayList<Note>();

    /** Адаптер для заметок, чтобы поместить их в ListView */
    private var notesAdapter = NotesAdapter(notesList);

    /** Обработка нажатий на кнопки. */
    private val buttonOnClickListener = (View.OnClickListener
    { v: View ->
        when (v as Button)
        {
            binding.buttonAddNewNote ->
            {
                // Переходим на другую страницу,
                // где можно будет добавить новую запись
                findNavController().navigate(R.id.action_nav_notes_to_nav_notes_add_note);
            }
        }
    });

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState);
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View?
    {
        binding = FragmentNotesBinding.inflate(inflater, container, false);

        // Обрабатываем нажатия на кнопки.
        binding.buttonAddNewNote.setOnClickListener(buttonOnClickListener);

        // Перезагружаем данные из базы.
        loadAllFromDB();

        return binding.root;
    }

    private fun loadAllFromDB()
    {
        val dbManager = DbManager(context!!);
        notesList.clear();
        notesList.addAll(dbManager.getList());

        // Привязываем список к адаптеру.
        binding.lvNotes.adapter = notesAdapter;
    }

    /** Адаптер для списка заметок для ListView. */
    inner class NotesAdapter : BaseAdapter
    {
        private var notesList = ArrayList<Note>();

        constructor(_notesList: ArrayList<Note>):super()
        {
            this.notesList = _notesList;
        }

        override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View
        {
            var binding = NotesRecordBinding.inflate(layoutInflater, null, false);
            var note = notesList[position];

            binding.textViewTitle.text = note.title;
            binding.textViewContent.text = note.content;

            binding.buttonDelete.setOnClickListener( View.OnClickListener
            {
                val dbManager = DbManager(context!!);
                dbManager.remove(note.noteId!!);
                // Так как удалили запись, перезагрузим список
                loadAllFromDB();
            });

            binding.buttonEdit.setOnClickListener( View.OnClickListener
            {
                // Переходим на страницу редактирования
                // и передаём текущие данные заметки.
                var bundle = Bundle();
                bundle.putInt("noteId", note.noteId!!);
                bundle.putString("title", note.title);
                bundle.putString("content", note.content);
                findNavController().navigate(R.id.action_nav_notes_to_nav_notes_edit_note, bundle);
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