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

    /** Обработка нажатий на кнопки. */
    private val buttonOnClickListener = (View.OnClickListener
    { v: View ->
        val selectedBtn = v as Button;

        when (selectedBtn.id)
        {
            R.id.button_addNewNote ->
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

        binding.buttonAddNewNote.setOnClickListener(buttonOnClickListener);

        // Добавляем тестовые данные
        notesList.add(Note(1, "Записка 1",
            "съешь же ещё этих мягких французских булок, да выпей чаю"));
        notesList.add(Note(2, "Записка 2",
            "съешь же ещё этих мягких французских булок, да выпей чаю"));
        notesList.add(Note(3, "Записка 3",
            "съешь же ещё этих мягких французских булок, да выпей чаю"));
        notesList.add(Note(4, "Записка 4",
            "съешь же ещё этих мягких французских булок, да выпей чаю"));
        notesList.add(Note(5, "Записка 5",
            "съешь же ещё этих мягких французских булок, да выпей чаю"));

        var notesAdapter = NotesAdapter(notesList);
        binding.lvNotes.adapter = notesAdapter;

        return binding.root;
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