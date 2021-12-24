package ru.sgakerru.androidapp

import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.Toast
import ru.sgakerru.androidapp.databinding.FragmentFileSystemBinding
import java.io.*
import java.lang.Exception

class FileSystemFragment : Fragment()
{
    private lateinit var binding: FragmentFileSystemBinding;
    private val filename = "AndroidAppFile.txt";
    private val filepath = "MyFileStorage";
    private var externalFile: File? = null;

    private val buttonOnClickListener = (View.OnClickListener
    { v: View ->
        when (v as Button)
        {
            binding.buttonSaveFile ->
            {
                try
                {
                    val fos = FileOutputStream(externalFile);
                    fos.write(binding.editTextFile.text.toString().toByteArray());
                    fos.close();
                } catch (e: Exception)
                {
                    Log.e("AndroidApp", e.toString());
                }

                binding.editTextFile.text.clear();
                Toast.makeText(
                    context, "$filename сохранен на диск!",
                    Toast.LENGTH_LONG
                ).show();
            }
            binding.buttonReadFile ->
            {
                try
                {
                    val fis = FileInputStream(externalFile);
                    val input = DataInputStream(fis);
                    val br = BufferedReader(InputStreamReader(input));

                    var line: String = "";

                    binding.editTextFile.text.clear();

                    do
                    {
                        line = br.readLine();
                        binding.editTextFile.text.appendLine(line);
                    } while (line != null)

                } catch (e: Exception)
                {
                    Log.e("AndroidApp", e.toString());
                }

                Toast.makeText(
                    context, "$filename загружен с диска!",
                    Toast.LENGTH_LONG
                ).show();

            }
        }
    });

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState);
        externalFile = File(context!!.getExternalFilesDir(null), filename);
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View?
    {
        binding = FragmentFileSystemBinding.inflate(inflater, container, false);

        binding.buttonSaveFile.setOnClickListener(buttonOnClickListener);
        binding.buttonReadFile.setOnClickListener(buttonOnClickListener);

        return binding.root;
    }
}