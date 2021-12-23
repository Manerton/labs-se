package ru.sgakerru.androidapp.notes

class Note
{
    var noteId: Int? = null;
    var title: String;
    var content: String;

    constructor(noteId: Int?, title: String, content: String)
    {
        this.noteId = noteId;
        this.title = title;
        this.content = content;
    }
}