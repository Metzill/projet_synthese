package controlleur;

import application.MyClass;
import javafx.collections.FXCollections;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.layout.Pane;
import javafx.scene.control.ScrollPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import java.io.File;  // Import the File class
import java.io.FileNotFoundException;  // Import this class to handle errors
import java.util.Scanner;

import java.io.File;
import java.util.ArrayList;

public class ControllerSchedule {

    @FXML
    private ComboBox<String> cmbBoxInputFile;

    @FXML
    private ComboBox<String> cmbBoxOutputFile;

    @FXML
   private RadioButton radioBtnSPT;

   @FXML
   private ToggleGroup Order;

   @FXML
   private RadioButton radioBtnLPT;

   @FXML
   private RadioButton radioBtnWSPT;

   @FXML
   private RadioButton radioBtnFCFS;

   @FXML
   private RadioButton radioBtnOL;

   @FXML
   private ToggleGroup dateType;

   @FXML
   private RadioButton radioBtnBST;

   @FXML
   private RadioButton radioBtnEBST;

   @FXML
   private CheckBox chckBoxEchelle;

   @FXML
   private CheckBox chckBoxBackfilling;

   @FXML
   private Button btnValider;

   @FXML
   private Label lblErreur;

   @FXML
  private ScrollPane paneResulsts;



    private ArrayList<String> listeInput=new ArrayList<>();
    private ArrayList<String> listeOutput=new ArrayList<>();
    private ArrayList<String> Resultat=new ArrayList<>();
    private final int  LARGEUR=100;
    private int Backfilling=0;


    @FXML
    void valider(ActionEvent event) {
        int ordre=1;
        int dataType=1;
        RadioButton selectedRadioButton = (RadioButton) Order.getSelectedToggle();
        String toogleGroupValue = selectedRadioButton.getText();
        switch (toogleGroupValue){
            case "SPT":ordre=1;break;
            case "LPT":ordre=2;break;
            case "WSPT":ordre=3;break;
            case "FCFS":ordre=4;break;
        }
        selectedRadioButton = (RadioButton) dateType.getSelectedToggle();
        toogleGroupValue = selectedRadioButton.getText();
        switch (toogleGroupValue){
            case "OL":dataType=1;break;
            case "BST":dataType=2;break;
            case "EBST":dataType=3;break;
        }
        MyClass my=new MyClass();

        if(chckBoxBackfilling.isSelected()){
          this.Backfilling=1;
        }else
          this.Backfilling=0;
        if(cmbBoxInputFile.getValue()!=null && cmbBoxOutputFile.getValue()!=null){
          this.lblErreur.setText("");
          my.getSchedule("../data/Input/"+cmbBoxInputFile.getValue(),"../data/Output/"+cmbBoxOutputFile.getValue(),dataType,ordre,Backfilling);
          loadGant();
          showGant();
      }else
        this.lblErreur.setText("Manque d'information");
    }



    public void loadGant(){
      Resultat.clear();
        try {
          File myObj = new File("../data/Output/"+cmbBoxOutputFile.getValue());
          Scanner myReader = new Scanner(myObj);
          while (myReader.hasNextLine()) {
            String data = myReader.nextLine();
            Resultat.add(data);
          }
          myReader.close();
        } catch (FileNotFoundException e) {
          System.out.println("An error occurred.");
          e.printStackTrace();
        }
    }


    public void showGant(){

      Rectangle rectangle;
      Label label;
      int y=0;
      int x=0;
      int temps=0;
      Pane receptacle=new Pane();
      for(String r : Resultat){
         String[] info=r.split(" ");
         x = Integer.parseInt(info[1]);
         temps = Integer.parseInt(info[2]);
         label = new Label("tache "+info[0]);

         label.setTextFill(Color.WHITE);
         label.setLayoutX(20+LARGEUR*x);
         label.setLayoutY(1+20*y+20);
         rectangle=new Rectangle();
         rectangle.setLayoutX(LARGEUR*x);
         rectangle.setLayoutY(20*y+20);
         rectangle.setWidth(LARGEUR*temps);
         rectangle.setHeight(20);
         rectangle.setFill(Color.color(Math.random(), Math.random(), Math.random()));
         rectangle.setStroke(Color.color(Math.random(), Math.random(), Math.random()));
         rectangle.setStyle("-fx-stroke-width: 3;");
         receptacle.getChildren().addAll(rectangle,label);
         y+=1;
     }
     if(chckBoxEchelle.isSelected())
       setBar(receptacle,x+temps);
     paneResulsts.setContent(receptacle);
    }


    public void setBar(Pane receptacle,int max){
      Label label;
      Rectangle rectangle;
      for(int i=0;i<max;i++){
        label=new Label(Integer.toString(i));
        label.setTextFill(Color.BLACK);
        label.setLayoutX(20+LARGEUR*i);
        label.setLayoutY(0);
        rectangle=new Rectangle();
        rectangle.setLayoutX(LARGEUR*i);
        rectangle.setLayoutY(0);
        rectangle.setWidth(100);
        rectangle.setHeight(20);
        rectangle.setStyle("-fx-fill: WHITE; -fx-stroke: BLACK; -fx-stroke-width: 3;");
        receptacle.getChildren().addAll(rectangle,label);
      }
    }

    public void initialize(){
        listFilesForFolder(new File("../data/Input"),listeInput);
        listFilesForFolder(new File("../data/Output"),listeOutput);
        cmbBoxInputFile.getItems().addAll(listeInput);
        cmbBoxOutputFile.getItems().addAll(listeOutput);
        radioBtnLPT.setSelected(true);
        radioBtnOL.setSelected(true);
    }

    public void listFilesForFolder(final File folder,ArrayList<String> Liste) {
        for (final File fileEntry : folder.listFiles()) {
            if (fileEntry.isDirectory()) {
                listFilesForFolder(fileEntry,Liste);
            } else {
                //System.out.println(fileEntry.getName());
                Liste.add(fileEntry.getName());
            }
        }
    }


}
