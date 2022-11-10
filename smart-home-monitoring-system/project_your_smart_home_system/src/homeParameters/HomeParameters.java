package homeParameters;

import java.util.Arrays;
import java.util.Scanner;

public class HomeParameters {
    int roomsNumber;
    int []roomArea;
    int windowsNumber;
    int doorsNumber;

    public HomeParameters(int roomsNumber, int[] roomArea, int windowsNumber, int doorsNumber) {
        this.roomsNumber = roomsNumber;
        this.roomArea = roomArea;
        this.windowsNumber = windowsNumber;
        this.doorsNumber = doorsNumber;
    }

    @Override
    public String toString() {
        return "HomeParameters{" +
                "roomsNumber=" + roomsNumber +
                ", roomArea=" + Arrays.toString(roomArea) +
                ", windowsNumber=" + windowsNumber +
                ", doorsNumber=" + doorsNumber +
                '}';
    }

    public int fireProtectionModule(){
        int numberOfSensors = this.roomsNumber;

        while(Arrays.stream(this.roomArea).sum()/numberOfSensors > 36){
            numberOfSensors++;
        }
        for (int i = 0; i < roomArea.length; i++) {

            while (this.roomArea[i] > 36){
                numberOfSensors++;
                this.roomArea[i] -= 36;
            }
        }
        return numberOfSensors;
    }

    public int[] theftProtectionModule(){

        // tab[0] = HC sensors, tab[1] = PIR sensors;
        int[] tab = {doorsNumber + windowsNumber, roomsNumber};

        return tab;


    }
}
