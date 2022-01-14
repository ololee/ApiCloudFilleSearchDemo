package cn.ololee.apicloudfillesearchdemo;

import android.Manifest;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import androidx.core.app.ActivityCompat;
import com.uzmap.pkg.openapi.ExternalActivity;

public class MainActivity extends ExternalActivity {

  @Override
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    ActivityCompat.requestPermissions(this,new String[]{ Manifest.permission.READ_EXTERNAL_STORAGE },101);
  }
}