package com.feifei.socket;


import java.io.*;
import java.net.Socket;
import java.nio.charset.StandardCharsets;


public class Esp8266Socket extends Thread{
    private Socket clientSocket;
    private PrintWriter out;
    private InputStream in;

    public static final String PATH = "C:\\Users\\飞飞超帅的\\Desktop\\1.jpg";

    public Esp8266Socket(Socket socket){
        this.clientSocket = socket;
    }

    public void run(){
        try {
            File imgData = new File(PATH);

            FileOutputStream imgDataSteam = new FileOutputStream(imgData,true);

            out = new PrintWriter(clientSocket.getOutputStream());

            in = clientSocket.getInputStream();

            while(true){
                String inputLine = "";
                byte[] temp = new byte[512];
                int length = in.read(temp);
                // inputLine += new String(temp,0,length);
                if(".".equals(inputLine)){
                    out.println("bye");
                    break;
                }
                imgDataSteam.write(temp);

                System.out.println("写入成功");
                // System.out.println(inputLine);
            }

            out.println("客户端已经取消连接");
            in.close();
            out.close();
            clientSocket.close();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            System.out.println("error");
        }
    }
}
