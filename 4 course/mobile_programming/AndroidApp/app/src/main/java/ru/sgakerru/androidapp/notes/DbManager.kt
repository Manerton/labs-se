package ru.sgakerru.androidapp.notes

import android.content.ContentValues
import android.content.Context
import android.database.Cursor
import android.database.sqlite.SQLiteDatabase
import android.database.sqlite.SQLiteOpenHelper
import android.database.sqlite.SQLiteQueryBuilder
import android.widget.Toast

class DbManager
{
    private val dbName = "NotesDB";
    private val dbTable = "Note";
    private val colId = "Id";
    private val colTitle = "Title";
    private val colContent = "Content";
    private val dbVersion = 1;

    private val sqlCreateTableQuery = "CREATE TABLE IF NOT EXISTS $dbTable " +
            "($colId INTEGER PRIMARY KEY" +
            ", $colTitle TEXT, $colContent TEXT);";

    private var sqlDb: SQLiteDatabase? = null;

    constructor(context: Context)
    {
        val dbHelper = DbHelperNotes(context);
        sqlDb = dbHelper.writableDatabase;
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
            db!!.execSQL("DROP TABLE IF EXISTS $dbTable");
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

    public fun insert(note: Note): Long
    {
        // Подготавливаем данные, которые хотим добавить.
        val values = ContentValues();
        values.put(colTitle, note.title);
        values.put(colContent, note.content);

        return sqlDb!!.insert(dbTable, "", values);
    }

    public fun getList() : ArrayList<Note>
    {
        var notesList = ArrayList<Note>();

        val cursor = this.query(null, null, null, null);
        if (cursor.moveToFirst())
        {
            do
            {
                val id = cursor.getInt(cursor.getColumnIndex(colId));
                val title = cursor.getString(cursor.getColumnIndex(colTitle));
                val content = cursor.getString(cursor.getColumnIndex(colContent));

                notesList.add(Note(id, title, content));
            }
            while (cursor.moveToNext())
        }

        return notesList;
    }

    public fun remove(id: Int) : Int
    {
        return sqlDb!!.delete(dbTable, "Id=$id", null);
    }

    public fun update(note: Note): Int
    {
        val values = ContentValues();
        values.put(colTitle, note.title);
        values.put(colContent, note.content);

        val count = sqlDb!!.update(dbTable, values, "Id=${note.noteId}", null);
        return count;
    }
}