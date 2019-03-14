package hanlonglin.com.ocrrecognization;

import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.AsyncTask;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.googlecode.tesseract.android.TessBaseAPI;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    TessBaseAPI tessBaseAPI;
    //String language = "afr";    //数字
    //String language="chi_sim";  //中文
    //String language="chi_tra";    //中文
    String language="ces";    //数字

    Button btn_chooseid, btn_searchid, btn_recognizeid;
    ImageView imageView;
    TextView txt_result;

    private String imagePath; //选择的相册图片路径

    private final static int REQ_CHOOSE_PIC = 10;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();

        new LoadTask().execute();
    }

    private void initView() {
        btn_chooseid = (Button) findViewById(R.id.btn_chooseid);
        btn_recognizeid = (Button) findViewById(R.id.btn_recognizeid);
        btn_searchid = (Button) findViewById(R.id.btn_searchid);
        imageView = (ImageView) findViewById(R.id.image);
        txt_result = (TextView) findViewById(R.id.txt_result);

        btn_chooseid.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onChooseId();
            }
        });
        btn_searchid.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onSearchId();
            }
        });
        btn_recognizeid.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onRecognizeId();
            }
        });
    }

    private void onChooseId() {
        Intent intent = new Intent(
                Intent.ACTION_PICK,
                android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        startActivityForResult(intent, REQ_CHOOSE_PIC);
    }

    private void onSearchId() {
        Bitmap bitmap_src = BitmapFactory.decodeFile(imagePath);
        Bitmap bitmap_result = getIdNumber(bitmap_src, Bitmap.Config.ARGB_8888);
        imageView.setImageBitmap(bitmap_result);

        tessBaseAPI.setImage(bitmap_result);
    }

    private void onRecognizeId() {
        String text = tessBaseAPI.getUTF8Text();
        txt_result.setText(text);
    }

    //异步加载 tess-two
    private class LoadTask extends AsyncTask {

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            tessBaseAPI = new TessBaseAPI();
        }

        @Override
        protected void onPostExecute(Object o) {
            super.onPostExecute(o);
            Boolean result = (Boolean) o;
            if (result) {
                Log.e("TAG", "初始化TessTwo成功");
            } else {
                Log.e("TAG", "初始化TessTwo异常");
            }
        }

        @Override
        protected Boolean doInBackground(Object[] objects) {
            //读取文件
            try {
                InputStream is = null;
                is = getAssets().open(language + ".traineddata");
                File file = new File("/sdcard/tess/tessdata/" + language + ".traineddata");
                if (!file.exists()) {
                    file.getParentFile().mkdirs();
                    FileOutputStream fos = new FileOutputStream(file);
                    byte[] buffer = new byte[2048];
                    int len;
                    while ((len = is.read(buffer)) != -1) {
                        fos.write(buffer, 0, len);
                    }
                    fos.close();
                }
                is.close();
                return tessBaseAPI.init("/sdcard/tess", language);
            } catch (IOException e) {
                e.printStackTrace();
                Log.e("TAG","读取出错1。。。"+e.getMessage());
                return false;
            }catch (Exception e){
                e.printStackTrace();
                Log.e("TAG","读取出错2。。。"+e.getMessage());
                return false;
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
//在相册里面选择好相片之后调回到现在的这个activity中
        switch (requestCode) {
            case REQ_CHOOSE_PIC://这里的requestCode是我自己设置的，就是确定返回到那个Activity的标志
                if (resultCode == RESULT_OK) {//resultcode是setResult里面设置的code值
                    try {
                        Uri selectedImage = data.getData(); //获取系统返回的照片的Uri
                        String[] filePathColumn = {MediaStore.Images.Media.DATA};
                        Cursor cursor = getContentResolver().query(selectedImage,
                                filePathColumn, null, null, null);//从系统表中查询指定Uri对应的照片
                        cursor.moveToFirst();
                        int columnIndex = cursor.getColumnIndex(filePathColumn[0]);
                        imagePath = cursor.getString(columnIndex);  //获取照片路径
                        cursor.close();
                        Bitmap bitmap = BitmapFactory.decodeFile(imagePath);
                        imageView.setImageBitmap(bitmap);
                    } catch (Exception e) {
                        // TODO Auto-generatedcatch block
                        e.printStackTrace();
                    }
                }
                break;
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native Bitmap getIdNumber(Bitmap src, Bitmap.Config config);
}
