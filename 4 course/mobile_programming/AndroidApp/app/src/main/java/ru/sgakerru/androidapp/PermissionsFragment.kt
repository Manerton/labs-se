package ru.sgakerru.androidapp

import android.app.Activity.RESULT_OK
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.provider.ContactsContract
import android.provider.MediaStore
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Toast
import androidx.core.app.ActivityCompat
import ru.sgakerru.androidapp.databinding.FragmentPermissionsBinding

class PermissionsFragment : Fragment()
{
    private lateinit var binding: FragmentPermissionsBinding;

    private val requestStoragePermissionCode = 2;
    private val requestContactsPermissionCode = 3;

    private val pickImageCode = 21;
    private val pickContacts = 31;

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState);
    }

    /** Обработка кнопок */
    private val buttonOnClickListener = (View.OnClickListener
    { v: View ->
        when (v as Button)
        {
            binding.buttonGallery ->
            {
                checkStoragePermission();
            }
            binding.buttonContacts ->
            {
                checkContactsPermission();
            }
        }
    });

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View?
    {
        binding = FragmentPermissionsBinding.inflate(inflater, container, false);

        binding.buttonGallery.setOnClickListener(buttonOnClickListener);
        binding.buttonContacts.setOnClickListener(buttonOnClickListener);

        return binding.root;
    }

    /** Проверяем разрешение на доступ к файловой системе. */
    private fun checkStoragePermission()
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)
        {
            if (ActivityCompat.checkSelfPermission(
                    context!!,
                    android.Manifest.permission.READ_EXTERNAL_STORAGE
                ) !=
                PackageManager.PERMISSION_GRANTED
            )
            {
                requestPermissions(
                    arrayOf(android.Manifest.permission.READ_EXTERNAL_STORAGE),
                    requestStoragePermissionCode
                );
                return;
            }
        }

        handleLoadImageFromGallery();
    }

    /** Проверяем разрешение на доступ к контактам. */
    private fun checkContactsPermission()
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)
        {
            if (ActivityCompat.checkSelfPermission(
                    context!!,
                    android.Manifest.permission.READ_CONTACTS
                ) !=
                PackageManager.PERMISSION_GRANTED
            )
            {
                requestPermissions(
                    arrayOf(android.Manifest.permission.READ_CONTACTS),
                    requestContactsPermissionCode
                );
                return;
            }
        }

        handleLoadContacts();
    }

    /** Обработка запроса на разрешения */
    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    )
    {
        when (requestCode)
        {
            requestStoragePermissionCode ->
            {
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED)
                {
                    handleLoadImageFromGallery();
                }
                else
                {
                    Toast.makeText(
                        activity,
                        "Не удалось получить доступ к файлам!",
                        Toast.LENGTH_LONG
                    ).show();
                }
            }
            requestContactsPermissionCode ->
            {
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED)
                {
                    handleLoadContacts();
                }
                else
                {
                    Toast.makeText(
                        activity,
                        "Не удалось получить доступ к контактам!",
                        Toast.LENGTH_LONG
                    ).show();
                }
            }
        }

        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

    }

    private fun handleLoadImageFromGallery()
    {
        val intent = Intent(
            Intent.ACTION_PICK,
            MediaStore.Images.Media.EXTERNAL_CONTENT_URI
        );
        startActivityForResult(intent, pickImageCode);
    }

    private fun handleLoadContacts()
    {
        val intent = Intent(
            Intent.ACTION_PICK,
            ContactsContract.Contacts.CONTENT_URI
        );
        startActivityForResult(intent, pickContacts);
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?)
    {
        super.onActivityResult(requestCode, resultCode, data);

        if (resultCode == RESULT_OK && data != null)
        {
            when (requestCode)
            {
                pickImageCode ->
                {
                    val selectedImageURI = data.data;
                    binding.imageViewGallery.setImageURI(selectedImageURI);
                }
                pickContacts ->
                {
                    val selectedContactURI = data.data;

                    val cursor = context!!.contentResolver.query(
                        selectedContactURI, arrayOf(ContactsContract.Contacts.DISPLAY_NAME),
                        null, null, null
                    );
                    cursor.moveToFirst();
                    val contactName = cursor.getString(0);
                    cursor.close();

                    binding.textViewContact.text = contactName;
                }
            }
        }
    }
}