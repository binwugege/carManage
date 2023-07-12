package com.feifei.socket;

import com.feifei.carInit.LicensePlate;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

/**建立窗体，创建socket server**/
public class PngServer{
    public static void CreatServer(){
        ServerSocket serverSocket;
        Socket socket;
        ImagePanel dsplayImagePanel = new ImagePanel();

        JFrame frame = new JFrame();  //创建一个窗口用来显示图片
        JPanel left = new JPanel();
        JPanel right = new JPanel();

        TextField textField = new TextField("请输入端口",60);
        Button button = new Button("login");


        frame.setLayout(new GridLayout(1,2));
        frame.setTitle("车位监控系统");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(800, 500);

        left.setLayout(new GridLayout(1,1));
        left.add(dsplayImagePanel);

        // right.setLayout(new GridLayout(4,1));
        // right.setLayout(new FlowLayout());
        // right.add(textField);
        // right.add(button);

        frame.add(left);
        // frame.add(right);
        frame.setVisible(true);

        try {
            serverSocket =new ServerSocket(55559);  //55559
            while(true)
            {
                socket = serverSocket.accept();
                System.out.println(socket.getRemoteSocketAddress());

                // 这里开启一个图片显示 在这里的时候循环的处理数据然后显示图片
                clientProcess newClient = new clientProcess(socket, dsplayImagePanel);
                // 如果出现故障就在这里socket服务器关闭
                socket.close();
            }
        }catch(IOException e) {
            System.out.println(e);
        }

    }

    public static void main(String[] args) {
        System.out.println("PngServer:");
        new MyThread().start();
        CreatServer();
    }
}

/**刷新图片并显示**/
class ImagePanel extends JPanel {

    private BufferedImage image;

    public ImagePanel() {  //构造函数，打开一张图并显示
        try {
            image = ImageIO.read(new File("C:\\Users\\飞飞超帅的\\Desktop\\1.png"));
        } catch (IOException ex) {
            // handle exception...
        }
    }

    /**更新图片的方法**/
    public void exchangeImage(InputStream in) {
        try {
            image = ImageIO.read(in);  //更新内存中图片数据
            repaint();  //重新显示
        } catch (IOException ex) {
            // handle exception...
        }
    }

    /**显示图片**/
    @Override
    public void paintComponent(Graphics g) {
        g.drawImage(image, 0, 0, null);
    }
}


/**socket接收数据并处理**/
class clientProcess{
    boolean carResult;
    Socket clientSocket;
    ImagePanel clientImagePanel;
    InputStream clientIs;
    OutputStream clientOs;

    static int frame = 0;
    static int netWorkSpeed = 0;

    /**构造函数，保存连接的socket对象信息，以及初始化一些变量**/
    clientProcess(Socket socket, ImagePanel ImagePanel){
        clientSocket = socket;
        clientImagePanel = ImagePanel;
        frame = 0;
        recvProcess();
    }

    /**数据接收并处理**/
    void recvProcess()
    {
        int count = 0;
        int i = 0;
        byte[] buffer = new byte[8192];
        int getPicFlag = 0;

        FileOutputStream imgDataSteam = null;
        String PATH = "C:\\Users\\飞飞超帅的\\Desktop\\1.jpg";

        File imgData = new File(PATH);

        try {
            imgDataSteam  = new FileOutputStream(imgData,false);

        }catch ( Exception e){
            e.printStackTrace();
        }

        // 接收字节流数据
        ByteArrayOutputStream StreamArray = new ByteArrayOutputStream();

        try {
            clientIs = clientSocket.getInputStream();
            clientOs = clientSocket.getOutputStream();
            while(true)
            {
                count = clientIs.read(buffer);
                if(count == -1)
                {
                    System.out.println("Client Disconnect.........");
                    break;
                }
                else
                {
                    if(getPicFlag == 1)
                    {
                        for(i = 0; i < count -1; i++)
                        {
                            if((buffer[i] == -1)&&(buffer[i+1 ]== -39)) //-1，-39为十六进制的0xFF,0xD9，为jpg图像数据的结束标志
                            {
                                StreamArray.write(buffer,0,i+2);
                                byte[] tempbuffer =  StreamArray.toByteArray();
                                InputStream ImageStream = byteTostream(tempbuffer);

                                // 写入图片保存下来
                                imgDataSteam  = new FileOutputStream(imgData);
                                imgDataSteam.write(tempbuffer);
                                clientImagePanel.exchangeImage(ImageStream);

                                /**
                                LicensePlate licensePlate = new LicensePlate();
                                carResult = licensePlate.carPlateInit();
                                if(carResult){
                                    buffer[0] = 'O';
                                    buffer[1] = 'K';
                                    clientOs.write(buffer, 0, 2);  // 回复stm32，开始发下一个数据包
                                }
                                **/
                                imgDataSteam.close();
                                StreamArray.close();
                                ImageStream.close();

                                // 重新打开数据流
                                StreamArray = new ByteArrayOutputStream();

                                frame++;        // 计算fps
                                getPicFlag = 0; // 重新接收图片数据
                            }
                        }
                        // 如果没有接收到数据的结束，直接将数据清楚掉
                        if(getPicFlag == 1)
                        {
                            StreamArray.write(buffer,0,count);
                        }
                    }

                    if(getPicFlag == 0)
                    {
                        for(i = 0; i < count -1; i++)
                        {
                            if((buffer[i] == -1) && (buffer[i+1] == -40))  //-1,-40为十六进制的0xFF，0xD8,为jpg图像数据的起始标志
                            {
                                getPicFlag = 1;
                                // 在i处找到了jpg数据的开始位吧剩下的数据放进去
                                StreamArray.write(buffer,i,count - i);
                            }
                        }
                    }


                    netWorkSpeed = netWorkSpeed + count/1024;  //计算接收速率的
                }
            }
        } catch (IOException e) {
            // TODO Auto-generated catch block
            System.out.println(e);
            e.printStackTrace();
            try {
                clientIs.close();
                clientOs.close();
                clientSocket.close();
            } catch (IOException e1) {
                // TODO Auto-generated catch block
                e1.printStackTrace();
            }
        }
    }

    public static int getFrame()
    {
        return frame;
    }

    public static void clearFrame()
    {
        frame = 0;
    }

    public static int getNetWorkSpeed()
    {
        return netWorkSpeed;
    }

    public static void clearNetWorkSpeed()
    {
        netWorkSpeed = 0;
    }

    public static final InputStream byteTostream(byte[] buf) {
        return new ByteArrayInputStream(buf);
    }
}

/**新建一个线程定时计算网速以及帧率**/
class MyThread extends Thread{
    public void run(){
        while(true){
            try {
                // 1s获取一下获取的图片参数
                sleep(1000);
                System.out.println("Frame  " + clientProcess.getFrame() + "fps/s" + "  NetWork:" + clientProcess.getNetWorkSpeed() + " Kb/s");
                clientProcess.clearFrame();
                clientProcess.clearNetWorkSpeed();
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }
}