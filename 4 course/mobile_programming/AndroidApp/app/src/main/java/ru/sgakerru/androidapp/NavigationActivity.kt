package ru.sgakerru.androidapp

import android.os.Bundle
import android.view.Menu
import com.google.android.material.snackbar.Snackbar
import com.google.android.material.navigation.NavigationView
import androidx.navigation.findNavController
import androidx.drawerlayout.widget.DrawerLayout
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.ui.*
import ru.sgakerru.androidapp.databinding.ActivityNavigationBinding

class NavigationActivity : AppCompatActivity()
{

    private lateinit var appBarConfiguration: AppBarConfiguration;
    private lateinit var binding: ActivityNavigationBinding;

    override fun onCreate(savedInstanceState: Bundle?)
    {
        super.onCreate(savedInstanceState);

        binding = ActivityNavigationBinding.inflate(layoutInflater);
        setContentView(binding.root);

        setSupportActionBar(binding.appBarNavigation.toolbar);

        val drawerLayout: androidx.drawerlayout.widget.DrawerLayout = binding.drawerLayout;
        val navView: NavigationView = binding.navView;
        val navController = findNavController(R.id.nav_host_fragment_content_navigation);

        appBarConfiguration = AppBarConfiguration(
            navController.graph, drawerLayout
        );

        setupActionBarWithNavController(navController, appBarConfiguration);
        navView.setupWithNavController(navController);
    }

    override fun onSupportNavigateUp(): Boolean
    {
        val navController = findNavController(R.id.nav_host_fragment_content_navigation);
        return navController.navigateUp(appBarConfiguration) || super.onSupportNavigateUp();
    }
}