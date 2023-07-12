package com.feifei.socket;

import java.io.IOException;
import java.net.ServerSocket;


public class TcpMultiServer {

    private ServerSocket serverSocket;

    public void start(int port) throws IOException
    {
        serverSocket = new ServerSocket(port);
        while(true) {
            new Esp8266Socket(serverSocket.accept()).start();
            System.out.println(serverSocket);
        }
    }
    public void stop() throws IOException
    {
        serverSocket.close();
    }
}
