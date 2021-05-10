package controlleur;

import application.MyClass;
import javafx.collections.FXCollections;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ComboBox;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.Pane;

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
    private CheckBox chckBoxBackfilling;

    @FXML
    private Button btnValider;

    @FXML
    private Pane paneResulsts;

    private ArrayList<String> listeInput=new ArrayList<>();
    private ArrayList<String> listeOutput=new ArrayList<>();
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

        System.out.println(dataType+"/"+ordre);

    }

    public void initialize(){
        listFilesForFolder(new File("data/Input"),listeInput);
        listFilesForFolder(new File("data/Output"),listeOutput);
        cmbBoxInputFile.getItems().addAll(listeInput);
        cmbBoxOutputFile.getItems().addAll(listeOutput);

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