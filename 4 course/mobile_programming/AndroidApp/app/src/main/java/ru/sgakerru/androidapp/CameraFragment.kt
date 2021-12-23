package ru.sgakerru.androidapp

import android.app.Activity.RESULT_OK
import android.content.Intent
import android.graphics.Bitmap
import android.os.Bundle
import android.provider.MediaStore
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import ru.sgakerru.androidapp.databinding.FragmentCameraBinding

class CameraFragment : Fragment()
{
    private lateinit var binding: FragmentCameraBinding;

    private val requestCameraAccessCode = 1;

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View?
    {
        binding = FragmentCameraBinding.inflate(inflater, container, false);

        binding.buttonCamera.setOnClickListener(View.OnClickListener
        {
            try
            {
                val captureIntent = Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                startActivityForResult(captureIntent, requestCameraAccessCode);
            } catch (e: Exception)
            {
                Toast.makeText(
                    context,
                    "Ваше устройство не поддерживает камеру!",
                    Toast.LENGTH_LONG
                ).show();
            }
        })

        return binding.root;
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?)
    {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK && requestCode == requestCameraAccessCode)
        {
            val thumbnailBitmap = data?.extras?.get("data") as Bitmap;
            binding.imageViewCamera.setImageBitmap(thumbnailBitmap);
        }
    }
}