package homeParameters;

public class HomeParameters {
    int roomsNumber;
    int windowsNumber;
    int doorsNumber;
    int roomsArea;

    public HomeParameters(int roomsNumber, int windowsNumber, int doorsNumber, int roomsArea) {
        this.roomsNumber = roomsNumber;
        this.windowsNumber = windowsNumber;
        this.doorsNumber = doorsNumber;
        this.roomsArea = roomsArea;
    }

    @Override
    public String toString() {
        return "HomeParameters{" +
                "roomsNumber=" + roomsNumber +
                ", windowsNumber=" + windowsNumber +
                ", doorsNumber=" + doorsNumber +
                ", roomArea=" + roomsArea +
                '}';
    }

    public int fireProtectionModule(){
        int numberOfSensors = this.roomsNumber;

        while(this.roomsArea/numberOfSensors > 36){
            numberOfSensors++;
        }

        return numberOfSensors;
    }
}
