package test.java.test001;

public class sum {
    public static void main(String[] args) {
        insum data = new sum_test();
        data.sun(10,20);
    }
}






class sum_test implements insum{
    @Override
    public void sun(int a,int b) {
        System.out.println(a+b);
    }

    @Override
    public void sub(int a,int b) {
        System.out.println(a/b);
    }
}

interface insum{
    void sun(int a,int b);
    void sub(int a, int b);
}