package ru.sgakerru.androidapp.notes

import android.content.Context

interface DbManager
{
    public fun insert(note: Note): Boolean;

    public fun remove(id: String): Boolean;

    public fun update(note: Note): Boolean;

    companion object Factory
    {
        private var instances = HashMap<DbManagerType, DbManager>();

        fun create(
            dbType: DbManagerType, context: Context,
            notesAdapter: NotesFragment.NotesAdapter
        ): DbManager
        {
            when (dbType)
            {
                DbManagerType.SQLite ->
                {
                    instances[dbType] = SQLiteDbManager(context, notesAdapter);
                }
                DbManagerType.Firebase ->
                {
                    instances[dbType] = FirebaseDbManager(notesAdapter)
                };
            }

            return instances[dbType]!!;
        };

        fun getInstance(dbType: DbManagerType): DbManager?
        {
            return instances[dbType];
        }
    }
}