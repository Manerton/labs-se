package ru.sgakerru.androidapp.notes

import android.content.ContentValues
import android.content.Context
import android.database.Cursor
import android.database.sqlite.SQLiteDatabase
import android.database.sqlite.SQLiteOpenHelper
import android.database.sqlite.SQLiteQueryBuilder
import android.widget.Toast
import java.util.*
import kotlin.collections.ArrayList

class SQLiteDbManager : DbManager
{
    private val dbName = "NotesDB";
    private val dbTable = "Note";
    private val colId = "Id";
    private val colTitle = "Title";
    private val colContent = "Content";
    private val dbVersion = 2;

    private val sqlCreateTableQuery = "CREATE TABLE IF NOT EXISTS $dbTable " +
            "($colId TEXT PRIMARY KEY" +
            ", $colTitle TEXT, $colContent TEXT);";

    private var sqlDb: SQLiteDatabase? = null;

    /** Адаптер списка для обновления данных на UI */
    private val notesAdapter: NotesFragment.NotesAdapter;

    constructor(context: Context, notesAdapter: NotesFragment.NotesAdapter)
    {
        this.notesAdapter = notesAdapter;
        val dbHelper = DbHelperNotes(context);
        sqlDb = dbHelper.writableDatabase;

        updateUi();
    }

    inner class DbHelperNotes : SQLiteOpenHelper
    {
        var context: Context? = null;

        constructor(context: Context) :
                super(context, dbName, null, dbVersion)
        {
            this.context = context;
        }

        override fun onCreate(db: SQLiteDatabase?)
        {
            db!!.execSQL(sqlCreateTableQuery);
            Toast.makeText(
                this.context,
                "Локальная база данных для заметок была создана!",
                Toast.LENGTH_LONG
            ).show();
        }

        override fun onUpgrade(db: SQLiteDatabase?, oldVersion: Int, newVersion: Int)
        {
            db!!.execSQL("DROP TABLE $dbTable");
        }
    }

    private fun query(
        projection: Array<String>?,
        selection: String?,
        selectionArgs: Array<String>?,
        sortOrder: String?
    ): Cursor
    {
        val query = SQLiteQueryBuilder();
        query.tables = dbTable;
        val cursor = query.query(
            sqlDb,
            projection, selection, selectionArgs,
            null, null, sortOrder
        );
        return cursor;
    }

    private fun getList(): ArrayList<Note>
    {
        var notesList = ArrayList<Note>();

        val cursor = this.query(null, null, null, null);
        if (cursor.moveToFirst())
        {
            do
            {
                val id = cursor.getString(cursor.getColumnIndex(colId));
                val title = cursor.getString(cursor.getColumnIndex(colTitle));
                val content = cursor.getString(cursor.getColumnIndex(colContent));

                notesList.add(Note(id, title, content));
            } while (cursor.moveToNext())
        }

        return notesList;
    }

    override fun insert(note: Note): Boolean
    {
        // Подготавливаем данные, которые хотим добавить.
        val values = ContentValues();
        values.put(colId, UUID.randomUUID().toString());
        values.put(colTitle, note.title);
        values.put(colContent, note.content);

        val count = sqlDb!!.insert(dbTable, "", values);

        if (count > 0)
        {
            updateUi();
        }

        return (count > 0);
    }

    override fun remove(id: String): Boolean
    {
        val count = sqlDb!!.delete(dbTable, "Id='$id'", null);

        if (count > 0)
        {
            updateUi();
        }

        return (count > 0);
    }

    override fun update(note: Note): Boolean
    {
        val values = ContentValues();
        values.put(colTitle, note.title);
        values.put(colContent, note.content);

        val count = sqlDb!!.update(dbTable, values, "Id='${note.noteId}'", null);

        if (count > 0)
        {
            updateUi();
        }

        return (count > 0);
    }

    private fun updateUi()
    {
        this.notesAdapter.notesList = this.getList();
        this.notesAdapter.notifyDataSetChanged();
    }
}