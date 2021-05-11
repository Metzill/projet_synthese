package application;

public class MyClass {

    public native void getSchedule(String inFileName, String outFileName, int datastructure, int order, int backfilling);

    public static void main(String[] args) {
    	new MyClass();
    }

    //Commande pour générer le .h : javac -h jni application/MyClass.java
    //Commande pour compiler le java : javac application/MyClass.java
    //Commande pour executer le java depuis le dossier java : java -Djava.library.path=. application.MyClass
    //Commande pour compiler le java  javac --module-path /usr/lib/jvm/javafx-sdk-11.0.2/lib --add-modules javafx.fxml,javafx.controls,javafx.graphics application/main.java
    // Commande pour executer le java depuis le dossier javaFx :  java -Djava.library.path=. --module-path /usr/lib/jvm/javafx-sdk-11.0.2/lib --add-modules javafx.fxml,javafx.controls,javafx.graphics application.main


    static {
        System.loadLibrary("mylibrary");
    }
}
