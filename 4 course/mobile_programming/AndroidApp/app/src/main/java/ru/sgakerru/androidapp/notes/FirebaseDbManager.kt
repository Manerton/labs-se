package ru.sgakerru.androidapp.notes

import android.content.Context
import android.util.Log
import com.google.android.gms.tasks.Task
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.ValueEventListener
import com.google.firebase.database.ktx.database
import com.google.firebase.ktx.Firebase
import java.util.*
import kotlin.collections.ArrayList
import kotlin.collections.HashMap
import kotlin.reflect.typeOf

class FirebaseDbManager : DbManager
{
    private val database =
        Firebase.database("https://androidapp-2cae1-default-rtdb.europe-west1.firebasedatabase.app");
    private val ref = database.reference;

    private var notesAdapter: NotesFragment.NotesAdapter;

    constructor(notesAdapter: NotesFragment.NotesAdapter)
    {
        this.notesAdapter = notesAdapter;

        val notesListener = object : ValueEventListener
        {
            override fun onDataChange(dataSnapshot: DataSnapshot)
            {
                updateUi(dataSnapshot);
            }

            override fun onCancelled(databaseError: DatabaseError)
            {
                Log.w("TEST", "loadNotes:onCancelled", databaseError.toException())
            }
        }

        ref.child("note").addValueEventListener(notesListener);
    }

    override fun insert(note: Note): Boolean
    {
        val id = UUID.randomUUID().toString();
        note.noteId = id;
        ref.child("note").child(id).setValue(note);

        return true;
    }

    override fun remove(id: String): Boolean
    {
        ref.child("note").child(id).setValue(null);
        return true;
    }

    override fun update(note: Note): Boolean
    {
        TODO("Not yet implemented")
    }

    private fun updateUi(dataSnapshot: DataSnapshot)
    {
        val valueMap = dataSnapshot.value as HashMap<String, HashMap<String, String>>?;

        this.notesAdapter.notesList.clear();

        if (valueMap != null)
        {
            for (mapValue in valueMap.values)
            {
                val note = Note(mapValue["noteId"], mapValue["title"]!!, mapValue["content"]!!);
                this.notesAdapter.notesList.add(note);
            }
        }

        this.notesAdapter.notifyDataSetChanged();
    }
}