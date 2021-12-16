package ru.sgakerru.androidapp.ui.home

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

class HomeViewModel : ViewModel()
{

    private val _text = MutableLiveData<String>().apply {
        value = "Это главная страница!\n\nС помощью меню слева вы можете выбрать необходимую лабораторную работу.";
    }
    val text: LiveData<String> = _text;
}