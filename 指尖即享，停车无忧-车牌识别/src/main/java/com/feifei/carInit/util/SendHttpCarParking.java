package com.feifei.carInit.util;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.HashMap;

/**
 * 发送请求获取用户的数据
 */
public class SendHttpCarParking {

    public String sendHttpPost( String requestUrl, String param )throws Exception{
        URL url = new URL(requestUrl);
        HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
        //设置请求方式，请求参数类型
        httpURLConnection.setRequestMethod("POST");
        httpURLConnection.setRequestProperty("content-type","application/json;charset=UTF-8");
        httpURLConnection.setDoOutput(true);

        OutputStream outputStream = httpURLConnection.getOutputStream();

        // 将参数写入输出流,param必须是JSON格式
        outputStream.write(param.getBytes());
        outputStream.flush();

        InputStream inputStream = httpURLConnection.getInputStream();
        ByteArrayOutputStream bout = new ByteArrayOutputStream();

        byte[] bytes = new byte[1024];
        int len = 0;

        while ((len=inputStream.read(bytes)) >= 0){
            bout.write(bytes,0,len);
        }
        inputStream.close();
        bout.close();

        String respone = new String(bout.toByteArray());
        return respone;
    }

    public String sendHttpGet( String requestUrl, HashMap<String,Object> hashMap ) throws Exception{
        StringBuilder stringBuilderParam = new StringBuilder();

        for (String s : hashMap.keySet()){
            stringBuilderParam.append(s).append("=").append( URLEncoder.encode((String)hashMap.get(s),"UTF-8")).append("&");
        }

        // 将最后以为&字符删除
        String param = stringBuilderParam.toString();
        param = param.substring(0,param.length()-1);

        // 拼接到url
        requestUrl = requestUrl + "?" + param;
        URL url = new URL(requestUrl);
        HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
        httpURLConnection.setRequestMethod("GET");
        httpURLConnection.setDoOutput(true);

        InputStream inputStream = httpURLConnection.getInputStream();
        ByteArrayOutputStream bout = new ByteArrayOutputStream();
        byte[] bytes = new byte[1024];
        int len = 0;
        while ((len=inputStream.read(bytes))>=0){
            bout.write(bytes,0,len);
        }
        inputStream.close();
        bout.close();
        String respone = new String(bout.toByteArray());
        return respone;
    }

}
