package ru.sgakerru.androidapp.ui.home

import android.arch.lifecycle.LiveData;
import android.arch.lifecycle.MutableLiveData;
import android.arch.lifecycle.ViewModel;

class HomeViewModel : ViewModel()
{

    private val _text = MutableLiveData<String>().apply {
        value = "Это главная страница!\n\nС помощью меню слева вы можете выбрать необходимую лабораторную работу.";
    }
    val text: LiveData<String> = _text;
}