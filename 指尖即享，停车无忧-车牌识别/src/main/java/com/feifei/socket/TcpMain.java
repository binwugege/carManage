package com.feifei.socket;

public class TcpMain {
    public static void main(String[] args)throws Exception {
        new TcpMultiServer().start(55559);
    }
}
