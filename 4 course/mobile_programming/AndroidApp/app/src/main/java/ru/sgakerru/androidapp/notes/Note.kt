package ru.sgakerru.androidapp.notes

class Note
{
    var noteId: Int? = null;
    var title: String? = null;
    var content: String? = null;

    constructor(noteId: Int?, title: String?, content: String?)
    {
        this.noteId = noteId;
        this.title = title;
        this.content = content;
    }
}