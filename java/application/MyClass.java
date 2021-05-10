package application;

public class MyClass {

    public native void getSchedule(String inFileName, String outFileName, int datastructure, int order, int backfilling);

    public static void main(String[] args) {
    	new MyClass().getSchedule("../data/exemple","../data/outFileName.txt",1,1,1);
    }

    //Commande pour générer le .h : javac -h jni application/MyClass.java
    //Commande pour compiler le java : javac application/MyClass.java
    //Commande pour executer le java depuis le dossier java : java -Djava.library.path=. application.MyClass


    static {
        System.loadLibrary("mylibrary");
    }
}
